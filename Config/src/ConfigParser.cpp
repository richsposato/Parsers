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

// $Header: /cvsroot/utilparsers/UtilParsers/Config/src/ConfigParser.cpp,v 1.7 2009/01/05 19:24:52 rich_sposato Exp $

/// @file ConfigParser.cpp Contains implementation for config file parser.


// ----------------------------------------------------------------------------
// Include Files

#include "../include/ConfigParser.hpp"

#include <string.h>
#include <memory.h>
#include <string>
#include <fstream>
#include <algorithm>

#include "../../Util/include/ParseUtil.hpp"
#include "../../Util/include/ParseInfo.hpp"
#include "../../Util/include/ErrorReceiver.hpp"

#include "ParserRules.hpp"
#include "CommonParsers.hpp"


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if defined( _MSC_VER )
    #pragma inline_depth(255)
    #pragma inline_recursion(on)
#endif

#if defined( _MSC_VER )
#pragma warning( disable : 4189 )
#endif


// ----------------------------------------------------------------------------
// Namespace resolution.

using namespace ::std;
using namespace ::boost::spirit;


// ----------------------------------------------------------------------------

typedef istream_iterator< char > FileStreamIter;

namespace
{

// ----------------------------------------------------------------------------

struct XMaxErrorException { };


// ----------------------------------------------------------------------------

inline bool IsGoodResult( ::Parser::ConfigParser::ParseResults result )
{
    return ( ( ::Parser::ConfigParser::AllValid == result )
          || ( ::Parser::ConfigParser::SomeValid == result ) );
}

// ----------------------------------------------------------------------------

::Parser::ConfigParser::ParseResults Convert( ::Parser::ParseInfo::ParseResult result )
{
    switch ( result )
    {
        case ::Parser::ParseInfo::AllValid:     return ::Parser::ConfigParser::AllValid;
        case ::Parser::ParseInfo::SomeValid:    return ::Parser::ConfigParser::SomeValid;
        case ::Parser::ParseInfo::NotValid:     return ::Parser::ConfigParser::NotValid;
        case ::Parser::ParseInfo::NotParsed:    return ::Parser::ConfigParser::NotParsed;
        case ::Parser::ParseInfo::NoFileName:   return ::Parser::ConfigParser::NoFileName;
        case ::Parser::ParseInfo::CantOpenFile: return ::Parser::ConfigParser::CantOpenFile;
        case ::Parser::ParseInfo::EndOfFile:    return ::Parser::ConfigParser::EndOfFile;
        default:
            break;
    }
    return ::Parser::ConfigParser::NotValid;
}

// ChangeEmbeddedNils -------------------------------------------------------------------------

bool ChangeEmbeddedNils( string & ss, char ch = '\0' )
{

    if ( ch == '\0' )
        return false;
    bool found = false;
    string::iterator here( ss.begin() );
    string::iterator last( ss.end() );
    while ( here != last )
    {
        if ( '\0' == *here )
        {
            found = true;
            *here = ch;
        }
        ++here;
    }

    return found;
}

// ReadFileIntoString -------------------------------------------------------------------------

bool ReadFileIntoString( const char * filename, string & target )
{

    target.clear();
    ifstream input( filename );
    if ( input.fail() )
        return false;
    input.unsetf( ios::skipws ); // Must turn off whitespace skipping.

    FileStreamIter first( input );
    FileStreamIter last;
    string temp( first, last );
    ChangeEmbeddedNils( temp );
    temp.swap( target );

    return true;
}

// ----------------------------------------------------------------------------

} // end anonymous namespace

// ----------------------------------------------------------------------------

#define PARSER_CATCH_MAX_ERROR_EXCP_BLOCK( message ) \
    catch ( const XMaxErrorException & ) \
    { \
        tempCount = m_ErrorCount; \
        m_ErrorCount = 0; \
        PrepareErrorMessage( ErrorLevel::Fatal, "Too many errors found. Stopping parsing." ); \
        PrepareErrorMessage( ErrorLevel::Fatal, message ); \
        m_ErrorCount = tempCount + 2; \
        EndParse(); \
        result = ConfigParser::Exception; \
    }

