/**
 * @file    test.cpp
 * @author  Phil Lockett <phillockett65@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Test the UTF-8 code Implementation.
 *
 * Build using:
 *    g++ -std=c++20 -c -o test.o test.cpp
 *    g++ -std=c++20 -c -o unittest.o unittest.cpp
 *    g++ -std=c++20 -c -o utf_8.o utf_8.cpp
 *    g++ -std=c++20 -o test test.o unittest.o
 *
 * Test using:
 *    ./test
 *
 *  Should generate 1 error.
 */

#include <iostream>

#include "unittest.h"
#include "utf_8.h"


///////////////////////////////////////////////////////////////////////////////

const char AData[2]{ 0x41, 0 };
const char zData[2]{ 0x7A, 0 };
const char copyrightData[3]{ (char)0xC2, (char)0xA9, 0 };
const char umlautData[3]{ (char)0xC3, (char)0xB6, 0 };
const char upArrowData[4]{ (char)0xE2, (char)0xAD, (char)0xA1, 0 };
const char clubs3Data[5]{ (char)0xF0, (char)0x9F, (char)0x83, (char)0x93, 0 };


///////////////////////////////////////////////////////////////////////////////

/**
 * @section 0 - unicodeToUtf8 translation test.
 */
UNIT_TEST(test0, "unicodeToUtf8 - Test null character.")

    SET_TOLERANCE(0.0)          // Don't care about timing yet!

    REQUIRE(unicodeToUtf8(0).compare("") == 0)

NEXT_CASE(test1, "unicodeToUtf8 - Test 'A' character.")

    REQUIRE(unicodeToUtf8(0x41).compare(AData) == 0)

NEXT_CASE(test2, "unicodeToUtf8 - Test 'z' character.")

    REQUIRE(unicodeToUtf8(0x7A).compare(zData) == 0)

NEXT_CASE(test3, "unicodeToUtf8 - Test copyright character.")

    REQUIRE(unicodeToUtf8(0xA9).compare(copyrightData) == 0)

NEXT_CASE(test4, "unicodeToUtf8 - Test o umlaut character.")

    REQUIRE(unicodeToUtf8(0xF6).compare(umlautData) == 0)

NEXT_CASE(test5, "unicodeToUtf8 - Test upwards triangle headed arrow character.")

    REQUIRE(unicodeToUtf8(0x2B61).compare(upArrowData) == 0)

NEXT_CASE(test6, "unicodeToUtf8 - Test 3 of clubs character.")

    REQUIRE(unicodeToUtf8(0x1F0D3).compare(clubs3Data) == 0)

END_TEST


/**
 * @section 1 - getUtf8Length length test.
 */
extern size_t getUtf8Length(int unicode, char & lead);

UNIT_TEST(test10, "getUtf8Length - Test null character length.")
char leadByte{};

    REQUIRE(getUtf8Length(0, leadByte) == 1)
    REQUIRE(leadByte == 0)

NEXT_CASE(test11, "getUtf8Length - Test 'A' character length.")

    REQUIRE(getUtf8Length(0x41, leadByte) == 1)
    REQUIRE(leadByte == 0x41)

NEXT_CASE(test12, "getUtf8Length - Test 'z' character length.")

    REQUIRE(getUtf8Length(0x7A, leadByte) == 1)
    REQUIRE(leadByte == 0x7A)

NEXT_CASE(test13, "getUtf8Length - Test copyright character length.")

    REQUIRE(getUtf8Length(0xA9, leadByte) == 2)
    REQUIRE((int)(unsigned char)leadByte == 0xC2)

NEXT_CASE(test14, "getUtf8Length - Test o umlaut character length.")

    REQUIRE(getUtf8Length(0xF6, leadByte) == 2)
    REQUIRE((int)(unsigned char)leadByte == 0xC3)

NEXT_CASE(test15, "getUtf8Length - Test upwards triangle headed arrow character length.")

    REQUIRE(getUtf8Length(0x2B61, leadByte) == 3)
    REQUIRE((int)(unsigned char)leadByte == 0xE2)

NEXT_CASE(test16, "getUtf8Length - Test 3 of clubs character length.")

    REQUIRE(getUtf8Length(0x1F0D3, leadByte) == 4)
    REQUIRE((int)(unsigned char)leadByte == 0xF0)

END_TEST


/**
 * @section 2 - utf8ToUnicode translation test.
 */
UNIT_TEST(test20, "utf8ToUnicode - Test null character.")
int unicode{};
int length{};

    REQUIRE(utf8ToUnicode("", unicode, length) == false)

NEXT_CASE(test21, "utf8ToUnicode - Test 'A' character.")

    REQUIRE(utf8ToUnicode(AData, unicode, length) == true)
    REQUIRE(unicode == 0x41)
    REQUIRE(length == 1)

