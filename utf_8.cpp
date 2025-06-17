/**
 * @file    utf_8.cpp
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
 * Unicode UTF-8 conversion code Implementation.
 */

#include <cstring>
#include <string_view>

#include "utf_8.h"

namespace utf8 {


///////////////////////////////////////////////////////////////////////////////
// Unicode to UTF-8.

/**
 * @brief Determine the number of bytes necessary to encode unicode value as
 * UTF-8 and set the first byte in lead.
 * 
 * @param unicode value to encode as UTF-8.
 * @param lead byte set up to indicate length and contains most sig bits.
 * @return size_t number of bytes necessary to encode unicode value as UTF-8.
 *         0 indicates unicode is invalid.
 */
size_t getUtf8Length(int unicode, char & lead)
{
    // Determine the number of bytes necessary and set the first byte.
    if (unicode < 0)
    {
        return 0;
    }

    if (unicode <= 0x7F)
    {
        lead = (char)unicode;

        return 1;
    }

    if (unicode <= 0x07FF)
    {
        lead = (char)(0xC0 + ((unicode >> 6) & 0x1F));

        return 2;
    }

    if (unicode <= 0xFFFF)
    {
        lead = (char)(0xE0 + ((unicode >> 12) & 0x0F));

        return 3;
    }

    if (unicode <= 0x10FFFF)
    {
        lead = (char)(0xF0 + ((unicode >> 18) & 0x07));

        return 4;
    }

    return 0;
}

/**
 * @brief Generate a UTF-8 character as a string from unicode.
 * 
 * @param unicode value to encode as a UTF-8 character.
 * @return std::string the encoded UTF-8 character. 
 */
std::string unicodeToUtf8(int unicode)
{
    char buffer[5]{};

    // Get UTF-8 length and set first byte with getUtf8Length().
    const size_t len{getUtf8Length(unicode, buffer[0])};

    if (!len)
        return "";

    // Set remaining bytes.
    size_t shift{6 * (len - 1)};
    for (int i{1}; i < len; ++i)
    {
        shift -= 6;
        const int value{(unicode >> shift) & 0x3F};
        buffer[i] = 0x80 + value;
    }

    return std::string(buffer);
}


///////////////////////////////////////////////////////////////////////////////
// UTF-8 to Unicode.

/**
 * @brief Determine the UTF-8 byte count from the string buffer.
 * 
 * @param buffer string containing the UTF-8 character.
 * @return size_t the UTF-8 byte count.
 */
size_t numUtf8Bytes(const std::string_view & buffer)
{
    if ((buffer[0] & 0x80) == 0x0)
        return 1;

    if ((buffer[0] & 0xE0) == 0xC0)
        return 2;

    if ((buffer[0] & 0xF0) == 0xE0)
        return 3;

    if ((buffer[0] & 0xF8) == 0xF0)
        return 4;

    return 0;
}

/**
 * @brief Check the string buffer contains a valid UTF-8 character based on the
 * provided byte count and the buffer containing the required continuation
 * bytes.
 * 
 * @param buffer string containing the UTF-8 character.
 * @param len the UTF-8 byte count.
 * @return true if buffer contains a valid UTF-8 character, false otherwise.
 */
bool isValidUtf8(const std::string_view & buffer, size_t len)
{
    if (len > buffer.length())
        return false;

    // Check for non-continuation bytes, note use of fall-thru.
    switch (len)
    {
    case 4: if ((buffer[3] & 0xC0) != 0x80) return false;
    case 3: if ((buffer[2] & 0xC0) != 0x80) return false;
    case 2: if ((buffer[1] & 0xC0) != 0x80) return false;
    }

    return true;
}

/**
 * @brief Get the unicode value from a string containing a valid UTF-8
 * character.
 * 
 * @param buffer string containing the valid UTF-8 character.
 * @param unicode returned value decoded from the UTF-8 character in buffer.
 * @param length returned UTF-8 byte count.
 * @return true if buffer contains a valid UTF-8 character.
 * @return false otherwise (unicode and length are unmodified).
 */
bool utf8ToUnicode(const std::string_view & buffer, int & unicode, int & length)
{
    const size_t len{numUtf8Bytes(buffer)};

    if (len < 1 || len > 4)
        return false;

    if (!isValidUtf8(buffer, len))
        return false;

    switch (len)
    {
    case 1: unicode = buffer[0] & 0x7F; break;
    case 2: unicode = buffer[0] & 0x1F; break;
    case 3: unicode = buffer[0] & 0x0F; break;
    case 4: unicode = buffer[0] & 0x07; break;
    }

    for (int i{1}; i < len; ++i)
    {
        unicode <<= 6;
        unicode |= (buffer[i] & 0x3F);
    }

    // Check for Overlong.
    char lead{};
    const size_t checkLen{getUtf8Length(unicode, lead)};
    if (checkLen != len)
        return false;

    length = len;

    return true;
}


///////////////////////////////////////////////////////////////////////////////
// Unicode and/or UTF-8 to HTML.

/**
 * @brief Replace ISO/IEC 8859-1 & UTF-8 characters in a given string with the
 * corresponding character references necessary for HTML & XML compatibility.
 * 
 * @param buffer containing ISO/IEC 8859-1 & UTF-8 characters to be replaced.
 */
void useCharacterRefs(std::string & buffer)
{
    // Note buffer length changes here.
    for (size_t i{}; i < buffer.length(); i++)
    {
        if (buffer[i] < 32)
        {
            const size_t pos{i};

            int value{};
            int length{1};
            if (utf8ToUnicode(buffer.substr(i), value, length))
                i += length-1;
            else
                value = (unsigned char)buffer[i];

            std::string to{"&#" + std::to_string(value) + ";"};
            buffer.replace(pos, length, to);
        }
    }
}

/**
 * @brief Replace ISO/IEC 8859-1 & UTF-8 characters in a given string with the
 * corresponding character references necessary for HTML & XML compatibility.
 * 
 * @param buffer containing ISO/IEC 8859-1 & UTF-8 characters to be replaced.
 * 
 * @return std::string with character references.
 */
std::string useCharacterRefs(const std::string & buffer)
{
    std::string work{buffer};
    useCharacterRefs(work);

    return work;
}


///////////////////////////////////////////////////////////////////////////////
// Change for a UTF-8 string.

/**
 * @brief Check if the given string starts with an uppercase character.
 * 
 * @param buffer possibly starting with an ASCII or UTF-8 character.
 * @return int 1 if uppercase ASCII, 2 if 2-byte uppercase UTF-8, 0 otherwise.
 */
int isUpper(const std::string_view & buffer)
{
    const unsigned char *cp((const unsigned char *)&buffer[0]);

    // Check uppercase ASCII.
    if ((*cp > 0x40) && (*cp < 0x5B))
        return 1;

    // Check not a 2-byte UTF-8 char.
    if (*cp != 0xC3)
        return 0;

    // Next byte.
    ++cp;

    // Is the symbol?
    if (*cp == 0x97)
        return 0;

    // Check 2-byte uppercase UTF-8 char.
    if ((*cp > 0x7F) && (*cp < 0x9F))
        return 2;

    return 0;
}

/**
 * @brief Check if the given string starts with a lowercase character.
 * 
 * @param buffer possibly starting with an ASCII or UTF-8 character.
 * @return int 1 if lowercase ASCII, 2 if 2-byte lowercase UTF-8, 0 otherwise.
 */
int isLower(const std::string_view & buffer)
{
    const unsigned char *cp((const unsigned char *)&buffer[0]);
    // Check lowercase ASCII.
    if ((*cp > 0x60) && (*cp < 0x7B))
        return 1;

    // Check not a 2-byte UTF-8 char.
    if (*cp != 0xC3)
        return 0;

    // Next byte.
    ++cp;

    // Is the symbol?
    if (*cp == 0xB7)
        return 0;

    // Check 2-byte lowercase UTF-8 char.
    if ((*cp > 0x9F) && (*cp < 0xBF))
        return 2;

    return 0;
}

/**
 * @brief Convert a lowercase character to uppercase at the start of string.
 * 
 * @param buffer possibly starting with an ASCII or UTF-8 lowercase character.
 * @return int 1 if uppercase ASCII, 2 if 2-byte uppercase UTF-8, 0 otherwise.
 */
int toUpper(std::string & buffer)
{
    const int lower{isLower(buffer)};
    if (lower == 0)
    {
        return 0;
    }

    unsigned char *cp((unsigned char *)buffer.c_str());

    // Move to next byte for UTF-8.
    if (lower == 2)
    {
        ++cp;
    }

    // Flip case.
    *cp ^= 0x20;

    return lower;
}

/**
 * @brief Convert a uppercase character to lowercase at the start of string.
 * 
 * @param buffer possibly starting with an ASCII or UTF-8  uppercase character.
 * @return int 1 if lowercase ASCII, 2 if 2-byte lowercase UTF-8, 0 otherwise.
 */
int toLower(std::string & buffer)
{
    const int upper{isUpper(buffer)};
    if (upper == 0)
    {
        return 0;
    }

    unsigned char *cp((unsigned char *)buffer.c_str());

    // Move to next byte for UTF-8.
    if (upper == 2)
    {
        ++cp;
    }

    // Flip case.
    *cp ^= 0x20;

    return upper;
}

/**
 * @brief Convert lowercase characters to uppercase in the given string.
 * 
 * @param buffer possibly containing ASCII or UTF-8 lowercase characters.
 */
void makeUpper(std::string & buffer)
{
    const std::string_view view{buffer};

    const auto length{buffer.length()};
    for (size_t i{}; i < length; i++)
    {
        auto lead{view.substr(i)};
        const int lower{isLower(lead)};
        
        if (lower == 0)
        {
            // Skip over any UTF-8 bytes.
            auto bytes{numUtf8Bytes(lead)};
            if (bytes)
            {
                i += bytes-1;
            }
            continue;
        }

        // Skip to next byte for UTF-8.
        if (lower == 2)
        {
            ++i;
        }

        // Flip case.
        buffer[i] ^= 0x20;
    }
}

/**
 * @brief Convert uppercase characters to lowercase in the given string.
 * 
 * @param buffer possibly containing ASCII or UTF-8 uppercase characters.
 */
void makeLower(std::string & buffer)
{
    const std::string_view view{buffer};

    const auto length{buffer.length()};
    for (size_t i{}; i < length; i++)
    {
        auto lead{view.substr(i)};
        const int upper{isUpper(lead)};
        
        if (upper == 0)
        {
            // Skip over any UTF-8 bytes.
            auto bytes{numUtf8Bytes(lead)};
            if (bytes)
            {
                i += bytes-1;
            }
            continue;
        }

        // Skip to next byte for UTF-8.
        if (upper == 2)
        {
            ++i;
        }

        // Flip case.
        buffer[i] ^= 0x20;
    }
}


}   // end namespace
