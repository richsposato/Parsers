# Parsers

Utilities for parsing files.

Copyright by Rich Sposato

Home Page: https://github.com/richsposato/Parsers


This parsing library was designed to parse files or strings for syntax and content. It checks for valid syntax and extracts content to provide to a listener object.


Directories in this project:
1 .	Home directory of entire project.
2 Util	Contains source code for low level utilities and common functions.
3 Config	Contains source code for config file parsing library.
4 Xml	Contains source code for xml parsing library.  Partially completed - do not use yet!

## Notes:

1. To use this class, you will need to install the Boost.Spirit library.

2. While parsing a file or string, this library calls one observor class to notify client code of any
   syntax errors found in the content.  It calls a separate observor class to provide the pasrsed
   content - one chunk at a time.

3. The main interface for this library is a facade class that implements the Pimpl Idiom to insulate
   client projects from the implementation of parsing.  The client code will not need to #include any
   boost files or link to boost libraries.

4. The intent is to simply client code that must deal with parsing files.  All the client code has to do
   is construct a parser, provide a filename or string, and then receive any content or error messages.

5. The Xml directory has various other files and subdirectories not described here.  This library is not
   ready for public use yet, so I chose not to describe all the files just yet.

6. This project compiles with both Visual Studio and GCC.
