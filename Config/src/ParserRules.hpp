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

// $Header: /cvsroot/utilparsers/UtilParsers/Config/src/ParserRules.hpp,v 1.4 2009/01/05 19:24:52 rich_sposato Exp $

/// @file ParserRules.cpp Contains class of spirit rules for config parser.


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if !defined( UTIL_PARSER_CONFIG_PARSER_RULES_H_INCLUDED )
/// file guardian.
#define UTIL_PARSER_CONFIG_PARSER_RULES_H_INCLUDED


// ----------------------------------------------------------------------------
// Include Files

#include <boost/spirit/core.hpp>

#include "../include/ConfigParser.hpp"


// ----------------------------------------------------------------------------
// Namespace resolution.

namespace Parser
{
    class IConfigReceiver;
    class LineCounter;
    class MessageStack;


// ----------------------------------------------------------------------------
// Class definitions.

class ConfigFileParser
{
public:

    inline static ConfigFileParser & GetIt( void ) { return *s_pParser; }

    ConfigFileParser( const ::Parser::ConfigParser::ParserPolicy & policy,
        ::Parser::MessageStack & stack, LineCounter & counter );

    ~ConfigFileParser( void );

    inline const ::boost::spirit::rule<> & GetRule( void ) const { return m_config; }

    inline void SetReceiver( IConfigReceiver * pReceiver ) { m_pReceiver = pReceiver; }

    void SetPolicy( const ::Parser::ConfigParser::ParserPolicy & policy,
        ::Parser::MessageStack & stack, LineCounter & lineCounter );

    inline IConfigReceiver * GetReceiver( void ) { return m_pReceiver; }

    inline bool IsValid( void ) const { return m_ValidSyntax; }

    inline void SetTrimWhitespace( bool trim ) { m_trim = trim; }

    inline bool GetTrimWhitespace( void ) const { return m_trim; }

private:

    void Clear( void );

    void ClearContents( void );

    inline void SetValidSyntax( bool valid ) { m_ValidSyntax = valid; }

    inline void SetValidContent( bool valid ) { m_ValidContent = valid; }

    void SetName( const char * first, const char * last );

    void SendSectionName( void );

    void SetValue( const char * first, const char * last );

    void SendKeyValuePair( void );

    void Done( void );

    struct FClear
    {
        inline void operator () ( const char *, const char * ) const { s_pParser->Clear(); }
    };

    struct FClearContents
    {
        inline void operator () ( const char *, const char * ) const
        { s_pParser->ClearContents(); }
    };

    struct FSetValidSyntax
    {
        inline explicit FSetValidSyntax( bool valid ) :
            m_valid( valid ) {}

        inline void operator () ( const char *, const char * ) const
        { s_pParser->SetValidSyntax( m_valid ); }

        inline void operator () ( char ) const { s_pParser->SetValidSyntax( m_valid ); }

        bool m_valid;
    };

    struct FSetValidContent
    {
        inline explicit FSetValidContent( bool valid ) :
            m_valid( valid ) {}

        inline void operator () ( const char *, const char * ) const
        { s_pParser->SetValidContent( m_valid ); }

        inline void operator () ( char ) const { s_pParser->SetValidContent( m_valid ); }

        bool m_valid;
    };

    struct FSetName
    {
        inline void operator () ( const char * first, const char * last ) const
        { s_pParser->SetName( first, last ); }
    };

    struct FSendSectionName
    {
        inline void operator () ( const char *, const char * ) const
        { s_pParser->SendSectionName(); }
    };

    struct FSetValue
    {
        inline void operator () ( const char * first, const char * last ) const
        { s_pParser->SetValue( first, last ); }
    };

    struct FSendKeyValuePair
    {
        inline void operator () ( const char *, const char * ) const
        { s_pParser->SendKeyValuePair(); }
    };

    struct FDone
    {
        inline void operator () ( const char *, const char * ) const
        { s_pParser->Done(); }
    };

    static ConfigFileParser * s_pParser;

    bool m_ValidSyntax;
    bool m_ValidContent;
    bool m_DidSection;
    bool m_trim;
    const char * m_keyStart;
    const char * m_keyEnd;
    const char * m_valueStart;
    const char * m_valueEnd;
    IConfigReceiver * m_pReceiver;

    ::boost::spirit::rule<> m_start;
    ::boost::spirit::rule<> m_name_rule;
    ::boost::spirit::rule<> m_start_section;
    ::boost::spirit::rule<> m_section_name;
    ::boost::spirit::rule<> m_end_section;
    ::boost::spirit::rule<> m_skip_section;
    ::boost::spirit::rule<> m_section;
    ::boost::spirit::rule<> m_line_comment;
    ::boost::spirit::rule<> m_block_comment_start;
    ::boost::spirit::rule<> m_comment_content;
    ::boost::spirit::rule<> m_block_comment_end;
    ::boost::spirit::rule<> m_skip_block_comment;
    ::boost::spirit::rule<> m_block_comment;
    ::boost::spirit::rule<> m_key_rule;
    ::boost::spirit::rule<> m_assign;
    ::boost::spirit::rule<> m_start_quote;
    ::boost::spirit::rule<> m_quoted_part;
    ::boost::spirit::rule<> m_end_quote;
    ::boost::spirit::rule<> m_skip_quote;
    ::boost::spirit::rule<> m_quoted_value;
    ::boost::spirit::rule<> m_bare_value;
    ::boost::spirit::rule<> m_value_rule;
    ::boost::spirit::rule<> m_key_value;
    ::boost::spirit::rule<> m_skip_over;
    ::boost::spirit::rule<> m_clear_content;
    ::boost::spirit::rule<> m_end_error;
    ::boost::spirit::rule<> m_content;
    ::boost::spirit::rule<> m_config;

    /// Not implemented.
    ConfigFileParser( void );
    /// Not implemented.
    ConfigFileParser( const ConfigFileParser & );
    /// Not implemented.
    ConfigFileParser & operator = ( const ConfigFileParser & );

};

// ----------------------------------------------------------------------------

}; // end namespace Parser

#endif // file guardian

// $Log: ParserRules.hpp,v $
// Revision 1.4  2009/01/05 19:24:52  rich_sposato
// Replaced tabs with spaces.
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
