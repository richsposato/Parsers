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

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/Tester/BasicTesters.cpp,v 1.1 2008/12/05 19:04:15 rich_sposato Exp $


#include "BasicTesters.hpp"

#include <string>
#include <iostream>

#include "../../Util/include/ParseInfo.hpp"

#include "../include/XmlParser.hpp"

#include "CommandLineArgs.hpp"


using namespace std;
using namespace Parser;


// ----------------------------------------------------------------------------

ParseInfo::ParseResult Convert( Xml::XmlParser::ParseResults result )
{
    switch ( result )
    {
        case Xml::XmlParser::AllValid:     return ParseInfo::AllValid;
        case Xml::XmlParser::SomeValid:    return ParseInfo::SomeValid;
        case Xml::XmlParser::NotValid:     return ParseInfo::NotValid;
        case Xml::XmlParser::NotParsed:    return ParseInfo::NotParsed;
        case Xml::XmlParser::NoFileName:   return ParseInfo::NoFileName;
        case Xml::XmlParser::CantOpenFile: return ParseInfo::CantOpenFile;
        case Xml::XmlParser::EndOfFile:    return ParseInfo::EndOfFile;
        case Xml::XmlParser::NullStart:    return ParseInfo::CantStart;
        case Xml::XmlParser::EmptyData:    return ParseInfo::CantStart;
        case Xml::XmlParser::EndTooLow:    return ParseInfo::CantStart;
        default: break;
    }
    return ParseInfo::NotValid;
}

// ----------------------------------------------------------------------------

const TestData s_commentTestCases[] =
{
    { ParseInfo::AllValid,  "<!-- a -->" },
    { ParseInfo::AllValid,  "<!-- -->" },
    { ParseInfo::AllValid,  "<!---->" },
    { ParseInfo::AllValid,  "<!-- - -->" },
    { ParseInfo::AllValid,  "<!-- abcd -->" },
    { ParseInfo::AllValid,  "<!--abcd-->" },
    { ParseInfo::AllValid,  "<!-- ab - cd -->" },
    { ParseInfo::AllValid,  "<!-- ab -> cd -->" },
    { ParseInfo::AllValid,  "<!-- abcd -->" },
    { ParseInfo::AllValid,  "<!--\tabcd\t-->" },
    { ParseInfo::AllValid,  "<!--\nabcd\n-->" },
    { ParseInfo::AllValid,  "<!--\rabcd\r-->" },
    { ParseInfo::SomeValid, "<!-- a --> def" },
    { ParseInfo::SomeValid, "<!-- a --> " },
    { ParseInfo::NotParsed, "<!-- abcd --->" },
    { ParseInfo::NotParsed, "<!-- abcd ->" },
    { ParseInfo::NotParsed, "<!-- ab -- d -->" },
    { ParseInfo::NotParsed, "<!-- abc -- >" },
    { ParseInfo::NotParsed, "<!------>" },
    { ParseInfo::NotParsed, "<!-- -- -->" },
    { ParseInfo::NotParsed, "<!- abcd -->" },
    { ParseInfo::NotParsed, "<-- -->" },
    { ParseInfo::NotParsed, "<<-- -->" },
    { ParseInfo::CantStart, "" },             // invalid if empty.
    { ParseInfo::CantStart, NULL }            // invalid if NULL.
};

const unsigned long s_commentTestCount =
    sizeof(s_commentTestCases) / sizeof(s_commentTestCases[0]);

// ----------------------------------------------------------------------------

CommentTester::CommentTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo ) :
    ICommentReceiver(),
    TestBase( "Comments", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

CommentTester::~CommentTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool CommentTester::AddComment( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );

    if ( ShowContent() )
    {
        string comment;
        if ( begin < end )
            comment.assign( begin, end-begin );
        else
            comment = "(empty)";
        cout << "Comment: [" << comment << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool CommentTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_commentTestCases, s_commentTestCount );
}

// ----------------------------------------------------------------------------

