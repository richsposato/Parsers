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

// $Header: /cvsroot/utilparsers/UtilParsers/Util/include/ParseInfo.hpp,v 1.3 2009/01/05 19:25:48 rich_sposato Exp $

/// @file ParseInfo.hpp File defines class used to store parsing info.


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if !defined( PARSER_INFO_HPP_INCLUDED )
/// File guardian.
#define PARSER_INFO_HPP_INCLUDED


// ----------------------------------------------------------------------------
// Included files.

#include <boost/spirit.hpp>
#include <boost/spirit/core.hpp>

#include <UtilParsers/Util/include/TypeDefs.hpp>


// ----------------------------------------------------------------------------
// Namespace resolution.

namespace Parser
{
    class MessageStack;
    class ContentStack;

    typedef ::boost::spirit::chlit< CharType > SpiritCharLit;  // literal char.
    typedef ::boost::spirit::chset< CharType > SpiritCharSet;  // set of chars.
    typedef ::boost::spirit::rule< CharType > SpiritRule;
    typedef ::boost::spirit::parse_info< const CharType * > SpiritInfo;


// ----------------------------------------------------------------------------

class ParseInfo
{
public:

    enum ParseResult
    {
        AllValid = 0, ///< All data was parsed and is valid.
        SomeValid,    ///< Start is valid, but ending part was not recognized.
        NotValid,     ///< Data was parsed, but is not valid.
        NotParsed,    ///< Data was not recognized by parsing rules.
        CantStart,    ///< Unable to start parsing.
        NoFileName,   ///< No file name provided for parsing.
        CantOpenFile, ///< Could not open file.
        EndOfFile,    ///< Reached end of file, but can't parse content.
    };

    static const CharType * Name( ParseResult result );

    inline static bool IsGoodResult( ParseResult result )
    { return ( AllValid == result ) || ( SomeValid == result ); }

    inline static bool ResultImpliesAttempt( ParseResult result )
    {
        return ( AllValid  == result )
            || ( SomeValid == result )
            || ( NotValid  == result )
            || ( NotParsed == result );
    }

    explicit ParseInfo( MessageStack * messages = 0,
        ContentStack * contents = 0 );

    ~ParseInfo( void );

    ParseInfo::ParseResult Parse( const char * start, const char * end,
        const SpiritRule & startRule );

    inline bool IsLegal( void ) const { return m_results.hit; }

    inline bool AteAll( void ) const { return m_results.full; }

    inline unsigned long GetLength( void ) const
    { return static_cast< unsigned long >( m_results.length ); }

    inline const CharType * GetStart( void ) const { return m_start; }

    inline const CharType * GetEnd( void ) const { return m_end; }

    inline const CharType * GetStop( void ) const { return m_results.stop; }

    inline void SetMessageStack( MessageStack * stack ) { m_Messages = stack; }

    inline MessageStack * GetMessageStack( void ) { return m_Messages; }

    inline const MessageStack * GetMessageStack( void ) const
    { return m_Messages; }

    inline void SetContentStack( ContentStack * stack ) { m_Contents = stack; }

    inline ContentStack * GetContentStack( void ) { return m_Contents; }

    inline const ContentStack * GetContentStack( void ) const
    { return m_Contents; }

private:

    ParseInfo( const ParseInfo & );
    ParseInfo & operator = ( const ParseInfo & );

    SpiritInfo m_results;

    const CharType * m_start;

    const CharType * m_end;

    MessageStack * m_Messages;

    ContentStack * m_Contents;

};

// ----------------------------------------------------------------------------

}; // end namespace Parser

#endif // file guardian

// $Log: ParseInfo.hpp,v $
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
