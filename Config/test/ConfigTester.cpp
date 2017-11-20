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

// $Header: /cvsroot/utilparsers/UtilParsers/Config/test/ConfigTester.cpp,v 1.5 2009/01/05 19:24:52 rich_sposato Exp $

/// @file ConfigTester.cpp Contains class used to test ConfigParser.


/// @todo Add comments to test cases.


// ----------------------------------------------------------------------------

#include "ConfigTester.hpp"

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>

#include "../include/ConfigParser.hpp"


// ----------------------------------------------------------------------------

using namespace ::std;
using namespace ::Parser;


// ----------------------------------------------------------------------------

ConfigTester::ConfigTester( ConfigParser & parser, ::ErrorLevel::Levels showErrorLevel,
    bool showContent, bool showInfo, bool doMeanTests ) :
    IConfigReceiver(),
    TestBase( "Config", showContent, showErrorLevel, showInfo ),
    m_parser( parser ),
    m_doMeanTests( doMeanTests )
{
    assert( NULL != this );
    if ( m_doMeanTests )
    {
        ::srand( ::time( NULL ) );
    }
}

// ----------------------------------------------------------------------------

ConfigTester::~ConfigTester( void )
{
    assert( NULL != this );
}

// ----------------------------------------------------------------------------

bool ConfigTester::AddGlobalKey( const char * keyStart, const char * keyEnd,
    const char * valueStart, const char * valueEnd )
{
    assert( NULL != this );
    if ( ShowContent() )
    {
        string key( keyStart, keyEnd - keyStart );
        string value( valueStart, valueEnd - valueStart );
        cout << "Config: Add Global Key: [" << key << "]\tValue: [" << value << "]\n";
    }
    if ( m_doMeanTests )
    {
        int number = ::rand();
        if ( ( number % 8 ) == 0 )
        {
            throw logic_error( "Can't accept global key!" );
        }
        number = ::rand();
        if ( ( number % 8 ) == 0 )
        {
            return false;
        }
    }

    return true;
}

// ----------------------------------------------------------------------------

bool ConfigTester::AddSection( const char * nameStart, const char * nameEnd )
{
    assert( NULL != this );
    if ( ShowContent() )
    {
        string name( nameStart, nameEnd - nameStart );
        cout << "Config: Add Section Name: [" << name << "]\n";
    }
    if ( m_doMeanTests )
    {
        int number = ::rand();
        if ( ( number % 8 ) == 0 )
        {
            throw logic_error( "Can't accept section name!" );
        }
        number = ::rand();
        if ( ( number % 8 ) == 0 )
        {
            return false;
        }
    }
    return true;
}

// ----------------------------------------------------------------------------

bool ConfigTester::AddSectionKey( const char * keyStart, const char * keyEnd,
    const char * valueStart, const char * valueEnd )
{
    assert( NULL != this );
    if ( ShowContent() )
    {
        string key( keyStart, keyEnd - keyStart );
        string value( valueStart, valueEnd - valueStart );
        cout << "Config: Add Section Key: [" << key << "]\tValue: [" << value << "]\n";
    }
    if ( m_doMeanTests )
    {
        int number = ::rand();
        if ( ( number % 8 ) == 0 )
        {
            throw logic_error( "Can't accept section key!" );
        }
        number = ::rand();
        if ( ( number % 8 ) == 0 )
        {
            return false;
        }
    }
    return true;
}

// ----------------------------------------------------------------------------

void ConfigTester::ParsedConfigFile( bool valid )
{
    assert( NULL != this );
    if ( ShowContent() )
        cout << "Parsed Config: Valid: [" << valid << "]\n";
    if ( m_doMeanTests )
    {
        int number = ::rand();
        if ( ( number % 8 ) == 0 )
        {
            throw logic_error( "Can't finish parsing config info!" );
        }
    }
}

// ----------------------------------------------------------------------------

bool ConfigTester::OnParse( const char * start, const char * end, unsigned long i )
{
    assert( NULL != this );

    ErrorReceiver * errorHandler = GetErrorReceiver();
    assert( NULL != errorHandler );
    IParseErrorReceiver * pErrorReceiver = dynamic_cast< IParseErrorReceiver * >( errorHandler );
    assert( NULL != pErrorReceiver );
    m_parser.SetMessageReceiver( pErrorReceiver );
    ConfigParser::ParseResults result = m_parser.Parse( start, end, this );

    ParseInfo::ParseResult parseResult =
        ( ConfigParser::AllValid  == result ) ? ParseInfo::AllValid  :
        ( ConfigParser::SomeValid == result ) ? ParseInfo::SomeValid :
        ( ConfigParser::NotValid  == result ) ? ParseInfo::NotValid  : ParseInfo::NotParsed;
    const unsigned count = errorHandler->GetCount();
    const bool didMatch = CheckResults( i, parseResult, count );

    return didMatch;
}

// ----------------------------------------------------------------------------

// $Log: ConfigTester.cpp,v $
// Revision 1.5  2009/01/05 19:24:52  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.4  2008/12/09 19:43:52  rich_sposato
// Added ability to test how parser responds to exceptions.
//
// Revision 1.3  2008/12/09 00:11:28  rich_sposato
// Changed receiver class and parser class to do memory allocation less often
// and improve performance.
//
// Revision 1.2  2008/12/08 23:11:43  rich_sposato
// A few changes to ConfigParser to make it more versatile and robust.
//
// Revision 1.1  2008/12/05 19:25:28  rich_sposato
// Adding files to CVS.
//
