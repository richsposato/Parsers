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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/src/BasicParsers.hpp,v 1.1 2008/12/05 19:04:16 rich_sposato Exp $


#ifndef PARSER_XML_BASIC_PARSERS_H_INCLUDED
#define PARSER_XML_BASIC_PARSERS_H_INCLUDED


// ----------------------------------------------------------------------------

#include "../../Util/include/ParseInfo.hpp"
#include "../../Util/include/ParseUtil.hpp"

#include "../include/Receivers.hpp"

#include "./CommonInfo.hpp"


namespace Parser
{

namespace Xml
{


// ----------------------------------------------------------------------------

class CommentParser
{
public:

    explicit CommentParser( ParserStacks & stacks );
    ~CommentParser( void );

    inline void SetReceiver( ::Parser::Xml::ICommentReceiver * receiver )
    {
        m_receiver = receiver;
    }

    inline bool IsValid( void ) const { return m_validSyntax; }

    inline const SpiritRule & GetRule( void ) const
    {
        return m_rule;
    }

    inline ParserStacks & GetStacks( void )
    {
        return m_stacks;
    }

    const ::Parser::CharType * GetBegin( void ) const { return m_pBegin; }
    const ::Parser::CharType * GetEnd( void ) const { return m_pEnd; }

private:

    typedef ::Parser::FStoreStackSize< CommentParser > FStoreStackSize;
    typedef ::Parser::FBreakPoint< CommentParser > FBreakPoint;
    typedef ::Parser::FSetValidSyntax< CommentParser > FSetValidSyntax;
    typedef ::Parser::FSetContent< CommentParser > FSetContent;
    typedef ::Parser::FClear< CommentParser > FClear;
    typedef ::Parser::FDone< CommentParser > FDone;

    friend struct ::Parser::FStoreStackSize< CommentParser >;
    friend struct ::Parser::FSetValidSyntax< CommentParser >;
    friend struct ::Parser::FSetContent< CommentParser >;
    friend struct ::Parser::FClear< CommentParser >;
    friend struct ::Parser::FDone< CommentParser >;

    CommentParser( const CommentParser & );
    CommentParser & operator = ( const CommentParser & );

    inline void Clear( void )
    {
        m_validSyntax = false;
    }

    void StoreStackSize( void );

    void Done( const Parser::CharType * begin,
        const Parser::CharType * end );

    inline void SetValidSyntax( bool valid )
    {
        m_validSyntax = valid;
    }

    inline void SetContent( const Parser::CharType * begin,
        const Parser::CharType * end )
    {
        m_pBegin = begin;
        m_pEnd = end;
    }

    ParserStacks & m_stacks;
    ::Parser::Xml::ICommentReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;

    const Parser::CharType * m_pBegin;
    const Parser::CharType * m_pEnd;

    SpiritRule m_start;
    SpiritRule m_skipOver;
    SpiritRule m_char;
    SpiritRule m_content;
    SpiritRule m_end;
    SpiritRule m_rule;

}; // end class CommentParser

// ----------------------------------------------------------------------------

class NameParser
{
public:

    explicit NameParser( ParserStacks & stacks );
    ~NameParser( void );

    inline void SetReceiver( ::Parser::Xml::INameReceiver * receiver )
    {
        m_receiver = receiver;
    }

    inline bool IsValid( void ) const { return m_validSyntax; }

    inline const SpiritRule & GetRule( void ) const
    {
        return m_name;
    }

    inline ParserStacks & GetStacks( void )
    {
        return m_stacks;
    }

private:

    typedef ::Parser::FStoreStackSize< NameParser > FStoreStackSize;
    typedef ::Parser::FBreakPoint< NameParser > FBreakPoint;
    typedef ::Parser::FSetValidSyntax< NameParser > FSetValidSyntax;
    typedef ::Parser::FClear< NameParser > FClear;
    typedef ::Parser::Xml::FSetName< NameParser > FSetName;

    friend struct ::Parser::FStoreStackSize< NameParser >;
    friend struct ::Parser::FSetValidSyntax< NameParser >;
    friend struct ::Parser::FClear< NameParser >;
    friend struct ::Parser::Xml::FSetName< NameParser >;

    NameParser( const NameParser & );
    NameParser & operator = ( const NameParser & );

    inline void Clear( void )
    {
        m_validSyntax = false;
    }

    void StoreStackSize( void );

    inline void SetValidSyntax( bool valid )
    {
        m_validSyntax = valid;
    }

    void SetName( const Parser::CharType * begin,
        const Parser::CharType * end );

    ParserStacks & m_stacks;
    ::Parser::Xml::INameReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;

    SpiritRule m_start;
    SpiritRule m_firstChar;
    SpiritRule m_nextChars;
    SpiritRule m_skipOver;
    SpiritRule m_goodName;
    SpiritRule m_name;

}; // end class NameParser

// ----------------------------------------------------------------------------

class ReferenceParser
{
public:

