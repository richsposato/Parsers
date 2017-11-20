// ----------------------------------------------------------------------------
// Parser Utility Testing
// Copyright (c) 2007 by Rich Sposato
//
// Permission to use, copy, modify, distribute and sell this software for any
// purpose is hereby granted under the terms stated in the GNU Library Public
// License, provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear in
// supporting documentation.
//
// ----------------------------------------------------------------------------

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/Tester/PrologTesters.hpp,v 1.1 2008/12/05 19:04:15 rich_sposato Exp $


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if !defined( BASIC_XML_PROLOG_TESTERS_HPP_INCLUDED )
/// File guardian.
#define BASIC_XML_PROLOG_TESTERS_HPP_INCLUDED


// ----------------------------------------------------------------------------
// Included files.

#include "../include/Receivers.hpp"
#include "../../Util/include/TestUtil.hpp"

namespace Parser
{
    namespace Xml
    {
        class XmlParser;
    };
};

class CommandLineArgs;


// ----------------------------------------------------------------------------

class PublicIdLiteralTester : public ::Parser::Xml::IPublicIdReceiver,
    public ::Parser::TestBase
{
public:

    PublicIdLiteralTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~PublicIdLiteralTester( void );

    virtual bool SetupTest( void );

private:

    PublicIdLiteralTester( const PublicIdLiteralTester & );
    PublicIdLiteralTester & operator = ( const PublicIdLiteralTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool SetPublicIdLiteral( const char * begin, const char * end,
        bool singleQuoted );

    Parser::Xml::XmlParser * m_pParser;
};

// ----------------------------------------------------------------------------

class ExternalIdLiteralTester : public ::Parser::Xml::IExternalIdReceiver,
    public ::Parser::TestBase
{
public:

    ExternalIdLiteralTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~ExternalIdLiteralTester( void );

    virtual bool SetupTest( void );

private:

    ExternalIdLiteralTester( const ExternalIdLiteralTester & );
    ExternalIdLiteralTester & operator = ( const ExternalIdLiteralTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool SetPublicIdLiteral( const char * begin, const char * end,
        bool singleQuoted );

    virtual bool SetSystemLiteral( const char * begin, const char * end,
        bool singleQuoted );

    virtual void DoneExternalIdLiteral( bool valid,  const char * begin,
        const char * end );

    Parser::Xml::XmlParser * m_pParser;
};

// ----------------------------------------------------------------------------

class PeReferenceTester : public Parser::Xml::IPeReferenceReceiver,
    public ::Parser::TestBase
{
public:

    PeReferenceTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~PeReferenceTester( void );

    virtual bool SetupTest( void );

private:

    PeReferenceTester( const PeReferenceTester & );
    PeReferenceTester & operator = ( const PeReferenceTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool AddPeReference( const char * begin, const char * end );

    Parser::Xml::XmlParser * m_pParser;
};

// ----------------------------------------------------------------------------

class EncodingTester : public Parser::Xml::IEncodingDeclReceiver,
    public ::Parser::TestBase
{
public:

    EncodingTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~EncodingTester( void );

    virtual bool SetupTest( void );

private:

    EncodingTester( const EncodingTester & );
    EncodingTester & operator = ( const EncodingTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool SetEncName( const char * begin, const char * end );

    virtual void DoneEncodingDecl( bool valid, bool singleQuoted,
        const char * begin, const char * end );

    Parser::Xml::XmlParser * m_pParser;
};

// ----------------------------------------------------------------------------

class XmlDeclarationTester : public ::Parser::Xml::IXmlDeclarationReceiver,
    public ::Parser::TestBase
{
public:

    XmlDeclarationTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~XmlDeclarationTester( void );

    virtual bool SetupTest( void );

private:

    XmlDeclarationTester( const XmlDeclarationTester & );
    XmlDeclarationTester & operator = ( const XmlDeclarationTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool SetEncName( const char * begin, const char * end );

    virtual void DoneEncodingDecl( bool valid, bool singleQuoted,
        const char * begin, const char * end );

    virtual bool SetVersionNumber( bool singleQuoted, const char * begin,
        const char * end );;

    virtual bool SetIsStandalone( bool standalone, bool singleQuoted );

    virtual void DoneXmlDeclaration( bool valid, const char * begin,
        const char * end );

    Parser::Xml::XmlParser * m_pParser;
};

// ----------------------------------------------------------------------------

class EnumeratedTypeTester : public Parser::Xml::IEnumeratedTypeReceiver,
    public Parser::TestBase
{
public:

    EnumeratedTypeTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~EnumeratedTypeTester( void );

    virtual bool SetupTest( void );

private:

    EnumeratedTypeTester( const EnumeratedTypeTester & );
    EnumeratedTypeTester & operator = ( const EnumeratedTypeTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool AddNotation( const char * begin, const char * end );

    virtual bool AddEnumeration( const char * begin, const char * end );

    virtual void DoneEnumeratedType( bool valid,
        const char * begin, const char * end );

    Parser::Xml::XmlParser * m_pParser;
};


// ----------------------------------------------------------------------------

class EntityValueTester : public Parser::Xml::IEntityValueReceiver,
    public Parser::TestBase
{
public:

    EntityValueTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~EntityValueTester( void );

    virtual bool SetupTest( void );

private:

    EntityValueTester( const EntityValueTester & );
    EntityValueTester & operator = ( const EntityValueTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool AddReference( const char * begin, const char * end, RefType refType );

    virtual bool AddPeReference( const char * begin, const char * end );

    virtual bool AddValue( const char * begin, const char * end );

    virtual void DoneEntityValue( bool valid, bool singleQuoted,
        const char * begin, const char * end );

    Parser::Xml::XmlParser * m_pParser;
};

// ----------------------------------------------------------------------------

class AttListDeclTester : public Parser::Xml::IAttListDeclReceiver,
    public Parser::TestBase
{
public:

    AttListDeclTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo,
        ::Parser::Xml::IAttributeValueReceiver * attributeValueReceiver,
        ::Parser::Xml::IEnumeratedTypeReceiver * enumeratedTypeReceiver );

    virtual ~AttListDeclTester( void );

    virtual bool SetupTest( void );

private:

    AttListDeclTester( const AttListDeclTester & );
    AttListDeclTester & operator = ( const AttListDeclTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool SetName( const char * begin, const char * end );

    virtual bool SetAttName( const char * begin, const char * end );

    virtual bool AddNotateName( const char * begin, const char * end );

    virtual bool SetAttType( AttType type );

    virtual bool SetDefaultDeclType( DefaultDeclType type );

    virtual ::Parser::Xml::IAttributeValueReceiver * AddAttributeValue( void );

    virtual ::Parser::Xml::IEnumeratedTypeReceiver * AddEnumeratedType( void );

    virtual void DoneAttListDecl( bool valid,
        const char * begin, const char * end );

    Parser::Xml::XmlParser * m_pParser;
    ::Parser::Xml::IAttributeValueReceiver * m_attributeValueReceiver;
    ::Parser::Xml::IEnumeratedTypeReceiver * m_enumeratedTypeReceiver;

};

// ----------------------------------------------------------------------------

#endif // file guardian

// $Log: PrologTesters.hpp,v $
// Revision 1.1  2008/12/05 19:04:15  rich_sposato
// Adding xml parsing files to project.
//