#define PARSER_CATCH_STD_EXCP_BLOCK( message ) \
    catch ( const std::exception & ex ) \
    { \
        tempCount = m_ErrorCount; \
        m_ErrorCount = 0; \
        PrepareErrorMessage( ErrorLevel::Except, ex.what() ); \
        PrepareErrorMessage( ErrorLevel::Except, message ); \
        m_ErrorCount = tempCount + 2; \
        EndParse(); \
        result = ConfigParser::Exception; \
    }

#define PARSER_CATCH_ALL_BLOCK( message ) \
    catch ( ... ) \
    { \
        tempCount = m_ErrorCount; \
        m_ErrorCount = 0; \
        PrepareErrorMessage( ErrorLevel::Except, message ); \
        m_ErrorCount = tempCount + 2; \
        EndParse(); \
        result = ConfigParser::Exception; \
    }

// ----------------------------------------------------------------------------

namespace Parser
{

// ----------------------------------------------------------------------------

ConfigParser::ParserPolicy::ParserPolicy( void ) :
    LineComment( ";" ),
    BlockCommentStarter( "/*" ),
    BlockCommentEnder( "*/" ),
    SectionNameStarter( "[" ),
    SectionNameEnder( "]" ),
    AssignOperator( "=" ),
    TrimWhiteSpace( false ),
    AlphaNumericNames( false ),
    AllowQuotedCommentInValue( false ),
    MaxErrorCount( 100 )
{
    assert( NULL != this );
}

// ----------------------------------------------------------------------------

class ConfigParserImpl : private IStackMessagePreparer
{
public:

    ConfigParserImpl( void );

    virtual ~ConfigParserImpl( void );

    bool SetPolicy( const ConfigParser::ParserPolicy & policy );

    void FreePolicy( void );

    virtual bool PrepareErrorMessage( ::Parser::ErrorLevel::Levels level,
        const char * message );

    virtual bool PrepareErrorMessage( ::Parser::ErrorLevel::Levels level,
        const char * first, const char * last );

    virtual bool PrepareContentMessage( const char * section );

    virtual bool PrepareContentMessage( const char * section, const char * name );

    virtual bool PrepareContentMessage( const char * section, const char * name,
        unsigned long line, unsigned long chars );

    ConfigParser::ParseResults ParseContents( const char * start, const char * end,
        IConfigReceiver * pReceiver );

    void StartParse( void );

    inline void EndParse( void ) { m_parsing = false; }

    bool m_parsing;
    bool m_allocateMemory;
    char m_Storage[ 32 ];
    unsigned int m_ErrorCount;
    IParseErrorReceiver * m_pErrorReceiver;

    ConfigParser::ParserPolicy m_policy;

