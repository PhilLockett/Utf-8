# UTF-8

Unicode to UTF-8 and vice versa conversion code.

## Overview

This small library provides functions to convert between UTF-8 and unicode.

### unicodeToUtf8()

Takes a `unicode` value and returns the equivalent UTF-8 character as a 
string.

    std::string unicodeToUtf8(int unicode);

### utf8ToUnicode()

Takes a string containing a UTF-8 character and extracts the unicode value it
contains. The function returns true if `buffer` contains a valid UTF-8 
character, or false otherwise. If valid, `unicode` is set to the decoded UTF-8
character and `length` is set to the UTF-8 byte count. If not valid, `unicode`
and `length` are both unchanged.

    bool utf8ToUnicode(const std::string & buffer, int & unicode, int & length);

### useCharacterRefs()

Replaces ISO/IEC 8859-1 & UTF-8 characters in a given string with the
corresponding character references necessary for HTML & XML compatibility. Two
versions are available, the first modifies the original string referenced, the
second returns a new string with the modifications, leaving the original string
unchanged.

    void useCharacterRefs(std::string & buffer);

    std::string useCharacterRefs(const std::string & buffer);

### unicodeToUtf8Vector()

Takes a `unicode` value and returns the equivalent UTF-8 character as a 
vector. The default type of the vector is `char`.

    template<typename T = char>
    std::vector<T> unicodeToUtf8Vector(int unicode);

## Cloning and Running

### Test Code
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

### Utility Code
The utility code can be built and run using the following for an example:

    make genutf8
    ./genutf8 0 0x41 0x7A 0xA9 0xF6 0x2B61 0x1F0D3


## Points of interest

This code has the following points of interest:

  * utf8ToUnicode() returns `length` so that the UTF-8 char can be removed.
  * utf8ToUnicode() checks for an unexpected continuation byte.
  * utf8ToUnicode() checks for missing continuation bytes.
  * utf8ToUnicode() checks for overlong encodings.
