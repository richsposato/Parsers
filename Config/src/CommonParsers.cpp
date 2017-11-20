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

// $Header: /cvsroot/utilparsers/UtilParsers/Config/src/CommonParsers.cpp,v 1.3 2009/01/05 19:24:52 rich_sposato Exp $

/// @file CommonParsers.cpp Parsers classes used by many grammers.


// ----------------------------------------------------------------------------
// Included files.

#include "CommonParsers.hpp"


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if defined( _MSC_VER )
    #pragma inline_depth(255)
    #pragma inline_recursion(on)
#endif


// ----------------------------------------------------------------------------
// Namespace resolution.

using namespace std;
using namespace boost::spirit;

namespace Parser
{


// ----------------------------------------------------------------------------

LineCounter::LineCounter( void ) :
    m_new_line(),
    m_line_count( 0 )
{
    m_new_line = ( !ch_p( '\r' ) >> ch_p( '\n' ) )
        [ FNewLineIncrement( this ) ];
}

// ----------------------------------------------------------------------------

LineCounter::~LineCounter( void )
{
}

// ----------------------------------------------------------------------------

CommentParser::CommentParser( LineCounter & lineCounter ) :
    m_new_line( "\n" ),
    m_line_comment_start( "//" ),
    m_line_comment( m_line_comment_start >> *( ~m_new_line ) ),
    m_comment_ender( str_p( "*/" ) ),
    m_comment_content( ~ch_p( '*' ) | ( ch_p( '*' ) >> ~ch_p( '/' ) ) ),
    m_block_comment( str_p( "/*" )
        >> ( *( blank_p | lineCounter.GetRule() | m_comment_content ) )
        >> m_comment_ender ),
    m_comments( blank_p | lineCounter.GetRule() |
        m_line_comment | m_block_comment )
{
}

// ----------------------------------------------------------------------------

CommentParser::~CommentParser( void )
{
}

// ----------------------------------------------------------------------------

}; // end namespace Parser

// $Log: CommonParsers.cpp,v $
// Revision 1.3  2009/01/05 19:24:52  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.2  2008/12/09 00:24:13  rich_sposato
// Put pragmas inside #if sections.
//
// Revision 1.1  2008/12/05 19:25:28  rich_sposato
// Adding files to CVS.
//
