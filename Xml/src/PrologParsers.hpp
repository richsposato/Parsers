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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/src/PrologParsers.hpp,v 1.2 2009/01/05 19:26:36 rich_sposato Exp $


#ifndef PARSER_XML_PROLOG_PARSERS_H_INCLUDED
#define PARSER_XML_PROLOG_PARSERS_H_INCLUDED


// ----------------------------------------------------------------------------

#include "../../Util/include/ParseInfo.hpp"
#include "../../Util/include/ParseUtil.hpp"

#include "./CommonInfo.hpp"

namespace Parser
{

namespace Xml
{

    class IPublicIdReceiver;
    class IExternalIdReceiver;
    class IEncodingDeclReceiver;
    class IPeReferenceReceiver;
    class IEnumeratedTypeReceiver;
    class IEntityValueReceiver;
    class IXmlDeclarationReceiver;
    class IAttListDeclReceiver;

    class NameParser;
    class ReferenceParser;
    class AttributeValueParser;


// ----------------------------------------------------------------------------

class PublicIdLiteralParser
{
public:

    explicit PublicIdLiteralParser( ParserStacks & stacks );
    ~PublicIdLiteralParser( void );

    inline void SetReceiver( ::Parser::Xml::IPublicIdReceiver * receiver )
    {
        m_receiver = receiver;
    }

    inline bool IsValid( void ) const { return m_validSyntax; }

    inline bool IsSingleQuoted( void ) const { return m_singleQuoted; }

    inline const SpiritRule & GetRule( void ) const
    {
        return m_rule;
    }

    inline ParserStacks & GetStacks( void )
    {
        return m_stacks;
    }

    const Parser::CharType * GetBegin( void ) const { return m_pBegin; }
    const Parser::CharType * GetEnd( void ) const { return m_pEnd; }

private:

    typedef ::Parser::FBreakPoint< PublicIdLiteralParser > FBreakPoint;
    typedef ::Parser::FStoreStackSize< PublicIdLiteralParser > FStoreStackSize;
    typedef ::Parser::FSetValidSyntax< PublicIdLiteralParser > FSetValidSyntax;
    typedef ::Parser::FSetContent< PublicIdLiteralParser > FSetContent;
    typedef ::Parser::FEnd< PublicIdLiteralParser > FEnd;
    typedef ::Parser::Xml::FSetQuoteType< PublicIdLiteralParser > FSetQuoteType;

    friend struct ::Parser::FStoreStackSize< PublicIdLiteralParser >;
    friend struct ::Parser::FSetValidSyntax< PublicIdLiteralParser >;
    friend struct ::Parser::FSetContent< PublicIdLiteralParser >;
    friend struct ::Parser::FEnd< PublicIdLiteralParser >;
    friend struct ::Parser::Xml::FSetQuoteType< PublicIdLiteralParser >;

    PublicIdLiteralParser( const PublicIdLiteralParser & );
    PublicIdLiteralParser & operator = ( const PublicIdLiteralParser & );

    void SetQuoteType( const Parser::CharType quote );

    void StoreStackSize( void );

    void End( void );

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
    ::Parser::Xml::IPublicIdReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;
    bool m_singleQuoted;

    const Parser::CharType * m_pBegin;
    const Parser::CharType * m_pEnd;

    SpiritRule m_skipOver;
    SpiritRule m_startSQ;
    SpiritRule m_sQuotedChars;
    SpiritRule m_sQuoteOrRef;
    SpiritRule m_endSQ;
    SpiritRule m_skipSQ;
    SpiritRule m_singleQuotedValue;
    SpiritRule m_startDQ;
    SpiritRule m_dQuoteOrRef;
    SpiritRule m_dQuotedChars;
    SpiritRule m_endDQ;
    SpiritRule m_skipDQ;
    SpiritRule m_doubleQuotedValue;
    SpiritRule m_rule;

}; // end class PublicIdLiteralParser

// ----------------------------------------------------------------------------

class ExternalIdLiteralParser
{
public:

    explicit ExternalIdLiteralParser( PublicIdLiteralParser & pubIdParser );
    ~ExternalIdLiteralParser( void );

    inline void SetReceiver( ::Parser::Xml::IExternalIdReceiver * receiver )
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

private:

