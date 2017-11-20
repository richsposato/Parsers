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

// $Header: /cvsroot/utilparsers/UtilParsers/Config/test/ConfigTester.hpp,v 1.5 2009/01/05 19:24:52 rich_sposato Exp $

// ----------------------------------------------------------------------------

#if !defined( INOVYS_STIL_PARSE_CONFIG_TEST_H_INCLUDED )
/// file guardian.
#define INOVYS_STIL_PARSE_CONFIG_TEST_H_INCLUDED


// ----------------------------------------------------------------------------

#include "../../Util/include/ErrorReceiver.hpp"
#include "../../Util/include/TestUtil.hpp"

#include "../include/ConfigParser.hpp"


// ----------------------------------------------------------------------------

class ConfigTester : public ::Parser::IConfigReceiver, public ::Parser::TestBase
{
public:

    ConfigTester( ::Parser::ConfigParser & parser, ::Parser::ErrorLevel::Levels showErrorLevel,
        bool showContent = false, bool m_showInfo = false, bool doMeanTests = false );

    virtual ~ConfigTester( void );

private:
    ConfigTester( const ConfigTester & );
    ConfigTester & operator = ( const ConfigTester & );

    virtual bool AddGlobalKey( const char * keyStart, const char * keyEnd,
        const char * valueStart, const char * valueEnd );

    virtual bool AddSection( const char * nameStart, const char * nameEnd );

    virtual bool AddSectionKey( const char * keyStart, const char * keyEnd,
        const char * valueStart, const char * valueEnd );

    virtual void ParsedConfigFile( bool valid );

    virtual bool OnParse( const char * start, const char * end, unsigned long i );

    ::Parser::ConfigParser & m_parser;

    bool m_doMeanTests;
};

// ----------------------------------------------------------------------------

#endif // file guardian

// $Log: ConfigTester.hpp,v $
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