    explicit ReferenceParser( NameParser & nameParser );
    ~ReferenceParser( void );

    inline bool IsValid( void ) const { return m_validSyntax; }

    inline const SpiritRule & GetRule( void ) const
    {
        return m_rule;
    }

    inline ParserStacks & GetStacks( void )
    {
        return m_stacks;
    }

    inline Parser::Xml::IReferenceReceiver::RefType GetRefType( void ) const
    {
        return m_refType;
    }

    inline void SetReceiver( Parser::Xml::IReferenceReceiver * receiver )
    {
        m_receiver = receiver;
    }

    inline Parser::Xml::IReferenceReceiver * GetReceiver( void ) { return m_receiver; }

    const Parser::CharType * GetBeginRef( void ) const { return m_pBegin; }
    const Parser::CharType * GetEndOfRef( void ) const { return m_pEnd; }

private:

    typedef ::Parser::FBreakPoint< ReferenceParser > XmlRefParserBreaker;
    typedef ::Parser::FCompareStackSize< ReferenceParser > FCompareStackSize;
    typedef ::Parser::FSetValidSyntax< ReferenceParser > FSetValidSyntax;
    typedef ::Parser::FClear< ReferenceParser > FClear;
    typedef ::Parser::FDone< ReferenceParser > FDone;
    typedef ::Parser::Xml::FSetReference< ReferenceParser > FSetReference;

    friend struct ::Parser::FCompareStackSize< ReferenceParser >;
    friend struct ::Parser::FSetValidSyntax< ReferenceParser >;
    friend struct ::Parser::FClear< ReferenceParser >;
    friend struct ::Parser::FDone< ReferenceParser >;
    friend struct ::Parser::Xml::FSetReference< ReferenceParser >;

    ReferenceParser( const ReferenceParser & );
    ReferenceParser & operator = ( const ReferenceParser & );

    void Clear( void );

    void Done( const Parser::CharType * begin, const Parser::CharType * end );

    inline void SetValidSyntax( bool valid )
    {
        m_validSyntax = valid;
    }

    inline void SetRefType( ::Parser::Xml::IReferenceReceiver::RefType refType )
    {
        m_refType = refType;
    }

    void SetReference( const Parser::CharType * begin,
        const Parser::CharType * end );

    struct FSetRefType
    {
        inline FSetRefType( ReferenceParser * pParser,
            Parser::Xml::IReferenceReceiver::RefType refType ) :
            m_pParser( pParser ), m_refType( refType ) {}
        inline void operator () ( const ::Parser::CharType *,
            const ::Parser::CharType * ) const
        {
            m_pParser->SetRefType( m_refType );
        }
        inline void operator () ( ::Parser::CharType ) const
        {
            m_pParser->SetRefType( m_refType );
        }
        ReferenceParser * m_pParser;
        Parser::Xml::IReferenceReceiver::RefType m_refType;
    };

    ParserStacks & m_stacks;
    unsigned int m_stackSize;
    bool m_validSyntax;
    Parser::Xml::IReferenceReceiver::RefType m_refType;
    Parser::Xml::IReferenceReceiver * m_receiver;

    const Parser::CharType * m_pBegin;
    const Parser::CharType * m_pEnd;

    SpiritRule m_start;
    SpiritRule m_skipOver;
    SpiritRule m_endRef;
    SpiritRule m_beginDecDigitRef;
    SpiritRule m_middleDecDigitRef;
    SpiritRule m_decDigitRef;
    SpiritRule m_beginHexDigitRef;
    SpiritRule m_middleHexDigitRef;
    SpiritRule m_hexDigitRef;
    SpiritRule m_nameRef;
    SpiritRule m_entityRef;
    SpiritRule m_rule;

}; // end class ReferenceParser

// ----------------------------------------------------------------------------

class AttributeValueParser
{
public:

    explicit AttributeValueParser( ReferenceParser & refParser );
    ~AttributeValueParser( void );

    inline void SetReceiver( ::Parser::Xml::IAttributeValueReceiver * receiver )
    {
        m_receiver = receiver;
    }

    inline bool IsValid( void ) const { return ( m_validSyntax && m_validContent ); }

    inline const SpiritRule & GetRule( void ) const
    {
        return m_rule;
    }

    inline ParserStacks & GetStacks( void )
    {
        return m_stacks;
    }

private:

    typedef ::Parser::FBreakPoint< AttributeValueParser > FBreakPoint;
    typedef ::Parser::FSetValidSyntax< AttributeValueParser > FSetValidSyntax;
    typedef ::Parser::FSetValidContent< AttributeValueParser > FSetValidContent;
    typedef ::Parser::FDone< AttributeValueParser > FDone;
    typedef ::Parser::Xml::FSetReference< AttributeValueParser > FSetReference;
    typedef ::Parser::Xml::FSetQuoteType< AttributeValueParser > FSetQuoteType;
    typedef ::Parser::Xml::FSetValue< AttributeValueParser > FSetValue;

