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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/Tester/PrologTesters.cpp,v 1.1 2008/12/05 19:04:15 rich_sposato Exp $


#include "PrologTesters.hpp"

#include <string>
#include <iostream>

#include "../../Util/include/ParseInfo.hpp"

#include "../include/XmlParser.hpp"

#include "CommandLineArgs.hpp"


using namespace std;
using namespace Parser;


extern ParseInfo::ParseResult Convert( Xml::XmlParser::ParseResults result );


// ----------------------------------------------------------------------------

const TestData s_pubIdLiteralTestCases[] =
{
    { ParseInfo::AllValid,  "\'abc\'" },
    { ParseInfo::AllValid,  "\"abc\"" },
    { ParseInfo::AllValid,  "\'\'" },
    { ParseInfo::AllValid,  "\"\"" },
    { ParseInfo::NotParsed, "\"abc\'" },
    { ParseInfo::NotParsed, "\"abc " },
    { ParseInfo::NotParsed, "\'abc " },
    { ParseInfo::NotParsed, " \'abc\'" },
    { ParseInfo::NotParsed, " \"abc\"" },
    { ParseInfo::NotParsed, "abc\'" },
    { ParseInfo::NotParsed, "abc\"" },
    { ParseInfo::CantStart, "" },             // invalid if empty.
    { ParseInfo::CantStart, NULL }            // invalid if NULL.
};

const unsigned long s_pubIdLiteralTestCount =
    sizeof(s_pubIdLiteralTestCases) / sizeof(s_pubIdLiteralTestCases[0]);

// ----------------------------------------------------------------------------

PublicIdLiteralTester::PublicIdLiteralTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo ) :
    IPublicIdReceiver(),
    TestBase( "Public Id Literal", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

PublicIdLiteralTester::~PublicIdLiteralTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool PublicIdLiteralTester::SetPublicIdLiteral( const char * begin, const char * end,
    bool singleQuoted )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin <= end );

    if ( ShowContent() )
    {
        string content;
        if ( begin == end )
            content = "(empty)";
        else
            content.assign( begin, end-begin );
        const char * quoting = ( singleQuoted ) ? "single" : "double";
        cout << "Public Id: [" << content << ']' << endl;
        cout << "Quoted: " << quoting << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool PublicIdLiteralTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_pubIdLiteralTestCases, s_pubIdLiteralTestCount );
}

// ----------------------------------------------------------------------------

