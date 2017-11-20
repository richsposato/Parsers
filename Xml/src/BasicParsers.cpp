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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/src/BasicParsers.cpp,v 1.1 2008/12/05 19:04:16 rich_sposato Exp $


// ----------------------------------------------------------------------------

#include "./BasicParsers.hpp"

#include "../include/Receivers.hpp"


using namespace std;
using namespace boost::spirit;

namespace Parser
{

namespace Xml
{


// ----------------------------------------------------------------------------

CommentParser::CommentParser( ParserStacks & stacks ) :
    m_stacks( stacks ),
    m_receiver( NULL ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_pBegin( NULL ),
    m_pEnd( NULL ),
    m_start(),
    m_skipOver(),
    m_char(),
    m_content(),
    m_end(),
    m_rule()
{
    assert( this != NULL );

    const CommonParserRules & commonRules = CommonParserRules::GetIt();
    Parser::MessageStack & messages = stacks.m_messages;

    m_start = str_p( "<!--" )
        [ FClear( this ) ]
        [ FStoreStackSize( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Minor,
            "Found start of comment, but not end of comment." ) ];

    m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Minor,
            "Comment has invalid format." ) ]
        [ Parser::FPopMessageStack( messages ) ];

    m_char = ( ( print_p - '-' ) | commonRules.m_whiteSpace );

    m_content = ( *( m_char | ( '-' >> m_char ) ) )
        [ FSetContent( this ) ];

    m_end = str_p( "-->" )
        [ Parser::FCancelMessage( messages ) ]
        [ FDone( this ) ];

