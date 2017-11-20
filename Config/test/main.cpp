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

// $Header: /cvsroot/utilparsers/UtilParsers/Config/test/main.cpp,v 1.5 2009/01/05 19:24:52 rich_sposato Exp $

/// @file main.cpp Test for config file parser.

/// @todo Add ability to parse files using command line interface.

/// @todo Add mean testing actions.


// ----------------------------------------------------------------------------
// Include Files

#include <iostream>
#include <stdlib.h>

#include "../../Util/include/ParseUtil.hpp"
#include "../include/ConfigParser.hpp"

#include "ConfigTester.hpp"


// ----------------------------------------------------------------------------
// Namespace resolution.

using namespace ::std;
using namespace ::Parser;

namespace
{


// ----------------------------------------------------------------------------

// Test cases for simple names, keys, and values.
const TestData s_BasicTestCases[] =
{
    { ParseInfo::AllValid,  "globalname\n" },  // 0
    { ParseInfo::AllValid,  "[Section1]\n" },  // 1
    { ParseInfo::AllValid,  "[Section1]\n"     // 2
                    "[Section2]" },
    { ParseInfo::AllValid,  "[Section1]\n\n"   // 3
                    "[Section2]\n" },
    { ParseInfo::AllValid,  "[Section1]\n"     // 4
                    "key1\n" },
    { ParseInfo::AllValid,  "[Section1]\n"     // 5
                    "key1 = value1\n\n"
                    "[Section2]\n"
                    "key2\n" },
    { ParseInfo::AllValid,  " globalname \n" }, // 6
    { ParseInfo::AllValid,  " [Section1]\n" },  // 7
    { ParseInfo::AllValid,  "[ Section1]\n"     // 8
                    " [ Section2 ]" },
    { ParseInfo::AllValid,  "[Section1 ] \n"    // 9
                    "[Section2]\n" },
    { ParseInfo::AllValid,  " [\t Section1 \t ] \n"  // 10
                    "\t key1\n" },
    { ParseInfo::AllValid,  "[Section1]\n"      // 11
                    "key1\t=\tvalue1   a      \t\n"
                    "\t[Section2]\n"
                    "\t\tkey2\t\n" },
    { ParseInfo::AllValid,  "globalname" },
    { ParseInfo::AllValid,  "[Section1]" },
    { ParseInfo::AllValid,  "[Section1] [Section2]" },
    { ParseInfo::AllValid,  "globalname\n"
                    "[Section1]\n"
                    "key1 = value1\n" },
    { ParseInfo::AllValid,  "[Section1] key1" },
    { ParseInfo::AllValid,  "globalkey1\n"
                    "globalkey2=\n"
                    "globalkey3=value3\n"
                    "[Section1]\n"
                    "key0\n"
                    "key1=\n"
                    "key2 =\n"
                    "key3 = \n\n"
                    "key4=value4\n"
                    "key5= value5\n"
                    "key6 =value6\n"
                    "key7 = value7\n" },
    { ParseInfo::NotValid,  "[Section1" },
    { ParseInfo::NotValid,  "Section1]" },
    { ParseInfo::NotParsed, "" },
    { ParseInfo::NotParsed, NULL },
};

const unsigned long s_BasicTestCount = sizeof( s_BasicTestCases ) / sizeof( s_BasicTestCases[0] );


// ----------------------------------------------------------------------------

const TestData s_QuoteTestCases[] =
{
    { ParseInfo::AllValid, "globalkey1\n"
                    "globalkey2=\n"
                    "globalkey3=value3\n" },
    { ParseInfo::AllValid, "globalkey=\"value\"\n"
                    "gk2 = \" v2 \" \n" },
    { ParseInfo::AllValid, "comment_in_quotes = \" stuff ; not-a-comment \"\n"
                    "no_comment_here = \" stuff \" ; comment \n" },
    { ParseInfo::AllValid, "empty_quotes = \"\"\n"
                    "spaced_quotes = \" \"\n" },
    { ParseInfo::NotValid, "no_ending_quote = \" v1 \n" },
    { ParseInfo::NotValid, "no_ending_quote = \"\n" },
};

const unsigned long s_QuoteTestCount = sizeof( s_QuoteTestCases ) / sizeof( s_QuoteTestCases[0] );


// ----------------------------------------------------------------------------

const TestData s_CommentTestCases[] =
{
    { ParseInfo::AllValid,  "; comment\n" },
    { ParseInfo::AllValid,  "/* comment */\n" },
    { ParseInfo::AllValid,  "/* comment1 */ ; comment2\n" },
    { ParseInfo::AllValid,  "[Section1]; comment\n"
                    "key1=value1\n"
                    "[Section2] ; comment\n"
                    "/* comment */\n"
                    "key2\n"
                    "quoted_comment = \" contents; more contents \" ; comment\n" },
    { ParseInfo::AllValid,  "globalkey1\n"
                    "globalkey2=\n"
                    "globalkey3=value3\n"
                    "[Section1]\n"
                    "key0\n"
                    "key1=\n"
                    "key2 = ; comment\n"
                    "key3 = /* comment */\n\n"
                    "key4=value4\n"
                    "key5= value5\n"
                    "key6 =value6 ; comment\n"
                    "key7 = value7 /* comment */\n" },
    { ParseInfo::NotValid,  "/* bad block comment /" },
    { ParseInfo::NotValid,  "/* bad block comment " },
    { ParseInfo::NotValid,  "/* bad block comment *" },
    { ParseInfo::NotValid,  "/* bad block comment * /" },
    { ParseInfo::NotValid,  "[" },
    { ParseInfo::NotValid,  "]" },
    { ParseInfo::NotValid,  "[]" },
};

const unsigned long s_CommentTestCount =
    sizeof( s_CommentTestCases ) / sizeof( s_CommentTestCases[0] );


// ----------------------------------------------------------------------------

const TestData s_NonAlphaNameTestCases[] =
{
    { ParseInfo::AllValid,  "global*name\n" },  // 0
    { ParseInfo::AllValid,  "[@Section1]\n" },  // 1
    { ParseInfo::AllValid,  "[#Section1]\n"     // 2
                    "[-Section2]" },
    { ParseInfo::AllValid,  "[Section1]\n"     // 3
                    "[Section2]\n" },
    { ParseInfo::AllValid,  "[Section1]\n"     // 4
                    "key1\n" },
    { ParseInfo::AllValid,  "[Section1]\n"     // 5
                    "key1 (*.txt) = value1 *.txt\n"
                    "[Section2]\n"
                    "key2\n" },
    { ParseInfo::AllValid,  " globalname \n" }, // 6
    { ParseInfo::AllValid,  " [Section1]\n" },  // 7
    { ParseInfo::AllValid,  "[ Section1]\n"     // 8
                    " [ Section2 ]" },
    { ParseInfo::AllValid,  "[Section1 ] \n"    // 9
                    "[Section2]\n" },
    { ParseInfo::AllValid,  " [\t Section1 \t ] \n"  // 10
                    "\t key1\n" },
    { ParseInfo::AllValid,  "[Section1]\n"      // 11
                    "key1\t=\tvalue1   a\t\n"
                    "\t[Section2]\n"
                    "\t\tkey2\t\n" },
    { ParseInfo::AllValid,  "globalname" },
    { ParseInfo::AllValid,  "[Section1]" },
    { ParseInfo::AllValid,  "[Section1] [Section2]" },
    { ParseInfo::AllValid,  "globalname\n"
                    "[Section1]\n"
                    "key1 = value1?\n" },
    { ParseInfo::AllValid,  "[Section1] key1" },
    { ParseInfo::AllValid,  "globalkey1\n"
                    "globalkey2=\n"
                    "globalkey3=value3\n"
                    "[Section1]\n"
                    "key0\n"
                    "key1=\n"
                    "key2 =\n"
                    "key3 = \n\n"
                    "key4=value4\n"
                    "key5= value5\n"
                    "key6 =value6\n"
                    "key7 = value7\n" },
    { ParseInfo::AllValid,  "Section1]" },
    { ParseInfo::NotValid,  "[Section1" },
};

const unsigned long s_NonAlphaNameTestCount =
    sizeof( s_NonAlphaNameTestCases ) / sizeof( s_NonAlphaNameTestCases[0] );


// ----------------------------------------------------------------------------

void ShowHelp( const char * myName )
{
    cout << "Usage: " << myName << " [-h] [-u] [-c] [-s] [-w:#] [-f:filename]\n";
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
    cout << "         * Return false to stop parser from providing content.\n";
    cout << "    -s = Show summary info.\n";
    cout << "    -w = Set warning level to 1 through 6. Default is -w:1\n";
    cout << "         Shows warnings at specified level or higher.\n";
    cout << "         1 = Informational message.\n";
    cout << "         2 = Content provided with warning or error messages.\n";
    cout << "         3 = Warning message.\n";
    cout << "         4 = Minor syntax error.\n";
    cout << "         5 = Major syntax error.\n";
    cout << "         6 = Fatal error. Syntax is invalid. Unable to continue parsing.\n";
    cout << "         7 = Show no error messages.\n";
    cout << "    -u = Execute unit tests\n";
}

// ----------------------------------------------------------------------------

void ShowUsage( const char * myName )
{
    cout << "Usage: " << myName << " [-h] [-u] [-m] [-c] [-s] [-w:#] [-f:filename]\n";
}

// ----------------------------------------------------------------------------

bool CheckParameters( unsigned int argc, const char * argv[], bool & doUnitTest,
    bool & doFileTest, bool & doMeanTests, bool & showContent, bool & showSummary,
    bool & showInfo, Parser::ErrorLevel::Levels & showErrorLevel, const char * & filename )
{
    if ( argc < 2 ) return false;

    bool showHelp = false;
    bool validCommands = true;
    const char * info = NULL;
    unsigned long length = 0;
    int level = 0;

    const char * myName = ::strrchr( argv[0], '/' );
    if ( ( NULL == myName ) || ( 0 == *myName ) ) myName = ::strrchr( argv[0], '\\' );
    if ( ( NULL == myName ) || ( 0 == *myName ) ) myName = "ConfigParser.exe"; else ++myName;

    for ( unsigned long ai = 1; ( validCommands ) && ( ai < argc ); ++ai )
    {
        length = static_cast< unsigned long >( ::strlen( argv[ ai ] ) );
        if ( ( *argv[ ai ] != '-' ) || ( length < 2 ) )
        {
            validCommands = false;
            break;
        }
        switch ( argv[ ai ][ 1 ] )
        {
            case 'w':
                if ( ( length != 4 )
                  || ( ':' != argv[ ai ][ 2 ] )
                  || ( !::isdigit( argv[ ai ][ 3 ] ) ) )
                    validCommands = false;
                else
                {
                    info = &( argv[ ai ][ 3 ] );
                    level = ::atoi( info );
                    if ( ( level < 1 ) || ( 7 < level ) )
                        validCommands = false;
                    else
                        showErrorLevel = static_cast< ::Parser::ErrorLevel::Levels >( level );
                }
                break;
            case 'f':
                if ( length < 4 )
                    validCommands = false;
                else
                {
                    filename = &( argv[ ai ][ 3 ] );
                    doFileTest = true;
                }
                break;
            case 'h': showHelp      = true;  break;
            case 'i': showInfo      = true;  break;
            case 's': showSummary   = true;  break;
            case 'u': doUnitTest    = true;  break;
            case 'c': showContent   = true;  break;
            case 'm': doMeanTests   = true;  break;
            default:  validCommands = false; break;
        }
    }

    if ( !doUnitTest && !doFileTest ) validCommands = false;
    if ( showHelp )
        ShowHelp( myName );
    else if ( !validCommands )
        ShowUsage( myName );

    return validCommands;
}

// ----------------------------------------------------------------------------

bool DoUnitTests( ConfigTester & tester, ConfigParser & parser )
{

    ConfigParser::ParserPolicy policy;
    policy.LineComment = ";";
    policy.BlockCommentStarter = "/*";
    policy.BlockCommentEnder = "*/";
    policy.SectionNameStarter = "[";
    policy.SectionNameEnder = "]";
    policy.AssignOperator = "=";
    policy.TrimWhiteSpace = true;
    policy.AlphaNumericNames = true;
    policy.AllowQuotedCommentInValue = false;
    policy.MaxErrorCount = 3;
    parser.SetPolicy( policy );

    bool passed = tester.Test( s_BasicTestCases, s_BasicTestCount );

//    MessageStack::SetDebugging( true );

    passed = tester.Test( s_CommentTestCases, s_CommentTestCount );

//    MessageStack::SetDebugging( false );

    policy.AlphaNumericNames = false;
    parser.SetPolicy( policy );
    passed = tester.Test( s_NonAlphaNameTestCases, s_NonAlphaNameTestCount );

    policy.AllowQuotedCommentInValue = true;
    parser.SetPolicy( policy );
    passed = tester.Test( s_QuoteTestCases, s_QuoteTestCount );

    return passed;
}

// ----------------------------------------------------------------------------

bool DoFileTests( ConfigTester & tester, ConfigParser & parser, const char * filename,
    bool showSummary )
{

    IConfigReceiver * pReceiver = dynamic_cast< IConfigReceiver * >( &tester );
    if ( NULL == pReceiver )
        return false;

    ConfigParser::ParserPolicy policy;
    policy.LineComment = ";";
    policy.BlockCommentStarter = "/*";
    policy.BlockCommentEnder = "*/";
    policy.SectionNameStarter = "[";
    policy.SectionNameEnder = "]";
    policy.AssignOperator = "=";
    policy.TrimWhiteSpace = true;
    policy.AlphaNumericNames = false;
    policy.AllowQuotedCommentInValue = true;
    policy.MaxErrorCount = 3;

    bool valid = false;
    ConfigParser::ParseResults result = ConfigParser::NotParsed;

    try
    {
        parser.SetPolicy( policy );
        parser.SetMessageReceiver( tester.AsErrorReceiver() );
        result = parser.Parse( filename, pReceiver );
    }
    catch ( ... )
    {
    }

    valid = ( ConfigParser::AllValid == result ) || ( ConfigParser::SomeValid == result );
       if ( showSummary )
    {
        const bool parsed = valid || ( ConfigParser::NotValid == result );
        const char * validResult = valid ? "yes" : "no";
        const char * parseResult = parsed ? "yes" : "no";
        cout << "Parsed: [" << parseResult << "]\tValid: [" << validResult << "]\n";
        cout << "Result: [" << ConfigParser::Name( result )
            << "] of parsing file: " << filename << "\n";
    }

    return valid;
}

// ----------------------------------------------------------------------------

}; // end anonymous namespace

