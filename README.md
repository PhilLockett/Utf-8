# UTF-8

Unicode to UTF-8 and vice versa conversion code.

## Overview

This small library provides functions to convert between UTF-8 and unicode.

### unicodeToUtf8()

Takes a `unicode` value and returns the equivalent UTF-8 character as a 
std::string.

    std::string unicodeToUtf8(int unicode);

### utf8ToUnicode()

Takes a string containing a UTF-8 character and extracts the unicode value it
contains. The function returns true if `buffer` contains a valid UTF-8 
character, or false otherwise. If valid, `unicode` is set to the decoded UTF-8
character and `length` is set to the UTF-8 byte count.

    bool utf8ToUnicode(const std::string & buffer, int & unicode, int & length);

## Cloning and Running

The test code is dependent on UnitTest. To compile and run the UTF-8 test code
you first need to clone the unit test code, then copy unittest.cpp and 
unittest.h into the UTF-8 directory before executing make. To clone the unit
test code use:

    git clone https://github.com/PhilLockett/UnitTest.git

To clone, build and run the UTF-8 code, including the test code, execute the 
following unix/linux commands:

    git clone https://github.com/PhilLockett/Utf-8.git
    cd Utf-8/
    cp ../UnitTest/unittest.cpp .
    cp ../UnitTest/unittest.h .
    make
    ./test

## Points of interest

This code has the following points of interest:

  * utf8ToUnicode() returns `length` so that the UTF-8 char can be removed.
  * utf8ToUnicode() checks for an unexpected continuation byte.
  * utf8ToUnicode() checks for missing continuation bytes.
  * utf8ToUnicode() checks for overlong encodings.
