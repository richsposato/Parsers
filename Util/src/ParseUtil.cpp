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

// $Header: /cvsroot/utilparsers/UtilParsers/Util/src/ParseUtil.cpp,v 1.5 2009/01/05 19:25:48 rich_sposato Exp $

/// @file ParseUtil.hpp Utility classes used by parsers.


// ----------------------------------------------------------------------------

#include "../include/ParseUtil.hpp"

#include <string.h>

#include <iostream>

#include <boost/spirit/core.hpp>


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if defined( _MSC_VER )
    #pragma inline_depth(255)
    #pragma inline_recursion(on)
#endif

#if defined( DEBUG ) || defined( _DEBUG )
    #define DEBUG_CODE( code ) code;
#else
    #define DEBUG_CODE( code ) ;
#endif


// ----------------------------------------------------------------------------
// Namespace resolution.

using namespace std;

namespace Parser
{


// ----------------------------------------------------------------------------

ContentStack::ContentInfo::ContentInfo( void ) :
    m_content(),
    m_section(),
    m_line( 0 ),
    m_char( 0 )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

ContentStack::ContentInfo::~ContentInfo( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void ContentStack::ContentInfo::Clear( void )
{
    assert( this != NULL );
    m_content.clear(),
    m_section.clear(),
    m_line = 0;
    m_char = 0;
}

// ----------------------------------------------------------------------------

ContentStack::ContentStack( IStackMessagePreparer * pPreparer ) :
    m_pPreparer( pPreparer ),
    m_stackIndex( EmptySpot )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

ContentStack::~ContentStack( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void ContentStack::Clear( void )
{
    assert( this != NULL );

    ContentInfo * pInfo = NULL;
    signed int ii = 0;

    if ( EmptySpot != m_stackIndex )
    {
        assert( m_stackIndex < MaxStackSize );
        assert( 0 <= m_stackIndex );
        for ( ; ii <= m_stackIndex; ++ii )
        {
            pInfo = &( m_messageStack[ ii ] );
            pInfo->Clear();
        }
    }

    m_stackIndex = EmptySpot;
}

// ----------------------------------------------------------------------------

void ContentStack::Push( const CharType * content, const CharType * section,
    unsigned long line, unsigned long chars )
{
    assert( this != NULL );

    if ( TopPlace <= m_stackIndex )
        return;
    if ( NULL == content )
        content = reinterpret_cast< const Parser::CharType * >( "" );
    ++m_stackIndex;
    m_messageStack[ m_stackIndex ].m_content = content;
    m_messageStack[ m_stackIndex ].m_section = section;
    m_messageStack[ m_stackIndex ].m_line = line;
    m_messageStack[ m_stackIndex ].m_char = chars;
}

// ----------------------------------------------------------------------------

const CharType * ContentStack::GetContentAt( unsigned long index ) const
{
    assert( this != NULL );

    if ( ( GetStackSize() <= index )
      || ( index < MaxStackSize ) )
        return NULL;
    return m_messageStack[ index ].m_content.c_str();
}

// ----------------------------------------------------------------------------

const CharType * ContentStack::GetSectionAt( unsigned long index ) const
{
    assert( this != NULL );

    if ( ( GetStackSize() <= index )
      || ( index < MaxStackSize ) )
        return NULL;
    return m_messageStack[ index ].m_section.c_str();
}

// ----------------------------------------------------------------------------

unsigned long ContentStack::GetLineAt( unsigned long index ) const
{
    assert( this != NULL );

    if ( ( GetStackSize() <= index )
      || ( index < MaxStackSize ) )
        return 0;
    return m_messageStack[ index ].m_line;
}

// ----------------------------------------------------------------------------

unsigned long ContentStack::GetCharColAt( unsigned long index ) const
{
    assert( this != NULL );

    if ( ( GetStackSize() <= index )
      || ( index < MaxStackSize ) )
        return 0;
    return m_messageStack[ index ].m_char;
}

// ----------------------------------------------------------------------------

bool ContentStack::OutputSubsetStack( unsigned long levels ) const
{
    assert( this != NULL );

    if ( NULL == m_pPreparer ) return false;
    unsigned long line;
    unsigned long chars = 0;
    const CharType * section = NULL;
    const CharType * cname = NULL;
    bool okay = true;

    for ( signed long index = m_stackIndex;
        ( index > EmptySpot ) && ( levels > 0 ) && ( okay ); --index, --levels )
    {
        section = m_messageStack[ index ].m_section.c_str();
        const StringType & name = m_messageStack[ index ].m_content;
        line = m_messageStack[ index ].m_line;

        if ( name.empty() )
        {
            okay = m_pPreparer->PrepareContentMessage( section );
            continue;
        }

        cname = name.c_str();
        if ( 0 == line )
            okay = m_pPreparer->PrepareContentMessage( section, cname );
        else
        {
            chars = m_messageStack[ index ].m_char;
            okay = m_pPreparer->PrepareContentMessage(
                section, cname, line, chars );
        }
    }

    return okay;
}

// ----------------------------------------------------------------------------

bool MessageStack::s_debug = false;

// ----------------------------------------------------------------------------

MessageStack::MessageStack( IStackMessagePreparer * pPreparer ) :
    m_pPreparer( pPreparer ),
    m_highestLevel( Parser::ErrorLevel::None ),
    m_stackIndex( EmptySpot )
{
    DEBUG_CODE( CheckInvariants() );
}

// ----------------------------------------------------------------------------

MessageStack::~MessageStack( void )
{
    DEBUG_CODE( CheckInvariants() );
}

// ----------------------------------------------------------------------------

void MessageStack::Clear( void )
{
    DEBUG_CODE( CheckInvariants() );
    DEBUG_CODE( InvariantChecker guard( this ); (void)guard; );

    MessageInfo * pInfo = NULL;
    signed int ii = 0;

    if ( EmptySpot != m_stackIndex )
    {
        assert( m_stackIndex < MaxStackSize );
        assert( 0 <= m_stackIndex );
        for ( ; ii <= m_stackIndex; ++ii )
        {
            pInfo = &( m_messageStack[ ii ] );
            pInfo->Clear();
        }
    }

    m_highestLevel = Parser::ErrorLevel::None;
    m_stackIndex = EmptySpot;
}

// ----------------------------------------------------------------------------

void MessageStack::DebugOutput( const char * function ) const
{
    cout << "Message Stack: \t" << this << endl;
    cout << "\t Action: \t" << function << endl;
    cout << "\t Index: \t" << m_stackIndex << endl;
    if ( m_stackIndex < 0 )
        return;

    const MessageInfo & info = m_messageStack[ m_stackIndex ];
    cout << "\t Message: \t" << info.m_message << endl;
    cout << "\t Level: \t" << info.m_level << endl;
    cout << "\t File:  \t" << info.m_file << endl;
    cout << "\t Rule:  \t" << info.m_ruleName << endl;
}

// ----------------------------------------------------------------------------

void MessageStack::Push( Parser::ErrorLevel::Levels level, const CharType * message,
    const char * file, const char * ruleName )
{
    DEBUG_CODE( CheckInvariants() );
    DEBUG_CODE( InvariantChecker guard( this ); (void)guard; );
    assert( ( 0 <= m_stackIndex ) || ( EmptySpot == m_stackIndex ) );
    assert( m_stackIndex <= TopPlace );

    if ( TopPlace <= m_stackIndex )
    {
        // Perhaps this should throw an excecption to show the stack is too small.
        // Or increase the stack size?
        return;
    }
    ++m_stackIndex;
    MessageInfo & info = m_messageStack[ m_stackIndex ];
    info.m_message = message;
    info.m_level = level;
    info.m_file = file;
    info.m_ruleName = ruleName;

    DEBUG_CODE( if ( IsDebugging() ) DebugOutput( __FUNCTION__ ); );
}

// ----------------------------------------------------------------------------

void MessageStack::Prepare( Parser::ErrorLevel::Levels level, const CharType * message,
    const char * file, const char * ruleName )
{
    DEBUG_CODE( CheckInvariants() );
    DEBUG_CODE( InvariantChecker guard( this ); (void)guard; );
    assert( 0 <= m_stackIndex );
    assert( m_stackIndex <= TopPlace );

    MessageInfo & info = m_messageStack[ m_stackIndex ];
    info.m_message = message;
    info.m_level = level;
    info.m_file = file;
    info.m_ruleName = ruleName;

    DEBUG_CODE( if ( IsDebugging() ) DebugOutput( __FUNCTION__ ); );
}

// ----------------------------------------------------------------------------

void MessageStack::Cancel( void )
{
    DEBUG_CODE( CheckInvariants() );
    DEBUG_CODE( InvariantChecker guard( this ); (void)guard; );
    assert( 0 <= m_stackIndex );
    assert( m_stackIndex <= TopPlace );

    if ( ( m_stackIndex < 0 )
      || ( TopPlace < m_stackIndex ) )
        return;

    DEBUG_CODE( if ( IsDebugging() ) DebugOutput( __FUNCTION__ ); );

    m_messageStack[ m_stackIndex ].Clear();
    --m_stackIndex;
}

// ----------------------------------------------------------------------------

void MessageStack::Pop( void )
{
    DEBUG_CODE( CheckInvariants() );
    DEBUG_CODE( InvariantChecker guard( this ); (void)guard; );
    assert( 0 <= m_stackIndex );
    assert( m_stackIndex <= TopPlace );

    if ( ( m_stackIndex < 0 )
      || ( TopPlace < m_stackIndex ) )
        return;

    DEBUG_CODE( if ( IsDebugging() ) DebugOutput( __FUNCTION__ ); );

    Send();
    m_messageStack[ m_stackIndex ].Clear();
    --m_stackIndex;
}

// ----------------------------------------------------------------------------

bool MessageStack::Send( void )
{
    DEBUG_CODE( CheckInvariants() );
    DEBUG_CODE( InvariantChecker guard( this ); (void)guard; );
    assert( 0 <= m_stackIndex );
    assert( m_stackIndex <= TopPlace );

    if ( ( m_stackIndex < 0 )
      || ( TopPlace < m_stackIndex ) )
        return false;

    MessageInfo & r( m_messageStack[ m_stackIndex ] );
    return Send( r.m_level, r.m_message );
}

// ----------------------------------------------------------------------------

bool MessageStack::Send( Parser::ErrorLevel::Levels level, const CharType * message )
{
    DEBUG_CODE( CheckInvariants() );
    DEBUG_CODE( InvariantChecker guard( this ); (void)guard; );

    if ( ( level <= Parser::ErrorLevel::None )
      || ( Parser::ErrorLevel::Fatal < level )
      || ( NULL == message )
      || ( '\0' == *message ) )
    {
        return false;
    }
    if ( m_highestLevel < level )
    {
        m_highestLevel = level;
    }
    return DoSend( level, message );
}

// ----------------------------------------------------------------------------

bool MessageStack::Send( const CharType * content )
{
    DEBUG_CODE( CheckInvariants() );
    DEBUG_CODE( InvariantChecker guard( this ); (void)guard; );

    if ( ( NULL == content )
      || ( '\0' == *content ) )
    {
        return false;
    }
    return DoSend( Parser::ErrorLevel::Content, content );
}

// ----------------------------------------------------------------------------

bool MessageStack::Send( const CharType * first, const CharType * last )
{
    DEBUG_CODE( CheckInvariants() );
    DEBUG_CODE( InvariantChecker guard( this ); (void)guard; );

    if ( ( NULL == first )
      || ( '\0' == *first )
      || ( NULL == last )
      || ( last <= first ) )
    {
        return false;
    }
    return DoSend( Parser::ErrorLevel::Content, first, last );
}

// ----------------------------------------------------------------------------

bool MessageStack::DoSend( Parser::ErrorLevel::Levels level, const CharType * first,
    const CharType * last )
{

    if ( NULL == m_pPreparer )
    {
        return false;
    }
    try
    {
        return m_pPreparer->PrepareErrorMessage( level, first, last );
    }
    catch ( ... )
    {
        m_pPreparer = NULL;
    }
    return false;
}

// ----------------------------------------------------------------------------

bool MessageStack::DoSend( Parser::ErrorLevel::Levels level, const CharType * message )
{

    if ( NULL == m_pPreparer )
    {
        return false;
    }
    try
    {
        return m_pPreparer->PrepareErrorMessage( level, message );
    }
    catch ( ... )
    {
        m_pPreparer = NULL;
    }
    return false;
}

// ----------------------------------------------------------------------------

void MessageStack::CheckInvariants( void ) const
{
    assert( this != NULL );
    assert( EmptySpot <= m_stackIndex );
    assert( m_stackIndex < MaxStackSize );

    const MessageInfo * pInfo = NULL;
    signed int ii = 0;

    if ( EmptySpot != m_stackIndex )
    {
        assert( m_stackIndex < MaxStackSize );
        assert( 0 <= m_stackIndex );
        for ( ; ii <= m_stackIndex; ++ii )
        {
            pInfo = &( m_messageStack[ ii ] );
            assert( pInfo->m_message != NULL );
            assert( pInfo->m_level != Parser::ErrorLevel::None );
        }
    }

    for ( ; ii < MaxStackSize; ++ii )
    {
        pInfo = &( m_messageStack[ ii ] );
        assert( pInfo->m_message == NULL );
        assert( pInfo->m_level == Parser::ErrorLevel::None );
    }
}

// ----------------------------------------------------------------------------

}; // end namespace Parser

// $Log: ParseUtil.cpp,v $
// Revision 1.5  2009/01/05 19:25:48  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.4  2008/12/09 19:38:27  rich_sposato
// Fixed bug related to maintaining class invariants even when exceptions occur.
//
// Revision 1.3  2008/12/09 00:24:14  rich_sposato
// Put pragmas inside #if sections.
//
// Revision 1.2  2008/12/06 08:47:04  rich_sposato
// Added code to provide better debugging abilities.
//
// Revision 1.1  2008/12/05 19:12:28  rich_sposato
// Adding files to CVS.
//
// Revision 1.2  2008/03/02 02:50:31  rich_sposato
// Added project files & new files to CVS.  Numerous changes to source files.
//
// Revision 1.1.1.1  2005/07/23 07:31:34  rich_sposato
// Initial Import.
//
