# utf_8

Unicode to utf-8 and vice versa conversion code.

## Overview

The small library privdes to functions to convert between utf-8 and unicode.

### unicodeToUtf8()

Takes a `unicode` value and returns the equivalent utf-8 character as a 
std::string.

    std::string unicodeToUtf8(int unicode);

### utf8ToUnicode()

Takes a string containing a utf-8 character and extracts the unicode value it
contains. The function returns true if `buffer` contains a valid utf-8 
character, or false otherwise. If valid, `unicode` is set to the decoded utf-8
character and `length` is set to the utf-8 byte count.

    bool utf8ToUnicode(const std::string & buffer, int & unicode, int & length);

## Cloning and Running (to be updated)

The test code is dependent on UnitTest. To compile and run the Utf-8 test code
you first need to clone the unit test code, then copy unittest.cpp and 
unittest.h into the Logger directory before executing make.

    git clone https://github.com/PhilLockett/UnitTest.git

To clone, build and run the Logger code, including the test code, execute the 
following unix/linux commands:

    git clone https://github.com/PhilLockett/Utf-8.git
    cd Utf-8/
    cp ../UnitTest/unittest.cpp .
    cp ../UnitTest/unittest.h .
    make
    ./test

## Points of interest

This code has the following points of interest:

  * utf8ToUnicode() checks for an unexpected continuation byte.
  * utf8ToUnicode() checks for missing continuation bytes.
  * utf8ToUnicode() checks for overlong encodings.
