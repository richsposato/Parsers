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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/include/Receivers.hpp,v 1.1 2008/12/05 19:04:15 rich_sposato Exp $


#ifndef PARSER_XML_PARSER_RECEIVERS_H_INCLUDED
#define PARSER_XML_PARSER_RECEIVERS_H_INCLUDED


// ----------------------------------------------------------------------------

namespace Parser
{

namespace Xml
{

// ----------------------------------------------------------------------------

class ICommentReceiver
{
public:

    virtual bool AddComment( const char * begin, const char * end ) = 0;

protected:
    inline ICommentReceiver() {}
    inline virtual ~ICommentReceiver() {}

private:
    ICommentReceiver( const ICommentReceiver & );
    ICommentReceiver & operator = ( const ICommentReceiver & );

}; // end class ICommentReceiver

// ----------------------------------------------------------------------------

class IPublicIdReceiver
{
public:

    virtual bool SetPublicIdLiteral( const char * begin, const char * end,
        bool singleQuoted ) = 0;

protected:
    inline IPublicIdReceiver() {}
    inline virtual ~IPublicIdReceiver() {}

private:
    IPublicIdReceiver( const IPublicIdReceiver & );
    IPublicIdReceiver & operator = ( const IPublicIdReceiver & );

}; // end class IPublicIdReceiver

// ----------------------------------------------------------------------------

class IExternalIdReceiver : public IPublicIdReceiver
{
public:

    virtual bool SetSystemLiteral( const char * begin, const char * end,
        bool singleQuoted ) = 0;

    virtual void DoneExternalIdLiteral( bool valid,  const char * begin,
        const char * end ) = 0;

protected:
    inline IExternalIdReceiver() {}
    inline virtual ~IExternalIdReceiver() {}

private:
    IExternalIdReceiver( const IExternalIdReceiver & );
    IExternalIdReceiver & operator = ( const IExternalIdReceiver & );

}; // end class IExternalIdReceiver

// ----------------------------------------------------------------------------

class INameReceiver
{
public:

    virtual bool SetName( const char * begin, const char * end ) = 0;

protected:
    inline INameReceiver() {}
    inline virtual ~INameReceiver() {}

private:
    INameReceiver( const INameReceiver & );
    INameReceiver & operator = ( const INameReceiver & );

}; // end class INameReceiver

// ----------------------------------------------------------------------------

class IPeReferenceReceiver
{
public:

    virtual bool AddPeReference( const char * begin, const char * end ) = 0;

protected:
    inline IPeReferenceReceiver() {}
    inline virtual ~IPeReferenceReceiver() {}

private:
    IPeReferenceReceiver( const IPeReferenceReceiver & );
    IPeReferenceReceiver & operator = ( const IPeReferenceReceiver & );

}; // end class IPeReferenceReceiver

// ----------------------------------------------------------------------------

class IParameterReferenceReceiver
{
public:

    virtual bool AddParameterReference( const char * begin, const char * end ) = 0;

protected:
    inline IParameterReferenceReceiver() {}
    inline virtual ~IParameterReferenceReceiver() {}

private:
    IParameterReferenceReceiver( const IParameterReferenceReceiver & );
    IParameterReferenceReceiver & operator = ( const IParameterReferenceReceiver & );

}; // end class IParameterReferenceReceiver

// ----------------------------------------------------------------------------

class IReferenceReceiver
{
public:

    enum RefType
    {
        Unknown = 0,
        Entity,
        Digits,
        HexDigits
    };

    virtual bool AddReference( const char * begin, const char * end,
        RefType refType ) = 0;

protected:
    inline IReferenceReceiver() {}
    inline virtual ~IReferenceReceiver() {}

private:
    IReferenceReceiver( const IReferenceReceiver & );
    IReferenceReceiver & operator = ( const IReferenceReceiver & );

}; // end class IReferenceReceiver

const char * GetRefTypeName( IReferenceReceiver::RefType refType );

// ----------------------------------------------------------------------------

class IAttributeValueReceiver : public IReferenceReceiver
{
public:

    virtual bool AddValue( const char * begin, const char * end ) = 0;

    virtual void DoneAttributeValue( bool valid, bool singleQuoted,
        const char * begin, const char * end ) = 0;

protected:
    inline IAttributeValueReceiver() : IReferenceReceiver() {}
    inline virtual ~IAttributeValueReceiver() {}

private:
    IAttributeValueReceiver( const IAttributeValueReceiver & );
    IAttributeValueReceiver & operator = ( const IAttributeValueReceiver & );

}; // end class IAttributeValueReceiver

// ----------------------------------------------------------------------------

class IAttributeReceiver : public INameReceiver,
    public IAttributeValueReceiver
{

protected:
    inline IAttributeReceiver() : INameReceiver(), IAttributeValueReceiver() {}
    inline virtual ~IAttributeReceiver() {}

private:
    IAttributeReceiver( const IAttributeReceiver & );
    IAttributeReceiver & operator = ( const IAttributeReceiver & );

}; // end class IAttributeReceiver

// ----------------------------------------------------------------------------

class IEnumeratedTypeReceiver
{
public:

    virtual bool AddNotation( const char * begin, const char * end ) = 0;

    virtual bool AddEnumeration( const char * begin, const char * end ) = 0;

    virtual void DoneEnumeratedType( bool valid,
        const char * begin, const char * end ) = 0;

protected:
    inline IEnumeratedTypeReceiver() {}
    inline virtual ~IEnumeratedTypeReceiver() {}

private:
    IEnumeratedTypeReceiver( const IEnumeratedTypeReceiver & );
    IEnumeratedTypeReceiver & operator = ( const IEnumeratedTypeReceiver & );

}; // end class IEnumeratedTypeReceiver

// ----------------------------------------------------------------------------

class IEntityValueReceiver : public IReferenceReceiver,
    public IPeReferenceReceiver
{
public:

    virtual bool AddValue( const char * begin, const char * end ) = 0;

    virtual void DoneEntityValue( bool valid, bool singleQuoted,
        const char * begin, const char * end ) = 0;

protected:
    inline IEntityValueReceiver() : IReferenceReceiver(),
        IPeReferenceReceiver() {}
    inline virtual ~IEntityValueReceiver() {}

private:
    IEntityValueReceiver( const IEntityValueReceiver & );
    IEntityValueReceiver & operator = ( const IEntityValueReceiver & );

}; // end class IEntityValueReceiver

// ----------------------------------------------------------------------------

class IEncodingDeclReceiver
{
public:

    virtual bool SetEncName( const char * begin, const char * end ) = 0;

    virtual void DoneEncodingDecl( bool valid, bool singleQuoted,
        const char * begin, const char * end ) = 0;

protected:
    inline IEncodingDeclReceiver() {}
    inline virtual ~IEncodingDeclReceiver() {}

private:
    IEncodingDeclReceiver( const IEncodingDeclReceiver & );
    IEncodingDeclReceiver & operator = ( const IEncodingDeclReceiver & );

}; // end class IEncodingDeclReceiver

// ----------------------------------------------------------------------------

class IXmlDeclarationReceiver : public IEncodingDeclReceiver
{
public:

    virtual bool SetVersionNumber( bool singleQuoted, const char * begin,
        const char * end ) = 0;

    virtual bool SetIsStandalone( bool standalone, bool singleQuoted ) = 0;

    virtual void DoneXmlDeclaration( bool valid, const char * begin,
        const char * end ) = 0;

protected:
    inline IXmlDeclarationReceiver( void ) : IEncodingDeclReceiver() {}
    inline virtual ~IXmlDeclarationReceiver() {}

private:
    IXmlDeclarationReceiver( const IXmlDeclarationReceiver & );
    IXmlDeclarationReceiver & operator = ( const IXmlDeclarationReceiver & );

}; // end class IXmlDeclarationReceiver

// ----------------------------------------------------------------------------

class IAttListDeclReceiver
{
public:

