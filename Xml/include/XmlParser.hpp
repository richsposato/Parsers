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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/include/XmlParser.hpp,v 1.2 2009/01/05 19:26:36 rich_sposato Exp $


#ifndef PARSER_XML_PARSER_H_INCLUDED
#define PARSER_XML_PARSER_H_INCLUDED

#include "./Receivers.hpp"

// ----------------------------------------------------------------------------

namespace Parser
{
    class IParseErrorReceiver;

namespace Xml
{

// ----------------------------------------------------------------------------

class XmlParserImpl;

class XmlParser
{
public:

    enum ParseResults
    {
        AllValid = 0,  ///< All data was parsed and valid.
        SomeValid,  ///< Start is valid, but ending part was not recognized.
        NotValid,   ///< Data was parsed, but is not valid.
        NotParsed,  ///< Data was not recognized by parsing rules.
        NoReceiver, ///< No content receiver provided.
        NullStart,  ///< Starting place is NULL.
        EmptyData,  ///< No content at starting place.
        NullEnd,    ///< Ending place is NULL.
        EndTooLow,  ///< Ending place less than starting place.
        Exception,  ///< Exception thrown.
        ParsingNow, ///< Parser is currently parsing other data.
        Receiving,  ///< Receiver reported an error.
        NoErrorRecv, ///< No error receiver provided.
        NoFileName, ///< No file name provided for parsing.
        CantOpenFile, ///< Could not open file.
        EndOfFile,  ///< Reached end of file, but can't parse content.
        NotReady,   ///< Parser can't parse now, may be busy parsing already.
        CantMakeParser,
    };

    XmlParser();
    ~XmlParser();

    bool SetErrorReceiver( IParseErrorReceiver * receiver );

    IParseErrorReceiver * GetErrorReceiver( void );

    ParseResults ParseComment( const char * begin, ICommentReceiver * receiver );

    ParseResults ParseComment( const char * begin, const char * end,
        ICommentReceiver * receiver );

    ParseResults ParsePublicIdLiteral( const char * begin, IPublicIdReceiver * receiver );

    ParseResults ParsePublicIdLiteral( const char * begin, const char * end,
        IPublicIdReceiver * receiver );

    ParseResults ParseExternalId( const char * begin,
        IExternalIdReceiver * receiver );

    ParseResults ParseExternalId( const char * begin, const char * end,
        IExternalIdReceiver * receiver );

    ParseResults ParseName( const char * begin, INameReceiver * receiver );

    ParseResults ParseName( const char * begin, const char * end,
        INameReceiver * receiver );

    ParseResults ParsePeReference( const char * begin,
        IPeReferenceReceiver * receiver );

    ParseResults ParsePeReference( const char * begin, const char * end,
        IPeReferenceReceiver * receiver );

    ParseResults ParseReference( const char * begin, IReferenceReceiver * receiver );

    ParseResults ParseReference( const char * begin, const char * end,
        IReferenceReceiver * receiver );

    ParseResults ParseAttributeValue( const char * begin,
        IAttributeValueReceiver * receiver );

    ParseResults ParseAttributeValue( const char * begin, const char * end,
        IAttributeValueReceiver * receiver );

    ParseResults ParseAttribute( const char * begin,
        IAttributeReceiver * receiver );

    ParseResults ParseAttribute( const char * begin, const char * end,
        IAttributeReceiver * receiver );

    XmlParser::ParseResults ParseEnumeratedType( const char * begin,
        IEnumeratedTypeReceiver * receiver );

    XmlParser::ParseResults ParseEnumeratedType( const char * begin,
         const char * end, IEnumeratedTypeReceiver * receiver );

    ParseResults ParseEntityValue( const char * begin,
        IEntityValueReceiver * receiver );

    ParseResults ParseEntityValue( const char * begin, const char * end,
        IEntityValueReceiver * receiver );

    ParseResults ParseEncoding( const char * begin,
        IEncodingDeclReceiver * receiver );

    ParseResults ParseEncoding( const char * begin, const char * end,
        IEncodingDeclReceiver * receiver );

    ParseResults ParseXmlDeclaration( const char * begin,
        IXmlDeclarationReceiver * receiver );

    ParseResults ParseXmlDeclaration( const char * begin,
        const char * end, IXmlDeclarationReceiver * receiver );

    ParseResults ParseAttListDecl( const char * begin,
        IAttListDeclReceiver * receiver );

    ParseResults ParseAttListDecl( const char * begin, const char * end,
        IAttListDeclReceiver * receiver );

    ParseResults ParseNode( const char * begin, INodeReceiver * receiver );

    ParseResults ParseNode( const char * begin, const char * end,
        INodeReceiver * receiver );

    ParseResults ParseDocument( const char * begin, IDocumentReceiver * receiver );

    ParseResults ParseDocument( const char * begin, const char * end,
        IDocumentReceiver * receiver );

    ParseResults ParseFile( const char * filename, IDocumentReceiver * receiver );

private:
    XmlParser( const XmlParser & );
    XmlParser & operator = ( const XmlParser & );

    XmlParserImpl * m_impl;

}; // end class XmlParser

// ----------------------------------------------------------------------------

}; // end namespace Xml

}; // end namespace Parser

#endif

// $Log: XmlParser.hpp,v $
// Revision 1.2  2009/01/05 19:26:36  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.1  2008/12/05 19:04:15  rich_sposato
// Adding xml parsing files to project.
//
