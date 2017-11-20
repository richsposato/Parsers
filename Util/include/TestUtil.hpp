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

// $Header: /cvsroot/utilparsers/UtilParsers/Util/include/TestUtil.hpp,v 1.3 2009/01/05 19:25:48 rich_sposato Exp $

/// @file TestUtil.hpp Unit test classes for testing parsers.


// ----------------------------------------------------------------------------

#if !defined( PARSER_TEST_UTIL_HPP_INCLUDED )
/// file guardian.
#define PARSER_TEST_UTIL_HPP_INCLUDED


// ----------------------------------------------------------------------------

#include <UtilParsers/Util/include/ErrorReceiver.hpp>
#include <UtilParsers/Util/include/ParseInfo.hpp>

namespace Parser
{

// ----------------------------------------------------------------------------

struct TestData
{
    ::Parser::ParseInfo::ParseResult m_result;
    const CharType * m_content;
};

// ----------------------------------------------------------------------------

class ErrorReceiver : public Parser::IParseErrorReceiver
{
protected:
    ErrorReceiver( const ErrorReceiver & );
    ErrorReceiver & operator = ( const ErrorReceiver & );

public:

    ErrorReceiver( Parser::ErrorLevel::Levels showErrorLevel,
        bool showContent = false );

    virtual ~ErrorReceiver( void );

    virtual bool GiveParseMessage( Parser::ErrorLevel::Levels level,
        const CharType * message );

    virtual bool GiveParseMessage( Parser::ErrorLevel::Levels level,
        const CharType * message, unsigned long line );

    virtual bool GiveParseMessage( Parser::ErrorLevel::Levels level,
        const CharType * message, const char * filename, unsigned long line );

    inline void Clear( void )
    {
        m_highest = Parser::ErrorLevel::None;
        m_count = 0;
    }

    inline unsigned long GetCount( void ) const { return m_count; }

    inline Parser::ErrorLevel::Levels GetHighest( void ) const
    { return m_highest; }

    inline bool ShowContent( void ) const { return m_showContent; }

    inline void SetShowContent( bool show ) { m_showContent = show; }

private:
    ::Parser::ErrorLevel::Levels m_highest;
    ::Parser::ErrorLevel::Levels m_showErrorLevel;
    unsigned long m_count;
    bool m_showContent;
};

// ----------------------------------------------------------------------------

class TestBase
{
public:

    bool Test( const TestData * paData, unsigned long count );

    inline void SetShowInfo( bool show ) { m_showInfo = show; }

    inline bool ShowInfo( void ) const { return m_showInfo; }

    inline bool ShowContent( void ) const { return m_Errors.ShowContent(); }

    inline void SetShowContent( bool show )
    {
        return m_Errors.SetShowContent( show );
    }

    inline unsigned long GetCount( void ) const { return m_count; }

    inline unsigned long GetFailCount( void ) const { return m_failCount; }
    inline unsigned long GetPassCount( void ) const { return m_passCount; }

    inline const char * GetTestName( void ) const { return m_testName; }

    inline Parser::ErrorReceiver * AsErrorReceiver( void )
    { return &m_Errors; }

protected:

    TestBase( const char * testName, bool showContent,
        ::Parser::ErrorLevel::Levels showErrorLevel, bool m_showInfo );

    virtual ~TestBase( void );

    bool TestAll( void );

    bool SetupTest( const ::Parser::TestData * paData, unsigned long count );

    bool DoOneTest( const ::Parser::TestData & rData, unsigned long i );

    bool CheckResults( unsigned long i, ::Parser::ParseInfo::ParseResult result,
        unsigned long errorCount );

    inline ::Parser::ErrorReceiver * GetErrorReceiver( void ) { return &m_Errors; }

    inline const ::Parser::ErrorReceiver * GetErrorReceiver( void ) const { return &m_Errors; }

private:
    TestBase( void );
    TestBase( const TestBase & );
    TestBase & operator = ( const TestBase & );

    bool virtual OnParse( const CharType * start, const CharType * end,
        unsigned long i ) = 0;

    ::Parser::ErrorReceiver m_Errors;
    const char * m_testName;
    unsigned long m_failCount;
    unsigned long m_passCount;
    unsigned long m_count;
    const ::Parser::TestData * m_aData;
    bool m_showInfo;
};

// ----------------------------------------------------------------------------

}; // end namespace Parser

#endif // file guardian

// $Log: TestUtil.hpp,v $
// Revision 1.3  2009/01/05 19:25:48  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.2  2008/12/11 20:37:26  rich_sposato
// Changed include paths to use the more general form.
//
// Revision 1.1  2008/12/05 19:12:28  rich_sposato
// Adding files to CVS.
//
// Revision 1.1  2008/03/02 02:50:31  rich_sposato
// Added project files & new files to CVS.  Numerous changes to source files.
//
