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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/Tester/main.cpp,v 1.1 2008/12/05 19:04:15 rich_sposato Exp $


#include <string.h>

#include <vector>
#include <iomanip>
#include <iostream>
#include <strstream>

#include "../include/XmlParser.hpp"

#include "../../Util/include/ErrorReceiver.hpp"
#include "../../Util/include/ParseInfo.hpp"
#include "../../Util/include/TestUtil.hpp"

#include "BasicTesters.hpp"
#include "PrologTesters.hpp"
#include "CommandLineArgs.hpp"


using namespace std;
using namespace Parser;

// ----------------------------------------------------------------------------

/// @todo Add command line option to find included files.

void ShowHelp( const char * myName )
{
    cout << "Usage: " << myName << " [-h] [-u[:#]] [-c] [-s] [-t] [-w:#] [-W] [-f:filename]\n";
    cout << "  You must use either -h, -u or -f command.\n";
    cout << "    -c = Show content of unit tests or file.\n";
    cout << "    -f = Test file for validity.  Use absolute path.\n";
    cout << "    -i = Show info on each statement.\n";
    cout << "    -h = Show this help info.\n";
    cout << "    -m = Be mean to parser during unit tests.  This checks if parser\n";
    cout << "         behaves robustly in adverse conditions.  Requires -u command.\n";
    cout << "         The mean behaviors are:\n";
    cout << "         * Start parser while it is currently parsing.\n";
    cout << "         * Throw std exceptions at random moments.\n";
    cout << "         * Throw unknown exceptions at random moments.\n";
    cout << "         * Provide NULLs sometimes instead of valid pointers.\n";
    cout << "         * Return false to stop parser from providing content.\n";
    cout << "    -s = Show summary info after each unit test.\n";
    cout << "    -t = Show table of all results after all unit tests." << endl;
    cout << "    -w = Set warning level to 0 through 7. Default is -w:1\n";
    cout << "         Shows warnings at specified level or higher.\n";
    cout << "         0 = No messages.\n";
    cout << "         1 = Informational message.\n";
    cout << "         2 = Content provided with warning or error messages.\n";
    cout << "         3 = Warning message.\n";
    cout << "         4 = Minor syntax error.\n";
    cout << "         5 = Major syntax error.\n";
    cout << "         6 = Fatal error. Syntax is invalid. Unable to continue parsing.\n";
    cout << "         7 = Show exception messages.\n";
    cout << "    -u = Execute unit tests instead of testing input file.\n";
    cout << "         Optional number indicates index of unit test.\n";
    cout << "    -W = Wait for human response after unit tests finish.\n";
}

// ----------------------------------------------------------------------------

void ShowUsage( const char * myName )
{
    cout << "Usage: " << myName << " [-W] [-h] [-u] [-m] [-c] [-s] [-t] [-w:#] [-f:filename]\n";
}

// ----------------------------------------------------------------------------

static Parser::Xml::XmlParser * s_pParser = NULL;

void DestroyXmlParser( void )
{
//    cout << __FUNCTION__ << '\t' << __LINE__ << endl;
    if ( NULL != s_pParser )
    {
//        cout << __FUNCTION__ << '\t' << __LINE__ << endl;
        delete s_pParser;
//        cout << __FUNCTION__ << '\t' << __LINE__ << endl;
        s_pParser = NULL;
    }
//    cout << __FUNCTION__ << '\t' << __LINE__ << endl;
}

// ----------------------------------------------------------------------------

Parser::Xml::XmlParser::ParseResults MakeXmlParser( void )
{
    if ( NULL == s_pParser )
    {
        s_pParser = new Parser::Xml::XmlParser;
        if ( NULL == s_pParser )
            return Parser::Xml::XmlParser::CantMakeParser;
        ::atexit( DestroyXmlParser );
    }
    return Parser::Xml::XmlParser::AllValid;
}

// ----------------------------------------------------------------------------
/// @class XmlParserTester

class XmlParserTester
{
public:

    XmlParserTester( const CommandLineArgs & argInfo );
    ~XmlParserTester( void );

    void DoTests( void );

private:
    typedef ::std::vector< Parser::TestBase * > TesterSet;
    typedef TesterSet::iterator TesterSetIter;
    typedef TesterSet::const_iterator TesterSetCIter;

