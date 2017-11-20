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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/Tester/BasicTesters.hpp,v 1.1 2008/12/05 19:04:15 rich_sposato Exp $


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if !defined( BASIC_XML_PARSER_TESTERS_HPP_INCLUDED )
/// File guardian.
#define BASIC_XML_PARSER_TESTERS_HPP_INCLUDED


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

class CommentTester : public Parser::Xml::ICommentReceiver, public Parser::TestBase
{
public:

    CommentTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~CommentTester( void );

    virtual bool SetupTest( void );

private:

    CommentTester( const CommentTester & );
    CommentTester & operator = ( const CommentTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool AddComment( const char * begin, const char * end );

    Parser::Xml::XmlParser * m_pParser;
};

// ----------------------------------------------------------------------------

class NameTester : public Parser::Xml::INameReceiver, public Parser::TestBase
{
public:

    NameTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~NameTester( void );

    virtual bool SetupTest( void );

private:

    NameTester( const NameTester & );
    NameTester & operator = ( const NameTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool SetName( const char * begin, const char * end );

    Parser::Xml::XmlParser * m_pParser;
};

// ----------------------------------------------------------------------------

class ReferenceTester : public Parser::Xml::IReferenceReceiver,
    public Parser::TestBase
{
public:

    ReferenceTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~ReferenceTester( void );

    virtual bool SetupTest( void );

private:

    ReferenceTester( const ReferenceTester & );
    ReferenceTester & operator = ( const ReferenceTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool AddReference( const char * begin, const char * end, RefType refType );

    Parser::Xml::XmlParser * m_pParser;
};

// ----------------------------------------------------------------------------

class AttributeValueTester : public Parser::Xml::IAttributeValueReceiver,
    public Parser::TestBase
{
public:

    AttributeValueTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~AttributeValueTester( void );

    virtual bool SetupTest( void );

private:

    AttributeValueTester( const AttributeValueTester & );
    AttributeValueTester & operator = ( const AttributeValueTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool AddValue( const char * begin, const char * end );

    virtual bool AddReference( const char * begin, const char * end, RefType refType );

    virtual void DoneAttributeValue( bool valid, bool singleQuoted,
        const char * begin, const char * end );

    Parser::Xml::XmlParser * m_pParser;
};

// ----------------------------------------------------------------------------

class AttributeTester : public Parser::Xml::IAttributeReceiver,
    public Parser::TestBase
{
public:

    AttributeTester( Parser::Xml::XmlParser * pParser,
        const CommandLineArgs & argInfo );

    virtual ~AttributeTester( void );

    virtual bool SetupTest( void );

private:

    AttributeTester( const AttributeTester & );
    AttributeTester & operator = ( const AttributeTester & );

    virtual bool OnParse( const char * begin, const char * end, unsigned long i );

    virtual bool SetName( const char * begin, const char * end );

    virtual bool AddValue( const char * begin, const char * end );

    virtual bool AddReference( const char * begin, const char * end, RefType refType );

    virtual void DoneAttributeValue( bool valid, bool singleQuoted,
        const char * begin, const char * end );

    Parser::Xml::XmlParser * m_pParser;
};

// ----------------------------------------------------------------------------

#endif // file guardian

// $Log: BasicTesters.hpp,v $
// Revision 1.1  2008/12/05 19:04:15  rich_sposato
// Adding xml parsing files to project.
//