    typedef ::Parser::FBreakPoint< ExternalIdLiteralParser > FBreakPoint;
    typedef ::Parser::FSetValidSyntax< ExternalIdLiteralParser > FSetValidSyntax;
    typedef ::Parser::FDone< ExternalIdLiteralParser > FDone;
    typedef ::Parser::Xml::FSetQuoteType< ExternalIdLiteralParser > FSetQuoteType;

    friend struct ::Parser::FSetValidSyntax< ExternalIdLiteralParser >;
    friend struct ::Parser::FDone< ExternalIdLiteralParser >;
    friend struct ::Parser::Xml::FSetQuoteType< ExternalIdLiteralParser >;

    struct FSetup
    {
        inline FSetup( ExternalIdLiteralParser * pParser,
            bool isPublicId ) : m_pParser( pParser ), m_isPublicId( isPublicId ) {}
        inline void operator () ( const Parser::CharType *,
            const Parser::CharType * ) const
        {
            m_pParser->Setup( m_isPublicId );
        }
        ExternalIdLiteralParser * m_pParser;
        bool m_isPublicId;
    };

    struct FSetSysLiteral
    {
        inline FSetSysLiteral( ExternalIdLiteralParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const Parser::CharType * begin,
            const Parser::CharType * end ) const
        {
            m_pParser->SetSysLiteral( begin, end );
        }
        ExternalIdLiteralParser * m_pParser;
    };

    struct FSetPubIdLiteral
    {
        inline FSetPubIdLiteral( ExternalIdLiteralParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const Parser::CharType * begin,
            const Parser::CharType * end ) const
        {
            m_pParser->SetPubIdLiteral( begin, end );
        }
        ExternalIdLiteralParser * m_pParser;
    };

    ExternalIdLiteralParser( const ExternalIdLiteralParser & );
    ExternalIdLiteralParser & operator = ( const ExternalIdLiteralParser & );

    void Setup( bool isPublicId );

    inline void SetQuoteType( const ::Parser::CharType quote )
    {
        m_singleQuoted = ( '\'' == quote );
    }

    inline void SetValidSyntax( bool valid )
    {
        m_validSyntax = valid;
    }

    void SetSysLiteral( const Parser::CharType * begin,
        const Parser::CharType * end );

    void SetPubIdLiteral( const Parser::CharType * begin,
        const Parser::CharType * end );

    void Done( const Parser::CharType * begin, const Parser::CharType * end );

    ParserStacks & m_stacks;
    PublicIdLiteralParser & m_pubIdParser;
    ::Parser::Xml::IExternalIdReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;
    bool m_singleQuoted;

    const Parser::CharType * m_pBegin;
    const Parser::CharType * m_pEnd;

    SpiritRule m_startSQ;
    SpiritRule m_sQuotedChars;
    SpiritRule m_endSQ;
    SpiritRule m_skipSQ;
    SpiritRule m_singleQuotedValue;
    SpiritRule m_startDQ;
    SpiritRule m_dQuotedChars;
    SpiritRule m_endDQ;
    SpiritRule m_skipDQ;
    SpiritRule m_doubleQuotedValue;
    SpiritRule m_system;
    SpiritRule m_sysLiteral;
    SpiritRule m_public;
    SpiritRule m_pubLiteral;
    SpiritRule m_whitespace;
    SpiritRule m_pubRules;
    SpiritRule m_skipOver;
    SpiritRule m_done;
    SpiritRule m_rule;

}; // end class ExternalIdLiteralParser

// ----------------------------------------------------------------------------

class PeReferenceParser
{
public:

    explicit PeReferenceParser( NameParser & nameParser );
    ~PeReferenceParser( void );

    inline void SetReceiver( ::Parser::Xml::IPeReferenceReceiver * receiver )
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

private:

    typedef ::Parser::FBreakPoint< PeReferenceParser > FBreakPoint;
    typedef ::Parser::FSetValidSyntax< PeReferenceParser > FSetValidSyntax;
    typedef ::Parser::FClear< PeReferenceParser > FClear;
    typedef ::Parser::FEnd< PeReferenceParser > FEnd;
    typedef ::Parser::Xml::FSetName< PeReferenceParser > FSetName;

