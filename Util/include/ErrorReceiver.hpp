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

// $Header: /cvsroot/utilparsers/UtilParsers/Util/include/ErrorReceiver.hpp,v 1.3 2009/01/05 19:25:48 rich_sposato Exp $

/// @file ErrorReceiver.hpp  Defines interface class used to receive error messages.


// ----------------------------------------------------------------------------

#if !defined( PARSER_ERROR_RECEIVER_HPP_INCLUDED )
#define PARSER_ERROR_RECEIVER_HPP_INCLUDED

#include <UtilParsers/Util/include/TypeDefs.hpp>


// ----------------------------------------------------------------------------
// Namespace resolution.

namespace Parser
{


// ----------------------------------------------------------------------------

class ErrorLevel
{
public:
    enum Levels
    {
        None    = 0,  ///< Used for parser policy configuration.
        Info    = 1,  ///< Informational message about content or syntax.
        Content = 2,  ///< Content of data that caused error or warning.
        Warn    = 3,  ///< Warning about content or syntax.
        Minor   = 4,  ///< Minor syntax error.
        Major   = 5,  ///< Major syntax error.
        Fatal   = 6,  ///< Parser can't continue.
        Except  = 7,  ///< An exception occurred while parsing.
        Count,        ///< Not an error level, just a count of how many exist.
    };

    static const CharType * Name( Levels level );

    static bool Valid( Levels level );
};


// ----------------------------------------------------------------------------

class IParseErrorReceiver
{
protected:
    /// Trivially implemented.
    inline IParseErrorReceiver( void ) {}
    /// Trivially implemented.
    inline virtual ~IParseErrorReceiver( void ) {}

public:

    virtual bool GiveParseMessage( ::Parser::ErrorLevel::Levels level,
        const CharType * message ) = 0;

    virtual bool GiveParseMessage( ::Parser::ErrorLevel::Levels level,
        const CharType * message, unsigned long line ) = 0;

    virtual bool GiveParseMessage( ::Parser::ErrorLevel::Levels level,
        const CharType * message, const char * filename, unsigned long line ) = 0;

private:
    /// Not implemented.
    IParseErrorReceiver( const IParseErrorReceiver & );
    /// Not implemented.
    IParseErrorReceiver & operator = ( const IParseErrorReceiver & );
};


// ----------------------------------------------------------------------------

}; // end namespace Parser

#endif // file guardian

// $Log: ErrorReceiver.hpp,v $
// Revision 1.3  2009/01/05 19:25:48  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.2  2008/12/11 20:37:26  rich_sposato
// Changed include paths to use the more general form.
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