    enum AttType
    {
        UnknownType = 0,
        CData       = 1,
        Id          = 2,
        IdRef       = 3,
        IdRefs      = 4,
        Entity      = 5,
        Entities    = 6,
        NmToken     = 7,
        NmTokens    = 8,
        Enumerated  = 9
    };

    enum DefaultDeclType
    {
        UnknownDecl = 0,
        Required    = 1,
        Implied     = 2,
        Fixed       = 3,
        JustValue   = 4
    };

    virtual bool SetName( const char * begin, const char * end ) = 0;

    virtual bool SetAttName( const char * begin, const char * end ) = 0;

    virtual bool AddNotateName( const char * begin, const char * end ) = 0;

    virtual bool SetAttType( AttType type ) = 0;

    virtual bool SetDefaultDeclType( DefaultDeclType type ) = 0;

    virtual IAttributeValueReceiver * AddAttributeValue( void ) = 0;

    virtual IEnumeratedTypeReceiver * AddEnumeratedType( void ) = 0;

    virtual void DoneAttListDecl( bool valid,
        const char * begin, const char * end ) = 0;

protected:
    inline IAttListDeclReceiver() {}
    inline virtual ~IAttListDeclReceiver() {}

private:
    IAttListDeclReceiver( const IAttListDeclReceiver & );
    IAttListDeclReceiver & operator = ( const IAttListDeclReceiver & );

}; // end class IAttListDeclReceiver

const char * GetAttDeclTypeName( IAttListDeclReceiver::AttType attType );

IAttListDeclReceiver::AttType GetAttDeclType( const char * name );

bool IsStringAttType( IAttListDeclReceiver::AttType attType );

bool IsTokenizedAttType( IAttListDeclReceiver::AttType attType );

const char * GetDefaultDeclTypeName( IAttListDeclReceiver::DefaultDeclType defType );

IAttListDeclReceiver::DefaultDeclType GetDefaultDeclType( const char * name );

// ----------------------------------------------------------------------------

class INodeReceiver
{
public:

    virtual bool SetTagName( const char * begin, const char * end ) = 0;

    virtual bool SetElementName( const char * begin, const char * end ) = 0;

    virtual bool AddComment( const char * begin, const char * end ) = 0;

    virtual bool AddCData( const char * begin, const char * end ) = 0;

    virtual bool SetAttributeName( const char * begin, const char * end ) = 0;

    virtual bool SetAttributeValue( const char * begin, const char * end ) = 0;

    virtual INodeReceiver * AddChild() = 0;

    virtual bool DoneNode( bool valid, const char * begin, const char * end ) = 0;

protected:
    inline INodeReceiver() {}
    inline virtual ~INodeReceiver() {}

private:
    INodeReceiver( const INodeReceiver & );
    INodeReceiver & operator = ( const INodeReceiver & );

}; // end class INodeReceiver

// ----------------------------------------------------------------------------

class IDocumentReceiver
{
public:

    virtual INodeReceiver * AddRoot() = 0;

    virtual bool AddComment( const char * begin, const char * end ) = 0;

    virtual bool SetStandalone( bool standalone ) = 0;

    virtual bool DoneDocument( bool valid, const char * begin, const char * end ) = 0;

protected:
    inline IDocumentReceiver() {}
    inline virtual ~IDocumentReceiver() {}

private:
    IDocumentReceiver( const IDocumentReceiver & );
    IDocumentReceiver & operator = ( const IDocumentReceiver & );

}; // end class IDocumentReceiver

// ----------------------------------------------------------------------------

}; // end namespace Xml

}; // end namespace Parser

#endif // file guardian

// $Log: Receivers.hpp,v $
// Revision 1.1  2008/12/05 19:04:15  rich_sposato
// Adding xml parsing files to project.
//