    friend struct ::Parser::FSetValidSyntax< PeReferenceParser >;
    friend struct ::Parser::FClear< PeReferenceParser >;
    friend struct ::Parser::FEnd< PeReferenceParser >;
    friend struct ::Parser::Xml::FSetName< PeReferenceParser >;

    PeReferenceParser( const PeReferenceParser & );
    PeReferenceParser & operator = ( const PeReferenceParser & );

    void Clear( void );

    inline void SetValidSyntax( bool valid )
    {
        m_validSyntax = valid;
    }

    void SetName( const Parser::CharType * begin, const Parser::CharType * end )
    {
        m_pBegin = begin;
        m_pEnd = end;
    }

    void End( void );

    ParserStacks & m_stacks;
    ::Parser::Xml::IPeReferenceReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;

    const Parser::CharType * m_pBegin;
    const Parser::CharType * m_pEnd;

    SpiritRule m_start;
    SpiritRule m_name;
    SpiritRule m_end;
    SpiritRule m_skipOver;
    SpiritRule m_rule;

}; // end class PeReferenceParser

// ----------------------------------------------------------------------------

class EnumeratedTypeParser
{
public:

    explicit EnumeratedTypeParser( NameParser & nameParser );
    ~EnumeratedTypeParser( void );

    inline void SetReceiver( ::Parser::Xml::IEnumeratedTypeReceiver * receiver )
    {
        m_receiver = receiver;
    }

    inline bool IsValid( void ) const { return m_validSyntax; }

    inline bool IsNotation( void ) const { return m_isNotation; }

    inline const SpiritRule & GetRule( void ) const
    {
        return m_rule;
    }

    inline ParserStacks & GetStacks( void )
    {
        return m_stacks;
    }

    inline NameParser & GetNameParser( void )
    {
        return m_nameParser;
    }

private:

    typedef ::Parser::FBreakPoint< EnumeratedTypeParser > FBreakPoint;
    typedef ::Parser::FSetValidContent< EnumeratedTypeParser > FSetValidContent;
    typedef ::Parser::FSetValidSyntax< EnumeratedTypeParser > FSetValidSyntax;
    typedef ::Parser::FDone< EnumeratedTypeParser > FDone;
    typedef ::Parser::Xml::FSetName< EnumeratedTypeParser > FSetName;

    friend struct ::Parser::FSetValidContent< EnumeratedTypeParser >;
    friend struct ::Parser::FSetValidSyntax< EnumeratedTypeParser >;
    friend struct ::Parser::FDone< EnumeratedTypeParser >;
    friend struct ::Parser::Xml::FSetName< EnumeratedTypeParser >;

    struct FSetEnumType
    {
        inline FSetEnumType( EnumeratedTypeParser * pParser, bool isNotation ) :
            m_pParser( pParser ), m_isNotation( isNotation ) {}
        inline void operator () ( const ::Parser::CharType *,
            const ::Parser::CharType * ) const
        {
            m_pParser->SetEnumType( m_isNotation );
        }
        EnumeratedTypeParser * m_pParser;
        bool m_isNotation;
    };

    EnumeratedTypeParser( const EnumeratedTypeParser & );
    EnumeratedTypeParser & operator = ( const EnumeratedTypeParser & );

    inline void SetValidSyntax( bool valid )
    {
        m_validSyntax = valid;
    }

    void SetEnumType( bool notation );

    void SetName( const Parser::CharType * begin, const Parser::CharType * end );

    void Done( const Parser::CharType * begin,
        const Parser::CharType * end );

    ParserStacks & m_stacks;
    NameParser & m_nameParser;
    ::Parser::Xml::IEnumeratedTypeReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;
    bool m_isNotation;

    SpiritRule m_noteStart;
    SpiritRule m_noteBeginP;
    SpiritRule m_name;
    SpiritRule m_nameDelimiter;
    SpiritRule m_endP;
    SpiritRule m_notation;
    SpiritRule m_enumStart;
    SpiritRule m_nameToken;
    SpiritRule m_enumeration;
    SpiritRule m_skipOver;
    SpiritRule m_rule;

}; // end class EnumeratedTypeParser

// ----------------------------------------------------------------------------

class EntityValueParser
{
public:

