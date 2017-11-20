// ----------------------------------------------------------------------------
// The Parser Library
// Copyright (c) 2008 by Rich Sposato
//
// Permission to use, copy, modify, distribute and sell this software for any
// purpose is hereby granted under the terms stated in the GNU Library Public
// License, provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear in
// supporting documentation.
//
// ----------------------------------------------------------------------------

// $Header: /cvsroot/utilparsers/UtilParsers/Config/src/ParserRules.cpp,v 1.6 2009/01/05 19:24:52 rich_sposato Exp $

/// @file ParserRules.cpp Contains class of spirit rules for config parser.


// ----------------------------------------------------------------------------
// Include Files

#include "ParserRules.hpp"

#include "../../Util/include/ParseUtil.hpp"

#include "CommonParsers.hpp"


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if defined( _MSC_VER )
    #pragma inline_depth(255)
    #pragma inline_recursion(on)
#endif

#if defined( _MSC_VER )
#pragma warning( disable : 4189 )
#endif


// ----------------------------------------------------------------------------
// Namespace resolution.

using namespace std;
using namespace boost::spirit;
using namespace Parser;

namespace
{


// ----------------------------------------------------------------------------

static const char s_Quote = '\"';

// ----------------------------------------------------------------------------

void TrimWhitespace( const char * & first, const char * & last )
{
    if ( last > first )
    {
        while ( ( last > first ) && ( ( '\t' == *first ) || ( ' ' == *first ) ) )
            first++;
        if ( last <= first )
            first--;
        do
            last--;
        while ( ( last > first ) && ( ( '\t' == *last ) || ( ' ' == *last ) ) );
        last++;
    }
}

// ----------------------------------------------------------------------------

} // end namespace Parser

ConfigFileParser * ConfigFileParser::s_pParser = NULL;