// ----------------------------------------------------------------------------

int main( int argc, const char * argv[] )
{

    bool doUnitTest = false;
    bool doFileTest = false;
    bool showContent = false;
    bool showSummary = false;
    bool doMeanTests = false;
    bool showInfo = false;
    ErrorLevel::Levels showErrorLevel = ErrorLevel::Info;
    const char * filename = NULL;

    if ( !CheckParameters( argc, argv, doUnitTest, doFileTest, doMeanTests,
        showContent, showSummary, showInfo, showErrorLevel, filename ) )
        return 1;

    if ( !doUnitTest && !doFileTest )
    {
        ShowUsage( argv[0] );
        return 1;
    }

    ConfigParser parser;
    ConfigTester tester( parser, showErrorLevel, showContent, showInfo, doMeanTests );
    bool passed = false;

    if ( doUnitTest )
        passed = DoUnitTests( tester, parser );

    if ( doFileTest )
        passed = DoFileTests( tester, parser, filename, showSummary );

    return passed ? 0 : 1;
}

// ----------------------------------------------------------------------------

// $Log: main.cpp,v $
// Revision 1.5  2009/01/05 19:24:52  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.4  2008/12/09 19:44:39  rich_sposato
// Changed policy names.  Added ability to do mean tests to parser.
//
// Revision 1.3  2008/12/08 23:11:43  rich_sposato
// A few changes to ConfigParser to make it more versatile and robust.
//
// Revision 1.2  2008/12/06 08:50:12  rich_sposato
// Changes to improve debugging abilities or fix a bug.
//
// Revision 1.1  2008/12/05 19:25:28  rich_sposato
// Adding files to CVS.
//
