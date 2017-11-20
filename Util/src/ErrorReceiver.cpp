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

// $Header: /cvsroot/utilparsers/UtilParsers/Util/src/ErrorReceiver.cpp,v 1.1 2008/12/05 19:12:28 rich_sposato Exp $

/// @file ErrorReceiver.cpp Contains functions defined in header file.


// ----------------------------------------------------------------------------
// Included files.

#include "../include/ErrorReceiver.hpp"


// ----------------------------------------------------------------------------

const Parser::CharType * Parser::ErrorLevel::Name( Parser::ErrorLevel::Levels level )
{
    const char * name = 0;
    switch ( level )
    {
        case None:    name = "None";    break;
        case Info:    name = "Info";    break;
        case Content: name = "Content"; break;
        case Warn:    name = "Warn";    break;
        case Minor:   name = "Minor";   break;
        case Major:   name = "Major";   break;
        case Fatal:   name = "Fatal";   break;
        case Except:  name = "Except";  break;
        default:      name = "Unknown"; break;
    }
    return reinterpret_cast< const Parser::CharType * >( name );
}

// ----------------------------------------------------------------------------

bool Parser::ErrorLevel::Valid( Parser::ErrorLevel::Levels level )
{
    return ( ErrorLevel::None <= level ) && ( level <= Except );
}

// ----------------------------------------------------------------------------

// $Log: ErrorReceiver.cpp,v $
// Revision 1.1  2008/12/05 19:12:28  rich_sposato
// Adding files to CVS.
//
// Revision 1.2  2008/03/02 02:50:31  rich_sposato
// Added project files & new files to CVS.  Numerous changes to source files.
//
// Revision 1.1.1.1  2005/07/23 07:31:34  rich_sposato
// Initial Import.
//
