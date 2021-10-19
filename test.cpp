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
 * Test the utf-8 code Implementation.
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



/**
 * @section 0 - unicodeToUtf8 translation test.
 */
UNIT_TEST(test0, "unicodeToUtf8 - Test 0 character.")

    SET_TOLERANCE(0.0)          // Don't care about timing yet!

    REQUIRE(unicodeToUtf8(0).compare("") == 0)

NEXT_CASE(test1, "unicodeToUtf8 - Test A character.")

    REQUIRE(unicodeToUtf8(65).compare("A") == 0)

NEXT_CASE(test2, "unicodeToUtf8 - Test z character.")

    REQUIRE(unicodeToUtf8(122).compare("z") == 0)

NEXT_CASE(test3, "unicodeToUtf8 - Test copyright character.")

    REQUIRE(unicodeToUtf8(169).compare("©") == 0)

NEXT_CASE(test4, "unicodeToUtf8 - Test o umlaut character.")

    REQUIRE(unicodeToUtf8(246).compare("ö") == 0)

NEXT_CASE(test5, "unicodeToUtf8 - Test upwards triangle headed arrow character.")

    REQUIRE(unicodeToUtf8(0x2B61).compare("⭡") == 0)

NEXT_CASE(test6, "unicodeToUtf8 - Test 3 of clubs character.")

    REQUIRE(unicodeToUtf8(0x1F0D3).compare("🃓") == 0)

END_TEST


/**
 * @section 1 - getUtf8Length length test.
 */
extern size_t getUtf8Length(int unicode, char & lead);

UNIT_TEST(test10, "getUtf8Length - Test 0 character length.")
char leadByte{};

    REQUIRE(getUtf8Length(0, leadByte) == 1)
    REQUIRE(leadByte == 0)

NEXT_CASE(test11, "getUtf8Length - Test A character length.")

    REQUIRE(getUtf8Length(0x41, leadByte) == 1)
    REQUIRE(leadByte == 0x41)

NEXT_CASE(test12, "getUtf8Length - Test z character length.")

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
UNIT_TEST(test20, "utf8ToUnicode - Test 0 character.")
int unicode{};
int length{};

    REQUIRE(utf8ToUnicode("", unicode, length) == false)

NEXT_CASE(test21, "utf8ToUnicode - Test A character.")

    REQUIRE(utf8ToUnicode("A", unicode, length) == true)
    REQUIRE(unicode == 65)
    REQUIRE(length == 1)

NEXT_CASE(test22, "utf8ToUnicode - Test z character.")

    REQUIRE(utf8ToUnicode("z", unicode, length) == true)
    REQUIRE(unicode == 122)
    REQUIRE(length == 1)

NEXT_CASE(test23, "utf8ToUnicode - Test copyright character.")

    REQUIRE(utf8ToUnicode("©", unicode, length) == true)
    REQUIRE(unicode == 169)
    REQUIRE(length == 2)

NEXT_CASE(test24, "utf8ToUnicode - Test o umlaut character.")

    REQUIRE(utf8ToUnicode("ö", unicode, length) == true)
    REQUIRE(unicode == 246)
    REQUIRE(length == 2)

NEXT_CASE(test25, "utf8ToUnicode - Test upwards triangle headed arrow character.")

    REQUIRE(utf8ToUnicode("⭡", unicode, length) == true)
    REQUIRE(unicode == 0x2B61)
    REQUIRE(length == 3)

NEXT_CASE(test26, "utf8ToUnicode - Test 3 of clubs character.")

    REQUIRE(utf8ToUnicode("🃓", unicode, length) == true)
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
UNIT_TEST(test30, "utf8ToUnicode - Test $ 1 char array.")
int unicode{};
int length{};

    const char data1[2]{ 0x24, 0 };
    REQUIRE(utf8ToUnicode(data1, unicode, length) == true)
    REQUIRE(unicode == 0x24)
    REQUIRE(length == 1)

NEXT_CASE(test31, "utf8ToUnicode - Test ¢ 2 char array.")

    const char data2[3]{ (char)0xC2, (char)0xA2, 0 };
    REQUIRE(utf8ToUnicode(data2, unicode, length) == true)
    REQUIRE(unicode == 0xA2)
    REQUIRE(length == 2)

NEXT_CASE(test32, "utf8ToUnicode - Test € 3 char array.")

    const char data3[4]{ (char)0xE2, (char)0x82, (char)0xAC, 0 };
    REQUIRE(utf8ToUnicode(data3, unicode, length) == true)
    REQUIRE(unicode == 0x20AC)
    REQUIRE(length == 3)

NEXT_CASE(test33, "utf8ToUnicode - Test 𐍈 4 char array.")

    const char data4[5]{ (char)0xF0, (char)0x90, (char)0x8D, (char)0x88, 0 };
    REQUIRE(utf8ToUnicode(data4, unicode, length) == true)
    REQUIRE(unicode == 0x10348)
    REQUIRE(length == 4)

NEXT_CASE(test34, "utf8ToUnicode - Test ¢ 2 char array without continuation byte.")

    const char data5[3]{ (char)0xC2, (char)0x22, 0 };
    REQUIRE(utf8ToUnicode(data5, unicode, length) == false)

NEXT_CASE(test35, "utf8ToUnicode - Test ¢ 1 char array too short.")

    const char data6[2]{ (char)0xC2, 0 };
    REQUIRE(utf8ToUnicode(data6, unicode, length) == false)

NEXT_CASE(test36, "utf8ToUnicode - Test 𐍈 4 char array missing continuation byte.")

    const char data7[5]{ (char)0xF0, (char)0x90, (char)0x8D, (char)0x08, 0 };
    REQUIRE(utf8ToUnicode(data7, unicode, length) == false)

NEXT_CASE(test37, "utf8ToUnicode - Test unexpected continuation byte.")

    const char data8[2]{ (char)0xA4, 0 };
    REQUIRE(utf8ToUnicode(data8, unicode, length) == false)

NEXT_CASE(test38, "utf8ToUnicode - Test € overlong encoding.")

    const char data9[5]{ (char)0xF0, (char)0x82, (char)0x82, (char)0xAC, 0 };
    REQUIRE(utf8ToUnicode(data9, unicode, length) == false)

END_TEST


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

