// ----------------------------------------------------------------------------
// Parser Utility Testing
// Copyright (c) 2007 by Rich Sposato
//
// Permission to use, copy, modify, distribute and sell this software for any
// purpose is hereby granted under the terms stated in the GNU Library Public
// License, provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear in
// supporting documentation.
//
// ----------------------------------------------------------------------------

// $Header: /cvsroot/utilparsers/UtilParsers/Xml/Tester/CommandLineArgs.hpp,v 1.1 2008/12/05 19:04:15 rich_sposato Exp $


// ----------------------------------------------------------------------------
// Preprocessor directives.

#if !defined( XML_TESTER_ARGS_HPP_INCLUDED )
/// File guardian.
#define XML_TESTER_ARGS_HPP_INCLUDED


#include "../../Util/include/ErrorReceiver.hpp"


// CommandLineArgs ------------------------------------------------------------

class CommandLineArgs
{
public:

    inline static const CommandLineArgs & GetCommandLineArgs( void )
    {
        return *s_instance;
    }

    /** Parses through and validates the command line parameters.
     @param[in] argc # of parameters.
     @param[in] argv Array of parameters.
     */
    CommandLineArgs( unsigned int argc, const char * const argv[] );

    inline ~CommandLineArgs( void )
    {
        s_instance = 0;
    }

    inline bool IsValid( void ) const { return m_valid; }
    inline bool DoWait( void ) const { return m_doWait; }
    inline bool DoShowHelp( void ) const { return m_showHelp; }
    inline bool DoUnitTests( void ) const { return m_doUnitTest; }
    inline bool DoFileTests( void ) const { return m_doFileTest; }
    inline bool DoShowContent( void ) const { return m_showContent; }
    inline bool DoShowSummary( void ) const { return m_showSummary; }
    inline bool DoShowTable( void ) const { return m_showTable; }
    inline bool DoMeanTests( void ) const { return m_doMeanTests; }
    inline bool DoShowInfo( void ) const { return m_showInfo; }
    inline bool DoSpecificUnitTest( void ) const
    {
        return ( 0 <= m_unitTestIndex );
    }
    inline unsigned int GetUnitTestIndex( void ) const
    {
        return m_unitTestIndex;
    }

    inline ::Parser::ErrorLevel::Levels GetErrorLevel( void ) const
    {
        return m_showErrorLevel;
    }
    inline const char * GetFileName( void ) const { return m_fileName; }
    inline const char * GetExeName( void ) const { return m_exeName; }
    inline const char * GetExePath( void ) const { return m_exePath; }

private:

    CommandLineArgs( void );
    CommandLineArgs( const CommandLineArgs & );
    CommandLineArgs & operator = ( const CommandLineArgs & );

    static CommandLineArgs * s_instance;

    bool m_valid;             ///< True if all command line parameters are valid.
    bool m_doWait;
    bool m_showHelp;
    bool m_doUnitTest;
    bool m_doFileTest;
    bool m_showContent;
    bool m_showSummary;
    bool m_showTable;
    bool m_doMeanTests;
    bool m_showInfo;
    signed int m_unitTestIndex;
    Parser::ErrorLevel::Levels m_showErrorLevel;
    const char * m_fileName;
    const char * m_exePath;
    const char * m_exeName;
    const char * const * const m_argv;
    const unsigned int m_argc;
};

// ----------------------------------------------------------------------------

#endif // file guardian

// $Log: CommandLineArgs.hpp,v $
// Revision 1.1  2008/12/05 19:04:15  rich_sposato
// Adding xml parsing files to project.
//
