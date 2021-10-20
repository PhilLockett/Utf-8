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
 * Unicode UTF-8 conversion code interface.
 */

#if !defined(_UTF_8_H__20211017_1938__INCLUDED_)
#define _UTF_8_H__20211017_1938__INCLUDED_

#include <string>


extern std::string unicodeToUtf8(int unicode);
extern bool utf8ToUnicode(const std::string & buffer, int & unicode, int & length);


#endif // !defined(_UTF_8_H__20211017_1938__INCLUDED_)
