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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/src/PrologParsers.cpp,v 1.1 2008/12/05 19:04:16 rich_sposato Exp $


// ----------------------------------------------------------------------------

#include "./PrologParsers.hpp"

#include <iostream>

#include "../include/Receivers.hpp"

#include "./BasicParsers.hpp"


using namespace std;
using namespace boost::spirit;

namespace Parser
{

namespace Xml
{


// ----------------------------------------------------------------------------

PublicIdLiteralParser::PublicIdLiteralParser( ParserStacks & stacks ) :
    m_stacks( stacks ),
    m_receiver( NULL ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_singleQuoted( false ),
    m_pBegin( NULL ),
    m_pEnd( NULL ),
    m_skipOver(),
    m_startSQ(),
    m_sQuotedChars(),
    m_sQuoteOrRef(),
    m_endSQ(),
    m_skipSQ(),
    m_singleQuotedValue(),
    m_startDQ(),
    m_dQuoteOrRef(),
    m_dQuotedChars(),
    m_endDQ(),
    m_skipDQ(),
    m_doubleQuotedValue(),
    m_rule()
{
    assert( this != NULL );

    const CommonParserRules & commonRules = CommonParserRules::GetIt();
    Parser::MessageStack & messages = stacks.m_messages;

    //m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
    //    [ FSetValidSyntax( this, false ) ]
    //    [ Parser::FPopMessageStack( messages ) ]
    //    [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
    //        "Unable to parse public identifier literal - skipping rest of content." ) ];

    m_startSQ = ( commonRules.m_singleQuote )
        [ FSetQuoteType( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Major,
            "Found starting single quote but no content for public identifier literal." ) ];

    m_sQuotedChars = ( *( commonRules.m_pubidChar - commonRules.m_singleQuote ) )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find ending single-quote for public identifier literal." ) ]
        [ FSetContent( this ) ];

    m_endSQ = ( commonRules.m_singleQuote )
        [ Parser::FCancelMessage( messages ) ]
        [ FSetValidSyntax( this, true ) ]
        [ FEnd( this ) ];

    m_skipSQ = ( *( ~commonRules.m_singleQuote ) >> commonRules.m_singleQuote )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse single-quoted public identifier literal - skipping rest of content." ) ];

    m_singleQuotedValue = ( m_startSQ >> ( ( m_sQuotedChars >> m_endSQ ) | m_skipSQ ) );

    m_startDQ = ( commonRules.m_doubleQuote )
        [ FSetQuoteType( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Major,
            "Found starting double quote but no content for public identifier literal." ) ];

    m_dQuotedChars = ( *( commonRules.m_pubidChar - commonRules.m_doubleQuote ) )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find ending double-quote for public identifier literal." ) ]
        [ FSetContent( this ) ];

    m_endDQ = ( commonRules.m_doubleQuote )
        [ Parser::FCancelMessage( messages ) ]
        [ FSetValidSyntax( this, true ) ]
        [ FEnd( this ) ];

    m_skipDQ = ( *( ~commonRules.m_doubleQuote ) >> commonRules.m_doubleQuote )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse double-quoted system literal - skipping rest of content." ) ];

    m_doubleQuotedValue = ( m_startDQ >> ( ( m_dQuotedChars >> m_endDQ ) | m_skipDQ ) );

    m_rule = ( m_singleQuotedValue | m_doubleQuotedValue );
}

// ----------------------------------------------------------------------------

PublicIdLiteralParser::~PublicIdLiteralParser( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void PublicIdLiteralParser::SetQuoteType( const Parser::CharType quote )
{
    assert( this != NULL );
    m_validSyntax = false;
    m_singleQuoted = ( '\'' == quote );
    m_pBegin = NULL;
    m_pEnd = NULL;
    m_stackSize = m_stacks.m_messages.GetStackSize();
}

// ----------------------------------------------------------------------------

void PublicIdLiteralParser::StoreStackSize( void )
{
    assert( this != NULL );
    m_stackSize = m_stacks.m_messages.GetStackSize();
}

// ----------------------------------------------------------------------------

void PublicIdLiteralParser::End( void )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( IsValid() )
    {
        assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
    }

    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->SetPublicIdLiteral( m_pBegin, m_pEnd, m_singleQuoted );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
}

// ----------------------------------------------------------------------------

ExternalIdLiteralParser::ExternalIdLiteralParser(
    PublicIdLiteralParser & pubIdParser ) :
    m_stacks( pubIdParser.GetStacks() ),
    m_pubIdParser( pubIdParser ),
    m_receiver( NULL ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_pBegin( NULL ),
    m_pEnd( NULL ),
    m_startSQ(),
    m_sQuotedChars(),
    m_endSQ(),
    m_skipSQ(),
    m_singleQuotedValue(),
    m_startDQ(),
    m_dQuotedChars(),
    m_endDQ(),
    m_skipDQ(),
    m_doubleQuotedValue(),
    m_system(),
    m_sysLiteral(),
    m_public(),
    m_pubLiteral(),
    m_whitespace(),
    m_pubRules(),
    m_skipOver(),
    m_rule()
{
    assert( this != NULL );

    Parser::MessageStack & messages = m_stacks.m_messages;
    const CommonParserRules & commonRules = CommonParserRules::GetIt();

    m_startSQ = ( commonRules.m_singleQuote )
        [ FSetQuoteType( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Major,
            "Found starting single quote but no content for system literal." ) ];

    m_sQuotedChars = ( *( anychar_p - commonRules.m_singleQuote ) )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find ending single-quote for system literal." ) ];

    m_endSQ = ( commonRules.m_singleQuote )
        [ Parser::FCancelMessage( messages ) ];

    m_skipSQ = ( *( ~commonRules.m_singleQuote ) >> commonRules.m_singleQuote )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse single-quoted system literal - skipping rest of content." ) ];

    m_singleQuotedValue = ( m_startSQ >> ( ( m_sQuotedChars >> m_endSQ ) | m_skipSQ ) );

    m_startDQ = ( commonRules.m_doubleQuote )
        [ FSetQuoteType( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Major,
            "Found starting double quote but no content for system literal." ) ];

    m_dQuotedChars = ( *( anychar_p - commonRules.m_doubleQuote ) )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find ending double-quote for system literal." ) ];

    m_endDQ = ( commonRules.m_doubleQuote )
        [ Parser::FCancelMessage( messages ) ];

    m_skipDQ = ( *( ~commonRules.m_doubleQuote ) >> commonRules.m_doubleQuote )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse double-quoted system literal - skipping rest of content." ) ];

    m_doubleQuotedValue = ( m_startDQ >> ( ( m_dQuotedChars >> m_endDQ ) | m_skipDQ ) );

    m_system = str_p( "SYSTEM" )
        [ FSetup( this, false ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Minor,
            "Found start of system literal, but no contents." ) ];

    m_sysLiteral = ( m_singleQuotedValue | m_doubleQuotedValue )
        [ FSetSysLiteral( this ) ]
        [ FSetValidSyntax( this, true ) ]
        [ Parser::FCancelMessage( messages ) ];

    m_public = str_p( "PUBLIC" )
        [ FSetup( this, true ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Minor,
            "Found start of public literal, but no contents." ) ];

    m_pubLiteral = pubIdParser.GetRule()
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Expected to find white space between public ID and system literals." ) ]
        [ FSetPubIdLiteral( this ) ];

    m_whitespace = ( commonRules.m_whiteSpaces )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Expected to find quoted public-id or system literal." ) ];

    m_pubRules = ( m_public >> m_whitespace >> m_pubLiteral );

    m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
        [ FSetValidSyntax( this, false ) ]
        [ ::Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse external ID reference." ) ]
        [ ::Parser::FPopMessageStack( messages ) ];

    m_rule = ( ( m_pubRules | m_system ) >>
        ( ( m_whitespace >> m_sysLiteral ) | m_skipOver ) )
        [ FDone( this ) ];
}

// ----------------------------------------------------------------------------

ExternalIdLiteralParser::~ExternalIdLiteralParser( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void ExternalIdLiteralParser::Setup( bool isPublicId )
{
    assert( this != NULL );
    m_stackSize = m_stacks.m_messages.GetStackSize();
    m_validSyntax = false;
    m_singleQuoted = false;
    m_pBegin = NULL;
    m_pEnd = NULL;

    if ( isPublicId && ( NULL != m_receiver ) )
    {
        m_pubIdParser.SetReceiver( m_receiver );
    }
}

// ----------------------------------------------------------------------------

void ExternalIdLiteralParser::SetSysLiteral( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( m_receiver == NULL )
        return;
    bool keep = true;
    try
    {
        keep = m_receiver->SetSystemLiteral( begin, end, m_singleQuoted );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void ExternalIdLiteralParser::SetPubIdLiteral( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    (void)begin;
    (void)end;
    if ( !m_pubIdParser.IsValid() )
        m_validSyntax = false;
}

// ----------------------------------------------------------------------------

void ExternalIdLiteralParser::Done( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( IsValid() )
    {
        assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
    }
    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->DoneExternalIdLiteral( IsValid(), begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

PeReferenceParser::PeReferenceParser( NameParser & nameParser ) :
    m_stacks( nameParser.GetStacks() ),
    m_receiver( NULL ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_pBegin( NULL ),
    m_pEnd( NULL ),
    m_start(),
    m_name(),
    m_end(),
    m_skipOver(),
    m_rule()
{
    assert( this != NULL );

    Parser::MessageStack & messages = m_stacks.m_messages;

    m_start = ch_p( '%' )
//        [ FBreakPoint( this, "" ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Minor,
            "Found start of PE reference, but no name." ) ]
        [ FClear( this ) ];

    m_name = ( nameParser.GetRule() )
//        [ FBreakPoint( this, "" ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Found name for PE reference, but no ending semicolon." ) ]
        [ FSetName( this ) ];

    m_end = ch_p( ';' )
//        [ FBreakPoint( this, "" ) ]
        [ FSetValidSyntax( this, true ) ]
        [ Parser::FCancelMessage( messages ) ];

    m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
//        [ FBreakPoint( this, "" ) ]
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Minor,
            "Unable to parse name for PE reference." ) ]
        [ Parser::FPopMessageStack( messages ) ];

    m_rule = ( m_start >> ( ( m_name >> m_end ) | m_skipOver ) );
//        [ FBreakPoint( this, "m_rule" ) ];
}

// ----------------------------------------------------------------------------

PeReferenceParser::~PeReferenceParser()
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void PeReferenceParser::Clear( void )
{
    assert( this != NULL );
    m_stackSize = m_stacks.m_messages.GetStackSize();
    m_validSyntax = false;
    m_pBegin = NULL;
    m_pEnd = NULL;
}

// ----------------------------------------------------------------------------

void PeReferenceParser::End( void )
{
    assert( this != NULL );
    assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
    m_validSyntax = true;
    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->AddPeReference( m_pBegin, m_pEnd );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

EnumeratedTypeParser::EnumeratedTypeParser( NameParser & nameParser ) :
    m_stacks( nameParser.GetStacks() ),
    m_nameParser( nameParser ),
    m_receiver( NULL ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_isNotation( false ),
    m_noteStart(),
    m_noteBeginP(),
    m_name(),
    m_nameDelimiter(),
    m_endP(),
    m_notation(),
    m_enumStart(),
    m_nameToken(),
    m_enumeration(),
    m_skipOver(),
    m_rule()
{
    assert( this != NULL );

    const CommonParserRules & commonRules = CommonParserRules::GetIt();
    Parser::MessageStack & messages = m_stacks.m_messages;

    m_noteStart = str_p( "NOTATION" )
        [ FSetEnumType( this, true ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Major,
            "Did not find starting paranthese for notation." ) ];

    m_noteBeginP = ( !commonRules.m_whiteSpaces >> '(' >> !commonRules.m_whiteSpaces )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find content for notation." ) ];

    m_name = nameParser.GetRule()
        [ FSetName( this ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find '|' delimiter or ending paranthese ')' after name in notation." ) ];

    m_nameDelimiter = ( !commonRules.m_whiteSpaces >> '|' >> !commonRules.m_whiteSpaces )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find name after '|' delimiter." ) ];

    m_endP = ( !commonRules.m_whiteSpaces >> ')' )
        [ FSetValidSyntax( this, true ) ]
        [ Parser::FCancelMessage( messages ) ];

    m_notation = ( m_noteStart >> m_noteBeginP >> m_name >>
        *( m_nameDelimiter >> m_name ) >> m_endP );

    m_enumStart = ( ch_p( '(' ) >> !commonRules.m_whiteSpaces )
        [ FSetEnumType( this, false ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Major,
            "Did not find content for enumeration." ) ];

    m_nameToken = commonRules.m_nameToken
        [ FSetName( this ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
             "Did not find '|' delimiter or ending paranthese ')' after name in enumeration." ) ];

    m_enumeration = ( m_enumStart >> m_nameToken >>
        *( m_nameDelimiter >> m_nameToken ) >> m_endP );

    m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
//        [ FBreakPoint( this, "m_skipOver" ) ]
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse enumerated type - skipping rest of content." ) ];

    m_rule = ( m_notation | m_enumeration | m_skipOver )
        [ FDone( this ) ];
}

// ----------------------------------------------------------------------------

EnumeratedTypeParser::~EnumeratedTypeParser( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void EnumeratedTypeParser::SetEnumType( bool notation )
{
    assert( this != NULL );
    m_validSyntax = false;
    m_isNotation = notation;
    m_stackSize = m_stacks.m_messages.GetStackSize();
}

// ----------------------------------------------------------------------------

void EnumeratedTypeParser::SetName( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );

    if ( !m_nameParser.IsValid() )
        SetValidSyntax( false );
    if ( m_receiver == NULL )
        return;
    bool keep = false;
    try
    {
        keep = ( IsNotation() ) ?
            m_receiver->AddNotation( begin, end ) :
            m_receiver->AddEnumeration( begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void EnumeratedTypeParser::Done( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );

    if ( IsValid() )
        assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->DoneEnumeratedType( IsValid(), begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

EntityValueParser::EntityValueParser( ReferenceParser & refParser,
    PeReferenceParser & peReferenceParser ) :
    m_peRefParser( peReferenceParser ),
    m_refParser( refParser ),
    m_stacks( refParser.GetStacks() ),
    m_receiver( NULL ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_validContent( false ),
    m_singleQuoted( false ),
    m_skipOver(),
    m_sqStart(),
    m_dqStart(),
    m_reference(),
    m_peReference(),
    m_sqValue(),
    m_dqValue(),
    m_sqContent(),
    m_dqContent(),
    m_sqEnd(),
    m_dqEnd(),
    m_sqEntity(),
    m_dqEntity(),
    m_rule()
{
    assert( this != NULL );

    const CommonParserRules & commonRules = CommonParserRules::GetIt();
    Parser::MessageStack & messages = m_stacks.m_messages;

    m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
//        [ FBreakPoint( this, "m_skipOver" )]
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse entity value - skipping rest of content." ) ];

    m_sqStart = ( commonRules.m_singleQuote )
//        [ FBreakPoint( this, "m_sqStart" )]
        [ FSetQuoteType( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Major,
            "Entity value has starting single-quote but no content." ) ];

    m_dqStart = ( commonRules.m_doubleQuote )
//        [ FBreakPoint( this, "m_dqStart" )]
        [ FSetQuoteType( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Minor,
            "Entity value has starting double-quote but no content." ) ];

    m_reference = refParser.GetRule()
//        [ FBreakPoint( this, "m_reference" )]
        [ FSetReference( this ) ];

    m_peReference = peReferenceParser.GetRule()
//        [ FBreakPoint( this, "m_peReference" )]
        [ FSetPeReference( this ) ];

    m_sqValue = ( +( anychar_p - ( SpiritCharSet( "%&'" ) ) ) )
//        [ FBreakPoint( this, "m_sqValue" )]
        [ FSetValue( this ) ];

    m_dqValue = ( +( anychar_p - ( SpiritCharSet( "%&\"" ) ) ) )
//        [ FBreakPoint( this, "m_dqValue" )]
        [ FSetValue( this ) ];

    m_sqContent = ( *( m_reference | m_peReference | m_sqValue ) )
//        [ FBreakPoint( this, "m_sqContent" )]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Entity value has no ending single-quote." ) ];

    m_dqContent = ( *( m_reference | m_peReference | m_dqValue ) )
//        [ FBreakPoint( this, "m_dqContent" )]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Entity value has no ending double-quote." ) ];

    m_sqEnd = ( commonRules.m_singleQuote );
//        [ FBreakPoint( this, "m_sqEnd" )];

    m_dqEnd = ( commonRules.m_doubleQuote );
//        [ FBreakPoint( this, "m_dqEnd" )];

    m_sqEntity = ( m_sqStart >> m_sqContent >> m_sqEnd )
//        [ FBreakPoint( this, "m_sqEntity" )]
        [ Parser::FCancelMessage( messages ) ]
        [ FDone( this ) ];

    m_dqEntity = ( m_dqStart >> m_dqContent >> m_dqEnd )
//        [ FBreakPoint( this, "m_dqEntity" )]
        [ Parser::FCancelMessage( messages ) ]
        [ FDone( this ) ];

    m_rule = ( m_sqEntity | m_dqEntity | m_skipOver );
//        [ FBreakPoint( this, "m_rule" )];
}

// ----------------------------------------------------------------------------

EntityValueParser::~EntityValueParser( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void EntityValueParser::SetQuoteType( char quote )
{
    assert( this != NULL );
    m_validSyntax = false;
    m_validContent = true;
    m_singleQuoted = ( '\'' == quote );
    m_refParser.SetReceiver( NULL );
    m_peRefParser.SetReceiver( NULL );
    m_stackSize = m_stacks.m_messages.GetStackSize();
}

// ----------------------------------------------------------------------------

void EntityValueParser::SetValue( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );

    if ( m_receiver == NULL )
        return;
    bool keep = false;
    try
    {
        keep = m_receiver->AddValue( begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void EntityValueParser::SetReference( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );

    if ( m_receiver == NULL )
        return;
    bool keep = false;
    try
    {
        if ( !m_refParser.IsValid() )
            SetValidContent( false );
        keep = m_receiver->AddReference( begin, end, m_refParser.GetRefType() );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void EntityValueParser::SetPeReference( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );

    if ( m_receiver == NULL )
        return;
    bool keep = false;
    try
    {
        if ( !m_peRefParser.IsValid() )
            SetValidContent( false );
        keep = m_receiver->AddPeReference( begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void EntityValueParser::Done( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    SetValidSyntax( true );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( IsValid() )
    {
        assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
    }
    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->DoneEntityValue( IsValid(), m_singleQuoted, begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

EncodingDeclParser::EncodingDeclParser( ParserStacks & stacks ) :
    m_stacks( stacks ),
    m_receiver( NULL ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_pBegin( NULL ),
    m_pEnd( NULL ),
    m_start(),
    m_equals(),
    m_encName(),
    m_startSQ(),
    m_endSQ(),
    m_skipSQ(),
    m_sQuotedContent(),
    m_startDQ(),
    m_endDQ(),
    m_skipDQ(),
    m_dQuotedContent(),
    m_content(),
    m_skipOver(),
    m_rule()
{
    assert( this != NULL );

    const CommonParserRules & commonRules = CommonParserRules::GetIt();
    Parser::MessageStack & messages = stacks.m_messages;

    m_start = str_p( "encoding" )
        [ FClear( this ) ]
        [ FStoreStackSize( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Minor,
            "Did not find equal sign for encoding." ) ];

    m_equals = commonRules.m_equals
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find name for encoding." ) ];

    m_encName = commonRules.m_encName
        [ FSetName( this ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find ending quote for encoding." ) ];

    m_startSQ = ( commonRules.m_singleQuote )
        [ FSetQuoteType( this ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Found starting single quote but no content for encoding." ) ];

    m_endSQ = ( commonRules.m_singleQuote )
        [ Parser::FCancelMessage( messages ) ]
        [ FSetValidSyntax( this, true ) ];

    m_skipSQ = ( *( ~commonRules.m_singleQuote ) >> commonRules.m_singleQuote )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse single-quoted encoding - skipping rest of content." ) ];

    m_sQuotedContent = ( m_startSQ >> ( ( m_encName >> m_endSQ ) | m_skipSQ ) );

    m_startDQ = ( commonRules.m_doubleQuote )
        [ FSetQuoteType( this ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Found starting double quote but no content for encoding." ) ];

    m_endDQ = ( commonRules.m_doubleQuote )
        [ Parser::FCancelMessage( messages ) ]
        [ FSetValidSyntax( this, true ) ];

    m_skipDQ = ( *( ~commonRules.m_doubleQuote ) >> commonRules.m_doubleQuote )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse double-quoted encoding - skipping rest of content." ) ];

    m_dQuotedContent = ( m_startDQ >> ( ( m_encName >> m_endDQ ) | m_skipDQ ) );

    m_content = ( m_equals >> ( m_dQuotedContent | m_sQuotedContent ) );

    m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Minor,
            "Encoding has invalid format." ) ]
        [ Parser::FPopMessageStack( messages ) ];

    m_rule = ( m_start >> ( m_content | m_skipOver ) )
        [ FDone( this ) ];
}

// ----------------------------------------------------------------------------

EncodingDeclParser::~EncodingDeclParser( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void EncodingDeclParser::StoreStackSize( void )
{
    assert( this != NULL );
    m_stackSize = m_stacks.m_messages.GetStackSize();
}

// ----------------------------------------------------------------------------

void EncodingDeclParser::SetName( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );

    m_pBegin = begin;
    m_pEnd = end;
    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->SetEncName( begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
}

// ----------------------------------------------------------------------------

void EncodingDeclParser::Done( const Parser::CharType * begin,
        const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( IsValid() )
    {
        assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
    }

    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->DoneEncodingDecl( IsValid(), m_singleQuoted, begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

XmlDeclarationParser::XmlDeclarationParser( EncodingDeclParser & encodingDeclParser ) :
    m_encodingDeclParser( encodingDeclParser ),
    m_stacks( encodingDeclParser.GetStacks() ),
    m_receiver( NULL ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_validContent( false ),
    m_singleQuoted( false ),
    m_versionValue( '\0' ),
    m_pBegin( NULL ),
    m_pEnd( NULL ),
    m_start(),
    m_version(),
    m_versionEqual(),
    m_versionNumber(),
    m_startSQ(),
    m_endSQ(),
    m_skipSQ(),
    m_versionSQ(),
    m_startDQ(),
    m_endDQ(),
    m_skipDQ(),
    m_versionDQ(),
    m_versionInfo(),
    m_encodingDecl(),
    m_standalone(),
    m_equals(),
    m_yesSQ(),
    m_yesDQ(),
    m_noSQ(),
    m_noDQ(),
    m_badDecl(),
    m_standaloneDecl(),
    m_end(),
    m_skipOver(),
    m_rule()
{
    assert( this != NULL );

    Parser::MessageStack & messages = m_stacks.m_messages;
    const CommonParserRules & commonRules = CommonParserRules::GetIt();

    m_start = str_p( "<?xml" )
        [ FClear( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Major,
            "Found start of XML declaration, but no content." ) ];

    m_version = ( commonRules.m_whiteSpaces >> str_p( "version" ) )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Found version keyword but no equal sign in xml declaration." ) ];

    m_versionEqual = ch_p( '=' )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Expected version number in single or double quotes after equal sign in xml declaration." ) ];

    m_versionNumber = +( commonRules.m_versionChars )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Expected ending quote after version number in xml declaration." ) ];

    m_startSQ = ( commonRules.m_singleQuote )
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Major,
            "Found starting single quote but no content for version number." ) ];

    m_endSQ = ( commonRules.m_singleQuote )
        [ Parser::FCancelMessage( messages ) ];

    m_skipSQ = ( *( ~commonRules.m_singleQuote ) >> commonRules.m_singleQuote )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse single-quoted version number - skipping rest of content." ) ];

    m_versionSQ = ( m_startSQ >> ( ( m_versionNumber >> m_endSQ ) | m_skipSQ ) );

    m_startDQ = ( commonRules.m_doubleQuote )
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Major,
            "Found starting double quote but no content for version number." ) ];

    m_endDQ = ( commonRules.m_doubleQuote )
        [ Parser::FCancelMessage( messages ) ];

    m_skipDQ = ( *( ~commonRules.m_doubleQuote ) >> commonRules.m_doubleQuote )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse double-quoted version number - skipping rest of content." ) ];

    m_versionDQ = ( m_startDQ >> ( ( m_versionNumber >> m_endDQ ) | m_skipDQ ) );

    m_versionInfo = ( m_version >> m_versionEqual >> ( m_versionSQ | m_versionDQ ) )
        [ FPostVersionNumber( this ) ];

    m_encodingDecl = ( commonRules.m_whiteSpaces >> encodingDeclParser.GetRule() )
        [ FPostEncodingDecl( this ) ];

    m_standalone = ( commonRules.m_whiteSpaces >> str_p( "standalone" ) )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Found standalone keyword but no equal sign." ) ];

    m_equals = ch_p( '=' )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Found equal sign for standalone declaration but no value." ) ];

    m_yesSQ = str_p( "\'yes\'" )
        [ FSetStandaloneType( this, true, true ) ];

    m_yesDQ = str_p( "\"yes\"" )
        [ FSetStandaloneType( this, true, false ) ];

    m_noSQ = str_p( "\'no\'" )
        [ FSetStandaloneType( this, false, true ) ];

    m_noDQ = str_p( "\"no\"" )
        [ FSetStandaloneType( this, false, false ) ];

    m_badDecl = nothing_p
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Minor,
            "Unable to parse value of xml standalone declaration." ) ]
        [ FSetValidSyntax( this, false ) ];

    m_standaloneDecl = ( m_standalone >> m_equals
        >> ( m_yesSQ | m_yesDQ | m_noSQ | m_noDQ | m_badDecl ) );

    m_end = str_p( "?>" )
        [ Parser::FCancelMessage( messages ) ]
        [ FSetValidSyntax( this, true ) ];

    m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Minor,
            "Unable to parse xml declaration." ) ]
        [ Parser::FPopMessageStack( messages ) ];

    m_rule = ( m_start >> ( ( m_versionInfo >> !m_encodingDecl
        >> !m_standaloneDecl >> !commonRules.m_whiteSpaces >> m_end )
        | m_skipOver ) )
        [ FDone( this ) ];
}

// ----------------------------------------------------------------------------

XmlDeclarationParser::~XmlDeclarationParser( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void XmlDeclarationParser::Clear( void )
{
    assert( this != NULL );
    m_stackSize = m_stacks.m_messages.GetStackSize();
    m_validSyntax = false;
    m_validContent = true;
    m_singleQuoted = false;
    m_versionValue = '\0';
    m_pBegin = NULL;
    m_pEnd = NULL;
    m_encodingDeclParser.SetReceiver( m_receiver );
}

// ----------------------------------------------------------------------------

void XmlDeclarationParser::PostVersionNumber( const ::Parser::CharType * begin,
    const ::Parser::CharType * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );

    if ( NULL == m_receiver )
        return;
    bool keep = false;
    const bool singleQuoted = ( '\'' == *begin );
    try
    {
        keep = m_receiver->SetVersionNumber( singleQuoted, begin+1, end-1 );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void XmlDeclarationParser::SetStandaloneType( bool isYes, bool singleQuoted )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( NULL == m_receiver )
        return;
    bool keep = false;
    try
    {
        keep = m_receiver->SetIsStandalone( isYes, singleQuoted );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void XmlDeclarationParser::PostEncodingDecl( const ::Parser::CharType * begin,
    const ::Parser::CharType * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin <= end );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    (void)begin;
    (void)end;
    cout << __FUNCTION__ << '\t' << __LINE__ << endl;
    if ( !m_encodingDeclParser.IsValid() )
    {
        SetValidContent( false );
        cout << __FUNCTION__ << '\t' << __LINE__ << endl;
    }
}

// ----------------------------------------------------------------------------

void XmlDeclarationParser::Done( const ::Parser::CharType * begin,
    const ::Parser::CharType * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin <= end );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( IsValid() )
    {
        assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
    }

    if ( NULL == m_receiver )
        return;
    try
    {
        m_receiver->DoneXmlDeclaration( IsValid(), begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
}

// ----------------------------------------------------------------------------

AttListDeclParser::AttListDeclParser( EnumeratedTypeParser & enumTypeParser,
    AttributeValueParser & attValueParser ) :
    m_stacks( attValueParser.GetStacks() ),
    m_attValueParser( attValueParser ),
    m_enumTypeParser( enumTypeParser ),
    m_receiver( NULL ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_validContent( false ),
    m_start(),
    m_skipOver(),
    m_name(),
    m_attName(),
    m_tokenizedType(),
    m_preEnumeration(),
    m_enumeratedType(),
    m_attType(),
//    m_notationType(),
    m_defaultDeclType(),
    m_defaultDeclFixed(),
    m_preAttValue(),
    m_attValue(),
    m_defaultDecl(),
    m_attDef(),
    m_end(),
    m_rule()
{
    assert( this != NULL );

    const CommonParserRules & commonRules = CommonParserRules::GetIt();
    Parser::MessageStack & messages = m_stacks.m_messages;
    NameParser & nameParser = enumTypeParser.GetNameParser();

    m_start = ( str_p( "<!ATTLIST" ) >> commonRules.m_whiteSpaces )
        [ FClear( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Major,
            "Did not find name for attribute declaration." ) ];

    m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Attribute declaration has invalid format." ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ FSetValidSyntax( this, false ) ];

    m_name = nameParser.GetRule()
        [ FSetName( this ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find content for attribute declaration." ) ];

    m_attName = nameParser.GetRule()
        [ FSetAttName( this ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find attribute type within declaration." ) ];

    m_tokenizedType = ( longest_d[
        str_p("ID") | "IDREF"   | "IDREFS"
                    | "ENTITY"  | "ENTITIES"
                    | "NMTOKEN" | "NMTOKENS"
                    | "CDATA" ] )
        [ FSetAttType( this ) ];

    m_preEnumeration = epsilon_p
        [ FPreEnumeration( this ) ];

    m_enumeratedType = ( m_preEnumeration >> enumTypeParser.GetRule() )
        [ FDoneEnumType( this ) ];

    m_attType = ( m_tokenizedType | m_enumeratedType )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find default declaration for attribute declaration." ) ];

    m_defaultDeclType = ( str_p( "#REQUIRED" ) | "#IMPLIED" )
        [ FSetDefaultDeclType( this ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find ending brace for attribute declaration." ) ];

    m_defaultDeclFixed = ( str_p( "#FIXED" ) >> commonRules.m_whiteSpaces )
        [ FSetDefaultDeclType( this ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find value for attribute declaration." ) ];

    m_preAttValue = epsilon_p
        [ FPreAttValue( this ) ];

    m_attValue = ( m_preAttValue >> attValueParser.GetRule() )
        [ FDoneAttValue( this ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Major,
            "Did not find ending brace for attribute declaration." ) ];

    m_defaultDecl = ( m_defaultDeclType | ( !m_defaultDeclFixed >> m_attValue ) );

    m_attDef = ( commonRules.m_whiteSpaces >> m_attName
        >> commonRules.m_whiteSpaces >> m_attType
        >> commonRules.m_whiteSpaces >> m_defaultDecl );

    m_end = ( !commonRules.m_whiteSpaces >> '>' )
        [ Parser::FCancelMessage( messages ) ]
        [ FSetValidSyntax( this, true ) ];

    m_rule = ( m_start >> ( ( m_name >> *m_attDef >> m_end ) | m_skipOver ) )
        [ FDone( this ) ];
}

// ----------------------------------------------------------------------------

AttListDeclParser::~AttListDeclParser( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void AttListDeclParser::Clear( void )
{
    assert( this != NULL );
    m_validSyntax = false;
    m_validContent = true;
    m_stackSize = m_stacks.m_messages.GetStackSize();
}

// ----------------------------------------------------------------------------

void AttListDeclParser::SetName( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );

    if ( m_receiver == NULL )
        return;
    bool keep = false;
    try
    {
        keep = m_receiver->SetName( begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void AttListDeclParser::SetAttName( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );

    if ( m_receiver == NULL )
        return;
    bool keep = false;
    try
    {
        keep = m_receiver->SetAttName( begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void AttListDeclParser::SetAttType( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    (void)end;

    if ( m_receiver == NULL )
        return;

    const IAttListDeclReceiver::AttType attType = GetAttDeclType( begin );
    bool keep = false;
    try
    {
        keep = m_receiver->SetAttType( attType );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void AttListDeclParser::SetDefaultDeclType( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    (void)end;

    if ( m_receiver == NULL )
        return;

    const IAttListDeclReceiver::DefaultDeclType type = GetDefaultDeclType( begin );
    bool keep = false;
    try
    {
        keep = m_receiver->SetDefaultDeclType( type );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void AttListDeclParser::AddNotateName( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );

    if ( m_receiver == NULL )
        return;

    bool keep = false;
    try
    {
        keep = m_receiver->AddNotateName( begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( !keep )
        SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void AttListDeclParser::PreAttValue( void )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( m_receiver == NULL )
        return;
    try
    {
        IAttributeValueReceiver * receiver = m_receiver->AddAttributeValue();
        m_attValueParser.SetReceiver( receiver );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
}

// ----------------------------------------------------------------------------

void AttListDeclParser::DoneAttValue( void )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( !m_attValueParser.IsValid() )
        m_validContent = false;
}

// ----------------------------------------------------------------------------

void AttListDeclParser::PreEnumeration( void )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( m_receiver == NULL )
        return;
    try
    {
        IEnumeratedTypeReceiver * receiver = m_receiver->AddEnumeratedType();
        m_enumTypeParser.SetReceiver( receiver );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
}

// ----------------------------------------------------------------------------

void AttListDeclParser::DoneEnumType( void )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( !m_enumTypeParser.IsValid() )
        m_validContent = false;
}

// ----------------------------------------------------------------------------

void AttListDeclParser::Done( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    if ( IsValid() )
    {
        assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
    }

    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->DoneAttListDecl( IsValid(), begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

}; // end namespace Xml

}; // end namespace Parser

// $Log: PrologParsers.cpp,v $
// Revision 1.1  2008/12/05 19:04:16  rich_sposato
// Adding xml parsing files to project.
//
