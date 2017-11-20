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

// $Header: /cvsroot/utilparsers/UtilParsers/Config/src/CommonParsers.hpp,v 1.2 2009/01/05 19:24:52 rich_sposato Exp $

/// @file ParseUtil.hpp Common parser classes used by many grammars.


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if !defined( UTIL_PARSER_COMMON_PARSERS_H_INCLUDED )
/// File guardian.
#define UTIL_PARSER_COMMON_PARSERS_H_INCLUDED


// ----------------------------------------------------------------------------
// Included files.

#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/chset.hpp>


// ----------------------------------------------------------------------------
// Namespace resolution.

namespace Parser
{


// ----------------------------------------------------------------------------
// Class definitions.

class LineCounter
{
public:

    LineCounter( void );
    ~LineCounter( void );

    inline const boost::spirit::rule<> & GetRule( void ) const { return m_new_line; }

    inline unsigned long GetLineCount( void ) const { return m_line_count; }
    inline unsigned long & Counter( void ) { return m_line_count; }

    inline void Reset( void ) { m_line_count = 0; }

private:

    inline void IncrementCount( void ) { ++m_line_count; }

    struct FNewLineIncrement
    {
        inline explicit FNewLineIncrement( LineCounter * pCounter ) : m_pCounter( pCounter ) {}

        inline void operator () ( char ) const
        { m_pCounter->IncrementCount(); }

        inline void operator () ( const char *, const char * ) const
        { m_pCounter->IncrementCount(); }

        LineCounter * m_pCounter;
    };

    boost::spirit::rule<> m_new_line;
    unsigned long m_line_count;

    /// Not implemented.
    LineCounter( const LineCounter & );
    /// Not implemented.
    LineCounter & operator = ( const LineCounter & );
};


// ----------------------------------------------------------------------------

class CommentParser
{
public:

    CommentParser( LineCounter & lineCounter );
    ~CommentParser( void );

    const boost::spirit::rule<> & GetRule( void ) const { return m_comments; }

private:

    /// Not implemented.
    CommentParser( void );
    /// Not implemented.
    CommentParser( const CommentParser & );
    /// Not implemented.
    CommentParser & operator = ( const CommentParser & );

    const boost::spirit::chset<> m_new_line;
    const boost::spirit::strlit<> m_line_comment_start;
    boost::spirit::rule<> m_line_comment;
    boost::spirit::rule<> m_comment_ender;
    boost::spirit::rule<> m_comment_content;
    boost::spirit::rule<> m_block_comment;
    boost::spirit::rule<> m_comments;
};


// ----------------------------------------------------------------------------

}; // end namespace Parser

#endif // file guardian

// $Log: CommonParsers.hpp,v $
// Revision 1.2  2009/01/05 19:24:52  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.1  2008/12/05 19:25:28  rich_sposato
// Adding files to CVS.
//