    EntityValueParser( ReferenceParser & refParser,
        PeReferenceParser & peReferenceParser );
    ~EntityValueParser( void );

    inline void SetReceiver( ::Parser::Xml::IEntityValueReceiver * receiver )
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

    typedef ::Parser::FBreakPoint< EntityValueParser > FBreakPoint;
    typedef ::Parser::FSetValidSyntax< EntityValueParser > FSetValidSyntax;
    typedef ::Parser::FSetValidContent< EntityValueParser > FSetValidContent;
    typedef ::Parser::FDone< EntityValueParser > FDone;
    typedef ::Parser::Xml::FSetReference< EntityValueParser > FSetReference;
    typedef ::Parser::Xml::FSetQuoteType< EntityValueParser > FSetQuoteType;
    typedef ::Parser::Xml::FSetValue< EntityValueParser > FSetValue;

    friend struct ::Parser::FSetValidContent< EntityValueParser >;
    friend struct ::Parser::FSetValidSyntax< EntityValueParser >;
    friend struct ::Parser::FDone< EntityValueParser >;
    friend struct ::Parser::Xml::FSetReference< EntityValueParser >;
    friend struct ::Parser::Xml::FSetQuoteType< EntityValueParser >;
    friend struct ::Parser::Xml::FSetValue< EntityValueParser >;

    EntityValueParser( const EntityValueParser & );
    EntityValueParser & operator = ( const EntityValueParser & );

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

    void SetValue( const Parser::CharType * begin,
        const Parser::CharType * end );

    void SetReference( const Parser::CharType * begin,
        const Parser::CharType * end );

    void SetPeReference( const Parser::CharType * begin,
        const Parser::CharType * end );

    struct FSetPeReference
    {
        inline explicit FSetPeReference( EntityValueParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const Parser::CharType * begin,
            const Parser::CharType * end ) const
        {
            m_pParser->SetPeReference( begin, end );
        }
        EntityValueParser * m_pParser;
    };

    PeReferenceParser & m_peRefParser;
    ReferenceParser & m_refParser;
    ParserStacks & m_stacks;
    ::Parser::Xml::IEntityValueReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;
    bool m_validContent;
    bool m_singleQuoted;

    SpiritRule m_skipOver;
    SpiritRule m_sqStart;
    SpiritRule m_dqStart;
    SpiritRule m_reference;
    SpiritRule m_peReference;
    SpiritRule m_sqValue;
    SpiritRule m_dqValue;
    SpiritRule m_sqContent;
    SpiritRule m_dqContent;
    SpiritRule m_sqEnd;
    SpiritRule m_dqEnd;
    SpiritRule m_sqEntity;
    SpiritRule m_dqEntity;
    SpiritRule m_rule;

}; // end class EntityValueParser

// ----------------------------------------------------------------------------

class EncodingDeclParser
{
public:

    explicit EncodingDeclParser( ParserStacks & stacks );
    ~EncodingDeclParser( void );

    inline void SetReceiver( ::Parser::Xml::IEncodingDeclReceiver * receiver )
    {
        m_receiver = receiver;
    }

    inline bool IsValid( void ) const { return m_validSyntax; }

    inline bool IsSingleQuoted( void ) const { return m_singleQuoted; }

    inline const SpiritRule & GetRule( void ) const
    {
        return m_rule;
    }

    inline ParserStacks & GetStacks( void )
    {
        return m_stacks;
    }

    const Parser::CharType * GetBegin( void ) const { return m_pBegin; }
    const Parser::CharType * GetEnd( void ) const { return m_pEnd; }

private:

    typedef ::Parser::FStoreStackSize< EncodingDeclParser > FStoreStackSize;
    typedef ::Parser::FBreakPoint< EncodingDeclParser > FBreakPoint;
    typedef ::Parser::FSetValidSyntax< EncodingDeclParser > FSetValidSyntax;
    typedef ::Parser::FSetContent< EncodingDeclParser > FSetContent;
    typedef ::Parser::FClear< EncodingDeclParser > FClear;
    typedef ::Parser::FDone< EncodingDeclParser > FDone;
    typedef ::Parser::Xml::FSetName< EncodingDeclParser > FSetName;
    typedef ::Parser::Xml::FSetQuoteType< EncodingDeclParser > FSetQuoteType;

