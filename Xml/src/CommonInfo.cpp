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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/src/CommonInfo.cpp,v 1.1 2008/12/05 19:04:16 rich_sposato Exp $


// ----------------------------------------------------------------------------

#include "./CommonInfo.hpp"


using namespace std;
using namespace boost::spirit;

namespace Parser
{

namespace Xml
{


// ----------------------------------------------------------------------------

ParserStacks::ParserStacks( Parser::IStackMessagePreparer * handler ) :
    m_messages( handler ),
    m_content( handler )
{
}

// ----------------------------------------------------------------------------

ParserStacks::~ParserStacks( void )
{
}

// ----------------------------------------------------------------------------

CommonParserRules * CommonParserRules::s_instance = NULL;
unsigned long CommonParserRules::s_count = 0;

// ----------------------------------------------------------------------------

void CommonParserRules::IncReference( void )
{
    if ( s_count == 0 )
    {
        assert( s_instance == NULL );
        s_instance = new CommonParserRules;
    }
    s_count++;
}

// ----------------------------------------------------------------------------

void CommonParserRules::DecReference( void )
{
    s_count--;
    if ( s_count == 0 )
    {
        assert( s_instance != NULL );
        delete s_instance;
    }
}

// ----------------------------------------------------------------------------

CommonParserRules::CommonParserRules( void ) :
    m_singleQuote( '\'' ),
    m_doubleQuote( '\"' ),
    m_char( "\x9\xA\xD\x20-\xFF" ),
    m_charNotDash( "\x9\xA\xD\x20-\x2C\x2E-\xFF" ),
    m_whiteSpace( " \t\r\n" ),
    m_letter( "a-zA-Z\xC0-\xD6\xD8-\xF6\xF8-\xFF" ),
    m_digit( "0-9" ),
    m_hexDigit( "0-9a-fA-F" ),
    m_extender( "\xB7" ),
    m_firstNameChar( m_letter | ch_p( '_' ) | ch_p( ':' ) ),
    m_nameChar( m_letter | m_digit | m_extender
        | (Parser::CharType)'.' | (Parser::CharType)'-'
        | (Parser::CharType)'_' | (Parser::CharType)':' ),
    m_pubidChar( "\x20\xD\xA'a-zA-Z0-9()+,./:=?;!*#@$_%-" ),
    m_versionChars( "a-zA-Z0-9_.:-" ),
    m_whiteSpaces( +m_whiteSpace ),
    m_equals( !m_whiteSpaces >> '=' >> !m_whiteSpaces ),
    m_name( ( m_letter | ch_p( '_' ) | ch_p( ':' ) ) >> *( m_nameChar ) ),
    m_nameToken( +m_nameChar ),
    m_encName( alpha_p >> *( alnum_p | '.' | '_' | '-' ) ),
    m_charRef( ( "&#"  >> +m_digit  >> ';' ) | ( "&#x" >> +m_hexDigit >> ';' ) ),
    m_entityRef( '&' >> m_name >> ';' )
{
    assert( this != NULL );
    assert( s_instance == NULL );
}

// ----------------------------------------------------------------------------

CommonParserRules::~CommonParserRules( void )
{
    assert( this != NULL );
    assert( s_instance == this );
    s_instance = NULL;
}

// ----------------------------------------------------------------------------

}; // end namespace Xml

}; // end namespace Parser

// $Log: CommonInfo.cpp,v $
// Revision 1.1  2008/12/05 19:04:16  rich_sposato
// Adding xml parsing files to project.
//