NEXT_CASE(test22, "utf8ToUnicode - Test 'z' character.")

    REQUIRE(utf8ToUnicode(zData, unicode, length) == true)
    REQUIRE(unicode == 0x7A)
    REQUIRE(length == 1)

NEXT_CASE(test23, "utf8ToUnicode - Test copyright character.")

    REQUIRE(utf8ToUnicode(copyrightData, unicode, length) == true)
    REQUIRE(unicode == 0xA9)
    REQUIRE(length == 2)

NEXT_CASE(test24, "utf8ToUnicode - Test o umlaut character.")

    REQUIRE(utf8ToUnicode(umlautData, unicode, length) == true)
    REQUIRE(unicode == 0xF6)
    REQUIRE(length == 2)

NEXT_CASE(test25, "utf8ToUnicode - Test upwards triangle headed arrow character.")

    REQUIRE(utf8ToUnicode(upArrowData, unicode, length) == true)
    REQUIRE(unicode == 0x2B61)
    REQUIRE(length == 3)

NEXT_CASE(test26, "utf8ToUnicode - Test 3 of clubs character.")

    REQUIRE(utf8ToUnicode(clubs3Data, unicode, length) == true)
    REQUIRE(unicode == 0x1F0D3)
    REQUIRE(length == 4)

END_TEST


/**
 * @section 3 - utf8ToUnicode error checking test.
 * 
 * Examples from:
 *     https://en.wikipedia.org/wiki/UTF-8#Overlong_encodings
 *     https://en.wikipedia.org/wiki/UTF-8#Invalid_sequences_and_error_handling
 */
UNIT_TEST(test30, "utf8ToUnicode - Test '$' 1 char array.")
int unicode{};
int length{};

    const char dollarData[2]{ 0x24, 0 };
    REQUIRE(utf8ToUnicode(dollarData, unicode, length) == true)
    REQUIRE(unicode == 0x24)
    REQUIRE(length == 1)

NEXT_CASE(test31, "utf8ToUnicode - Test cent 2 char array.")

    const char centData[3]{ (char)0xC2, (char)0xA2, 0 };
    REQUIRE(utf8ToUnicode(centData, unicode, length) == true)
    REQUIRE(unicode == 0xA2)
    REQUIRE(length == 2)

NEXT_CASE(test32, "utf8ToUnicode - Test Euro 3 char array.")

    const char euroData[4]{ (char)0xE2, (char)0x82, (char)0xAC, 0 };
    REQUIRE(utf8ToUnicode(euroData, unicode, length) == true)
    REQUIRE(unicode == 0x20AC)
    REQUIRE(length == 3)

NEXT_CASE(test33, "utf8ToUnicode - Test Hwair 4 char array.")

    const char hwairData[5]{ (char)0xF0, (char)0x90, (char)0x8D, (char)0x88, 0 };
    REQUIRE(utf8ToUnicode(hwairData, unicode, length) == true)
    REQUIRE(unicode == 0x10348)
    REQUIRE(length == 4)

NEXT_CASE(test34, "utf8ToUnicode - Test cent 2 char array without continuation byte.")

    const char cent2Data[3]{ (char)0xC2, (char)0x22, 0 };
    REQUIRE(utf8ToUnicode(cent2Data, unicode, length) == false)

NEXT_CASE(test35, "utf8ToUnicode - Test cent 1 char array too short.")

    const char cent1Data[2]{ (char)0xC2, 0 };
    REQUIRE(utf8ToUnicode(cent1Data, unicode, length) == false)

NEXT_CASE(test36, "utf8ToUnicode - Test Hwair 4 char array missing continuation byte.")

    const char hwair4Data[5]{ (char)0xF0, (char)0x90, (char)0x8D, (char)0x08, 0 };
    REQUIRE(utf8ToUnicode(hwair4Data, unicode, length) == false)

NEXT_CASE(test37, "utf8ToUnicode - Test unexpected continuation byte.")

    const char continuationData[2]{ (char)0xA4, 0 };
    REQUIRE(utf8ToUnicode(continuationData, unicode, length) == false)

NEXT_CASE(test38, "utf8ToUnicode - Test Euro overlong encoding.")

    const char euroLongData[5]{ (char)0xF0, (char)0x82, (char)0x82, (char)0xAC, 0 };
    REQUIRE(utf8ToUnicode(euroLongData, unicode, length) == false)

END_TEST


///////////////////////////////////////////////////////////////////////////////

int runTests(void)
{
    std::cout << "Executing all tests.\n";

    RUN_TEST(test0)
    RUN_TEST(test10)
    RUN_TEST(test20)
    RUN_TEST(test30)

    const int err = FINISHED;
    if (err)
        std::cerr << err << " ERROR(S) encountered!.\n";
    else
        std::cout << "All tests passed.\n";

    return err;
}

/**
 * Test system entry point.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return error value or 0 if no errors.
 */
int main(int argc, char *argv[])
{
    return runTests();
}