    friend struct ::Parser::FStoreStackSize< EncodingDeclParser >;
    friend struct ::Parser::FSetValidSyntax< EncodingDeclParser >;
    friend struct ::Parser::FSetContent< EncodingDeclParser >;
    friend struct ::Parser::FClear< EncodingDeclParser >;
    friend struct ::Parser::FDone< EncodingDeclParser >;
    friend struct ::Parser::Xml::FSetName< EncodingDeclParser >;
    friend struct ::Parser::Xml::FSetQuoteType< EncodingDeclParser >;

    EncodingDeclParser( const EncodingDeclParser & );
    EncodingDeclParser & operator = ( const EncodingDeclParser & );

    inline void Clear( void )
    {
        m_validSyntax = false;
        m_singleQuoted = false;
    }

    void StoreStackSize( void );

    void Done( const Parser::CharType * begin,
        const Parser::CharType * end );

    inline void SetValidSyntax( bool valid )
    {
        m_validSyntax = valid;
    }

    inline void SetQuoteType( const Parser::CharType ch )
    {
        m_singleQuoted = ( '\'' == ch );
    }

    void SetName( const Parser::CharType * begin,
        const Parser::CharType * end );

    ParserStacks & m_stacks;
    ::Parser::Xml::IEncodingDeclReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;
    bool m_singleQuoted;

    const Parser::CharType * m_pBegin;
    const Parser::CharType * m_pEnd;

    SpiritRule m_start;
    SpiritRule m_equals;
    SpiritRule m_encName;
    SpiritRule m_startSQ;
    SpiritRule m_endSQ;
    SpiritRule m_skipSQ;
    SpiritRule m_sQuotedContent;
    SpiritRule m_startDQ;
    SpiritRule m_endDQ;
    SpiritRule m_skipDQ;
    SpiritRule m_dQuotedContent;
    SpiritRule m_content;
    SpiritRule m_skipOver;
    SpiritRule m_rule;

}; // end class EncodingDeclParser

// ----------------------------------------------------------------------------

class XmlDeclarationParser
{
public:

    explicit XmlDeclarationParser( EncodingDeclParser & encodingDeclParser );
    ~XmlDeclarationParser( void );

    inline void SetReceiver( ::Parser::Xml::IXmlDeclarationReceiver * receiver )
    {
        m_receiver = receiver;
    }

    inline bool IsValid( void ) const
    {
        return ( m_validContent && m_validSyntax );
    }

    inline const SpiritRule & GetRule( void ) const
    {
        return m_rule;
    }

    inline ParserStacks & GetStacks( void )
    {
        return m_stacks;
    }

private:

    typedef ::Parser::FBreakPoint< XmlDeclarationParser > FBreakPoint;
    typedef ::Parser::FSetValidSyntax< XmlDeclarationParser > FSetValidSyntax;
    typedef ::Parser::FSetContent< XmlDeclarationParser > FSetContent;
    typedef ::Parser::FClear< XmlDeclarationParser > FClear;
    typedef ::Parser::FDone< XmlDeclarationParser > FDone;
    typedef ::Parser::Xml::FSetQuoteType< XmlDeclarationParser > FSetQuoteType;

    friend struct ::Parser::FSetValidSyntax< XmlDeclarationParser >;
    friend struct ::Parser::FSetContent< XmlDeclarationParser >;
    friend struct ::Parser::FClear< XmlDeclarationParser >;
    friend struct ::Parser::FDone< XmlDeclarationParser >;
    friend struct ::Parser::Xml::FSetQuoteType< XmlDeclarationParser >;

    struct FSetStandaloneType
    {
        inline FSetStandaloneType( XmlDeclarationParser * pParser, bool isYes,
            bool singleQuoted ) : m_pParser( pParser ), m_isYes( isYes ),
            m_singleQuoted( singleQuoted ) {}
        inline void operator () ( const ::Parser::CharType *,
            const ::Parser::CharType * ) const
        {
            m_pParser->SetStandaloneType( m_isYes, m_singleQuoted );
        }
        XmlDeclarationParser * m_pParser;
        bool m_isYes;
        bool m_singleQuoted;
    };