bool CommentTester::OnParse( const char * begin, const char * end, unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    m_pParser->SetErrorReceiver( errorCounter );
    ICommentReceiver * commentReceiver =
        dynamic_cast< ICommentReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParseComment( begin, end, commentReceiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

const TestData s_nameTestCases[] =
{
    { ParseInfo::AllValid,  "a" },
    { ParseInfo::AllValid,  ":" },
    { ParseInfo::AllValid,  "_" },
    { ParseInfo::AllValid,  "A" },
    { ParseInfo::AllValid,  "ab" },
    { ParseInfo::AllValid,  "a.b" },
    { ParseInfo::AllValid,  "a:b" },
    { ParseInfo::AllValid,  "a-b" },
    { ParseInfo::AllValid,  "a_b" },
    { ParseInfo::AllValid,  "a\xB7" },
    { ParseInfo::AllValid,  "a.1" },
    { ParseInfo::AllValid,  "a:1" },
    { ParseInfo::AllValid,  ":1" },
    { ParseInfo::AllValid,  ":a1" },
    { ParseInfo::AllValid,  "a-1" },
    { ParseInfo::AllValid,  "a_1" },
    { ParseInfo::AllValid,  "a\xB7" },
    { ParseInfo::AllValid,  "abc." },
    { ParseInfo::AllValid,  "a." },
    { ParseInfo::AllValid,  "abc" },
    { ParseInfo::SomeValid, "abc. def" },
    { ParseInfo::SomeValid, "ab^&l( " },
    { ParseInfo::NotParsed, "\"abc\". def" },
    { ParseInfo::NotParsed, "\"abc\".\"a" },
    { ParseInfo::NotParsed, "\"abc\"." },
    { ParseInfo::NotParsed, " starts with space" },
    { ParseInfo::NotParsed, "^&l( " },
    { ParseInfo::NotParsed, "\"" },
    { ParseInfo::NotParsed, "\" " },
    { ParseInfo::NotParsed, "1" },
    { ParseInfo::CantStart, "" },             // invalid if empty.
    { ParseInfo::CantStart, NULL }            // invalid if NULL.
};

const unsigned long s_nameTestCount =
    sizeof(s_nameTestCases) / sizeof(s_nameTestCases[0]);

// ----------------------------------------------------------------------------

NameTester::NameTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo ) :
    INameReceiver(),
    TestBase( "Name", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

NameTester::~NameTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool NameTester::SetName( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string name( begin, end-begin );
        cout << "Name: [" << name << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool NameTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_nameTestCases, s_nameTestCount );
}

// ----------------------------------------------------------------------------

bool NameTester::OnParse( const char * begin, const char * end, unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    m_pParser->SetErrorReceiver( errorCounter );
    INameReceiver * nameReceiver = dynamic_cast< INameReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParseName( begin, end, nameReceiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

const TestData s_referenceTestCases[] =
{
    { ParseInfo::AllValid,  "&abc;" },  // value is an entity-reference.
    { ParseInfo::AllValid,  "&#000;" }, // value is a digit-reference.
    { ParseInfo::AllValid,  "&#x00;" }, // value is a hexdigit-reference.
    { ParseInfo::AllValid,  "&#xab;" }, // value is a hexdigit-reference.
    { ParseInfo::AllValid,  "&#xAB;" }, // value is a hexdigit-reference.
    { ParseInfo::AllValid,  "&#xF0;" }, // value is a hexdigit-reference.
    { ParseInfo::NotParsed, "&000;" },  // invalid entity-reference value.
    { ParseInfo::NotParsed, "&#abc;" }, // value is an entity-reference, but prefix is digit-ref.
    { ParseInfo::NotParsed, "&#xFz;" }, // invalid hexdigit-reference value.
    { ParseInfo::NotParsed, "&#abc;" }, // prefix for digit-ref, but value is hexdigit-reference.
    { ParseInfo::NotParsed, "&;" },     // value is missing an entity-reference.
    { ParseInfo::NotParsed, "& abc;" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, " &abc;" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "&abc ;" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "&ab c;" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "&abc" },   // value with entity-reference is missing a semicolon.
    { ParseInfo::NotParsed, "&# 000;" }, // value of digit-reference has extra space.
    { ParseInfo::NotParsed, "&# x00;" }, // value of hexdigit-reference has extra space.
    { ParseInfo::NotParsed, "&#000 ;" }, // value of digit-reference has extra space.
    { ParseInfo::NotParsed, "&#x00 ;" }, // value of hexdigit-reference has extra space.
    { ParseInfo::NotParsed, "& #000;" }, // value of digit-reference has extra space.
    { ParseInfo::NotParsed, "& #x00;" }, // value of hexdigit-reference has extra space.
    { ParseInfo::NotParsed, " &#000;" }, // value of digit-reference has extra space.
    { ParseInfo::NotParsed, " &#x00;" }, // value of hexdigit-reference has extra space.
    { ParseInfo::CantStart, "" },       // invalid if empty.
    { ParseInfo::CantStart, NULL }      // invalid if NULL.
};

const unsigned long s_referenceTestCount =
    sizeof(s_referenceTestCases) / sizeof(s_referenceTestCases[0]);

// ----------------------------------------------------------------------------

ReferenceTester::ReferenceTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo ) :
    IReferenceReceiver(),
    TestBase( "Reference", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

ReferenceTester::~ReferenceTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool ReferenceTester::AddReference( const char * begin, const char * end, RefType refType )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string value( begin, end-begin );
        cout << "Reference: [" << value << ']' << endl;
        const char * typeName = Parser::Xml::GetRefTypeName( refType );
        cout << "Type of Reference: [" << typeName << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool ReferenceTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_referenceTestCases, s_referenceTestCount );
}

// ----------------------------------------------------------------------------

bool ReferenceTester::OnParse( const char * begin, const char * end, unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    m_pParser->SetErrorReceiver( errorCounter );
    IReferenceReceiver * receiver = dynamic_cast< IReferenceReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParseReference( begin, end, receiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

const TestData s_attributeValueTestCases[] =
{
    { ParseInfo::AllValid,  "\'b\'" },      // test single-quoted attribute.
    { ParseInfo::AllValid,  "\"b\"" },      // test double-quoted attribute.
    { ParseInfo::AllValid,  "\"b \'c\"" },  // test double-quoted attribute with single quote inside value.
    { ParseInfo::AllValid,  "\'b \"c\'" },  // test single-quoted attribute with double quote inside value.
    { ParseInfo::AllValid,  "\'b\'" },
    { ParseInfo::AllValid,  "\"b\"" },
    { ParseInfo::AllValid,  "\'b\'" },      // test of more than one concurrent whitespace.
    { ParseInfo::AllValid,  "\"b\"" },      // test of more than one concurrent whitespace.
    { ParseInfo::AllValid,  "\"b \'c\"" },
    { ParseInfo::AllValid,  "\'b\'" },
    { ParseInfo::AllValid,  "\"b\"" },
    { ParseInfo::AllValid,  "\"b \'c\"" },
    { ParseInfo::AllValid,  "\'\'" },       // single-quoted empty value is valid.
    { ParseInfo::AllValid,  "\"\"" },       // double-quoted empty value is valid.
    { ParseInfo::AllValid,  "\'&abc;\'" },  // value is an entity-reference.
    { ParseInfo::AllValid,  "\'&abc; \'" }, // value of entity-reference has extra space.
    { ParseInfo::AllValid,  "\' &abc;\'" }, // value of entity-reference has extra space.
    { ParseInfo::AllValid,  "\'b&#000;\'" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "\'b&#xbc;\'" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "\'&#000;&#xbc;\'" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "\'&#000;\'" },  // test single-quoted attribute with value.
    { ParseInfo::AllValid,  "\"b&#000;\"" }, // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "\"b&#xbc;\"" }, // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "\"&#000;&#xbc;\"" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "\"&#000;\"" }, // test single-quoted attribute with value.
    { ParseInfo::NotParsed, "\'&;\'" },     // value is missing an entity-reference.
    { ParseInfo::NotParsed, "\'& abc;\'" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "\'&abc ;\'" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "\'&abc\'" },   // value with entity-reference is missing a semicolon.
    { ParseInfo::NotParsed, "\'b" },        // invalid if no ending single-quote.
    { ParseInfo::NotParsed, "\"b" },        // invalid if no ending double-quote.
    { ParseInfo::NotParsed, "b\'" },        // invalid if no starting single-quote.
    { ParseInfo::NotParsed, "b\"" },        // invalid if no starting double-quote.
    { ParseInfo::NotParsed, "\"b\'" },      // invalid if starting quote does not match ending quote.
    { ParseInfo::NotParsed, "\'b\"" },      // invalid if starting quote does not match ending quote.
    { ParseInfo::NotParsed, "A" },          // invalid if name only.
    { ParseInfo::NotParsed, "\'" },         // invalid if nothing after single-quote.
    { ParseInfo::NotParsed, "\"" },         // invalid if nothing after double-quote.
    { ParseInfo::CantStart, "" },           // invalid if empty.
    { ParseInfo::CantStart, NULL }          // invalid if NULL.
};

const unsigned long s_attributeValueTestCount =
    sizeof(s_attributeValueTestCases) / sizeof(s_attributeValueTestCases[0]);

// ----------------------------------------------------------------------------

AttributeValueTester::AttributeValueTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo ) :
    IAttributeValueReceiver(),
    TestBase( "Attribute Value", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

AttributeValueTester::~AttributeValueTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool AttributeValueTester::AddValue( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string value( begin, end-begin );
        cout << "Value: [" << value << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool AttributeValueTester::AddReference( const char * begin, const char * end,
    RefType refType )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string value( begin, end-begin );
        cout << "Reference: [" << value << ']' << endl;
        const char * typeName = Parser::Xml::GetRefTypeName( refType );
        cout << "Type of Reference: [" << typeName << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

void AttributeValueTester::DoneAttributeValue( bool valid, bool singleQuoted,
    const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    (void)begin;
    (void)end;
    if ( ShowContent() )
    {
        const char * result = ( valid ) ? "yes" : "no";
        const char * quoteType = ( singleQuoted ) ? "single" : "double";
        cout << "Done! Valid: [" << result << "] \t Quotes: ["
             << quoteType << ']' << endl;
    }
}

// ----------------------------------------------------------------------------

bool AttributeValueTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_attributeValueTestCases, s_attributeValueTestCount );
}

// ----------------------------------------------------------------------------

bool AttributeValueTester::OnParse( const char * begin, const char * end, unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    m_pParser->SetErrorReceiver( errorCounter );
    IAttributeValueReceiver * receiver =
        dynamic_cast< IAttributeValueReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParseAttributeValue( begin, end, receiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

const TestData s_attributeTestCases[] =
{
    { ParseInfo::AllValid,  "a=\'b\'" },      // test single-quoted attribute.
    { ParseInfo::AllValid,  "a=\"b\"" },      // test double-quoted attribute.
    { ParseInfo::AllValid,  "a=\"b \'c\"" },  // test double-quoted attribute with single quote inside value.
    { ParseInfo::AllValid,  "a=\'b \"c\'" },  // test single-quoted attribute with double quote inside value.
    { ParseInfo::AllValid,  "a = \'b\'" },
    { ParseInfo::AllValid,  "a = \"b\"" },
    { ParseInfo::AllValid,  "a  = \'b\'" },   // test of more than one concurrent whitespace.
    { ParseInfo::AllValid,  "a =  \"b\"" },   // test of more than one concurrent whitespace.
    { ParseInfo::AllValid,  "a = \"b \'c\"" },
    { ParseInfo::AllValid,  "a= \'b\'" },
    { ParseInfo::AllValid,  "a =\"b\"" },
    { ParseInfo::AllValid,  "a= \"b \'c\"" },
    { ParseInfo::AllValid,  "a=\'\'" },       // single-quoted empty value is valid.
    { ParseInfo::AllValid,  "a=\"\"" },       // double-quoted empty value is valid.
    { ParseInfo::AllValid,  "a=\'&abc;\'" },  // value is an entity-reference.
    { ParseInfo::AllValid,  "a=\'&abc; \'" }, // value of entity-reference has extra space.
    { ParseInfo::AllValid,  "a=\' &abc;\'" }, // value of entity-reference has extra space.
    { ParseInfo::AllValid,  "a=\'b&#000;\'" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "a=\'b&#xbc;\'" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "a=\'&#000;&#xbc;\'" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "a=\'&#000;\'" },  // test single-quoted attribute with value.
    { ParseInfo::AllValid,  "a=\"b&#000;\"" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "a=\"b&#xbc;\"" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "a=\"&#000;&#xbc;\"" },  // test single-quoted attribute with value and reference.
    { ParseInfo::AllValid,  "a=\"&#000;\"" },  // test single-quoted attribute with value.
    { ParseInfo::NotParsed, "a=\'&;\'" },     // value is missing an entity-reference.
    { ParseInfo::NotParsed, "a=\'& abc;\'" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "a=\'&abc ;\'" }, // value of entity-reference has extra space.
    { ParseInfo::NotParsed, "a=\'&abc\'" },   // value with entity-reference is missing a semicolon.
    { ParseInfo::NotParsed, "a=\'b" },        // invalid if no ending single-quote.
    { ParseInfo::NotParsed, "a=\"b" },        // invalid if no ending double-quote.
    { ParseInfo::NotParsed, "a=b\'" },        // invalid if no starting single-quote.
    { ParseInfo::NotParsed, "a=b\"" },        // invalid if no starting double-quote.
    { ParseInfo::NotParsed, "a\"b\"" },       // invalid if no equal sign.
    { ParseInfo::NotParsed, "a=\"b\'" },      // invalid if starting quote does not match ending quote.
    { ParseInfo::NotParsed, "a=\'b\"" },      // invalid if starting quote does not match ending quote.
    { ParseInfo::NotParsed, "A" },            // invalid if name only.
    { ParseInfo::NotParsed, "ab=" },          // invalid if nothing after equal sign.
    { ParseInfo::NotParsed, "ab= \'" },       // invalid if nothing after single-quote.
    { ParseInfo::NotParsed, "ab=\"" },        // invalid if nothing after double-quote.
    { ParseInfo::NotParsed, "=\'b\'" },       // invalid if no name.
    { ParseInfo::NotParsed, "=\"b\"" },       // invalid if no name.
    { ParseInfo::CantStart, "" },             // invalid if empty.
    { ParseInfo::CantStart, NULL }            // invalid if NULL.
};

const unsigned long s_attributeTestCount =
    sizeof(s_attributeTestCases) / sizeof(s_attributeTestCases[0]);

// ----------------------------------------------------------------------------

AttributeTester::AttributeTester( Parser::Xml::XmlParser * pParser,
    const CommandLineArgs & argInfo ) :
    IAttributeReceiver(),
    TestBase( "Attribute", argInfo.DoShowContent(),
        argInfo.GetErrorLevel(), argInfo.DoShowInfo() ),
    m_pParser( pParser )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

AttributeTester::~AttributeTester( void )
{
    assert( this != NULL );
}

// ----------------------------------------------------------------------------

bool AttributeTester::SetName( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string name( begin, end-begin );
        cout << "Name: [" << name << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool AttributeTester::AddValue( const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string value( begin, end-begin );
        cout << "Value: [" << value << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

bool AttributeTester::AddReference( const char * begin, const char * end,
    RefType refType )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );

    if ( ShowContent() )
    {
        string value( begin, end-begin );
        cout << "Reference: [" << value << ']' << endl;
        const char * typeName = Parser::Xml::GetRefTypeName( refType );
        cout << "Type of Reference: [" << typeName << ']' << endl;
    }

    return true;
}

// ----------------------------------------------------------------------------

void AttributeTester::DoneAttributeValue( bool valid, bool singleQuoted,
    const char * begin, const char * end )
{
    assert( this != NULL );
    assert( begin != NULL );
    assert( end != NULL );
    assert( begin < end );
    (void)begin;
    (void)end;
    if ( ShowContent() )
    {
        const char * result = ( valid ) ? "yes" : "no";
        const char * quoteType = ( singleQuoted ) ? "single" : "double";
        cout << "Done! Valid: [" << result << "] \t Quotes: ["
             << quoteType << ']' << endl;
    }
}

// ----------------------------------------------------------------------------

bool AttributeTester::SetupTest( void )
{
    assert( this != NULL );
    return TestBase::SetupTest( s_attributeTestCases, s_attributeTestCount );
}

// ----------------------------------------------------------------------------

bool AttributeTester::OnParse( const char * begin, const char * end, unsigned long i )
{
    assert( this != NULL );

    Parser::ErrorReceiver * errorCounter = AsErrorReceiver();
    m_pParser->SetErrorReceiver( errorCounter );
    IAttributeReceiver * receiver = dynamic_cast< IAttributeReceiver * >( this );
    const Parser::Xml::XmlParser::ParseResults xmlResult =
        m_pParser->ParseAttribute( begin, end, receiver );
    const ParseInfo::ParseResult result = Convert( xmlResult );

    return CheckResults( i, result, errorCounter->GetCount() );
}

// ----------------------------------------------------------------------------

// $Log: BasicTesters.cpp,v $
// Revision 1.1  2008/12/05 19:04:15  rich_sposato
// Adding xml parsing files to project.
//