    XmlParserTester( void );
    XmlParserTester( const XmlParserTester & );
    XmlParserTester & operator = ( const XmlParserTester & );

    void DoUnitTest( void );

    void DoUnitTests( void );

    void DoFileTests( void );

    void ShowSummaryTable( void );

    bool DoTest( unsigned long & itemPassCount, unsigned long & itemFailCount,
        Parser::TestBase & tester );

    CommentTester           m_commentTest;
    PublicIdLiteralTester   m_publicIdTest;
    ExternalIdLiteralTester m_externalIdTest;
    NameTester              m_nameTest;
    PeReferenceTester       m_peReferenceTest;
    ReferenceTester         m_referenceTest;
    AttributeValueTester    m_attributeValueTest;
    AttributeTester         m_attributeTest;
    EnumeratedTypeTester    m_enumeratedTypeTester;
    EntityValueTester       m_entityValueTest;
    EncodingTester          m_encodingTester;
    XmlDeclarationTester    m_xmlDeclarationTester;
    AttListDeclTester       m_attListDeclTester;
//    FileTester m_fileTester;

   TesterSet m_testers;
};

// ----------------------------------------------------------------------------

XmlParserTester::XmlParserTester( const CommandLineArgs & argInfo ) :
    m_commentTest( s_pParser, argInfo ),
    m_publicIdTest( s_pParser, argInfo ),
    m_externalIdTest( s_pParser, argInfo ),
    m_nameTest( s_pParser, argInfo ),
    m_peReferenceTest( s_pParser, argInfo ),
    m_referenceTest( s_pParser, argInfo ),
    m_attributeValueTest( s_pParser, argInfo ),
    m_attributeTest( s_pParser, argInfo ),
    m_enumeratedTypeTester( s_pParser, argInfo ),
    m_entityValueTest( s_pParser, argInfo ),
    m_encodingTester( s_pParser, argInfo ),
    m_xmlDeclarationTester( s_pParser, argInfo ),
    m_attListDeclTester( s_pParser, argInfo, &m_attributeValueTest,
        &m_enumeratedTypeTester )
{
    assert( this != NULL );

    m_testers.reserve( 32 );
    m_testers.push_back( &m_commentTest );
    m_testers.push_back( &m_publicIdTest );
    m_testers.push_back( &m_externalIdTest );
    m_testers.push_back( &m_nameTest );
    m_testers.push_back( &m_peReferenceTest );
    m_testers.push_back( &m_referenceTest );
    m_testers.push_back( &m_attributeValueTest );
    m_testers.push_back( &m_attributeTest );
    m_testers.push_back( &m_enumeratedTypeTester );
    m_testers.push_back( &m_entityValueTest );
    m_testers.push_back( &m_encodingTester );
    m_testers.push_back( &m_xmlDeclarationTester );
    m_testers.push_back( &m_attListDeclTester );
}

// ----------------------------------------------------------------------------

XmlParserTester::~XmlParserTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void XmlParserTester::DoTests( void )
{
    assert( this != NULL );
    const CommandLineArgs & argInfo = CommandLineArgs::GetCommandLineArgs();
    if ( argInfo.DoUnitTests() )
    {
        if ( argInfo.DoSpecificUnitTest() )
            DoUnitTest();
        else
            DoUnitTests();
    }
    if ( argInfo.DoFileTests() )
        DoFileTests();
}

// ----------------------------------------------------------------------------

void XmlParserTester::DoUnitTest( void )
{
    assert( this != NULL );

    const CommandLineArgs & argInfo = CommandLineArgs::GetCommandLineArgs();
//    const bool doMeanTests = argInfo.DoMeanTests();
    if ( argInfo.DoShowSummary() )
        cout << "\n XML Parser UnitTests\n";

    const unsigned int index = argInfo.GetUnitTestIndex();
    const unsigned int maxIndex = static_cast< unsigned int >( m_testers.size() ) - 1;
    if ( maxIndex < index )
    {
        cout << "Invalid index for unit test.  Max index is " << maxIndex
             << ".\n";
        return;
    }

    unsigned long failCount = 0;
    unsigned long passCount = 0;
    unsigned long itemFailCount = 0;
    unsigned long itemPassCount = 0;

    TesterSetIter it( m_testers.begin() );
    it += index;
    Parser::TestBase * test = *it;
    if ( DoTest( itemPassCount, itemFailCount, *test ) )
        ++passCount;
    else
        ++failCount;

    if ( argInfo.DoShowSummary() )
    {
        const char * result = ( 0 == failCount ) ? "Pass" : "Fail";
        cout << "\n\nItem Totals:\tPassed: [" << itemPassCount << "]\tFailed: ["
            << itemFailCount <<"]\tTotal: [" << itemFailCount + itemPassCount << "]\n";
        cout << "\nTotals:\tPassed: [" << passCount << "]\tFailed: [" << failCount
            << "]\tTotal: [" << failCount + passCount << "]\n";
        cout << "Summary: " << result << "\n";
    }
}

