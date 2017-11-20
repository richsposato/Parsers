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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/src/CommonInfo.hpp,v 1.1 2008/12/05 19:04:16 rich_sposato Exp $


#ifndef PARSER_XML_COMMON_INFO_H_INCLUDED
#define PARSER_XML_COMMON_INFO_H_INCLUDED


// ----------------------------------------------------------------------------

#include "../../Util/include/ParseInfo.hpp"
#include "../../Util/include/ParseUtil.hpp"


namespace Parser
{

namespace Xml
{


// ----------------------------------------------------------------------------

template< class ParserClass >
struct FSetName
{
    inline explicit FSetName( ParserClass * pParser ) : m_pParser( pParser ) {}
    inline void operator () ( const ::Parser::CharType * begin,
        const ::Parser::CharType * end ) const
    {
        m_pParser->SetName( begin, end );
    }
    inline void operator () ( ::Parser::CharType ch ) const
    {
        m_pParser->SetName( ch );
    }
    ParserClass * m_pParser;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FSetValue
{
    inline explicit FSetValue( ParserClass * pParser ) :
        m_pParser( pParser ) {}
    inline void operator () ( const Parser::CharType * begin,
        const Parser::CharType * end ) const
    {
        m_pParser->SetValue( begin, end );
    }
    inline void operator () ( ::Parser::CharType ch ) const
    {
        m_pParser->SetValue( ch );
    }
    ParserClass * m_pParser;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FSetReference
{
    inline explicit FSetReference( ParserClass * pParser ) : m_pParser( pParser ) {}
    inline void operator () ( const ::Parser::CharType * begin,
        const ::Parser::CharType * end ) const
    {
        m_pParser->SetReference( begin, end );
    }
    inline void operator () ( ::Parser::CharType ch ) const
    {
        m_pParser->SetReference( ch );
    }
    ParserClass * m_pParser;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FSetQuoteType
{
    inline explicit FSetQuoteType( ParserClass * pParser ) :
        m_pParser( pParser ) {}
    inline void operator () ( const ::Parser::CharType * begin,
        const ::Parser::CharType * end ) const
    {
        m_pParser->SetQuoteType( begin, end );
    }
    inline void operator () ( const ::Parser::CharType ch ) const
    {
        m_pParser->SetQuoteType( ch );
    }
    ParserClass * m_pParser;
};

// ----------------------------------------------------------------------------

class ParserStacks
{
public:

    explicit ParserStacks( Parser::IStackMessagePreparer * handler );
    ~ParserStacks( void );

    Parser::MessageStack m_messages;
    Parser::ContentStack m_content;

private:
    ParserStacks( void );
    ParserStacks( const ParserStacks & );
    ParserStacks & operator = ( const ParserStacks & );
};

// ----------------------------------------------------------------------------

class CommonParserRules
{
public:

    static void IncReference( void );
    static void DecReference( void );

    static const CommonParserRules & GetIt( void ) { return *s_instance; }

    const SpiritCharLit m_singleQuote;
    const SpiritCharLit m_doubleQuote;

    const SpiritCharSet m_char;
    const SpiritCharSet m_charNotDash;
    const SpiritCharSet m_whiteSpace;
    const SpiritCharSet m_letter;
    const SpiritCharSet m_digit;
    const SpiritCharSet m_hexDigit;
    const SpiritCharSet m_extender;
    const SpiritCharSet m_firstNameChar;
    const SpiritCharSet m_nameChar;
    const SpiritCharSet m_pubidChar;
    const SpiritCharSet m_versionChars;

    const SpiritRule m_whiteSpaces;
    const SpiritRule m_equals;
    const SpiritRule m_name;
    const SpiritRule m_nameToken;
    const SpiritRule m_encName;
    const SpiritRule m_charRef;
    const SpiritRule m_entityRef;

private:
    CommonParserRules( void );
    ~CommonParserRules( void );

    CommonParserRules( const CommonParserRules & );
    CommonParserRules & operator = ( const CommonParserRules & );

    static CommonParserRules * s_instance;
    static unsigned long s_count;

}; // end class CommonParserRules

// ----------------------------------------------------------------------------

}; // end namespace Xml

}; // end namespace Parser

#endif // file guardian

// $Log: CommonInfo.hpp,v $
// Revision 1.1  2008/12/05 19:04:16  rich_sposato
// Adding xml parsing files to project.
//
