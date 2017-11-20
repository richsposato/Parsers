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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/Tester/CommandLineArgs.cpp,v 1.1 2008/12/05 19:04:15 rich_sposato Exp $


// --------------------------------------------------------------------------------------------

#include "CommandLineArgs.hpp"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


CommandLineArgs * CommandLineArgs::s_instance = NULL;


// --------------------------------------------------------------------------------------------

CommandLineArgs::CommandLineArgs( unsigned int argc, const char * const argv[] ) :
    m_valid( false ),
    m_doWait( false ),
    m_showHelp( false ),
    m_doUnitTest( false ),
    m_doFileTest( false ),
    m_showContent( false ),
    m_showSummary( false ),
    m_showTable( false ),
    m_doMeanTests( false ),
    m_showInfo( false ),
    m_unitTestIndex( -1 ),
    m_showErrorLevel( ::Parser::ErrorLevel::Info ),
    m_fileName( NULL ),
    m_exePath( argv[0] ),
    m_exeName( NULL ),
    m_argv( argv ),
    m_argc( argc )
{
    assert( this != NULL );
    s_instance = this;

    const char * info = NULL;
    unsigned long length = 0;
    int level = 0;
    const char * myName = ::strrchr( argv[0], '/' );
    if ( ( NULL == myName ) || ( 0 == *myName ) ) myName = ::strrchr( argv[0], '\\' );
    if ( ( NULL == myName ) || ( 0 == *myName ) ) myName = "XmlParser.exe"; else ++myName;
    if ( argc < 2 ) return;

    bool validCommands = true;
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
                        m_showErrorLevel = static_cast< Parser::ErrorLevel::Levels >( level );
                }
                break;
            case 'f':
                if ( length < 4 )
                    validCommands = false;
                else
                {
                    m_fileName = &( argv[ ai ][ 3 ] );
                    m_doFileTest = true;
                }
                break;
            case 'u':
                m_doUnitTest = true;
                if ( ( ':' == argv[ ai ][ 2 ] ) && ( 4 <= length ) )
                {
                    info = &( argv[ ai ][ 3 ] );
                    m_unitTestIndex = ::atoi( info );
                }
                break;
            case 'h': m_showHelp    = true;  break;
            case 'i': m_showInfo    = true;  break;
            case 's': m_showSummary = true;  break;
            case 't': m_showTable   = true;  break;
            case 'c': m_showContent = true;  break;
            case 'm': m_doMeanTests = true;  break;
            case 'W': m_doWait      = true;  break;
            default:  validCommands = false; break;
        }
    }

    m_valid = ( validCommands
        && ( m_doUnitTest || m_doFileTest || m_showHelp ) );
}

// --------------------------------------------------------------------------------------------

// $Log: CommandLineArgs.cpp,v $
// Revision 1.1  2008/12/05 19:04:15  rich_sposato
// Adding xml parsing files to project.
//