// ----------------------------------------------------------------------------

void XmlParserTester::ShowSummaryTable( void )
{
    assert( this != NULL );

    cout << endl;
    const char * const spaces = "                              ";
    //       0         1         2         3         4         5
    //       012345678901234567890123456789012345678901234567890
    cout << "  #  Unit Test Name                 Pass  Fail  Total   Result" << endl;
    cout << "--------------------------------------------------------------" << endl;

    char buffer[ 2048 ];
    unsigned long ii = 0;
    unsigned long totalPassCount = 0;
    unsigned long totalFailCount = 0;
    unsigned long totalAllCount = 0;
    unsigned long testPassCount = 0;
    unsigned long testFailCount = 0;
    TesterSetIter it( m_testers.begin() );
    const TesterSetIter end( m_testers.end() );
    const char * result = "unknown";

    while ( it != end )
    {
        strstream content( buffer, sizeof(buffer) );
        Parser::TestBase * test = *it;
        const unsigned long passCount = test->GetPassCount();
        const unsigned long failCount = test->GetFailCount();
        const unsigned long allCount  = test->GetCount();
        const char * const name = test->GetTestName();
        const unsigned long nameSize = ::strlen( name );
        const char * const filler = ( nameSize < 28 ) ? spaces + nameSize : " ";
        result =
            ( 0 == allCount  ) ? "Empty!" :
            ( 0 == failCount ) ? "Passed!" : "FAILED!";
        content << setw( 3 ) << setfill( ' ' ) << ii << "  "
            << name << filler
            <<         setw( 4 ) << setfill( ' ' ) << passCount
            << '\t' << setw( 4 ) << setfill( ' ' ) << failCount
            << '\t' << setw( 4 ) << setfill( ' ' ) << allCount
            << '\t' << result << endl << ends;
        cout << buffer << flush;
        totalPassCount += passCount;
        totalFailCount += failCount;
        totalAllCount  += allCount;
        if ( 0 != failCount )
            testFailCount++;
        else
            testPassCount++;
        ++it;
        ++ii;
    }

    cout << "---------------------------------------------------------------" << endl;
    result =
        ( 0 == totalAllCount  ) ? "Empty!" :
        ( 0 == totalFailCount ) ? "Passed!" : "FAILED!";
    {
        strstream totalLine( buffer, sizeof(buffer) );
        totalLine << setw( 3 ) << ii << "  Item Totals:                  "
            <<         setw( 4 ) << setfill( ' ' ) << totalPassCount
            << '\t' << setw( 4 ) << setfill( ' ' ) << totalFailCount
            << '\t' << setw( 4 ) << setfill( ' ' ) << totalAllCount
            << '\t' << result << endl << ends;
        cout << buffer << flush;
    }
    {
        strstream totalLine( buffer, sizeof(buffer) );
        totalLine << "     Unit Test Totals:             "
            <<         setw( 4 ) << setfill( ' ' ) << testPassCount
            << '\t' << setw( 4 ) << setfill( ' ' ) << testFailCount
            << '\t' << setw( 4 ) << setfill( ' ' ) << ii
            << '\t' << result << endl << ends;
        cout << buffer << flush;
    }
}

// ----------------------------------------------------------------------------