    struct FPostEncodingDecl
    {
        inline explicit FPostEncodingDecl( XmlDeclarationParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const ::Parser::CharType * begin,
            const ::Parser::CharType * end ) const
        {
            m_pParser->PostEncodingDecl( begin, end );
        }
        XmlDeclarationParser * m_pParser;
    };

    struct FPostVersionNumber
    {
        inline explicit FPostVersionNumber( XmlDeclarationParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const ::Parser::CharType * begin,
            const ::Parser::CharType * end ) const
        {
            m_pParser->PostVersionNumber( begin, end );
        }
        XmlDeclarationParser * m_pParser;
    };

    XmlDeclarationParser( const XmlDeclarationParser & );
    XmlDeclarationParser & operator = ( const XmlDeclarationParser & );

    void Clear( void );

    void PostVersionNumber( const ::Parser::CharType * begin,
        const ::Parser::CharType * end );

    void PostEncodingDecl( const ::Parser::CharType * begin,
        const ::Parser::CharType * end );

    void SetStandaloneType( bool isYes, bool singleQuoted );

    inline void SetValidSyntax( bool valid )
    {
        m_validSyntax = valid;
    }

    inline void SetValidContent( bool valid )
    {
        m_validContent = valid;
    }

    void Done( const ::Parser::CharType * begin, const ::Parser::CharType * end );

    EncodingDeclParser & m_encodingDeclParser;
    ParserStacks & m_stacks;
    ::Parser::Xml::IXmlDeclarationReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;
    bool m_validContent;
    bool m_singleQuoted;
    ::Parser::CharType m_versionValue;

    const ::Parser::CharType * m_pBegin;
    const ::Parser::CharType * m_pEnd;

    SpiritRule m_start;
    SpiritRule m_version;
    SpiritRule m_versionEqual;
    SpiritRule m_versionNumber;
    SpiritRule m_startSQ;
    SpiritRule m_endSQ;
    SpiritRule m_skipSQ;
    SpiritRule m_versionSQ;
    SpiritRule m_startDQ;
    SpiritRule m_endDQ;
    SpiritRule m_skipDQ;
    SpiritRule m_versionDQ;
    SpiritRule m_versionInfo;
    SpiritRule m_encodingDecl;
    SpiritRule m_standalone;
    SpiritRule m_equals;
    SpiritRule m_yesSQ;
    SpiritRule m_yesDQ;
    SpiritRule m_noSQ;
    SpiritRule m_noDQ;
    SpiritRule m_badDecl;
    SpiritRule m_standaloneDecl;
    SpiritRule m_end;
    SpiritRule m_skipOver;
    SpiritRule m_rule;

}; // end class XmlDeclarationParser

// ----------------------------------------------------------------------------

class AttListDeclParser
{
public:

    AttListDeclParser( EnumeratedTypeParser & enumTypeParser,
        AttributeValueParser & attValueParser );

    ~AttListDeclParser( void );

    inline void SetReceiver( ::Parser::Xml::IAttListDeclReceiver * receiver )
    {
        m_receiver = receiver;
    }

    inline bool IsValid( void ) const
    {
        return ( m_validSyntax && m_validContent );
    }

    inline const SpiritRule & GetRule( void ) const
    {
        return m_rule;
    }

    inline ParserStacks & GetStacks( void )
    {
        return m_stacks;
    }

private:

    typedef ::Parser::FBreakPoint< AttListDeclParser > FBreakPoint;
    typedef ::Parser::FSetValidSyntax< AttListDeclParser > FSetValidSyntax;
    typedef ::Parser::FClear< AttListDeclParser > FClear;
    typedef ::Parser::FDone< AttListDeclParser > FDone;
    typedef ::Parser::Xml::FSetName< AttListDeclParser > FSetName;

    friend struct ::Parser::FSetValidSyntax< AttListDeclParser >;
    friend struct ::Parser::FClear< AttListDeclParser >;
    friend struct ::Parser::FDone< AttListDeclParser >;
    friend struct ::Parser::Xml::FSetName< AttListDeclParser >;

    struct FSetAttName
    {
        inline explicit FSetAttName( AttListDeclParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const ::Parser::CharType * begin,
            const ::Parser::CharType * end ) const
        {
            m_pParser->SetAttName( begin, end );
        }
        AttListDeclParser * m_pParser;
    };

