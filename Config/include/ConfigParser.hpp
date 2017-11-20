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

// $Header: /cvsroot/utilparsers/UtilParsers/Config/include/ConfigParser.hpp,v 1.5 2009/01/05 19:24:52 rich_sposato Exp $

/// @file ConfigParser.hpp Contains interface for config file parser.


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if !defined( UTIL_CONFIG_PARSER_H_INCLUDED )
/// file guardian.
#define UTIL_CONFIG_PARSER_H_INCLUDED


// ----------------------------------------------------------------------------
// Namespace resolution.

namespace Parser
{
    class IParseErrorReceiver;
    class ConfigParserImpl;


// ----------------------------------------------------------------------------

class IConfigReceiver
{
protected:

    /// Trivially implemented.
    inline IConfigReceiver( void ) {}

    /// Trivially implemented.
    inline virtual ~IConfigReceiver( void ) {}

public:

    /** Called to provide name and value for global key - a key before any section.
     @param keyStart Start of name of key.
     @param keyEnd Pointer to character right after last part of name of key.
     @param valueStart Start of name of value.
     @param valueEnd Pointer to character right after last part of name of value.
     @return True to keep receiving data, false to stop receiving.
     */
    virtual bool AddGlobalKey( const char * keyStart, const char * keyEnd,
        const char * valueStart, const char * valueEnd ) = 0;

    /** Called to provide name of section.  Called when parser starts new section.
     This call also means any previous sections have ended, and there are no more
     global keys.
     @param nameStart Start of name of section.
     @param nameEnd Pointer to character right after last part of name of section.
     @return True to keep receiving data, false to stop receiving.
     */
    virtual bool AddSection( const char * nameStart, const char * nameEnd ) = 0;

    /** Called to provide name and value for key within current section.
     @param keyStart Start of name of key.
     @param keyEnd Pointer to character right after last part of name of key.
     @param valueStart Start of name of value.
     @param valueEnd Pointer to character right after last part of name of value.
     @return True to keep receiving data, false to stop receiving.
     */
    virtual bool AddSectionKey( const char * keyStart, const char * keyEnd,
        const char * valueStart, const char * valueEnd ) = 0;

    /** Called when done parsing to say if contents are valid.  The parser will not
     call this receiver after this function call ends.
     */
    virtual void ParsedConfigFile( bool valid ) = 0;

private:
    /// Not implemented.
    IConfigReceiver( const IConfigReceiver & );
    /// Not implemented.
    IConfigReceiver & operator = ( const IConfigReceiver & );
};

// ----------------------------------------------------------------------------

/** @class ConfigParser
 * @brief Parses config files for proper syntax and provides content to a receiver.
 *
 * @code
 *   GlobalKey = GlobalValue // any key-value pair before section is global.
 *   // comment
 * [Section1]
 *   Key1 = Value1 // comment
 *   Key2 = value2
 *   KeyWithNoValue
 *   OtherKeyWithNoValue =
 *   YetAnotherKeyWithNoValue = // comment
 * [Section2]
 * // comment
 *   AnotherKey = AnotherValue
 *   LastKey = LastValue
 * [Section1] // allowed to repeat section names.
 * @endcode
 *
 * @par Policies
 * You can use the ParserPolicy struct to configure the ConfigParser.  The Parser
 * provides default policies.
 * - Default line comment delimiter is: ;
 * - Default starting block comment delimiter is: / *
 * - Default ending block comment delimiter is: * /
 * - Default section name starting delimiter is: [
 * - Default section name ending delimiter is: ]
 * - Section delimiters may not be the same as any comment delimiters.
 * - Line comment delimiter may not be the same as strarting block comment delimiter.
 * - None of the pointers in the ParserPolicy struct may be NULL.
 */
class ConfigParser
{
    public:

        struct ParserPolicy
        {
            char * LineComment;
            char * BlockCommentStarter;
            char * BlockCommentEnder;
            char * SectionNameStarter;
            char * SectionNameEnder;
            char * AssignOperator;
            bool TrimWhiteSpace;
            bool AlphaNumericNames;
            bool AllowQuotedCommentInValue;
            unsigned int MaxErrorCount;

            ParserPolicy( void );
        };

        enum ParseResults
        {
            AllValid = 0,  ///< All data was parsed and valid.
            SomeValid,  ///< Start is valid, but ending part was not recognized.
            NotValid,   ///< Data was parsed, but is not valid.
            NotParsed,  ///< Data was not recognized by parsing rules.
            NoReceiver, ///< No content receiver provided.
            NoStart,    ///< Starting place is NULL.
            EmptyData,  ///< No content at starting place.
            NoEnd,      ///< Ending place is NULL.
            LowerEnd,   ///< Ending place less than starting place.
            Exception,  ///< Exception thrown.
            ParsingNow, ///< Parser is currently parsing other data.
            Receiving,  ///< Receiver reported an error.
            NoErrorRecv, ///< No error receiver provided.
            NoFileName, ///< No file name provided for parsing.
            CantOpenFile, ///< Could not open file.
            EmptyFile,  ///< File has no contents.
            EndOfFile,  ///< Reached end of file, but can't parse content.
        };

        static const char * Name( ParseResults result );

        static unsigned long GetMajorVersion( void );

        static unsigned long GetMinorVersion( void );

        ConfigParser( void );

        ~ConfigParser( void );

        bool SetMessageReceiver( IParseErrorReceiver * pReceiver );

        IParseErrorReceiver * GetMessageReceiver( void );

        bool IsParsing( void ) const;

        bool SetPolicy( const ConfigParser::ParserPolicy & policy );

        const ConfigParser::ParserPolicy & GetPolicy( void ) const;

        ConfigParser::ParseResults Parse( const char * start, const char * end,
            IConfigReceiver * pReceiver );

        ConfigParser::ParseResults Parse( const char * filename,
            IConfigReceiver * pReceiver );

    private:
        ConfigParser( const ConfigParser & );
        ConfigParser & operator = ( const ConfigParser & );

        ConfigParserImpl * m_impl;
};


// ----------------------------------------------------------------------------

}; // end namespace Parser

#endif // file guardian

// $Log: ConfigParser.hpp,v $
// Revision 1.5  2009/01/05 19:24:52  rich_sposato
// Replaced tabs with spaces.
//
// Revision 1.4  2008/12/09 19:41:28  rich_sposato
// Changed policy names.
//
// Revision 1.3  2008/12/09 00:11:28  rich_sposato
// Changed receiver class and parser class to do memory allocation less often
// and improve performance.
//
// Revision 1.2  2008/12/08 23:11:43  rich_sposato
// A few changes to ConfigParser to make it more versatile and robust.
//
// Revision 1.1  2008/12/05 19:25:28  rich_sposato
// Adding files to CVS.
//