namespace Parser
{

// ----------------------------------------------------------------------------

ConfigFileParser::ConfigFileParser( const ConfigParser::ParserPolicy & policy,
    MessageStack & stack, LineCounter & lineCounter ) :
    m_ValidSyntax( false ),
    m_ValidContent( false ),
    m_DidSection( false ),
    m_trim( false ),
    m_keyStart( NULL ),
    m_keyEnd( NULL ),
    m_valueStart( NULL ),
    m_valueEnd( NULL ),
    m_pReceiver( NULL ),
    m_start(),
    m_name_rule(),
    m_start_section(),
    m_section_name(),
    m_end_section(),
    m_skip_section(),
    m_section(),
    m_line_comment(),
    m_block_comment_start(),
    m_comment_content(),
    m_block_comment_end(),
    m_skip_block_comment(),
    m_block_comment(),
    m_key_rule(),
    m_assign(),
    m_quoted_part(),
    m_skip_quote(),
    m_quoted_value(),
    m_bare_value(),
    m_value_rule(),
    m_key_value(),
    m_clear_content(),
    m_end_error(),
    m_content(),
    m_config()
{
    SetPolicy( policy, stack, lineCounter );
    s_pParser = this;
}

// ----------------------------------------------------------------------------

ConfigFileParser::~ConfigFileParser( void )
{
    s_pParser = NULL;
}

// ----------------------------------------------------------------------------

void ConfigFileParser::SetPolicy( const ConfigParser::ParserPolicy & policy,
    MessageStack & stack, LineCounter & lineCounter )
{

    m_start = epsilon_p
        [ FClear() ];

    m_line_comment =
        (
          str_p( policy.LineComment )
          >> ( *( print_p - eol_p ) )
          >> lineCounter.GetRule()
        );

    m_skip_block_comment = ( *print_p )
        [ FPopMessageStack( stack ) ]
        [ FSetValidSyntax( false ) ];

    m_block_comment_start = str_p( policy.BlockCommentStarter )
        [ FPushMessage( stack, ErrorLevel::Major, "Found start of comment, but no comment content.", __FILE__, "m_block_comment_start" ) ];

    m_comment_content = ( *( print_p - str_p( policy.BlockCommentEnder ) ) )
        [ FPrepareMessage( stack, ErrorLevel::Major, "Found comment, but no end of comment.", __FILE__, "m_comment_content" ) ];

    m_block_comment_end = str_p( policy.BlockCommentEnder )
        [ FCancelMessage( stack ) ];

    m_block_comment =
        ( m_block_comment_start
          >> ( *( blank_p )
            >> ( m_comment_content >> m_block_comment_end )
               | m_skip_block_comment
             )
        );

    m_assign =
        (
          *( blank_p )
          >> str_p( policy.AssignOperator )
          >> *( blank_p )
        );
    if ( policy.AllowQuotedCommentInValue )
    {
        m_quoted_part =
            (
              *( print_p -
                 ( ch_p( s_Quote ) | eol_p | end_p )
               )
            );
        m_skip_quote =
            (
              *( print_p - eol_p ) | end_p
            )
            [ FSendMessageNow( stack, ErrorLevel::Major, "Could not find ending quote for value." ) ]
            [ FSetValidContent( false ) ]
            [ FSetValidSyntax( false ) ];
        m_quoted_value =
            (
              ch_p( s_Quote )
              >> (
                   ( m_quoted_part >> ch_p( s_Quote ) )
                   | m_skip_quote
                 )
            );
        m_bare_value =
            (
              *( print_p -
                 (
                   str_p( policy.BlockCommentStarter )
                   | str_p( policy.LineComment )
                   | eol_p
                 )
              )
            );
        m_value_rule = ( m_quoted_value | m_bare_value )
            [ FSetValue() ];
    }
    else
    {
        m_value_rule =
            (
              *( print_p -
                 ( str_p( policy.BlockCommentStarter ) | m_line_comment | eol_p )
              )
            )
            [ FSetValue() ];
    }
    m_key_value
        = ( m_key_rule
            >> !( m_assign >> ( !m_value_rule ) )
            >> !( m_line_comment | m_block_comment )
          )
        [ FSendKeyValuePair() ];
    m_clear_content = epsilon_p
        [ FClearContents() ];
    m_end_error = ( *print_p )
        [ FSendMessageNow( stack, ErrorLevel::Fatal, "Could not parse contents." ) ]
        [ FSetValidSyntax( false ) ];

    m_start_section = ( str_p( policy.SectionNameStarter ) >> *( blank_p ) )
        [ FPushMessage( stack, ErrorLevel::Major, "Found start of section, but no section name.", __FILE__, "m_start_section" ) ];
    m_end_section = ( *( blank_p ) >> str_p( policy.SectionNameEnder ) )
        [ FSendSectionName() ]
        [ FCancelMessage( stack ) ];
    m_skip_section =
        (
         ( *( print_p - str_p( policy.SectionNameEnder ) ) - eol_p )
          >> ( !eol_p )
        )
        [ FSetValidSyntax( false ) ]
        [ FPopMessageStack( stack ) ];
    m_section =
        ( m_start_section
          >> (
               ( m_section_name >> m_end_section )
               | m_skip_section
             )
        );

    if ( policy.AlphaNumericNames )
    {
        m_name_rule =
            ( ( alpha_p | ch_p( '_' ) )
              >> ( *( alnum_p | ch_p( '_' ) ) )
            )
            [ FSetName() ];
        m_section_name = ( m_name_rule )
            [ FPrepareMessage( stack, ErrorLevel::Major, "Found section name, but no end of section.", __FILE__, "m_section_name 1" ) ];
        m_key_rule = m_name_rule;
    }
    else
    {
        m_section_name =
            (
              +( print_p -
                 (
                   str_p( policy.SectionNameEnder )
                   | str_p( policy.SectionNameStarter )
                   | eol_p
                 )
              )
            )
            [ FSetName() ]
            [ FPrepareMessage( stack, ErrorLevel::Major, "Found section name, but no end of section.", __FILE__, "m_section_name 2" ) ];
        m_key_rule =
            ( +( print_p - ( m_start_section | m_assign | eol_p ) ) )
            [ FSetName() ];
    }

    m_content =
        ( m_clear_content
          >> ( *( blank_p ) )
          >> ( m_block_comment
             | m_line_comment
             | m_section
             | m_key_value )
          >> ( *( blank_p ) )
          >> ( *( lineCounter.GetRule() ) )
        );
    m_config =
        ( m_start
          >> ( ( *( m_content ) >> end_p )
             | m_end_error )
        )
        [ FDone() ];

    m_trim = policy.TrimWhiteSpace;
}

// ----------------------------------------------------------------------------

void ConfigFileParser::Clear( void )
{
    m_ValidSyntax = true;
    m_DidSection = false;
}

// ----------------------------------------------------------------------------

void ConfigFileParser::ClearContents( void )
{
    m_keyStart = NULL;
    m_keyEnd = NULL;
    m_valueStart = NULL;
    m_valueEnd = NULL;
    m_ValidContent = true;
}

// ----------------------------------------------------------------------------

void ConfigFileParser::SetName( const char * first, const char * last )
{
    if ( m_trim )
        TrimWhitespace( first, last );
    m_keyStart = first;
    m_keyEnd = last;
}

// ----------------------------------------------------------------------------

void ConfigFileParser::SendSectionName( void )
{
    m_DidSection = true;
    if ( NULL == m_pReceiver )
        return;

    try
    {
        if ( !m_pReceiver->AddSection( m_keyStart, m_keyEnd ) )
            m_pReceiver = NULL;
    }
    catch ( ... )
    {
        m_pReceiver = NULL;
        throw;
    }
}

// ----------------------------------------------------------------------------

void ConfigFileParser::SetValue( const char * first, const char * last )
{
    if ( m_trim )
        TrimWhitespace( first, last );
    m_valueStart = first;
    m_valueEnd = last;
}

// ----------------------------------------------------------------------------

void ConfigFileParser::SendKeyValuePair( void )
{
    if ( NULL == m_pReceiver )
        return;
    if ( !m_ValidContent )
        return;

    try
    {
        if ( m_DidSection )
        {
            if ( !m_pReceiver->AddSectionKey( m_keyStart, m_keyEnd, m_valueStart, m_valueEnd ) )
                m_pReceiver = NULL;
        }
        else
        {
            if ( !m_pReceiver->AddGlobalKey( m_keyStart, m_keyEnd, m_valueStart, m_valueEnd ) )
                m_pReceiver = NULL;
        }
    }
    catch ( ... )
    {
        m_pReceiver = NULL;
        throw;
    }
}

// ----------------------------------------------------------------------------

void ConfigFileParser::Done( void )
{
    if ( NULL == m_pReceiver )
        return;
    try
    {
        m_pReceiver->ParsedConfigFile( IsValid() );
    }
    catch ( ... )
    {
        m_pReceiver = NULL;
        throw;
    }
    m_pReceiver = NULL;
}

// ----------------------------------------------------------------------------

} // end namespace Parser

// $Log: ParserRules.cpp,v $
// Revision 1.6  2009/01/05 19:24:52  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.5  2008/12/09 19:41:28  rich_sposato
// Changed policy names.
//
// Revision 1.4  2008/12/09 00:24:13  rich_sposato
// Put pragmas inside #if sections.
//
// Revision 1.3  2008/12/09 00:11:28  rich_sposato
// Changed receiver class and parser class to do memory allocation less often
// and improve performance.
//
// Revision 1.2  2008/12/06 08:50:12  rich_sposato
// Changes to improve debugging abilities or fix a bug.
//
// Revision 1.1  2008/12/05 19:25:28  rich_sposato
// Adding files to CVS.
//