    struct FSetAttType
    {
        inline explicit FSetAttType( AttListDeclParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const ::Parser::CharType * begin,
            const ::Parser::CharType * end ) const
        {
            m_pParser->SetAttType( begin, end );
        }
        AttListDeclParser * m_pParser;
    };

    struct FAddNotateName
    {
        inline explicit FAddNotateName( AttListDeclParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const ::Parser::CharType * begin,
            const ::Parser::CharType * end ) const
        {
            m_pParser->AddNotateName( begin, end );
        }
        AttListDeclParser * m_pParser;
    };

    struct FSetDefaultDeclType
    {
        inline FSetDefaultDeclType( AttListDeclParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const ::Parser::CharType * begin,
            const ::Parser::CharType * end ) const
        {
            m_pParser->SetDefaultDeclType( begin, end );
        }
        AttListDeclParser * m_pParser;
    };

    struct FPreAttValue
    {
        inline FPreAttValue( AttListDeclParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const ::Parser::CharType *,
            const ::Parser::CharType * ) const
        {
            m_pParser->PreAttValue();
        }
        AttListDeclParser * m_pParser;
    };

    struct FDoneAttValue
    {
        inline FDoneAttValue( AttListDeclParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const ::Parser::CharType *,
            const ::Parser::CharType * ) const
        {
            m_pParser->DoneAttValue();
        }
        AttListDeclParser * m_pParser;
    };

    struct FPreEnumeration
    {
        inline FPreEnumeration( AttListDeclParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const ::Parser::CharType *,
            const ::Parser::CharType * ) const
        {
            m_pParser->PreEnumeration();
        }
        AttListDeclParser * m_pParser;
    };

    struct FDoneEnumType
    {
        inline FDoneEnumType( AttListDeclParser * pParser ) :
            m_pParser( pParser ) {}
        inline void operator () ( const ::Parser::CharType *,
            const ::Parser::CharType * ) const
        {
            m_pParser->DoneEnumType();
        }
        AttListDeclParser * m_pParser;
    };

    AttListDeclParser( const AttListDeclParser & );
    AttListDeclParser & operator = ( const AttListDeclParser & );

    void Clear( void );

    inline void SetValidSyntax( bool valid )
    {
        m_validSyntax = valid;
    }

    void SetName( const Parser::CharType * begin,
        const Parser::CharType * end );

    void SetAttName( const Parser::CharType * begin,
        const Parser::CharType * end );

    void AddNotateName( const Parser::CharType * begin,
        const Parser::CharType * end );

    void SetAttType( const Parser::CharType * begin,
        const Parser::CharType * end );

    void SetDefaultDeclType( const Parser::CharType * begin,
        const Parser::CharType * end );

    void PreAttValue( void );

    void DoneAttValue( void );

    void PreEnumeration( void );

    void DoneEnumType( void );

    void Done( const Parser::CharType * begin,
        const Parser::CharType * end );

    ParserStacks & m_stacks;
    AttributeValueParser & m_attValueParser;
    EnumeratedTypeParser & m_enumTypeParser;
    ::Parser::Xml::IAttListDeclReceiver * m_receiver;
    unsigned int m_stackSize;
    bool m_validSyntax;
    bool m_validContent;

    SpiritRule m_start;
    SpiritRule m_skipOver;
    SpiritRule m_name;
    SpiritRule m_attName;
    SpiritRule m_tokenizedType;
    SpiritRule m_preEnumeration;
    SpiritRule m_enumeratedType;
    SpiritRule m_attType;
//    SpiritRule m_notationType;
    SpiritRule m_defaultDeclType;
    SpiritRule m_defaultDeclFixed;
    SpiritRule m_preAttValue;
    SpiritRule m_attValue;
    SpiritRule m_defaultDecl;
    SpiritRule m_attDef;
    SpiritRule m_end;
    SpiritRule m_rule;

}; // end class AttListDeclParser

// ----------------------------------------------------------------------------

}; // end namespace Xml

}; // end namespace Parser

#endif // file guardian

// $Log: PrologParsers.hpp,v $
// Revision 1.2  2009/01/05 19:26:36  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.1  2008/12/05 19:04:16  rich_sposato
// Adding xml parsing files to project.
//
