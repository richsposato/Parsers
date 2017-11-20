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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/src/XmlParser.cpp,v 1.1 2008/12/05 19:04:16 rich_sposato Exp $


#include "../include/XmlParser.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <strstream>

#include <boost/spirit.hpp>
#include <boost/spirit/core.hpp>

#include <loki/SafeFormat.h>

#include "../../Util/include/ParseUtil.hpp"
#include "../../Util/include/ParseInfo.hpp"

#include "./BasicParsers.hpp"
#include "./PrologParsers.hpp"


#ifdef DEBUG
    #define DEBUG_CODE( x ) x
#else
    #define DEBUG_CODE( x )
#endif

DEBUG_CODE( #include <iostream> )


using namespace std;
using namespace boost::spirit;

typedef std::vector< Parser::CharType > TCharBuffer;



// ChangeEmbeddedNils ---------------------------------------------------------

bool ChangeEmbeddedNils( string & ss, Parser::CharType ch = ' ' )
{

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

// ReadFileIntoString ---------------------------------------------------------

bool ReadFileIntoString( const char * filename, string & target )
{

    target.clear();
    ifstream input( filename );
    if ( input.fail() ) return false;
    input.unsetf( ios::skipws ); // Must turn off whitespace skipping.

    istream_iterator< Parser::CharType > begin( input );
    istream_iterator< Parser::CharType > end;
    string temp( begin, end );
    ChangeEmbeddedNils( temp );
    temp.swap( target );

    return true;
}

// ----------------------------------------------------------------------------

namespace Parser
{

namespace Xml
{

// ----------------------------------------------------------------------------

XmlParser::ParseResults Convert( Parser::ParseInfo::ParseResult result )
{
    switch ( result )
    {
        case Parser::ParseInfo::AllValid:     return XmlParser::AllValid;
        case Parser::ParseInfo::SomeValid:    return XmlParser::SomeValid;
        case Parser::ParseInfo::NotValid:     return XmlParser::NotValid;
        case Parser::ParseInfo::NotParsed:    return XmlParser::NotParsed;
        case Parser::ParseInfo::NoFileName:   return XmlParser::NoFileName;
        case Parser::ParseInfo::CantOpenFile: return XmlParser::CantOpenFile;
        case Parser::ParseInfo::EndOfFile:    return XmlParser::EndOfFile;
        default:
            break;
    }
    return XmlParser::NotValid;
}

// ----------------------------------------------------------------------------

inline bool IsGoodResult( XmlParser::ParseResults result )
{
    return ( ( XmlParser::AllValid == result )
          || ( XmlParser::SomeValid == result ) );
}

// ----------------------------------------------------------------------------

class XmlParserImpl : public Parser::IStackMessagePreparer
{
public:

    XmlParserImpl( void );
    virtual ~XmlParserImpl( void );

    Parser::IParseErrorReceiver * GetErrorReceiver( void ) { return m_errorReceiver; }
    void SetErrorReceiver( Parser::IParseErrorReceiver * receiver )
    { m_errorReceiver = receiver; }

    bool IsReady( void ) const
    {
        return ( NULL != m_errorReceiver ) && ( !m_parsing );
    }

    inline XmlParser::ParseResults DoPreliminaryChecks( const char * begin,
        const char * end )
    {
        if ( !IsReady() )
            return XmlParser::NotReady;
        if ( NULL == begin )
            return XmlParser::NullStart;
        if ( ( begin == end ) || ( '\0' == *begin ) )
            return XmlParser::EmptyData;
        if ( NULL == end )
            return XmlParser::NullEnd;
        if ( end < begin )
            return XmlParser::EndTooLow;
        return XmlParser::AllValid;
    }

    inline XmlParser::ParseResults DoPreliminaryChecks( const char * begin )
    {
        if ( !IsReady() )
            return XmlParser::NotReady;
        if ( NULL == begin )
            return XmlParser::NullStart;
        if ( '\0' == *begin )
            return XmlParser::EmptyData;
        return XmlParser::AllValid;
    }

    inline XmlParser::ParseResults ParseComment( const char * begin,
        const char * end, ICommentReceiver * receiver )
    {
        return DoParse( begin, end, m_commentParser, receiver );
    }

    inline XmlParser::ParseResults ParsePublicIdLiteral( const char * begin,
        const char * end, IPublicIdReceiver * receiver )
    {
        return DoParse( begin, end, m_publicIdLiteralParser, receiver );
    }

    inline XmlParser::ParseResults ParseExternalId( const char * begin,
        const char * end, IExternalIdReceiver * receiver )
    {
        return DoParse( begin, end, m_externalIdLiteralParser, receiver );
    }

    inline XmlParser::ParseResults ParseName( const CharType * begin,
        const CharType * end, INameReceiver * receiver )
    {
        return DoParse( begin, end, m_nameParser, receiver );
    }

    inline XmlParser::ParseResults ParsePeReference(
        const char * begin, const char * end, IPeReferenceReceiver * receiver )
    {
        return DoParse( begin, end, m_peRefParser, receiver );
    }

    inline XmlParser::ParseResults ParseReference(
        const char * begin, const char * end, IReferenceReceiver * receiver )
    {
        return DoParse( begin, end, m_referenceParser, receiver );
    }

    inline XmlParser::ParseResults ParseAttributeValue( const char * begin,
        const char * end, IAttributeValueReceiver * receiver )
    {
        return DoParse( begin, end, m_attributeValueParser, receiver );
    }

    inline XmlParser::ParseResults ParseAttribute( const CharType * begin,
        const CharType * end, IAttributeReceiver * receiver )
    {
        return DoParse( begin, end, m_attributeParser, receiver );
    }

    inline XmlParser::ParseResults ParseEnumeratedType( const CharType * begin,
        const CharType * end, IEnumeratedTypeReceiver * receiver )
    {
        return DoParse( begin, end, m_enumeratedTypeParser, receiver );
    }

    inline XmlParser::ParseResults ParseEntityValue( const CharType * begin,
        const CharType * end, IEntityValueReceiver * receiver )
    {
        return DoParse( begin, end, m_entityValueParser, receiver );
    }

    inline XmlParser::ParseResults ParseEncoding( const CharType * begin,
        const CharType * end, IEncodingDeclReceiver * receiver )
    {
        return DoParse( begin, end, m_encodingDeclParser, receiver );
    }

    inline XmlParser::ParseResults ParseXmlDeclaration( const CharType * begin,
        const CharType * end, IXmlDeclarationReceiver * receiver )
    {
        return DoParse( begin, end, m_xmlDeclarationParser, receiver );
    }

    inline XmlParser::ParseResults ParseAttListDecl( const CharType * begin,
        const CharType * end, IAttListDeclReceiver * receiver )
    {
        return DoParse( begin, end, m_attListDeclParser, receiver );
    }

    inline XmlParser::ParseResults ParseNode( const CharType * begin,
        const CharType * end, INodeReceiver * receiver )
    {
        (void)begin;
        (void)end;
        (void)receiver;
        return XmlParser::NotValid;
    }

    inline XmlParser::ParseResults ParseDocument( const CharType * begin,
        const CharType * end, IDocumentReceiver * receiver )
    {
        (void)begin;
        (void)end;
        (void)receiver;
        return XmlParser::NotValid;
    }

private:

    XmlParserImpl( const XmlParserImpl & );
    XmlParserImpl & operator = ( const XmlParserImpl & );

    void Setup( void );

    inline void Cleanup( void )
    {
        m_parsing = false;
        m_errorCount = 0;
    }

    class Cleaner
    {
    public:
        inline explicit Cleaner( XmlParserImpl * p ) : m_p( p ) {}
        inline ~Cleaner( void )
        {
            m_p->Cleanup();
        }
        XmlParserImpl * m_p;
    };

    template < class MyParser, class IReceiver >
    inline XmlParser::ParseResults DoParse( const CharType * begin,
        const CharType * end, MyParser & parser, IReceiver * receiver )
    {

        Cleaner cleaner( this );
        Setup();
        parser.SetReceiver( receiver );
        const SpiritRule & rule = parser.GetRule();
        const ParseInfo::ParseResult rawResult = m_results.Parse( begin, end, rule );
        XmlParser::ParseResults result( Convert( rawResult ) );
        if ( IsGoodResult( result ) )
        {
            if ( !parser.IsValid() )
                result = XmlParser::NotValid;
        }

        return result;
    }

    bool PrepareErrorMessage( Parser::ErrorLevel::Levels level,
        const CharType * message );

    virtual bool PrepareErrorMessage( Parser::ErrorLevel::Levels level,
        const CharType * begin, const CharType * end );

    virtual bool PrepareContentMessage( const CharType * section );

    virtual bool PrepareContentMessage( const CharType * section,
        const CharType * name );

    virtual bool PrepareContentMessage( const CharType * section,
        const CharType * name, unsigned long line, unsigned long chars );

    bool m_parsing;
    unsigned long m_errorCount;
    unsigned long m_maxErrorCount;

    Parser::IParseErrorReceiver * m_errorReceiver;
    ParserStacks m_stacks;
    Parser::ParseInfo m_results;

    CommentParser           m_commentParser;
    PublicIdLiteralParser   m_publicIdLiteralParser;
    ExternalIdLiteralParser m_externalIdLiteralParser;
    NameParser              m_nameParser;
    PeReferenceParser       m_peRefParser;
    ReferenceParser         m_referenceParser;
    AttributeValueParser    m_attributeValueParser;
    AttributeParser         m_attributeParser;
    EnumeratedTypeParser    m_enumeratedTypeParser;
    EntityValueParser       m_entityValueParser;
    EncodingDeclParser      m_encodingDeclParser;
    XmlDeclarationParser    m_xmlDeclarationParser;
    AttListDeclParser       m_attListDeclParser;

}; // end class XmlParserImpl

// ----------------------------------------------------------------------------

XmlParserImpl::XmlParserImpl() :
    IStackMessagePreparer(),
    m_parsing( false ),
    m_errorCount( 0 ),
    m_maxErrorCount( 100 ),
    m_errorReceiver( NULL ),
    m_stacks( this ),
    m_results( &m_stacks.m_messages, &m_stacks.m_content ),
    m_commentParser( m_stacks ),
    m_publicIdLiteralParser( m_stacks ),
    m_externalIdLiteralParser( m_publicIdLiteralParser ),
    m_nameParser( m_stacks ),
    m_peRefParser( m_nameParser ),
    m_referenceParser( m_nameParser ),
    m_attributeValueParser( m_referenceParser ),
    m_attributeParser( m_nameParser, m_attributeValueParser ),
    m_enumeratedTypeParser( m_nameParser ),
    m_entityValueParser( m_referenceParser, m_peRefParser ),
    m_encodingDeclParser( m_stacks ),
    m_xmlDeclarationParser( m_encodingDeclParser ),
    m_attListDeclParser( m_enumeratedTypeParser, m_attributeValueParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

XmlParserImpl::~XmlParserImpl()
{
//    DEBUG_CODE( cout << __FUNCTION__ << '\t' << __LINE__ << endl; )
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

void XmlParserImpl::Setup( void )
{
    assert( this != NULL );
    m_parsing = true;
    m_errorCount = 0;
    m_stacks.m_messages.SetPreparer( this );
    m_stacks.m_content.SetPreparer( this );
}

// ----------------------------------------------------------------------------

bool XmlParserImpl::PrepareErrorMessage( Parser::ErrorLevel::Levels level,
    const CharType * message )
{
    assert( this != NULL );

    if ( NULL == m_errorReceiver )
        return false;
    bool okay = false;
    try
    {
        okay = m_errorReceiver->GiveParseMessage( level, message );
        if ( !okay )
            m_errorReceiver = NULL;
    }
    catch ( ... )
    {
        m_errorReceiver = NULL;
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    if ( Parser::ErrorLevel::Minor <= level )
    {
        ++m_errorCount;
        if ( m_maxErrorCount <= m_errorCount )
            throw Parser::XMaxErrorException();
    }

    return okay;
}

// ----------------------------------------------------------------------------

bool XmlParserImpl::PrepareErrorMessage( Parser::ErrorLevel::Levels level,
    const CharType * begin, const CharType * end )
{
    assert( this != NULL );

    if ( NULL == m_errorReceiver )
        return false;
    string message( begin, end-begin );
    return PrepareErrorMessage( level, message.c_str() );
}

// ----------------------------------------------------------------------------

bool XmlParserImpl::PrepareContentMessage( const CharType * section )
{
    assert( this != NULL );

    if ( NULL == m_errorReceiver )
        return false;
    bool okay = false;
    try
    {
        okay = m_errorReceiver->GiveParseMessage( Parser::ErrorLevel::Content, section );
        if ( !okay )
            m_errorReceiver = NULL;
    }
    catch ( ... )
    {
        m_errorReceiver = NULL;
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    return okay;
}

// ----------------------------------------------------------------------------

bool XmlParserImpl::PrepareContentMessage( const CharType * section,
    const CharType * name )
{
    assert( this != NULL );

    if ( NULL == m_errorReceiver )
        return false;
    const unsigned long length = static_cast< unsigned long >
        ( ::strlen( section ) + ::strlen( name ) + 100 );
    std::string buffer;
    buffer.reserve( length );
//    char * pBuffer = const_cast< char * >( buffer.c_str() );
//    ::_snprintf( pBuffer, length, "Inside %s section for %s", section, name );
    ::Loki::SPrintf( buffer, "Inside %s section for %s" )( section )( name );
    bool okay = false;
    try
    {
        okay = m_errorReceiver->GiveParseMessage( Parser::ErrorLevel::Content, buffer.c_str() );
        if ( !okay )
            m_errorReceiver = NULL;
    }
    catch ( ... )
    {
        m_errorReceiver = NULL;
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }
    return okay;
}

// ----------------------------------------------------------------------------

bool XmlParserImpl::PrepareContentMessage( const CharType * section,
    const CharType * name, unsigned long line, unsigned long chars )
{
    assert( this != NULL );

    if ( NULL == m_errorReceiver )
        return false;
    const unsigned long length = static_cast< unsigned long >
        ( ::strlen( section ) + ::strlen( name ) + 100 );
    std::string buffer;
    buffer.reserve( length );
//    char * pBuffer = const_cast< char * >( buffer.c_str() );
//    ::_snprintf( pBuffer, length, "Inside %s section for %s on line %u at char %u.",
//        section, name, line, chars );
    ::Loki::SPrintf( buffer, "Inside %s section for %s on line %u at char %u." )
        ( section )( name )( line )( chars );
    bool okay = false;
    try
    {
        okay = m_errorReceiver->GiveParseMessage( Parser::ErrorLevel::Content, buffer.c_str() );
        if ( !okay )
            m_errorReceiver = NULL;
    }
    catch ( ... )
    {
        m_errorReceiver = NULL;
        // throw exception back up to indicate Parser::ErrorLevel::Except
    }

    return okay;
}

// ----------------------------------------------------------------------------

XmlParser::XmlParser() : m_impl( NULL )
{
    assert( this != NULL );
    CommonParserRules::IncReference();
    m_impl = new XmlParserImpl;
    assert( m_impl != NULL );
}

// ----------------------------------------------------------------------------

XmlParser::~XmlParser()
{
//    DEBUG_CODE( cout << __FUNCTION__ << '\t' << __LINE__ << endl; )
    assert( this != NULL );
    assert( m_impl != NULL );
    delete m_impl;
    CommonParserRules::DecReference();
}

// ----------------------------------------------------------------------------

bool XmlParser::SetErrorReceiver( IParseErrorReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );
    if ( NULL == receiver )
        return false;
    m_impl->SetErrorReceiver( receiver );
    return true;
}

// ----------------------------------------------------------------------------

Parser::IParseErrorReceiver * XmlParser::GetErrorReceiver( void )
{
    assert( this != NULL );
    assert( m_impl != NULL );
    return m_impl->GetErrorReceiver();
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseFile(
    const char * filename, IDocumentReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    if ( NULL == filename )
        return XmlParser::NoFileName;
    if ( !m_impl->IsReady() )
        return XmlParser::NotReady;
    string contents;
    if ( !ReadFileIntoString( filename, contents ) )
        return XmlParser::CantOpenFile;
    if ( contents.empty() )
        return XmlParser::EndOfFile;
    const CharType * begin = contents.c_str();
    const CharType * end = begin + contents.size() + 1;
    const XmlParser::ParseResults result =
        m_impl->ParseDocument( begin, end, receiver );

    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseDocument(
    const CharType * begin, IDocumentReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseDocument( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseDocument( const CharType * begin,
    const CharType * end, IDocumentReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseDocument( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseNode(
    const CharType * begin, INodeReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseNode( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseNode( const CharType * begin,
    const CharType * end, INodeReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseNode( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseAttributeValue( const char * begin,
    IAttributeValueReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseAttributeValue( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseAttributeValue( const char * begin,
    const char * end, IAttributeValueReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseAttributeValue( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseAttribute(
    const char * begin, IAttributeReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseAttribute( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseAttribute(
    const char * begin, const char * end, IAttributeReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseAttribute( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseEnumeratedType( const char * begin,
    IEnumeratedTypeReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseEnumeratedType( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseEnumeratedType( const char * begin,
    const char * end, IEnumeratedTypeReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseEnumeratedType( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseEntityValue( const char * begin,
    IEntityValueReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseEntityValue( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseEntityValue( const char * begin,
    const char * end, IEntityValueReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseEntityValue( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseEncoding( const char * begin,
    IEncodingDeclReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseEncoding( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseEncoding( const char * begin,
    const char * end, IEncodingDeclReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseEncoding( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseXmlDeclaration( const char * begin,
    IXmlDeclarationReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseXmlDeclaration( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseXmlDeclaration( const char * begin,
    const char * end, IXmlDeclarationReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseXmlDeclaration( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseAttListDecl( const char * begin,
    IAttListDeclReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseAttListDecl( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseAttListDecl( const char * begin,
    const char * end, IAttListDeclReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseAttListDecl( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseComment( const char * begin,
    ICommentReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseComment( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseComment( const char * begin,
    const char * end, ICommentReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseComment( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParsePublicIdLiteral( const char * begin,
    IPublicIdReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParsePublicIdLiteral( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParsePublicIdLiteral( const char * begin,
    const char * end, IPublicIdReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParsePublicIdLiteral( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseExternalId( const char * begin,
    IExternalIdReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseExternalId( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseExternalId( const char * begin,
    const char * end, IExternalIdReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseExternalId( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseName(
    const CharType * begin, INameReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseName( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseName(
    const CharType * begin, const CharType * end, INameReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseName( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParsePeReference(
    const CharType * begin, IPeReferenceReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParsePeReference( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParsePeReference( const CharType * begin,
    const CharType * end, IPeReferenceReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParsePeReference( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseReference(
    const char * begin, IReferenceReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin );
    if ( result == XmlParser::AllValid )
    {
        const CharType * end = ::strlen( begin ) + begin;
        result = m_impl->ParseReference( begin, end, receiver );
    }
    return result;
}

// ----------------------------------------------------------------------------

XmlParser::ParseResults XmlParser::ParseReference(
    const char * begin, const char * end, IReferenceReceiver * receiver )
{
    assert( this != NULL );
    assert( m_impl != NULL );

    XmlParser::ParseResults result = m_impl->DoPreliminaryChecks( begin, end );
    if ( result == XmlParser::AllValid )
        result = m_impl->ParseReference( begin, end, receiver );
    return result;
}

// ----------------------------------------------------------------------------

}; // end namespace Xml

}; // end namespace Parser

// $Log: XmlParser.cpp,v $
// Revision 1.1  2008/12/05 19:04:16  rich_sposato
// Adding xml parsing files to project.
//
