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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/src/Receivers.cpp,v 1.1 2008/12/05 19:04:16 rich_sposato Exp $


#include "../include/Receivers.hpp"

#include "string.h"

// ----------------------------------------------------------------------------

namespace Parser
{

namespace Xml
{


// ----------------------------------------------------------------------------

const char * GetRefTypeName( IReferenceReceiver::RefType refType )
{
    switch ( refType )
    {
        case IReferenceReceiver::HexDigits: return "hexdigits";
        case IReferenceReceiver::Digits:    return "digits";
        case IReferenceReceiver::Entity:    return "entity";
        case IReferenceReceiver::Unknown:
        default: break;
    }
    return "Unknown";
}

// ----------------------------------------------------------------------------

const char * GetAttDeclTypeName( IAttListDeclReceiver::AttType attType )
{
    switch ( attType )
    {
        case IAttListDeclReceiver::CData:    return "CDATA";
        case IAttListDeclReceiver::Id:       return "ID";
        case IAttListDeclReceiver::IdRef:    return "IDREF";
        case IAttListDeclReceiver::IdRefs:   return "IDREFS";
        case IAttListDeclReceiver::Entity:   return "ENTITY";
        case IAttListDeclReceiver::Entities: return "ENTITIES";
        case IAttListDeclReceiver::NmToken:  return "NMTOKEN";
        case IAttListDeclReceiver::NmTokens: return "NMTOKENS";
        case IAttListDeclReceiver::UnknownType:
        default: break;
    }
    return "Unknown";
}

// ----------------------------------------------------------------------------

bool IsStringAttType( IAttListDeclReceiver::AttType attType )
{
    return ( IAttListDeclReceiver::CData == attType );
}

// ----------------------------------------------------------------------------

bool IsTokenizedAttType( IAttListDeclReceiver::AttType attType )
{
    return (
        ( IAttListDeclReceiver::Id       == attType ) ||
        ( IAttListDeclReceiver::IdRef    == attType ) ||
        ( IAttListDeclReceiver::IdRefs   == attType ) ||
        ( IAttListDeclReceiver::Entity   == attType ) ||
        ( IAttListDeclReceiver::Entities == attType ) ||
        ( IAttListDeclReceiver::NmToken  == attType ) ||
        ( IAttListDeclReceiver::NmTokens == attType ) );
}

// ----------------------------------------------------------------------------

IAttListDeclReceiver::AttType GetAttDeclType( const char * name )
{
    if ( ( NULL == name ) || ( '\0' == *name ) )
        return IAttListDeclReceiver::UnknownType;
    if ( ::strncmp( name, "CDATA",    5 ) == 0 )
        return IAttListDeclReceiver::CData;
    if ( ::strncmp( name, "ID",       2 ) == 0 )
        return IAttListDeclReceiver::Id;
    if ( ::strncmp( name, "IDREF",    5 ) == 0 )
        return IAttListDeclReceiver::IdRef;
    if ( ::strncmp( name, "IDREFS",   6 ) == 0 )
        return IAttListDeclReceiver::IdRefs;
    if ( ::strncmp( name, "ENTITY",   6 ) == 0 )
        return IAttListDeclReceiver::Entity;
    if ( ::strncmp( name, "ENTITIES", 8 ) == 0 )
        return IAttListDeclReceiver::Entities;
    if ( ::strncmp( name, "NMTOKEN",  7 ) == 0 )
        return IAttListDeclReceiver::NmToken;
    if ( ::strncmp( name, "NMTOKENS", 8 ) == 0 )
        return IAttListDeclReceiver::NmTokens;
    return IAttListDeclReceiver::UnknownType;
}

// ----------------------------------------------------------------------------

const char * GetDefaultDeclTypeName( IAttListDeclReceiver::DefaultDeclType defType )
{
    switch ( defType )
    {
        case IAttListDeclReceiver::Required:  return "Required";
        case IAttListDeclReceiver::Implied:   return "Implied";
        case IAttListDeclReceiver::Fixed:     return "Fixed";
        case IAttListDeclReceiver::JustValue: return "JustValue";
        case IAttListDeclReceiver::UnknownDecl:
        default: break;
    }
    return "Unknown";
}

// ----------------------------------------------------------------------------

IAttListDeclReceiver::DefaultDeclType GetDefaultDeclType( const char * name )
{
    if ( ( NULL == name ) || ( '\0' == *name ) )
        return IAttListDeclReceiver::UnknownDecl;
    if ( ::strncmp( name, "#REQUIRED", 9 ) == 0 )
        return IAttListDeclReceiver::Required;
    if ( ::strncmp( name, "#IMPLIED",  8 ) == 0 )
        return IAttListDeclReceiver::Implied;
    if ( ::strncmp( name, "#FIXED",    6 ) == 0 )
        return IAttListDeclReceiver::Fixed;
    if ( ::strncmp( name, "Required",  8 ) == 0 )
        return IAttListDeclReceiver::Required;
    if ( ::strncmp( name, "Implied",   7 ) == 0 )
        return IAttListDeclReceiver::Implied;
    if ( ::strncmp( name, "Fixed",     5 ) == 0 )
        return IAttListDeclReceiver::Fixed;
    if ( ::strncmp( name, "JustValue", 9 ) == 0 )
        return IAttListDeclReceiver::Fixed;
    return IAttListDeclReceiver::UnknownDecl;
}

// ----------------------------------------------------------------------------

}; // end namespace Xml

}; // end namespace Parser

// $Log: Receivers.cpp,v $
// Revision 1.1  2008/12/05 19:04:16  rich_sposato
// Adding xml parsing files to project.
//
