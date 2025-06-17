/**
 * @file    genutf8.cpp
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
 *    g++ -std=c++20 -c -o genutf8.o genutf8.cpp
 *    g++ -std=c++20 -c -o utf_8.o utf_8.cpp
 *    g++ -std=c++20 -o genutf8 genutf8.o utf_8.o
 *
 * Usage:
 *    ./genutf8 <unicode> ...
 *
 */

#include <iostream>
#include <string>
#include <string_view>

#include "utf_8.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Convert 'unicode' value to equivalent UTF-8 and display the result.
 * 
 * @param unicode value to be converted to UTF-8.
 */
void dumpUtf8(int unicode)
{
    std::vector<int> output{utf8::unicodeToUtf8Vector<int>(unicode)};

    std::cout << "0x" << std::hex << unicode << std::dec << " ("<< unicode << ")\t= " << std::hex;

    for (auto byte : output)
        std::cout << "0x" << byte << " ";

    std::cout << std::dec << "\t( ";

    for (auto byte : output)
        std::cout << byte << " ";

    std::cout << ")\n";
}


constexpr std::string_view IS_HEX{"0x"};
constexpr std::string_view IS_OCT{"0"};

/**
 * @brief Get the integer value from an 'ascii' string.
 * 
 * @param ascii string containing the ascii representtion of the integer value.
 * @return int value of the given 'ascii' string.
 */
int getInt(std::string ascii)
{
    auto base = [& ascii](const std::string_view & b) { return (ascii.compare(0, b.length(), b) == 0); };
    std::size_t pos;

    if (base(IS_HEX))
        return std::stoi(ascii, &pos, 16);
    else
    if (base(IS_OCT))
        return std::stoi(ascii, &pos, 8);

    return std::stoi(ascii);
}


/**
 * Display UTF-8 equivalent of unicode values from the command line.
 *
 * @param  argc - command line argument count.
 * @param  argv - command line argument vector.
 * @return error value or 0 if no errors.
 */
int main(int argc, char *argv[])
{
    for (int i{1}; i < argc; ++i)
        dumpUtf8(getInt(argv[i]));

    return 0;
}