void XmlParserTester::DoUnitTests( void )
{
    assert( this != NULL );

    unsigned long failCount = 0;
    unsigned long passCount = 0;
    unsigned long itemFailCount = 0;
    unsigned long itemPassCount = 0;

    const CommandLineArgs & argInfo = CommandLineArgs::GetCommandLineArgs();
//    const bool doMeanTests = argInfo.DoMeanTests();
    if ( argInfo.DoShowSummary() )
        cout << "\n XML Parser UnitTests\n";

//    m_fileTester.SetGiveIncludePath( false );

    TesterSetIter it( m_testers.begin() );
    const TesterSetIter end( m_testers.end() );
    while ( it != end )
    {
        Parser::TestBase * test = *it;
        if ( DoTest( itemPassCount, itemFailCount, *test ) )
            ++passCount;
        else
            ++failCount;
        ++it;
    }

    if ( argInfo.DoShowTable() )
    {
        ShowSummaryTable();
    }

    if ( argInfo.DoShowSummary() )
    {
        const char * result = ( 0 == failCount ) ? "Pass" : "Fail";
        cout << "\n\nItem Totals:\tPassed: [" << itemPassCount << "]\tFailed: ["
            << itemFailCount <<"]\tTotal: [" << itemFailCount + itemPassCount << "]\n";
        cout << "\nTotals:\tPassed: [" << passCount << "]\tFailed: [" << failCount
            << "]\tTotal: [" << failCount + passCount << "]\n";
        cout << "Summary: " << result << "\n";
    }
}

// ----------------------------------------------------------------------------

bool XmlParserTester::DoTest( unsigned long & itemPassCount,
    unsigned long & itemFailCount, Parser::TestBase & tester )
{
    assert( this != NULL );

    const char * testName = NULL;
    const CommandLineArgs & argInfo = CommandLineArgs::GetCommandLineArgs();
    if ( argInfo.DoShowSummary() )
    {
        testName = tester.GetTestName();
        cout << "\n" << testName << " Parse Test\n";
    }

//    if ( !tester.SetupTest() )
//    {
//        cout << "Unable to do tests.\n";
//        return false;
//    }
    const bool passed = tester.Test( );
    if ( argInfo.DoShowSummary() )
    {
        const char * result = passed ? "Pass" : "Fail";
        cout << testName << " Test: [" << result << "]\n";
    }

    itemFailCount += tester.GetFailCount();
    itemPassCount += tester.GetPassCount();

    return passed;
}

// ----------------------------------------------------------------------------

void XmlParserTester::DoFileTests( void )
{
    assert( this != NULL );
    const CommandLineArgs & argInfo = CommandLineArgs::GetCommandLineArgs();
    const char * filename = argInfo.GetFileName();
//    unsigned long parseLength = 0;

       if ( argInfo.DoShowSummary() )
        cout << "\n XML Parser File Test: [" << filename << "]\n";

//    m_fileTester.SetGiveIncludePath( true );

//    parser.SetErrorReceiver( m_fileTester.AsErrorReceiver() );
    Parser::ParseInfo::ParseResult result = Parser::ParseInfo::AllValid;
//    Parser::ParseInfo::ParseResult result =
//        parser.ParseFile( filename, parseLength, &m_fileTester );

       if ( argInfo.DoShowSummary() )
    {
        const bool valid = ( Parser::ParseInfo::AllValid == result )
                        || ( Parser::ParseInfo::SomeValid == result );
        const bool parsed = valid || ( Parser::ParseInfo::NotValid == result );
        const char * validResult = valid ? "yes" : "no";
        const char * parseResult = parsed ? "yes" : "no";
        cout << "Parsed: [" << parseResult << "]\tValid: ["
             << validResult << "]\n";
        cout << "Result: [" << Parser::ParseInfo::Name( result )
             << "] of parsing file: " << filename << "\n";
    }
}

// ----------------------------------------------------------------------------

int main( unsigned int argc, const char * argv[] )
{

    const CommandLineArgs argInfo( argc, argv );
    if ( !argInfo.IsValid() )
    {
        ShowUsage( argInfo.GetExePath() );
        return -1;
    }
    else if ( argInfo.DoShowHelp() )
    {
        ShowHelp( argInfo.GetExePath() );
    }
    else
    {
        const Parser::Xml::XmlParser::ParseResults result = MakeXmlParser();
        if ( result != Parser::Xml::XmlParser::AllValid )
        {
            cout << "Failed to create Xml Parser!\n";
            return 1;
        }
        XmlParserTester tester( argInfo );
        tester.DoTests();
    }

    if ( argInfo.DoWait() )
    {
        ::system( "pause" );
    }

    return 0;
}

// ----------------------------------------------------------------------------

// $Log: main.cpp,v $
// Revision 1.1  2008/12/05 19:04:15  rich_sposato
// Adding xml parsing files to project.
//
