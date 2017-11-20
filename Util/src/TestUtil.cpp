// ----------------------------------------------------------------------------
// The Parser Utility Library
// Copyright (c) 2005, 2006, 2007, 2008 by Rich Sposato
//
// Permission to use, copy, modify, distribute and sell this software for any
// purpose is hereby granted under the terms stated in the GNU Library Public
// License, provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear in
// supporting documentation.
//
// ----------------------------------------------------------------------------

// $Header: /cvsroot/utilparsers/UtilParsers/Util/src/TestUtil.cpp,v 1.2 2009/01/05 19:25:48 rich_sposato Exp $

/// @file TestUtil.cpp Contains functions used for unit testing parsers.


// ----------------------------------------------------------------------------

#include "../include/TestUtil.hpp"

#include <assert.h>
#include <iostream>


// ----------------------------------------------------------------------------

using namespace std;


// ----------------------------------------------------------------------------

Parser::ErrorReceiver::ErrorReceiver( Parser::ErrorLevel::Levels showErrorLevel,
    bool showContent ) :
    IParseErrorReceiver(),
    m_highest( Parser::ErrorLevel::None ),
    m_showErrorLevel( showErrorLevel ),
    m_count( 0 ),
    m_showContent( showContent )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

Parser::ErrorReceiver::~ErrorReceiver( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool Parser::ErrorReceiver::GiveParseMessage( Parser::ErrorLevel::Levels level,
    const CharType * message )
{
    assert( this != NULL );
//    cout << __FUNCTION__ << '\t' << __LINE__ << endl;
    if ( m_showErrorLevel <= level )
        cout << Parser::ErrorLevel::Name( level ) << ": " << message << "\n";
    if ( m_highest < level )
        m_highest = level;
    ++m_count;
    return true;
}

// ----------------------------------------------------------------------------

bool Parser::ErrorReceiver::GiveParseMessage( Parser::ErrorLevel::Levels level,
    const CharType * message, unsigned long lineCount )
{
    assert( this != NULL );
//    cout << __FUNCTION__ << '\t' << __LINE__ << endl;
    if ( m_showErrorLevel <= level )
        cout << Parser::ErrorLevel::Name( level ) << ": Line:" << lineCount
             << " " << message << "\n";
    if ( m_highest < level )
        m_highest = level;
    ++m_count;
    return true;
}

// ----------------------------------------------------------------------------

bool Parser::ErrorReceiver::GiveParseMessage( Parser::ErrorLevel::Levels level,
    const CharType * message, const char * filename, unsigned long lineCount )
{
    assert( this != NULL );
//    cout << __FUNCTION__ << '\t' << __LINE__ << endl;
    if ( m_showErrorLevel <= level )
        cout << Parser::ErrorLevel::Name( level ) << ": " << filename
            << " Line:" << lineCount << " " << message << "\n";
    if ( m_highest < level )
        m_highest = level;
    ++m_count;
    return true;
}

// ----------------------------------------------------------------------------

Parser::TestBase::TestBase( const char * testName, bool showContent,
    ::Parser::ErrorLevel::Levels showErrorLevel, bool showInfo ) :
    m_Errors( showErrorLevel, showContent ),
    m_testName( testName ),
    m_failCount( 0 ),
    m_passCount( 0 ),
    m_count( 0 ),
    m_aData( NULL ),
    m_showInfo( showInfo )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

Parser::TestBase::~TestBase( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool Parser::TestBase::Test( const TestData * paData, unsigned long count )
{
    assert( this != NULL );

    bool okay = SetupTest( paData, count );
    if ( !okay )
        return false;
    okay = TestAll();
    return okay;
}

// ----------------------------------------------------------------------------

bool Parser::TestBase::SetupTest( const TestData * paData, unsigned long count )
{
    assert( this != NULL );

    if ( ( NULL == paData ) || ( 0 == count ) )
        return false;
    m_aData = paData;
    m_count = count;
    return true;
}

// ----------------------------------------------------------------------------

bool Parser::TestBase::CheckResults( unsigned long i,
    Parser::ParseInfo::ParseResult result, unsigned long errorCount )
{
    assert( this != NULL );

    const bool valid = ( Parser::ParseInfo::AllValid  == result )
                    || ( Parser::ParseInfo::SomeValid == result );
    const bool parsed = valid || ( Parser::ParseInfo::NotValid == result );
    const bool triedParsing = Parser::ParseInfo::ResultImpliesAttempt( result );
    const TestData & rData = m_aData[ i ];
    const char * validResult = valid ? "yes" : "no";
    const char * parseResult = ( parsed ) ? "yes" : "no";
    const bool matches = ( result == rData.m_result );
    const char * matchResult = matches ? "yes" : "no";
    if ( ShowInfo() || !matches )
    {
        if ( !ShowInfo() )
            cout << "\ntest case: [" << i << "]\t";
        cout << "Match: [" << matchResult << "]\tParsed: [" << parseResult
            << "]\tValid: [" << validResult << "]\tErrors: [" << errorCount << "]\n";
    }
    if ( triedParsing )
    {
        if ( ( errorCount == 0 ) && !valid )
            cout << "\tBad Test! Parser reported no errors, but content is not valid!\n";
        else if ( ( 0 < errorCount ) && valid )
            cout << "\tBad Test! Parser reported errors, but content is valid!\n";
    }
    if ( matches )
        ++m_passCount;
    else
        ++m_failCount;

    return matches;
}

// ----------------------------------------------------------------------------

bool Parser::TestBase::TestAll( void )
{
    assert( this != NULL );

    unsigned long matchCount = 0;
    for ( unsigned long ii = 0; ii < m_count; ++ii )
    {
        if ( ShowInfo() )
            cout << "\ntest case: [" << ii << "]\n";
        if ( DoOneTest( m_aData[ ii ], ii ) )
            ++matchCount;
    }

    const unsigned long failCount = m_count - matchCount;
    if ( ShowInfo() ) cout << "\n";
    cout << "Test Ratio: Pass: [" << matchCount << "]\tFail: [" << failCount
        << "]\tTotal: [" << m_count << "]\n";
    return ( matchCount == m_count );
}

// ----------------------------------------------------------------------------

bool Parser::TestBase::DoOneTest( const TestData & rData, unsigned long i )
{
    assert( this != NULL );

    m_Errors.Clear();
    const CharType * content = rData.m_content;
    const bool isNull = ( NULL == content );
    const char * pContent = reinterpret_cast< const char * >( content );
    const unsigned long length = isNull ? 0 : static_cast< unsigned long >
        ( ::strlen( pContent ) );
    const CharType * pNull = reinterpret_cast< const CharType * >( "NULL" );
    const CharType * output = isNull ? pNull : content;
    if ( ShowContent() )
        cout << "Content: [" << output << "]\n";
    return OnParse( content, content + length, i );
}

// ----------------------------------------------------------------------------

// $Log: TestUtil.cpp,v $
// Revision 1.2  2009/01/05 19:25:48  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.1  2008/12/05 19:12:28  rich_sposato
// Adding files to CVS.
//
// Revision 1.1  2008/03/02 02:50:31  rich_sposato
// Added project files & new files to CVS.  Numerous changes to source files.
//