    ParseInfo m_results;
    LineCounter m_Counter;
    MessageStack m_Messages;
    ConfigFileParser m_parser;

private:
    /// Not implemented.
    inline ConfigParserImpl( const ConfigParserImpl & );
    /// Not implemented.
    inline ConfigParserImpl & operator = ( const ConfigParserImpl & );

};

// ----------------------------------------------------------------------------

ConfigParserImpl::ConfigParserImpl( void ) :
    IStackMessagePreparer(),
    m_parsing( false ),
    m_allocateMemory( false ),
    m_ErrorCount( 0 ),
    m_pErrorReceiver( NULL ),
    m_policy(),
    m_results(),
    m_Counter(),
    m_Messages( this ),
    m_parser( m_policy, m_Messages, m_Counter )
{
    assert( NULL != this );
}

// ----------------------------------------------------------------------------

ConfigParserImpl::~ConfigParserImpl( void )
{
    assert( NULL != this );
    FreePolicy();
}

// ----------------------------------------------------------------------------

void ConfigParserImpl::FreePolicy( void )
{
    assert( NULL != this );
    if ( m_allocateMemory && ( NULL != m_policy.LineComment ) )
    {
        assert( m_policy.LineComment != m_Storage );
        ::free( m_policy.LineComment );
    }
}

// ----------------------------------------------------------------------------

bool ConfigParserImpl::SetPolicy( const ConfigParser::ParserPolicy & policy )
{
    assert( NULL != this );

    const unsigned long lineCommentSize         = ::strlen( policy.LineComment ) + 1;
    const unsigned long blockCommentStarterSize = ::strlen( policy.BlockCommentStarter ) + 1;
    const unsigned long blockCommentEnderSize   = ::strlen( policy.BlockCommentEnder ) + 1;
    const unsigned long sectionNameStarterSize  = ::strlen( policy.SectionNameStarter ) + 1;
    const unsigned long sectionNameEnderSize    = ::strlen( policy.SectionNameEnder ) + 1;
    const unsigned long assignOperatorSize      = ::strlen( policy.AssignOperator ) + 1;

    const unsigned long totalLength = lineCommentSize +
        blockCommentStarterSize + blockCommentEnderSize +
        sectionNameStarterSize + sectionNameEnderSize + assignOperatorSize;

    FreePolicy();
    m_allocateMemory = ( sizeof(m_Storage) < totalLength );
    m_policy.LineComment = m_allocateMemory ?
        reinterpret_cast< char * >( ::malloc( totalLength ) ) : m_Storage;

    m_policy.BlockCommentStarter = m_policy.LineComment         + lineCommentSize;
    m_policy.BlockCommentEnder   = m_policy.BlockCommentStarter + blockCommentStarterSize;
    m_policy.SectionNameStarter  = m_policy.BlockCommentEnder   + blockCommentEnderSize;
    m_policy.SectionNameEnder    = m_policy.SectionNameStarter  + sectionNameStarterSize;
    m_policy.AssignOperator      = m_policy.SectionNameEnder    + sectionNameEnderSize;

    ::strcpy( m_policy.LineComment,         policy.LineComment );
    ::strcpy( m_policy.BlockCommentStarter, policy.BlockCommentStarter );
    ::strcpy( m_policy.BlockCommentEnder,   policy.BlockCommentEnder );
    ::strcpy( m_policy.SectionNameStarter,  policy.SectionNameStarter );
    ::strcpy( m_policy.SectionNameEnder,    policy.SectionNameEnder );
    ::strcpy( m_policy.AssignOperator,      policy.AssignOperator );

    m_policy.TrimWhiteSpace = policy.TrimWhiteSpace;
    m_policy.AlphaNumericNames = policy.AlphaNumericNames;
    m_policy.AllowQuotedCommentInValue = policy.AllowQuotedCommentInValue;
    m_policy.MaxErrorCount = policy.MaxErrorCount;
    if ( m_policy.MaxErrorCount < 4 )
        m_policy.MaxErrorCount = 4;
    m_parser.SetPolicy( policy, m_Messages, m_Counter );

    return true;
}

// ----------------------------------------------------------------------------

void ConfigParserImpl::StartParse( void )
{
    assert( NULL != this );
    m_parsing = true;
    m_ErrorCount = 0;
    m_Counter.Reset();
    m_Messages.Clear();
}

// ----------------------------------------------------------------------------

ConfigParser::ParseResults ConfigParserImpl::ParseContents( const char * start, const char * end,
    IConfigReceiver * pReceiver )
{
    assert( NULL != this );

    ConfigParser::ParseResults result = ConfigParser::NotParsed;
    unsigned long tempCount = 0;
    try
    {
        StartParse();
        m_parser.SetReceiver( pReceiver );
        const SpiritRule & myRule = m_parser.GetRule();
        const ParseInfo::ParseResult result1 = m_results.Parse( start, end, myRule );
        result = Convert( result1 );
        if ( IsGoodResult( result ) && !m_parser.IsValid() )
            result = ConfigParser::NotValid;
        EndParse();
    }
    PARSER_CATCH_MAX_ERROR_EXCP_BLOCK( "Unable to continue parsing config contents." )
    PARSER_CATCH_STD_EXCP_BLOCK( "Exception thrown when parsing config contents!" )
    PARSER_CATCH_ALL_BLOCK( "Unknown exception thrown when parsing config contents!" )

    return result;
}

// ----------------------------------------------------------------------------

bool ConfigParserImpl::PrepareErrorMessage( ::Parser::ErrorLevel::Levels level,
    const char * message )
{
    assert( NULL != this );

    if ( ( NULL == message ) || ( '\0' == *message ) )
        return false;
    const unsigned int length = ::strlen( message );
    const CharType * last = message + length;
    const bool okay = PrepareErrorMessage( level, message, last );
    return okay;
}

// ----------------------------------------------------------------------------

bool ConfigParserImpl::PrepareErrorMessage( ::Parser::ErrorLevel::Levels level,
    const char * first, const char * last )
{
    assert( NULL != this );
    assert( NULL != first );
    assert( NULL != last );
    assert( first < last );
    (void)last;

    if ( NULL == m_pErrorReceiver )
        return false;
    bool okay = true;
    if ( !m_pErrorReceiver->GiveParseMessage( level, first ) )
    {
        m_pErrorReceiver = NULL;
        okay = false;
    }
    if ( ErrorLevel::Minor <= level )
    {
        ++m_ErrorCount;
        if ( m_policy.MaxErrorCount <= m_ErrorCount )
        {
            throw XMaxErrorException();
        }
    }
    return okay;
}

// ----------------------------------------------------------------------------

bool ConfigParserImpl::PrepareContentMessage( const char * section )
{
    assert( NULL != this );
    if ( NULL == m_pErrorReceiver )
        return false;
    if ( !m_pErrorReceiver->GiveParseMessage( ErrorLevel::Content, section ) )
    {
        m_pErrorReceiver = NULL;
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------

bool ConfigParserImpl::PrepareContentMessage( const char * section, const char * name )
{
    assert( NULL != this );
    if ( NULL == m_pErrorReceiver )
        return false;

    unsigned long length = ::strlen( section ) + ::strlen( name ) + 100;
    char * buffer = reinterpret_cast< char * >( ::malloc( length ) );
    if ( NULL == buffer )
        return false;
    auto_ptr< char > temp( buffer );

    ::sprintf( buffer, "Inside %s section for %s\n", section, name );
    if ( !m_pErrorReceiver->GiveParseMessage( ErrorLevel::Content, buffer ) )
    {
        m_pErrorReceiver = NULL;
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool ConfigParserImpl::PrepareContentMessage( const char * section,
    const char * name, unsigned long line, unsigned long chars )
{
    assert( NULL != this );

    if ( NULL == m_pErrorReceiver )
        return false;

    unsigned long length = ::strlen( section ) + ::strlen( name ) + 200;
    char * buffer = reinterpret_cast< char * >( ::malloc( length ) );
    if ( NULL == buffer )
        return false;
    auto_ptr< char > temp( buffer );

    ::sprintf( buffer, "Inside %s section for %s on line %u at char %u.\n",
        section, name, line, chars );
    if ( !m_pErrorReceiver->GiveParseMessage( ErrorLevel::Content, buffer ) )
    {
        m_pErrorReceiver = NULL;
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------

unsigned long ::Parser::ConfigParser::GetMajorVersion( void )
{
    return 1;
}

// ----------------------------------------------------------------------------

unsigned long ::Parser::ConfigParser::GetMinorVersion( void )
{
    return 0;
}

// ----------------------------------------------------------------------------

const char * ::Parser::ConfigParser::Name( ConfigParser::ParseResults result )
{
    switch ( result )
    {
        case AllValid:        return "All data was parsed and valid.";
        case SomeValid:        return "Start is valid, but ending part was not recognized.";
        case NotValid:        return "Data was parsed, but is not valid.";
        case NotParsed:        return "Data was not recognized by parsing rules.";
        case NoReceiver:    return "No content receiver provided.";
        case NoStart:        return "Starting place is NULL.";
        case EmptyData:        return "No content at starting place.";
        case NoEnd:            return "Ending place is NULL.";
        case LowerEnd:        return "Ending place less than starting place.";
        case Exception:        return "Exception thrown.";
        case ParsingNow:    return "Parser is currently parsing other data.";
        case Receiving:        return "Receiver reported an error.";
        case NoErrorRecv:    return "No error receiver provided.";
        case NoFileName:    return "No file name provided for parsing.";
        case CantOpenFile:    return "Could not open file.";
        case EmptyFile:     return "File has no contents.";
        case EndOfFile:        return "Reached end of file, but can't parse content.";
    }
    return "Unknown parsing result";
}

// ----------------------------------------------------------------------------

ConfigParser::ConfigParser( void ) :
    m_impl( NULL )
{
    assert( NULL != this );
    m_impl = new ConfigParserImpl;
    assert( NULL != m_impl );
}

// ----------------------------------------------------------------------------

ConfigParser::~ConfigParser( void )
{
    assert( NULL != this );
    assert( NULL != m_impl );
    delete m_impl;
}

// ----------------------------------------------------------------------------

bool ConfigParser::SetMessageReceiver( IParseErrorReceiver * pReceiver )
{
    assert( NULL != this );
    assert( NULL != m_impl );
    if ( NULL == pReceiver )
        return false;
    m_impl->m_pErrorReceiver = pReceiver;
    return true;
}

// ----------------------------------------------------------------------------

IParseErrorReceiver * ConfigParser::GetMessageReceiver( void )
{
    assert( NULL != this );
    assert( NULL != m_impl );
    return m_impl->m_pErrorReceiver;
}

// ----------------------------------------------------------------------------

bool ConfigParser::IsParsing( void ) const
{
    assert( NULL != this );
    assert( NULL != m_impl );
    return m_impl->m_parsing;
}

// ----------------------------------------------------------------------------

bool ConfigParser::SetPolicy( const ConfigParser::ParserPolicy & policy )
{
    assert( NULL != this );
    assert( NULL != m_impl );

    if ( NULL == policy.LineComment         ) return false;
    if ( NULL == policy.BlockCommentStarter ) return false;
    if ( NULL == policy.BlockCommentEnder   ) return false;
    if ( NULL == policy.SectionNameStarter  ) return false;
    if ( NULL == policy.SectionNameEnder    ) return false;
    if ( NULL == policy.AssignOperator      ) return false;

    if ( ::strcmp( policy.LineComment, policy.BlockCommentStarter ) == 0 )
        return false;
    if ( ::strcmp( policy.LineComment, policy.BlockCommentEnder ) == 0 )
        return false;
    if ( ::strcmp( policy.LineComment, policy.SectionNameStarter ) == 0 )
        return false;
    if ( ::strcmp( policy.LineComment, policy.SectionNameEnder ) == 0 )
        return false;
    if ( ::strcmp( policy.LineComment, policy.AssignOperator ) == 0 )
        return false;

    if ( ::strcmp( policy.BlockCommentStarter, policy.BlockCommentEnder ) == 0 )
        return false;
    if ( ::strcmp( policy.BlockCommentStarter, policy.SectionNameStarter ) == 0 )
        return false;
    if ( ::strcmp( policy.BlockCommentStarter, policy.SectionNameEnder ) == 0 )
        return false;
    if ( ::strcmp( policy.BlockCommentStarter, policy.AssignOperator ) == 0 )
        return false;

    if ( ::strcmp( policy.BlockCommentEnder, policy.SectionNameStarter ) == 0 )
        return false;
    if ( ::strcmp( policy.BlockCommentEnder, policy.SectionNameEnder ) == 0 )
        return false;
    if ( ::strcmp( policy.BlockCommentEnder, policy.AssignOperator ) == 0 )
        return false;

    if ( ::strcmp( policy.SectionNameEnder, policy.AssignOperator ) == 0 )
        return false;

    return m_impl->SetPolicy( policy );
}

// ----------------------------------------------------------------------------

const ConfigParser::ParserPolicy & ConfigParser::GetPolicy( void ) const
{
    assert( NULL != this );
    assert( NULL != m_impl );
    return m_impl->m_policy;
}

// ----------------------------------------------------------------------------

ConfigParser::ParseResults ConfigParser::Parse( const char * start, const char * end,
    IConfigReceiver * pReceiver )
{
    assert( NULL != this );
    assert( NULL != m_impl );

    if ( m_impl->m_parsing )
        return ConfigParser::ParsingNow;
    if ( NULL == start )
        return ConfigParser::NoStart;
    if ( '\0' == *start )
        return ConfigParser::EmptyData;
    if ( NULL == end )
        return ConfigParser::NoEnd;
    if ( end  <= start )
        return ConfigParser::LowerEnd;
    if ( NULL == pReceiver )
        return ConfigParser::NoReceiver;
    if ( NULL == m_impl->m_pErrorReceiver )
        return ConfigParser::NoErrorRecv;

    return m_impl->ParseContents( start, end, pReceiver );
}

// ----------------------------------------------------------------------------

ConfigParser::ParseResults ConfigParser::Parse( const char * filename,
    IConfigReceiver * pReceiver )
{
    assert( NULL != this );
    assert( NULL != m_impl );

    if ( m_impl->m_parsing )
        return ConfigParser::ParsingNow;
    if ( NULL == filename )
        return ConfigParser::NoFileName;
    if ( '\0' == *filename )
        return ConfigParser::NoFileName;
    if ( NULL == pReceiver )
        return ConfigParser::NoReceiver;
    if ( NULL == m_impl->m_pErrorReceiver )
        return ConfigParser::NoErrorRecv;

    string fileContents;
    unsigned long contentSize = 0;
    try
    {
        if ( !ReadFileIntoString( filename, fileContents ) )
        {
            return ConfigParser::CantOpenFile;
        }

        contentSize = fileContents.size();
        if ( 0 == contentSize )
            return ConfigParser::EmptyFile;
        // Ensure that we have enough trailing newlines to eliminate
        // the need to check for end of file in the grammar.
        fileContents.push_back( '\n' );
    }
    catch ( ... )
    {
        return ConfigParser::CantOpenFile;
    }

    const char * start = fileContents.c_str();
    const char * end = start + contentSize + 1;
    return m_impl->ParseContents( start, end, pReceiver );
}

// ----------------------------------------------------------------------------

} // end namespace Parser

// $Log: ConfigParser.cpp,v $
// Revision 1.7  2009/01/05 19:24:52  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.6  2009/01/05 07:31:36  rich_sposato
// Added paths to project settings.
//
// Revision 1.5  2008/12/09 19:41:28  rich_sposato
// Changed policy names.
//
// Revision 1.4  2008/12/09 00:24:13  rich_sposato
// Put pragmas inside #if sections.
//
// Revision 1.3  2008/12/08 23:11:43  rich_sposato
// A few changes to ConfigParser to make it more versatile and robust.
//
// Revision 1.2  2008/12/06 08:50:12  rich_sposato
// Changes to improve debugging abilities or fix a bug.
//
// Revision 1.1  2008/12/05 19:25:28  rich_sposato
// Adding files to CVS.
//