bool PublicIdLiteralTester::OnParse( const char * begin, const char * end,
    unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    m_pParser->SetErrorReceiver( errorCounter );
    IPublicIdReceiver * receiver =
        dynamic_cast< IPublicIdReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParsePublicIdLiteral( begin, end, receiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

const TestData s_externalIdLiteralTestCases[] =
{
    { ParseInfo::AllValid,  "SYSTEM \'abc\'" },
    { ParseInfo::AllValid,  "SYSTEM\t\"abc\"" },
    { ParseInfo::AllValid,  "SYSTEM\n\r\'\'" },
    { ParseInfo::AllValid,  "SYSTEM \"\"" },
    { ParseInfo::AllValid,  "SYSTEM   \'def\'" },
    { ParseInfo::AllValid,  "SYSTEM  \'def\'" },
    { ParseInfo::AllValid,  "PUBLIC \'abc\' \'def\'" },
    { ParseInfo::AllValid,  "PUBLIC\t\"abc\" \'def\'" },
    { ParseInfo::AllValid,  "PUBLIC\n\r\'\' \'def\'" },
    { ParseInfo::AllValid,  "PUBLIC \"\" \'def\'" },
    { ParseInfo::AllValid,  "PUBLIC  \'abc\'  \'def\'" },
    { ParseInfo::AllValid,  "PUBLIC  \"abc\"  \'def\'" },
    { ParseInfo::NotParsed, "SYSTEM \"abc\'" },
    { ParseInfo::NotParsed, "SYSTEM \"abc" },
    { ParseInfo::NotParsed, "SYSTEM \'abc" },
    { ParseInfo::NotParsed, "SYSTEM abc\'" },
    { ParseInfo::NotParsed, "SYSTEM abc\"" },
    { ParseInfo::NotParsed, "PUBLIC \"abc\' \'def\'" },
    { ParseInfo::NotParsed, "PUBLIC \"abc \'def\'" },
    { ParseInfo::NotParsed, "PUBLIC \'abc \'def\'" },
    { ParseInfo::NotParsed, "PUBLIC abc\' \'def\'" },
    { ParseInfo::NotParsed, "PUBLIC abc\" \'def\'" },
    { ParseInfo::NotParsed, "SYSTEM\'abc\'" },
    { ParseInfo::NotParsed, "SYSTEM\"abc\"" },
    { ParseInfo::NotParsed, "SYSTEM\n\r\'" },
    { ParseInfo::NotParsed, "SYSTEM \"" },
    { ParseInfo::NotParsed, "SYSTEM   " },
    { ParseInfo::NotParsed, "SYSTEM  \'def" },
    { ParseInfo::NotParsed, "PUBLIC \'abc\'" },
    { ParseInfo::NotParsed, "PUBLIC\t\"abc\"" },
    { ParseInfo::NotParsed, "PUBLIC\n\r\'\'" },
    { ParseInfo::NotParsed, "PUBLIC \"\"" },
    { ParseInfo::NotParsed, "PUBLIC  \'abc\' " },
    { ParseInfo::NotParsed, "PUBLIC  \"abc\"  " },
    { ParseInfo::CantStart, "" },             // invalid if empty.
    { ParseInfo::CantStart, NULL }            // invalid if NULL.
};

const unsigned long s_externalIdLiteralTestCount =
    sizeof(s_externalIdLiteralTestCases) / sizeof(s_externalIdLiteralTestCases[0]);

// ----------------------------------------------------------------------------

ExternalIdLiteralTester::ExternalIdLiteralTester(
    Parser::Xml::XmlParser * pParser, const CommandLineArgs & argInfo ) :
    IExternalIdReceiver(),
    TestBase( "External Id Literal", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

ExternalIdLiteralTester::~ExternalIdLiteralTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool ExternalIdLiteralTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_externalIdLiteralTestCases,
        s_externalIdLiteralTestCount );
}

// ----------------------------------------------------------------------------

bool ExternalIdLiteralTester::OnParse( const char * begin, const char * end,
    unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    m_pParser->SetErrorReceiver( errorCounter );
    IExternalIdReceiver * receiver =
        dynamic_cast< IExternalIdReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParseExternalId( begin, end, receiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

bool ExternalIdLiteralTester::SetPublicIdLiteral( const char * begin,
    const char * end, bool singleQuoted )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin <= end );

    if ( ShowContent() )
    {
        string content;
        if ( begin == end )
            content = "(empty)";
        else
            content.assign( begin, end-begin );
        const char * quoting = ( singleQuoted ) ? "single" : "double";
        cout << "Public Id: [" << content << ']' << endl;
        cout << "Quoted: " << quoting << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool ExternalIdLiteralTester::SetSystemLiteral( const char * begin,
    const char * end, bool singleQuoted )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin <= end );

    if ( ShowContent() )
    {
        string content;
        if ( begin == end )
            content = "(empty)";
        else
            content.assign( begin, end-begin );
        const char * quoting = ( singleQuoted ) ? "single" : "double";
        cout << "System Literal: [" << content << ']' << endl;
        cout << "Quoted: " << quoting << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

void ExternalIdLiteralTester::DoneExternalIdLiteral( bool valid,
    const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin <= end );
    (void)begin;
    (void)end;

    if ( ShowContent() )
    {
        const char * result = ( valid ) ? "yes" : "no";
        cout << "Done! Valid: [" << result << ']' << endl;
    }
}

// ----------------------------------------------------------------------------

const TestData s_peReferenceTestCases[] =
{
    { ParseInfo::AllValid,  "%a;" },
    { ParseInfo::AllValid,  "%abc.def;" },
    { ParseInfo::SomeValid, "%ab; " },
    { ParseInfo::SomeValid, "%ab;;" },
    { ParseInfo::NotParsed, "%aa" },
    { ParseInfo::NotParsed, "%a ;" },
    { ParseInfo::NotParsed, "% a;" },
    { ParseInfo::NotParsed, "%%a;" },
    { ParseInfo::NotParsed, ";%a;" },
    { ParseInfo::NotParsed, "%ab%" },
    { ParseInfo::NotParsed, ";" },
    { ParseInfo::NotParsed, "a;" },
    { ParseInfo::CantStart, "" },             // invalid if empty.
    { ParseInfo::CantStart, NULL }            // invalid if NULL.
};

const unsigned long s_peReferenceTestCount =
    sizeof(s_peReferenceTestCases) / sizeof(s_peReferenceTestCases[0]);

// ----------------------------------------------------------------------------

PeReferenceTester::PeReferenceTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo ) :
    IPeReferenceReceiver(),
    TestBase( "PE Reference", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

PeReferenceTester::~PeReferenceTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool PeReferenceTester::AddPeReference( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string value( begin, end-begin );
        cout << "PE Reference: [" << value << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool PeReferenceTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_peReferenceTestCases, s_peReferenceTestCount );
}

// ----------------------------------------------------------------------------

bool PeReferenceTester::OnParse( const char * begin, const char * end,
    unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    m_pParser->SetErrorReceiver( errorCounter );
    IPeReferenceReceiver * receiver =
        dynamic_cast< IPeReferenceReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParsePeReference( begin, end, receiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

const TestData s_encodingTestCases[] =
{
    { ParseInfo::AllValid,  "encoding=\'a\'" },
    { ParseInfo::AllValid,  "encoding=\"a\"" },
    { ParseInfo::AllValid,  "encoding  = \"a\"" },
    { ParseInfo::AllValid,  "encoding   =\"a\"" },
    { ParseInfo::AllValid,  "encoding=   \"a\"" },
    { ParseInfo::AllValid,  "encoding=\"a.b\"" },
    { ParseInfo::AllValid,  "encoding=\"a2\"" },
    { ParseInfo::AllValid,  "encoding=\"ab\"" },
    { ParseInfo::AllValid,  "encoding=\'r2_d2\'" },
    { ParseInfo::AllValid,  "encoding=\'r2-d2\'" },
    { ParseInfo::AllValid,  "encoding=\'r2.d2\'" },
    { ParseInfo::SomeValid, "encoding=\"a\" " },
    { ParseInfo::NotValid,  "encoding=\"a2 \"" },
    { ParseInfo::NotValid,  "encoding=\" a2\"" },
    { ParseInfo::NotValid,  "encoding=\"2\"" },
    { ParseInfo::NotParsed, "encoding\'a\'" },
    { ParseInfo::NotParsed, "encoding\"a\"" },
    { ParseInfo::NotParsed, "encoding=a\"" },
    { ParseInfo::NotParsed, "encoding=a\'" },
    { ParseInfo::NotParsed, "encoding=a" },
    { ParseInfo::NotParsed, "encoding=\'a\"" },
    { ParseInfo::NotParsed, "encoding=\'a" },
    { ParseInfo::NotParsed, "encoding=\"a\'" },
    { ParseInfo::NotParsed, "encoding=\"a" },
    { ParseInfo::CantStart, "" },             // invalid if empty.
    { ParseInfo::CantStart, NULL }            // invalid if NULL.
};

const unsigned long s_encodingTestCount =
    sizeof(s_encodingTestCases) / sizeof(s_encodingTestCases[0]);

// ----------------------------------------------------------------------------

EncodingTester::EncodingTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo ) :
    IEncodingDeclReceiver(),
    TestBase( "Encoding", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

EncodingTester::~EncodingTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool EncodingTester::SetEncName( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string name( begin, end-begin );
        cout << "EncName: [" << name << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

void EncodingTester::DoneEncodingDecl( bool valid, bool singleQuoted,
    const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    (void)begin;
    (void)end;

    if ( ShowContent() )
    {
        const char * result = ( valid ) ? "yes" : "no";
        const char * quoteType = ( singleQuoted ) ? "single" : "double";
        cout << "Done! Valid: [" << result << "] \t Quotes: ["
             << quoteType << ']' << endl;
    }
}

// ----------------------------------------------------------------------------

bool EncodingTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_encodingTestCases, s_encodingTestCount );
}

// ----------------------------------------------------------------------------

bool EncodingTester::OnParse( const char * begin, const char * end, unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    assert( errorCounter != NULL );
    m_pParser->SetErrorReceiver( errorCounter );
    IEncodingDeclReceiver * receiver =
        dynamic_cast< IEncodingDeclReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParseEncoding( begin, end, receiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

const TestData s_xmlDeclarationTestCases[] =
{
    { ParseInfo::AllValid,  "<?xml version='1' encoding=\'a\' standalone='yes'?>" },
    { ParseInfo::AllValid,  "<?xml version='1.2' encoding=\"a\"  standalone='no'?>" },
    { ParseInfo::AllValid,  "<?xml version=\"1.0\" encoding  = \"a\"  standalone=\"yes\" ?>" },
    { ParseInfo::AllValid,  "<?xml version='1'  standalone=\"no\" ?>" },
    { ParseInfo::AllValid,  "<?xml version='1' encoding=\'a\' ?>" },
    { ParseInfo::AllValid,  "<?xml version='1.2' encoding=\"a\" ?>" },
    { ParseInfo::AllValid,  "<?xml version=\"1.0\" encoding  = \"a\" ?>" },
    { ParseInfo::AllValid,  "<?xml version='1' encoding   =\"a\" ?>" },
    { ParseInfo::AllValid,  "<?xml version='1-2' encoding=   \"a\" ?>" },
    { ParseInfo::AllValid,  "<?xml version='1:01' encoding=\"a.b\" ?>" },
    { ParseInfo::AllValid,  "<?xml version='1.2.3' encoding=\"a2\" ?>" },
    { ParseInfo::AllValid,  "<?xml version='1' encoding=\"ab\" ?>" },
    { ParseInfo::AllValid,  "<?xml version='abc.3' encoding=\'r2_d2\' ?>" },
    { ParseInfo::AllValid,  "<?xml version='1_5' encoding=\'r2-d2\' ?>" },
    { ParseInfo::AllValid,  "<?xml version='1' encoding=\'r2.d2\' ?>" },
    { ParseInfo::NotValid,  "<?xml version='1_5' encoding=\"a2 \" ?>" },
    { ParseInfo::NotValid,  "<?xml version='1_5' encoding=\" a2\" ?>" },
    { ParseInfo::NotValid,  "<?xml version='1_5' encoding=\"2\" ?>" },
    { ParseInfo::NotParsed, "<?xml version='1' encoding=\'a\' standalone='yes\"?>" },
    { ParseInfo::NotParsed, "<?xml version='1.2' encoding=\"a\"  standalone=\"no'?>" },
    { ParseInfo::NotParsed, "<?xml version=\"1.0\" encoding  = \"a\"  standalone= ?>" },
    { ParseInfo::NotParsed, "<?xml version='1'  standalone=no ?>" },
    { ParseInfo::NotParsed, "<?xml version='1' encoding=\'a\' standalone'yes'?>" },
    { ParseInfo::NotParsed, "<?xml version='1.2' encoding=\"a\"  Standalone='no'?>" },
    { ParseInfo::NotParsed, "<?xml version=\"1.0\" encoding  = \"a\"  standalone=yes ?>" },
    { ParseInfo::NotParsed, "<?xml version='1'  standalone\"no\" ?>" },
    { ParseInfo::NotParsed, "<?xml encoding=\"a\"  ?>" },
    { ParseInfo::NotParsed, "<?xml version='1_5' encoding\'a\'?>" },
    { ParseInfo::NotParsed, "<?xml version='1_5' encoding\"a\" ?>" },
    { ParseInfo::NotParsed, "<?xml version='1_5' encoding=a\" ?>" },
    { ParseInfo::NotParsed, "<?xml version='1_5' encoding=a\' ?>" },
    { ParseInfo::NotParsed, "<?xml version='1_5' encoding=a ?>" },
    { ParseInfo::NotParsed, "<?xml version='1_5' encoding=\'a\" ?>" },
    { ParseInfo::NotParsed, "<?xml version='1_5' encoding=\'a ?>" },
    { ParseInfo::NotParsed, "<?xml version='1_5' encoding=\"a\' ?>" },
    { ParseInfo::NotParsed, "<?xml version='1_5' encoding=\"a ?>" },
    { ParseInfo::CantStart, "" },             // invalid if empty.
    { ParseInfo::CantStart, NULL }            // invalid if NULL.
};

const unsigned long s_xmlDeclarationTestCount =
    sizeof(s_xmlDeclarationTestCases) / sizeof(s_xmlDeclarationTestCases[0]);

// ----------------------------------------------------------------------------

XmlDeclarationTester::XmlDeclarationTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo ) :
    IXmlDeclarationReceiver(),
    TestBase( "Xml Declaration", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

XmlDeclarationTester::~XmlDeclarationTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool XmlDeclarationTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_xmlDeclarationTestCases,
        s_xmlDeclarationTestCount );
}

// ----------------------------------------------------------------------------

bool XmlDeclarationTester::OnParse( const char * begin, const char * end, unsigned long i )
{
    assert( this != NULL );
    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
//    cout << __FUNCTION__ << '\t' << __LINE__ << '\t' << errorCounter << endl;
    assert( errorCounter != NULL );
    m_pParser->SetErrorReceiver( errorCounter );
    IXmlDeclarationReceiver * receiver =
        dynamic_cast< IXmlDeclarationReceiver * >( this );
    assert( receiver != NULL );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParseXmlDeclaration( begin, end, receiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

bool XmlDeclarationTester::SetEncName( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string name( begin, end-begin );
        cout << "EncName: [" << name << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

void XmlDeclarationTester::DoneEncodingDecl( bool valid, bool singleQuoted,
    const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    (void)begin;
    (void)end;

    if ( ShowContent() )
    {
        const char * result = ( valid ) ? "yes" : "no";
        const char * quoteType = ( singleQuoted ) ? "single" : "double";
        cout << "Done! Valid: [" << result << "] \t Quotes: ["
             << quoteType << ']' << endl;
    }
}

// ----------------------------------------------------------------------------

bool XmlDeclarationTester::SetVersionNumber( bool singleQuoted, const char * begin,
    const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string version( begin, end-begin );
        const char * quoteType = ( singleQuoted ) ? "single" : "double";
        cout << "Version Number: [" << version << ']' << endl;
        cout << "Quote Type? [" << quoteType << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool XmlDeclarationTester::SetIsStandalone( bool standalone, bool singleQuoted )
{
    assert( this != NULL );

    if ( ShowContent() )
    {
        const char * result = ( standalone ) ? "yes" : "no";
        const char * quoteType = ( singleQuoted ) ? "single" : "double";
        cout << "Standalone? [" << result << "]   Quote Type? [" << quoteType << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

void XmlDeclarationTester::DoneXmlDeclaration( bool valid, const char * begin,
    const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    (void)begin;
    (void)end;

    if ( ShowContent() )
    {
        const char * result = ( valid ) ? "yes" : "no";
        cout << "Done! Valid: [" << result << ']' << endl;
    }
}

// ----------------------------------------------------------------------------

const TestData s_enumTypeTestCases[] =
{
    { ParseInfo::AllValid,  "NOTATION (a)" },
    { ParseInfo::AllValid,  "NOTATION (a|b)" },
    { ParseInfo::AllValid,  "NOTATION ( a | b )" },
    { ParseInfo::AllValid,  "NOTATION ( a | b | x )" },
    { ParseInfo::AllValid,  "(a)" },
    { ParseInfo::AllValid,  "(a|b)" },
    { ParseInfo::AllValid,  "( a | b )" },
    { ParseInfo::AllValid,  "( a | b | x )" },
    { ParseInfo::SomeValid, "NOTATION (a) abc" },
    { ParseInfo::SomeValid, "(a) abc" },
    { ParseInfo::NotParsed, " " },
    { ParseInfo::NotParsed, "()" },
    { ParseInfo::NotParsed, "(a|)" },
    { ParseInfo::NotParsed, "(a|" },
    { ParseInfo::NotParsed, "(a" },
    { ParseInfo::NotParsed, "|a)" },
    { ParseInfo::NotParsed, "(| b |c)" },
    { ParseInfo::NotParsed, "b |c)" },
    { ParseInfo::NotParsed, "NOTATION " },
    { ParseInfo::NotParsed, "NOTATION ()" },
    { ParseInfo::NotParsed, "NOTATION (a|)" },
    { ParseInfo::NotParsed, "NOTATION (a|" },
    { ParseInfo::NotParsed, "NOTATION (a" },
    { ParseInfo::NotParsed, "NOTATION |a)" },
    { ParseInfo::NotParsed, "NOTATION (| b |c)" },
    { ParseInfo::NotParsed, "NOTATION b |c)" },
    { ParseInfo::CantStart, "" },             // invalid if empty.
    { ParseInfo::CantStart, NULL }            // invalid if NULL.
};

const unsigned long s_enumTypeTestCount =
    sizeof(s_enumTypeTestCases) / sizeof(s_enumTypeTestCases[0]);

// ----------------------------------------------------------------------------

EnumeratedTypeTester::EnumeratedTypeTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo ) :
    IEnumeratedTypeReceiver(),
    TestBase( "Enumeration", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

EnumeratedTypeTester::~EnumeratedTypeTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool EnumeratedTypeTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_enumTypeTestCases, s_enumTypeTestCount );
}

// ----------------------------------------------------------------------------

bool EnumeratedTypeTester::OnParse( const char * begin, const char * end,
    unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    m_pParser->SetErrorReceiver( errorCounter );
    IEnumeratedTypeReceiver * receiver =
        dynamic_cast< IEnumeratedTypeReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParseEnumeratedType( begin, end, receiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

bool EnumeratedTypeTester::AddNotation( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string name( begin, end-begin );
        cout << "Notation: [" << name << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool EnumeratedTypeTester::AddEnumeration( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string name( begin, end-begin );
        cout << "Enumeration: [" << name << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

void EnumeratedTypeTester::DoneEnumeratedType( bool valid,
    const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    (void)begin;
    (void)end;

    if ( ShowContent() )
    {
        const char * result = ( valid ) ? "yes" : "no";
        cout << "Done! Valid: [" << result << ']' << endl;
    }
}

// ----------------------------------------------------------------------------

const TestData s_entityValueTestCases[] =
{
    { ParseInfo::AllValid,  "\'%a;\'" },
    { ParseInfo::AllValid,  "\'%abc.def;\'" },
    { ParseInfo::AllValid,  "\'abc\'" },
    { ParseInfo::AllValid,  "\"abc\"" },
    { ParseInfo::AllValid,  "\'\'" },
    { ParseInfo::AllValid,  "\"\"" },
    { ParseInfo::AllValid,  "\"&abc;%a;\"" },  // value is an entity-reference.
    { ParseInfo::AllValid,  "\"&#000;\"" }, // value is a digit-reference.
    { ParseInfo::AllValid,  "\"&#x00;\"" }, // value is a hexdigit-reference.
    { ParseInfo::AllValid,  "\"&#xab;\"" }, // value is a hexdigit-reference.
    { ParseInfo::AllValid,  "\"&#xAB;\"" }, // value is a hexdigit-reference.
    { ParseInfo::AllValid,  "\"&#xF0; %a;\"" }, // value is a hexdigit-reference.
    { ParseInfo::AllValid,  "\'%ab; \'" },
    { ParseInfo::AllValid,  "\'%ab;;\'" },
    { ParseInfo::AllValid,  "\';%a;\'" },
    { ParseInfo::AllValid,  "\';\'" },
    { ParseInfo::AllValid,  "\'a;\'" },
    { ParseInfo::AllValid,  "\" &abc;\"" }, // value of entity-reference has extra space.
    { ParseInfo::AllValid,  "\" &#000;\"" }, // value of digit-reference has extra space.
    { ParseInfo::AllValid,  "\" &#x00;\"" }, // value of hexdigit-reference has extra space.
    { ParseInfo::AllValid,  "\'&#000;%a;\'" },
    { ParseInfo::AllValid,  "\'%abc.def;&#000;\'" },
    { ParseInfo::AllValid,  "\'abc%a;\'" },
    { ParseInfo::AllValid,  "\"abc&#x00;\"" },
    { ParseInfo::AllValid,  "\'a\"bc\'" },
    { ParseInfo::AllValid,  "\"ab\'c\"" },
    { ParseInfo::SomeValid, "\'%ab;\' " },
    { ParseInfo::SomeValid, "\'%ab;\';" },
    { ParseInfo::NotParsed, "\'%aa\'" },
    { ParseInfo::NotParsed, "\'%a ;\'" },
    { ParseInfo::NotParsed, "\'% a;\'" },
    { ParseInfo::NotParsed, "\'%%a;\'" },
    { ParseInfo::NotParsed, "\'%ab%\'" },
    { ParseInfo::NotParsed, "\"&000;\"" },  // invalid entity-reference value.
    { ParseInfo::NotParsed, "\"&#abc;\"" }, // value is an entity-reference, but prefix is digit-ref.
    { ParseInfo::NotParsed, "\"&#xFz;\"" }, // invalid hexdigit-reference value.
    { ParseInfo::NotParsed, "\"&#abc;\"" }, // prefix for digit-ref, but value is hexdigit-reference.
    { ParseInfo::NotParsed, "\"&;\"" },     // value is missing an entity-reference.
    { ParseInfo::NotParsed, "\"& abc;\"" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "\"&abc ;\"" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "\"&ab c;\"" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "\"&abc\"" },   // value with entity-reference is missing a semicolon.
    { ParseInfo::NotParsed, "\"&# 000;\"" }, // value of digit-reference has extra space.
    { ParseInfo::NotParsed, "\"&# x00;\"" }, // value of hexdigit-reference has extra space.
    { ParseInfo::NotParsed, "\"&#000 ;\"" }, // value of digit-reference has extra space.
    { ParseInfo::NotParsed, "\"&#x00 ;\"" }, // value of hexdigit-reference has extra space.
    { ParseInfo::NotParsed, "\"& #000;\"" }, // value of digit-reference has extra space.
    { ParseInfo::NotParsed, "\"& #x00;\"" }, // value of hexdigit-reference has extra space.
    { ParseInfo::NotParsed, "\"abc\'" },
    { ParseInfo::NotParsed, "\"abc " },
    { ParseInfo::NotParsed, "\'abc " },
    { ParseInfo::NotParsed, " \'abc\'" },
    { ParseInfo::NotParsed, " \"abc\"" },
    { ParseInfo::NotParsed, "abc\'" },
    { ParseInfo::NotParsed, "abc\"" },
    { ParseInfo::CantStart, "" },             // invalid if empty.
    { ParseInfo::CantStart, NULL }            // invalid if NULL.
};

const unsigned long s_entityValueTestCount =
    sizeof(s_entityValueTestCases) / sizeof(s_entityValueTestCases[0]);

// ----------------------------------------------------------------------------

EntityValueTester::EntityValueTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo ) :
    IEntityValueReceiver(),
    TestBase( "Entity Value", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

EntityValueTester::~EntityValueTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool EntityValueTester::AddReference( const char * begin, const char * end,
    RefType refType )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string value( begin, end-begin );
        cout << "Reference: [" << value << ']' << endl;
        const char * typeName = Parser::Xml::GetRefTypeName( refType );
        cout << "Type of Reference: [" << typeName << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool EntityValueTester::AddPeReference( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string value( begin, end-begin );
        cout << "PE Reference: [" << value << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool EntityValueTester::AddValue( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string value( begin, end-begin );
        cout << "Value: [" << value << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

void EntityValueTester::DoneEntityValue( bool valid, bool singleQuoted,
    const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    (void)begin;
    (void)end;

    if ( ShowContent() )
    {
        const char * result = ( valid ) ? "yes" : "no";
        const char * quoteType = ( singleQuoted ) ? "single" : "double";
        cout << "Done! Valid: [" << result << "] \t Quotes: ["
             << quoteType << ']' << endl;
    }
}

// ----------------------------------------------------------------------------

bool EntityValueTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_entityValueTestCases, s_entityValueTestCount );
}

// ----------------------------------------------------------------------------

bool EntityValueTester::OnParse( const char * begin, const char * end, unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    m_pParser->SetErrorReceiver( errorCounter );
    IEntityValueReceiver * receiver =
        dynamic_cast< IEntityValueReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParseEntityValue( begin, end, receiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

const TestData s_attListDeclTestCases[] =
{
    { ParseInfo::AllValid,  "<!ATTLIST abc.def >" },
    { ParseInfo::AllValid,  "<!ATTLIST abc.def>" },
    { ParseInfo::AllValid,  "<!ATTLIST a b NOTATION (a) #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST a b NOTATION (a|b) #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST a b NOTATION ( a | b ) #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST a b NOTATION ( a | b | x ) #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST a b (a) #IMPLIED >" },
    { ParseInfo::AllValid,  "<!ATTLIST a b (a|b) #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST a b ( a | b ) #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST a b ( a | b | x ) #IMPLIED >" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def CDATA #REQUIRED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def CDATA #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def CDATA #REQUIRED >" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def CDATA #IMPLIED >" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #REQUIRED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def IDREF #REQUIRED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def IDREF #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def IDREFS #REQUIRED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def IDREFS #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ENTITY #REQUIRED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ENTITY #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ENTITIES #REQUIRED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ENTITIES #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def NMTOKEN #REQUIRED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def NMTOKEN #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def NMTOKENS #REQUIRED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def NMTOKENS #IMPLIED>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID \'b\'>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID \"b\" >" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID \'b\'>" },      // test of more than one concurrent whitespace.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID \"b\">" },      // test of more than one concurrent whitespace.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID \"b \'c\">" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID \'&#000;&#xbc;\'>" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID \'&#000;\'>" },  // test single-quoted attribute with value.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID \"b&#000;\">" }, // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID \"b&#xbc;\">" }, // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID \"&#000;&#xbc;\">" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'b\'>" },      // test single-quoted attribute.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"b\">" },      // test double-quoted attribute.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"b \'c\">" },  // test double-quoted attribute with single quote inside value.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'b \"c\'>" },  // test single-quoted attribute with double quote inside value.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'b\'>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"b\" >" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'b\'>" },      // test of more than one concurrent whitespace.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"b\">" },      // test of more than one concurrent whitespace.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"b \'c\">" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'b\'>" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"b\">" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"b \'c\">" },
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'\'>" },       // single-quoted empty value is valid.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"\">" },       // double-quoted empty value is valid.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'&abc;\'>" },  // value is an entity-reference.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'&abc; \'>" }, // value of entity-reference has extra space.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \' &abc;\'>" }, // value of entity-reference has extra space.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'b&#000;\'>" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'b&#xbc;\'>" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'&#000;&#xbc;\'>" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \'&#000;\'>" },  // test single-quoted attribute with value.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"b&#000;\">" }, // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"b&#xbc;\">" }, // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"&#000;&#xbc;\">" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "<!ATTLIST abc def ID #FIXED \"&#000;\">" }, // test single-quoted attribute with value.
    { ParseInfo::NotParsed, "<!ATTLIST a ()>" },
    { ParseInfo::NotParsed, "<!ATTLIST a (a|)>" },
    { ParseInfo::NotParsed, "<!ATTLIST a (a|>" },
    { ParseInfo::NotParsed, "<!ATTLIST a (a>" },
    { ParseInfo::NotParsed, "<!ATTLIST a |a)>" },
    { ParseInfo::NotParsed, "<!ATTLIST a (| b |c)>" },
    { ParseInfo::NotParsed, "<!ATTLIST a b |c)>" },
    { ParseInfo::NotParsed, "<!ATTLIST a NOTATION (a|b)#IMPLIED>" }, //
    { ParseInfo::NotParsed, "<!ATTLIST a NOTATION ( a | b )#IMPLIED>" }, //
    { ParseInfo::NotParsed, "<!ATTLIST a ( a | b )#IMPLIED>" }, //
    { ParseInfo::NotParsed, "<!ATTLIST a NOTATION #IMPLIED>" },
    { ParseInfo::NotParsed, "<!ATTLIST a NOTATION () #IMPLIED >" },
    { ParseInfo::NotParsed, "<!ATTLIST a NOTATION (a|) #IMPLIED >" },
    { ParseInfo::NotParsed, "<!ATTLIST a NOTATION (a| #IMPLIED>" },
    { ParseInfo::NotParsed, "<!ATTLIST a NOTATION (a #IMPLIED >" },
    { ParseInfo::NotParsed, "<!ATTLIST a NOTATION |a) #IMPLIED >" },
    { ParseInfo::NotParsed, "<!ATTLIST a NOTATION (| b |c) #IMPLIED >" },
    { ParseInfo::NotParsed, "<!ATTLIST a NOTATION b |c) #IMPLIED >" },
    { ParseInfo::NotParsed, "<! ATTLIST abc def CDATA #IMPLIED>" },
    { ParseInfo::NotParsed, "< !ATTLIST abc def CDATA #IMPLIED>" },
    { ParseInfo::NotParsed, "<!AttList abc def CDATA #IMPLIED>" },
    { ParseInfo::NotParsed, "<!attlist abc def CDATA #IMPLIED>" },
    { ParseInfo::NotParsed, "<!ATTLISTabc def CDATA #IMPLIED>" },
    { ParseInfo::NotParsed, "!ATTLIST abc def CDATA #IMPLIED>" },
    { ParseInfo::NotParsed, "!ATTLIST abc def CDATA #IMPLIED>" },
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID \'b>" },        // invalid if no ending single-quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID \"b>" },        // invalid if no ending double-quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID b\'>" },        // invalid if no starting single-quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID b\">" },        // invalid if no starting double-quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID \"b\'>" },      // invalid if starting quote does not match ending quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED \'&;\'>" },     // value is missing an entity-reference.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED \'& abc;\'>" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED \'&abc ;\'>" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED \'&abc\'>" },   // value with entity-reference is missing a semicolon.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED \'b>" },        // invalid if no ending single-quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED \"b>" },        // invalid if no ending double-quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED b\'>" },        // invalid if no starting single-quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED b\">" },        // invalid if no starting double-quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED \"b\'>" },      // invalid if starting quote does not match ending quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED \'b\">" },      // invalid if starting quote does not match ending quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED A>" },          // invalid if name only.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED \'>" },         // invalid if nothing after single-quote.
    { ParseInfo::NotParsed, "<!ATTLIST abc def ID #FIXED \">" },         // invalid if nothing after double-quote.
    { ParseInfo::CantStart, "" },             // invalid if empty.
    { ParseInfo::CantStart, NULL }            // invalid if NULL.
};

const unsigned long s_attListDeclTestCount =
    sizeof(s_attListDeclTestCases) / sizeof(s_attListDeclTestCases[0]);

// ----------------------------------------------------------------------------

AttListDeclTester::AttListDeclTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo,
    ::Parser::Xml::IAttributeValueReceiver * attributeValueReceiver,
    ::Parser::Xml::IEnumeratedTypeReceiver * enumeratedTypeReceiver ) :
    IAttListDeclReceiver(),
    TestBase( "Attribute List Declaration", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser ),
    m_attributeValueReceiver( attributeValueReceiver ),
    m_enumeratedTypeReceiver( enumeratedTypeReceiver )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

AttListDeclTester::~AttListDeclTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool AttListDeclTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_attListDeclTestCases, s_attListDeclTestCount );
}

// ----------------------------------------------------------------------------

bool AttListDeclTester::OnParse( const char * begin, const char * end, unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    m_pParser->SetErrorReceiver( errorCounter );
    IAttListDeclReceiver * receiver =
        dynamic_cast< IAttListDeclReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParseAttListDecl( begin, end, receiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

bool AttListDeclTester::SetName( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string name( begin, end-begin );
        cout << "Name: [" << name << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool AttListDeclTester::SetAttName( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string name( begin, end-begin );
        cout << "Att Name: [" << name << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool AttListDeclTester::AddNotateName( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string name( begin, end-begin );
        cout << "Notation Name: [" << name << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool AttListDeclTester::SetAttType( AttType type )
{
    assert( this != NULL );

    if ( ShowContent() )
    {
        const char * name = ::Parser::Xml::GetAttDeclTypeName( type );
        cout << "Attribute Type: [" << name << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool AttListDeclTester::SetDefaultDeclType( DefaultDeclType type )
{
    assert( this != NULL );

    if ( ShowContent() )
    {
        const char * name = ::Parser::Xml::GetDefaultDeclTypeName( type );
        cout << "Default Declaration Type: [" << name << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

::Parser::Xml::IAttributeValueReceiver * AttListDeclTester::AddAttributeValue( void )
{
    assert( this != NULL );
    return m_attributeValueReceiver;
}

// ----------------------------------------------------------------------------

::Parser::Xml::IEnumeratedTypeReceiver * AttListDeclTester::AddEnumeratedType( void )
{
    assert( this != NULL );
    return m_enumeratedTypeReceiver;
}

// ----------------------------------------------------------------------------

void AttListDeclTester::DoneAttListDecl( bool valid,
    const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    (void)begin;
    (void)end;

    if ( ShowInfo() )
    {
        const char * result = ( valid ) ? "yes" : "no";
        cout << "Done! Valid: [" << result << ']' << endl;
    }
}

// ----------------------------------------------------------------------------

// $Log: PrologTesters.cpp,v $
// Revision 1.1  2008/12/05 19:04:15  rich_sposato
// Adding xml parsing files to project.
//