    friend struct ::Parser::FSetValidContent< AttributeValueParser >;
    friend struct ::Parser::FSetValidSyntax< AttributeValueParser >;
    friend struct ::Parser::FDone< AttributeValueParser >;
    friend struct ::Parser::Xml::FSetReference< AttributeValueParser >;
    friend struct ::Parser::Xml::FSetQuoteType< AttributeValueParser >;
    friend struct ::Parser::Xml::FSetValue< AttributeValueParser >;

    AttributeValueParser( const AttributeValueParser & );
    AttributeValueParser & operator = ( const AttributeValueParser & );

    void Done( const Parser::CharType * begin, const Parser::CharType * end );

    inline void SetValidSyntax( bool valid )
    {
        m_validSyntax = valid;
    }

    inline void SetValidContent( bool valid )
    {
        m_validContent = valid;
    }

    void SetQuoteType( char quote );

    void SetValue( const Parser::CharType * begin, const Parser::CharType * end );

    void SetReference( const Parser::CharType * begin, const Parser::CharType * end );

    ReferenceParser & m_refParser;
    ParserStacks & m_stacks;
    ::Parser::Xml::IAttributeValueReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;
    bool m_validContent;
    bool m_singleQuoted;

    SpiritRule m_skipOver;
    SpiritRule m_sqStart;
    SpiritRule m_dqStart;
    SpiritRule m_reference;
    SpiritRule m_sqValue;
    SpiritRule m_dqValue;
    SpiritRule m_sqContent;
    SpiritRule m_dqContent;
    SpiritRule m_sqEnd;
    SpiritRule m_dqEnd;
    SpiritRule m_sqEntity;
    SpiritRule m_dqEntity;
    SpiritRule m_rule;

}; // end class AttributeValueParser

// ----------------------------------------------------------------------------

class AttributeParser
{
public:

    AttributeParser( NameParser & nameParser, AttributeValueParser & valueParser );
    ~AttributeParser( void );

    inline void SetReceiver( ::Parser::Xml::IAttributeReceiver * receiver )
    {
        m_receiver = receiver;
    }

    inline bool IsValid( void ) const { return ( m_validSyntax && m_validContent ); }

    inline const SpiritRule & GetRule( void ) const
    {
        return m_rule;
    }

    inline ParserStacks & GetStacks( void )
    {
        return m_stacks;
    }

private:

    typedef ::Parser::FBreakPoint< AttributeParser > FBreakPoint;
    typedef ::Parser::FSetValidSyntax< AttributeParser > FSetValidSyntax;
    typedef ::Parser::FSetValidContent< AttributeParser > FSetValidContent;
    typedef ::Parser::FDone< AttributeParser > FDone;
    typedef ::Parser::Xml::FSetReference< AttributeParser > FSetReference;
    typedef ::Parser::Xml::FSetQuoteType< AttributeParser > FSetQuoteType;
    typedef ::Parser::Xml::FSetValue< AttributeParser > FSetValue;
    typedef ::Parser::Xml::FSetName< AttributeParser > FSetName;

    friend struct ::Parser::FSetValidContent< AttributeParser >;
    friend struct ::Parser::FSetValidSyntax< AttributeParser >;
    friend struct ::Parser::FDone< AttributeParser >;
    friend struct ::Parser::Xml::FSetReference< AttributeParser >;
    friend struct ::Parser::Xml::FSetQuoteType< AttributeParser >;
    friend struct ::Parser::Xml::FSetValue< AttributeParser >;
    friend struct ::Parser::Xml::FSetName< AttributeParser >;

    AttributeParser( const AttributeParser & );
    AttributeParser & operator = ( const AttributeParser & );

    void Done( const Parser::CharType * begin, const Parser::CharType * end );

    inline void SetValidSyntax( bool valid )
    {
        m_validSyntax = valid;
    }

    inline void SetValidContent( bool valid )
    {
        m_validContent = valid;
    }

    inline void SetQuoteType( char quote )
    {
        m_singleQuoted = ( '\'' == quote );
    }

    void SetName( const Parser::CharType * begin, const Parser::CharType * end );

    AttributeValueParser & m_valueParser;
    NameParser & m_nameParser;
    ParserStacks & m_stacks;
    ::Parser::Xml::IAttributeReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;
    bool m_validContent;
    bool m_singleQuoted;

    SpiritRule m_skipOver;
    SpiritRule m_name;
    SpiritRule m_equals;
    SpiritRule m_value;
    SpiritRule m_attribute;
    SpiritRule m_rule;

}; // end class AttributeParser

// ----------------------------------------------------------------------------

}; // end namespace Xml

}; // end namespace Parser

#endif // file guardian

// $Log: BasicParsers.hpp,v $
// Revision 1.1  2008/12/05 19:04:16  rich_sposato
// Adding xml parsing files to project.
//
