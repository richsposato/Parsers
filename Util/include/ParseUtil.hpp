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

// $Header: /cvsroot/utilparsers/UtilParsers/Util/include/ParseUtil.hpp,v 1.5 2009/01/05 19:25:48 rich_sposato Exp $

/// @file ParseUtil.hpp Utility classes used by parsers.


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if !defined( PARSER_PARSE_UTIL_HPP_INCLUDED )
/// File guardian.
#define PARSER_PARSE_UTIL_HPP_INCLUDED


// ----------------------------------------------------------------------------
// Included files.

#include <string>

#include <UtilParsers/Util/include/ErrorReceiver.hpp>
#include <UtilParsers/Util/include/TypeDefs.hpp>


// ----------------------------------------------------------------------------
// Namespace resolution.

namespace Parser
{

    typedef std::basic_string< CharType > StringType;

// ----------------------------------------------------------------------------

struct XMaxErrorException { };

// ----------------------------------------------------------------------------

template < typename T >
struct InvariantChecker
{
    inline explicit InvariantChecker( const T * p ) : m_p( p ) {}
    inline ~InvariantChecker( void ) { m_p->CheckInvariants(); }
    const T * m_p;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FBreakPoint
{
    inline FBreakPoint( ParserClass * pParser, const CharType * info ) :
        m_info( info ), m_pParser( pParser ) {}
    inline bool operator () ( CharType c ) const
    {
        (void)c;
        return true;
    }
    inline bool operator () ( const CharType * first, const CharType * last ) const
    {
        return ( first <= last );
    }
    const CharType * m_info;
    ParserClass * m_pParser;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FClear
{
    explicit inline FClear( ParserClass * pParser ) : m_pParser( pParser ) {}
    inline void operator () ( const Parser::CharType *, const Parser::CharType * ) const
    {
        m_pParser->Clear();
    }
    inline void operator () ( const Parser::CharType ) const
    {
        m_pParser->Clear();
    }
    ParserClass * m_pParser;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FSetValidSyntax
{
    inline FSetValidSyntax( ParserClass * pParser, bool valid ) :
        m_pParser( pParser ), m_valid( valid ) {}
    inline FSetValidSyntax( const FSetValidSyntax & that ) :
        m_pParser( that.m_pParser ), m_valid( that.m_valid ) {}

    inline void operator () ( const Parser::CharType *, const Parser::CharType * ) const
    {
        m_pParser->SetValidSyntax( m_valid );
    }
    inline void operator () ( const Parser::CharType ) const
    {
        m_pParser->SetValidSyntax( m_valid );
    }

    ParserClass * m_pParser;
    bool m_valid;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FSetValidContent
{
    inline FSetValidContent( ParserClass * pParser, bool valid ) :
        m_pParser( pParser ), m_valid( valid ) {}

    inline void operator () ( const Parser::CharType *, const Parser::CharType * ) const
    {
        m_pParser->SetValidContent( m_valid );
    }
    inline void operator () ( const Parser::CharType ) const
    {
        m_pParser->SetValidContent( m_valid );
    }

    ParserClass * m_pParser;
    bool m_valid;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FSetContent
{
    inline explicit FSetContent( ParserClass * pParser ) :
        m_pParser( pParser ) {}
    inline void operator () ( const Parser::CharType * begin,
        const Parser::CharType * end ) const
    {
        m_pParser->SetContent( begin, end );
    }
    inline void operator () ( const Parser::CharType ch ) const
    {
        m_pParser->SetContent( ch );
    }
    ParserClass * m_pParser;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FDone
{
    explicit inline FDone( ParserClass * pParser ) : m_pParser( pParser ) {}
    inline void operator () ( const Parser::CharType * begin, const Parser::CharType * end ) const
    {
        m_pParser->Done( begin, end );
    }
    inline void operator () ( const Parser::CharType ch ) const
    {
        m_pParser->Done( ch );
    }
    ParserClass * m_pParser;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FEnd
{
    explicit inline FEnd( ParserClass * pParser ) : m_pParser( pParser ) {}
    inline void operator () ( const Parser::CharType * , const Parser::CharType * ) const
    {
        m_pParser->End();
    }
    inline void operator () ( const Parser::CharType ) const
    {
        m_pParser->End();
    }
    ParserClass * m_pParser;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FStoreStackSize
{
    explicit inline FStoreStackSize( ParserClass * pParser ) : m_pParser( pParser ) {}
    inline void operator () ( const Parser::CharType *, const Parser::CharType * ) const
    {
        m_pParser->StoreStackSize();
    }
    inline void operator () ( const Parser::CharType ) const
    {
        m_pParser->StoreStackSize();
    }
    ParserClass * m_pParser;
};

// ----------------------------------------------------------------------------

template< class ParserClass >
struct FCompareStackSize
{
    explicit inline FCompareStackSize( ParserClass * pParser ) : m_pParser( pParser ) {}
    inline void operator () ( const Parser::CharType *, const Parser::CharType * ) const
    {
        m_pParser->CompareStackSize();
    }
    inline void operator () ( const Parser::CharType ) const
    {
        m_pParser->CompareStackSize();
    }
    ParserClass * m_pParser;
};

// ----------------------------------------------------------------------------

class IStackMessagePreparer
{
protected:
    /// Trivially implemented.
    inline IStackMessagePreparer( void ) {}
    /// Trivially implemented.
    inline virtual ~IStackMessagePreparer( void ) {}

public:

    virtual bool PrepareErrorMessage( ::Parser::ErrorLevel::Levels level,
        const CharType * message ) = 0;

    virtual bool PrepareErrorMessage( ::Parser::ErrorLevel::Levels level,
        const CharType * first, const CharType * last ) = 0;

    virtual bool PrepareContentMessage( const CharType * section ) = 0;

    virtual bool PrepareContentMessage( const CharType * section,
        const CharType * name ) = 0;

    virtual bool PrepareContentMessage( const CharType * section,
        const CharType * name, unsigned long line, unsigned long chars ) = 0;

private:
    /// Not implemented.
    IStackMessagePreparer( const IStackMessagePreparer & );
    /// Not implemented.
    IStackMessagePreparer & operator = ( const IStackMessagePreparer & );
};


// ----------------------------------------------------------------------------

class ContentStack
{

public:

    explicit ContentStack( IStackMessagePreparer * pPreparer = 0 );

    ~ContentStack( void );

    inline IStackMessagePreparer * GetPreparer( void ) { return m_pPreparer; }

    inline void SetPreparer( IStackMessagePreparer * pPreparer )
    {
        m_pPreparer = pPreparer;
    }

    void Clear( void );

    inline void Pop( void ) { if ( 0 <= m_stackIndex ) --m_stackIndex; }

    void Push( const CharType * content, const CharType * type, unsigned long line,
        unsigned long chars );

    inline bool OutputWholeStack( void ) const
    {
        return OutputSubsetStack( GetStackSize() );
    }

    bool OutputSubsetStack( unsigned long levels ) const;

    inline unsigned long GetStackSize( void ) const { return m_stackIndex+1; }

    const CharType * GetContentAt( unsigned long index ) const;

    const CharType * GetSectionAt( unsigned long index ) const;

    unsigned long GetLineAt( unsigned long index ) const;

    unsigned long GetCharColAt( unsigned long index ) const;

private:

    ContentStack( const ContentStack & );
    ContentStack & operator = ( const ContentStack & );

    enum Constants
    {
        EmptySpot = -1,
        TopPlace = 63,
        MaxStackSize
    };

    struct ContentInfo
    {
        StringType m_content;
        StringType m_section;
        unsigned long m_line;
        unsigned long m_char;

        ContentInfo( void );
        ~ContentInfo( void );
        void Clear( void );
    };

    IStackMessagePreparer * m_pPreparer;

    signed long m_stackIndex;

    ContentInfo m_messageStack[ MaxStackSize ];

};

// ----------------------------------------------------------------------------

struct FPushContent
{
    inline FPushContent( ContentStack & stack, const CharType * name,
        const CharType * type, unsigned long line, unsigned long chars ) :
        m_stack( stack ), m_name( name ), m_type( type ),
        m_line( line ), m_char( chars ) {}

    inline FPushContent( const FPushContent & that ) :
        m_stack( that.m_stack ), m_name( that.m_name ), m_type( that.m_type ),
        m_line( that.m_line ), m_char( that.m_char ) {}

    inline void operator () ( const CharType *, const CharType * ) const
    {
        m_stack.Push( m_name, m_type, m_line, m_char );
    }

    inline void operator () ( CharType ) const
    {
        m_stack.Push( m_name, m_type, m_line, m_char );
    }

    ContentStack & m_stack;
    const CharType * m_name;
    const CharType * m_type;
    unsigned long m_line;
    unsigned long m_char;

private:
    FPushContent();
    FPushContent & operator = ( const FPushContent & );
};

// ----------------------------------------------------------------------------

struct FPopContentStack
{
    inline explicit FPopContentStack( ContentStack & stack ) :
        m_stack( stack ) {}

    inline FPopContentStack( const FPopContentStack & that ) :
        m_stack( that.m_stack ) {}

    inline void operator () ( const CharType *, const CharType * ) const
    {
        m_stack.Pop();
    }

    inline void operator () ( CharType ) const
    {
        m_stack.Pop();
    }

    ContentStack & m_stack;

private:
    FPopContentStack();
    FPopContentStack & operator = ( const FPopContentStack & );
};

// ----------------------------------------------------------------------------

class MessageStack
{
public:

    static void SetDebugging( bool debug ) { s_debug = debug; }

    static bool IsDebugging( void ) { return s_debug; }

    explicit MessageStack( IStackMessagePreparer * pPreparer = 0 );

    ~MessageStack( void );

    inline IStackMessagePreparer * GetPreparer( void ) { return m_pPreparer; }

    inline void SetPreparer( IStackMessagePreparer * pPreparer )
    {
        m_pPreparer = pPreparer;
    }

    void Clear( void );

    void Prepare( Parser::ErrorLevel::Levels level, const CharType * message,
        const char * file = NULL, const char * ruleName = NULL );

    bool Send( void );

    bool Send( Parser::ErrorLevel::Levels level, const CharType * message );

    bool Send( const CharType * first, const CharType * last );

    bool Send( const CharType * content );

    void Cancel( void );

    void Push( Parser::ErrorLevel::Levels level, const CharType * message,
        const char * file = NULL, const char * ruleName = NULL );

    void Pop( void );

    // void CancelAndPop( void );

    inline unsigned long GetStackSize( void ) const { return m_stackIndex+1; }

private:

    typedef ::Parser::InvariantChecker< MessageStack > InvariantChecker;
    friend struct ::Parser::InvariantChecker< MessageStack >;

    enum Constants
    {
        EmptySpot = -1,
        TopPlace = 63,
        MaxStackSize
    };

    struct MessageInfo
    {
        Parser::ErrorLevel::Levels m_level;
        const CharType * m_message;
        const char * m_file;
        const char * m_ruleName;

        inline MessageInfo( void ) :
            m_level( Parser::ErrorLevel::None ),
            m_message( NULL ),
            m_file( NULL ),
            m_ruleName( NULL ) {}

        inline void Clear( void )
        {
            m_level = Parser::ErrorLevel::None;
            m_message = 0;
        }
    };

    MessageStack( const MessageStack & );
    MessageStack & operator = ( const MessageStack & );

    /// Sends info about stack to output.
    void DebugOutput( const char * function ) const;

    /// Checks if any class invariants were broken.
    void CheckInvariants( void ) const;

    bool DoSend( Parser::ErrorLevel::Levels level, const CharType * message );

    bool DoSend( Parser::ErrorLevel::Levels level, const CharType * first,
        const CharType * last );

    static bool s_debug;

    IStackMessagePreparer * m_pPreparer;

    Parser::ErrorLevel::Levels m_highestLevel;

    signed long m_stackIndex;

    MessageInfo m_messageStack[ MaxStackSize ];

};

// ----------------------------------------------------------------------------

struct FPushMessage
{
    inline FPushMessage( MessageStack & stack, Parser::ErrorLevel::Levels level,
        const CharType * message, const char * file = NULL, const char * ruleName = NULL ) :
        m_stack( stack ), m_level( level ), m_message( message ),
        m_file( file ), m_ruleName( ruleName ) {}

    inline FPushMessage( const FPushMessage & that ) :
        m_stack( that.m_stack ), m_level( that.m_level ), m_message( that.m_message ),
        m_file( that.m_file ), m_ruleName( that.m_ruleName ) {}

    inline void operator () ( const CharType *, const CharType * ) const
    {
        m_stack.Push( m_level, m_message, m_file, m_ruleName );
    }

    inline void operator () ( CharType ) const
    {
        m_stack.Push( m_level, m_message, m_file, m_ruleName );
    }

    MessageStack & m_stack;

    Parser::ErrorLevel::Levels m_level;

    const CharType * m_message;

    const char * m_file;

    const char * m_ruleName;

private:
    FPushMessage();
    FPushMessage & operator = ( const FPushMessage & );
};

// ----------------------------------------------------------------------------

struct FPopMessageStack
{
    inline explicit FPopMessageStack( MessageStack & stack ) :
        m_stack( stack ) {}

    inline FPopMessageStack( const FPopMessageStack & that ) :
        m_stack( that.m_stack ) {}

    inline void operator () ( const CharType *, const CharType * ) const
    {
        m_stack.Pop();
    }

    inline void operator () ( CharType ) const
    {
        m_stack.Pop();
    }

    MessageStack & m_stack;

private:
    FPopMessageStack();
    FPopMessageStack & operator = ( const FPopMessageStack & );
};

// ----------------------------------------------------------------------------

struct FPrepareMessage
{
    inline FPrepareMessage( MessageStack & stack,
        Parser::ErrorLevel::Levels level, const CharType * message,
        const char * file = NULL, const char * ruleName = NULL ) :
        m_stack( stack ), m_level( level ), m_message( message ),
        m_file( file ), m_ruleName( ruleName ) {}

    inline FPrepareMessage( const FPrepareMessage & that ) :
        m_stack( that.m_stack ), m_level( that.m_level ), m_message( that.m_message ),
        m_file( that.m_file ), m_ruleName( that.m_ruleName ) {}

    inline void operator () ( const CharType *, const CharType * ) const
    {
        m_stack.Prepare( m_level, m_message, m_file, m_ruleName );
    }

    inline void operator () ( CharType ) const
    {
        m_stack.Prepare( m_level, m_message, m_file, m_ruleName );
    }

    MessageStack & m_stack;

    Parser::ErrorLevel::Levels m_level;

    const CharType * m_message;

    const char * m_file;

    const char * m_ruleName;

private:
    FPrepareMessage();
    FPrepareMessage & operator = ( const FPrepareMessage & );
};

// ----------------------------------------------------------------------------

struct FSendMessageNow
{
    inline FSendMessageNow( MessageStack & stack,
        Parser::ErrorLevel::Levels level, const CharType * message ) :
        m_stack( stack ), m_level( level ), m_message( message ) {}

    inline FSendMessageNow( const FSendMessageNow & that ) :
        m_stack( that.m_stack ), m_level( that.m_level ),
        m_message( that.m_message ) {}

    inline void operator () ( const CharType *, const CharType * ) const
    {
        m_stack.Send( m_level, m_message );
    }

    inline void operator () ( CharType ) const
    {
        m_stack.Send( m_level, m_message );
    }

    MessageStack & m_stack;

    Parser::ErrorLevel::Levels m_level;

    const CharType * m_message;

private:
    FSendMessageNow();
    FSendMessageNow & operator = ( const FSendMessageNow & );
};

// ----------------------------------------------------------------------------

struct FCancelMessage
{
    inline explicit FCancelMessage( MessageStack & stack ) :
        m_stack( stack ) {}

    inline FCancelMessage( const FCancelMessage & that ) :
        m_stack( that.m_stack ) {}

    inline void operator () ( const CharType *, const CharType * ) const
    {
        m_stack.Cancel();
    }

    inline void operator () ( CharType ) const
    {
        m_stack.Cancel();
    }

    MessageStack & m_stack;

private:
    FCancelMessage();
    FCancelMessage & operator = ( const FCancelMessage & );
};

// ----------------------------------------------------------------------------
//
//struct FCancelMessageAndPop
//{
//    inline explicit FCancelMessageAndPop( MessageStack & stack ) :
//        m_stack( stack ) {}
//
//    inline FCancelMessageAndPop( const FCancelMessageAndPop & that ) :
//        m_stack( that.m_stack ) {}
//
//    inline void operator () ( const CharType *, const CharType * ) const
//    {
//        m_stack.CancelAndPop();
//    }
//
//    inline void operator () ( CharType ) const
//    {
//        m_stack.CancelAndPop();
//    }
//
//    MessageStack & m_stack;
//
//private:
//    FCancelMessageAndPop();
//    FCancelMessageAndPop & operator = ( const FCancelMessageAndPop & );
//};

// ----------------------------------------------------------------------------

struct FSendTopMessage
{
    inline explicit FSendTopMessage( MessageStack & stack ) :
        m_stack( stack ) {}

    inline FSendTopMessage( const FSendTopMessage & that ) :
        m_stack( that.m_stack ) {}

    inline void operator () ( const CharType *, const CharType * ) const
    {
        m_stack.Send();
    }

    inline void operator () ( CharType ) const
    {
        m_stack.Send();
    }

    MessageStack & m_stack;

private:
    FSendTopMessage();
    FSendTopMessage & operator = ( const FSendTopMessage & );
};


// ----------------------------------------------------------------------------

struct FBadContentMsg
{
    inline FBadContentMsg( MessageStack & stack,
        Parser::ErrorLevel::Levels level, const CharType * message ) :
        m_stack( stack ), m_level( level ), m_message( message ) {}

    inline FBadContentMsg( const FBadContentMsg & that ) :
        m_stack( that.m_stack ), m_level( that.m_level ),
        m_message( that.m_message ) {}

    inline void operator () ( const CharType * first, const CharType * last ) const
    {
        m_stack.Send( m_level, m_message );
        m_stack.Send( first, last );
    }

    inline void operator () ( CharType ) const
    {
        m_stack.Send( m_level, m_message );
    }

    MessageStack & m_stack;

    Parser::ErrorLevel::Levels m_level;

    const CharType * m_message;

private:
    FBadContentMsg();
    FBadContentMsg & operator = ( const FBadContentMsg & );
};


// ----------------------------------------------------------------------------

}; // end namespace Parser

#endif // file guardian

// $Log: ParseUtil.hpp,v $
// Revision 1.5  2009/01/05 19:25:48  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.4  2008/12/11 20:37:26  rich_sposato
// Changed include paths to use the more general form.
//
// Revision 1.3  2008/12/09 19:38:27  rich_sposato
// Fixed bug related to maintaining class invariants even when exceptions occur.
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