    m_rule = ( m_start >> ( ( m_content >> m_end ) | m_skipOver ) );
}

// ----------------------------------------------------------------------------

CommentParser::~CommentParser( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void CommentParser::StoreStackSize( void )
{
    assert( this != NULL );
    m_stackSize = m_stacks.m_messages.GetStackSize();
}

// ----------------------------------------------------------------------------

void CommentParser::Done( const Parser::CharType * begin,
        const Parser::CharType * end )
{
    assert( this != NULL );
    (void)begin;
    (void)end;
    assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
    SetValidSyntax( true );
    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->AddComment( m_pBegin, m_pEnd );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

NameParser::NameParser( ParserStacks & stacks ) :
    m_stacks( stacks ),
    m_receiver( NULL ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_start(),
    m_firstChar(),
    m_nextChars(),
    m_skipOver(),
    m_goodName(),
    m_name()
{
    assert( this != NULL );

    const CommonParserRules & commonRules = CommonParserRules::GetIt();
    Parser::MessageStack & messages = stacks.m_messages;

    m_start = ( epsilon_p )
        [ FClear( this ) ];

    m_firstChar = ( commonRules.m_firstNameChar )
        [ FStoreStackSize( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Minor,
            "Name has invalid first character." ) ];

    m_nextChars = ( *( commonRules.m_nameChar ) )
        [ Parser::FPrepareMessage( messages,
            Parser::ErrorLevel::Minor, "Could not parse rest of name." ) ];

    m_goodName = ( m_firstChar >> m_nextChars )
        [ Parser::FCancelMessage( messages ) ]
        [ FSetName( this ) ];

    m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Minor,
            "Name has an invalid character." ) ]
        [ Parser::FPopMessageStack( messages ) ];

    m_name = ( m_start >> ( m_goodName | m_skipOver ) );
}

// ----------------------------------------------------------------------------

NameParser::~NameParser()
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void NameParser::StoreStackSize( void )
{
    assert( this != NULL );
    m_stackSize = m_stacks.m_messages.GetStackSize();
}

// ----------------------------------------------------------------------------

void NameParser::SetName( const Parser::CharType * begin, const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
    m_validSyntax = true;
    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->SetName( begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

ReferenceParser::ReferenceParser( NameParser & nameParser ) :
    m_stacks( nameParser.GetStacks() ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_refType( Parser::Xml::IReferenceReceiver::Unknown ),
    m_receiver( NULL ),
    m_pBegin( NULL ),
    m_pEnd( NULL ),
    m_start(),
    m_skipOver(),
    m_beginDecDigitRef(),
    m_middleDecDigitRef(),
    m_decDigitRef(),
    m_beginHexDigitRef(),
    m_middleHexDigitRef(),
    m_hexDigitRef(),
    m_nameRef(),
    m_entityRef(),
    m_rule()
{
    assert( this != NULL );

    const CommonParserRules & commonRules = CommonParserRules::GetIt();
    Parser::MessageStack & messages = m_stacks.m_messages;

    m_start = ( ch_p( '&' ) )
        [ FClear( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Minor,
            "Found start of reference, but not rest of its content.") ];

    m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Minor,
            "Unable to parse reference - skipping rest of content." ) ];

    m_endRef = ( ch_p( ';' ) )
        [ FSetValidSyntax( this, true ) ]
        [ Parser::FCancelMessage( messages ) ];

    m_beginDecDigitRef = ( ch_p( '#' ) )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Found '&#' for start of digit reference, but no digits.") ];

    m_middleDecDigitRef = ( +commonRules.m_digit )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Found numbers for digits reference but no ending semicolon.") ];

    m_decDigitRef = ( m_beginDecDigitRef >> m_middleDecDigitRef )
        [ FSetRefType( this, Parser::Xml::IReferenceReceiver::Digits ) ]
        [ FSetReference( this ) ];

    m_beginHexDigitRef = ( str_p( "#x" ) )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Found '&#x' for start of hexdigit reference, but no hexdigits.") ];

    m_middleHexDigitRef = ( +commonRules.m_hexDigit )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Found numbers for hexdigit reference but no ending semicolon.") ];

    m_hexDigitRef = ( m_beginHexDigitRef >> m_middleHexDigitRef )
        [ FSetRefType( this, Parser::Xml::IReferenceReceiver::HexDigits ) ]
        [ FSetReference( this ) ];

    m_nameRef = ( nameParser.GetRule() )
        [ FSetRefType( this, Parser::Xml::IReferenceReceiver::Entity ) ]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Found name for entity reference but no ending semicolon.") ];

    m_entityRef = ( ( m_hexDigitRef | m_decDigitRef | m_nameRef ) >> m_endRef )
        [ FSetReference( this ) ];

    m_rule = ( m_start >> ( m_entityRef | m_skipOver ) )
        [ FDone( this ) ];
}

// ----------------------------------------------------------------------------

ReferenceParser::~ReferenceParser( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void ReferenceParser::Clear( void )
{
    assert( this != NULL );
    m_validSyntax = false;
    m_refType = Parser::Xml::IReferenceReceiver::Unknown;
    m_pBegin = NULL;
    m_pEnd = NULL;
    m_stackSize = m_stacks.m_messages.GetStackSize();
}

// ----------------------------------------------------------------------------

void ReferenceParser::SetReference( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    assert( m_refType != Parser::Xml::IReferenceReceiver::Unknown );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );

    m_pBegin = begin;
    m_pEnd = end;
    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->AddReference( m_pBegin, m_pEnd, m_refType );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

void ReferenceParser::Done( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    assert( m_stackSize <= m_stacks.m_messages.GetStackSize() );
    (void)begin;
    (void)end;
    if ( IsValid() )
        assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
}

// ----------------------------------------------------------------------------

AttributeValueParser::AttributeValueParser( ReferenceParser & refParser ) :
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

    m_sqValue = ( +( anychar_p - ( SpiritCharSet( "%&'" ) ) ) )
//        [ FBreakPoint( this, "m_sqValue" )]
        [ FSetValue( this ) ];

    m_dqValue = ( +( anychar_p - ( SpiritCharSet( "%&\"" ) ) ) )
//        [ FBreakPoint( this, "m_dqValue" )]
        [ FSetValue( this ) ];

    m_sqContent = ( *( m_reference | m_sqValue ) )
//        [ FBreakPoint( this, "m_sqContent" )]
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Entity value has no ending single-quote." ) ];

    m_dqContent = ( *( m_reference | m_dqValue ) )
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

AttributeValueParser::~AttributeValueParser( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void AttributeValueParser::SetQuoteType( char quote )
{
    assert( this != NULL );
    m_validSyntax = false;
    m_validContent = true;
    m_singleQuoted = ( '\'' == quote );
//    m_refParser.SetReceiver( NULL );
    m_stackSize = m_stacks.m_messages.GetStackSize();
}

// ----------------------------------------------------------------------------

void AttributeValueParser::SetValue( const Parser::CharType * begin,
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

void AttributeValueParser::SetReference( const Parser::CharType * begin,
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

void AttributeValueParser::Done( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    SetValidSyntax( true );
    assert( m_stackSize == m_stacks.m_messages.GetStackSize() );
    if ( m_receiver == NULL )
        return;
    try
    {
        m_receiver->DoneAttributeValue( IsValid(), m_singleQuoted, begin, end );
    }
    catch ( ... )
    {
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    SetReceiver( NULL );
}

// ----------------------------------------------------------------------------

AttributeParser::AttributeParser( NameParser & nameParser,
    AttributeValueParser & valueParser ) :
    m_valueParser( valueParser ),
    m_nameParser( nameParser ),
    m_stacks( nameParser.GetStacks() ),
    m_receiver( NULL ),
    m_stackSize( 0 ),
    m_validSyntax( false ),
    m_validContent( false ),
    m_singleQuoted( false ),
    m_skipOver(),
    m_name(),
    m_equals(),
    m_value(),
    m_attribute(),
    m_rule()
{
    assert( this != NULL );

    const CommonParserRules & commonRules = CommonParserRules::GetIt();
    Parser::MessageStack & messages = m_stacks.m_messages;

    m_name = ( nameParser.GetRule() )
        [ FSetName( this ) ]
        [ Parser::FPushMessage( messages, Parser::ErrorLevel::Minor,
            "Found name but no equal sign for attribute.") ];

    m_equals = ( commonRules.m_equals )
        [ Parser::FPrepareMessage( messages, Parser::ErrorLevel::Minor,
            "Found equal sign but not value in attribute.") ];

    m_skipOver = ( *( ~ch_p( '\0' ) ) >> ch_p( '\0' ) )
        [ FSetValidSyntax( this, false ) ]
        [ Parser::FPopMessageStack( messages ) ]
        [ Parser::FSendMessageNow( messages, Parser::ErrorLevel::Major,
            "Unable to parse attribute - skipping rest of content." ) ];

    m_value = valueParser.GetRule();

    m_attribute = ( m_name >> m_equals >> m_value )
        [ Parser::FCancelMessage( messages ) ]
        [ FSetValidSyntax( this, true ) ]
        [ FDone( this ) ];

    m_rule = ( m_attribute | m_skipOver );
}

// ----------------------------------------------------------------------------

AttributeParser::~AttributeParser()
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void AttributeParser::SetName( const Parser::CharType * begin,
    const Parser::CharType * end )
{
    assert( this != NULL );
    m_validSyntax = false;
    m_validContent = true;
    m_singleQuoted = false;
    m_nameParser.SetReceiver( NULL );
    m_stackSize = m_stacks.m_messages.GetStackSize();
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
    else
        m_valueParser.SetReceiver( m_receiver );
}

// ----------------------------------------------------------------------------

void AttributeParser::Done( const Parser::CharType * begin,
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
        m_receiver->DoneAttributeValue( IsValid(), m_singleQuoted, begin, end );
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

// $Log: BasicParsers.cpp,v $
// Revision 1.1  2008/12/05 19:04:16  rich_sposato
// Adding xml parsing files to project.
//
