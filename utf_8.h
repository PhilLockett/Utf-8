/**
 * @file    utf_8.h
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
 * useCharacterRefs UTF-8 conversion code interface.
 */

#if !defined(_UTF_8_H__20211017_1938__INCLUDED_)
#define _UTF_8_H__20211017_1938__INCLUDED_

#include <string>
#include <vector>

namespace utf8 {

extern size_t getUtf8Length(int unicode, char & lead);
extern std::string unicodeToUtf8(int unicode);
extern bool utf8ToUnicode(const std::string & buffer, int & unicode, int & length);

extern void useCharacterRefs(std::string & buffer);
extern std::string useCharacterRefs(const std::string & buffer);


/**
 * @brief Generate a UTF-8 character as a vector from unicode.
 * 
 * @param unicode value to encode as a UTF-8 character.
 * @return std::vector<T> of the encoded UTF-8 character. 
 */
template<typename T = char>
std::vector<T> unicodeToUtf8Vector(int unicode)
{
    // Get UTF-8 length and get first byte with getUtf8Length().
    char buffer{};
    const size_t len{getUtf8Length(unicode, buffer)};

    std::vector<T> output{};
    if (!len)
        return output;

    output.reserve(5);
    output.push_back((T)((unsigned char)buffer));

    // Set remaining bytes.
    size_t shift{6 * (len - 1)};
    for (int i{1}; i < len; ++i)
    {
        shift -= 6;
        const T value = (unicode >> shift) & 0x3F;

        output.push_back(0x80 + value);
    }

    return output;
}

}   // end namespace

#endif // !defined(_UTF_8_H__20211017_1938__INCLUDED_)
