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

// $Header: /cvsroot/utilparsers/UtilParsers/Util/src/ParseInfo.cpp,v 1.2 2009/01/05 19:25:48 rich_sposato Exp $

/// @file ParseInfo.cpp Contains functions for ParseInfo class.


// ----------------------------------------------------------------------------

#include "../include/ParseInfo.hpp"

#include "../include/ParseUtil.hpp"


// ----------------------------------------------------------------------------
// Namespace resolution.

using namespace ::std;
using namespace ::boost::spirit;

namespace Parser
{


// ----------------------------------------------------------------------------

const CharType * ParseInfo::Name( ParseResult result )
{
    const char * name = "Unknown";
    switch ( result )
    {
        default:
        break; case ParseInfo::AllValid:     name = "All contents are valid.";
        break; case ParseInfo::SomeValid:    name = "Some contents are valid.";
        break; case ParseInfo::NotValid:     name = "Content is not valid.";
        break; case ParseInfo::NotParsed:    name = "Contents were not parsed.";
        break; case ParseInfo::NoFileName:   name = "No filename provided";
        break; case ParseInfo::CantOpenFile: name = "Could not open file.";
        break; case ParseInfo::EndOfFile:    name = "End of file reached before done parsing.";
    }
    return reinterpret_cast< const CharType * >( name );
}

// ----------------------------------------------------------------------------

ParseInfo::ParseInfo( MessageStack * messages, ContentStack * contents ) :
    m_results(),
    m_start( NULL ),
    m_end( NULL ),
    m_Messages( messages ),
    m_Contents( contents )
{
}

// ----------------------------------------------------------------------------

ParseInfo::~ParseInfo( void )
{
}

// ----------------------------------------------------------------------------

ParseInfo::ParseResult ParseInfo::Parse( const char * start, const char * end,
    const SpiritRule & startRule )
{

    m_start = start;
    m_end = end;
    m_results = parse( m_start, m_end, startRule );
    const bool okay = IsLegal();

    if ( NULL != m_Messages )
    {
        while ( m_Messages->GetStackSize() > 0 )
            m_Messages->Pop();
    }
    if ( NULL != m_Contents )
    {
        while ( m_Contents->GetStackSize() > 0 )
            m_Contents->Pop();
    }

    if ( !okay )
    {
        if ( NULL != m_Messages )
        {
            m_Messages->Send( Parser::ErrorLevel::Fatal, "Unable to finish parsing data." );
        }
        return ParseInfo::NotParsed;
    }
    ++m_results.length;

    return ( AteAll() ) ? ParseInfo::AllValid : ParseInfo::SomeValid;
}

// ----------------------------------------------------------------------------

}; // end namespace Parser

// $Log: ParseInfo.cpp,v $
// Revision 1.2  2009/01/05 19:25:48  rich_sposato
// Replaced tabs with spaces.
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
