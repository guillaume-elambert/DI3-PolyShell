/*-------------------------------------------------------------------------*
 | Copyright (C) 2018 Département Informatique de PolyTech Tours.          |
 |                                                                         |
 | This file is part of PolyShell, yet another shell.                      |
 |                                                                         |
 | PolyShell is free software; you can redistribute it and/or modify       |
 | it under the terms of the GNU General Public License as published by    |
 | the Free Software Foundation; either version 3 of the License,          |
 | or (at your option) any later version.                                  |
 |                                                                         |
 | PolyShell is distributed in the hope that it will be useful,            |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
 | GNU General Public License for more details.                            |
 |                                                                         |
 | You should have received a copy of the GNU General Public License       |
 | along with this program. If not, see <http://www.gnu.org/licenses/>.    |
 |                                                                         |
 | Additional permission under GNU GPL version 3 section 7 ---- If you     |
 | modify PolyShell, or any covered work, by linking or combining it with  |
 | libprovided (or a modified version of that library), containing parts   |
 | covered by the terms of the Creative Commons BY-NC-ND 4.0 International |
 | License, the licensors of  PolyShell grant you additional permission    |
 | to convey the resulting work.                                           |
 *-------------------------------------------------------------------------*/

#include "interactive/header.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include <assert.h>
#include "misc/string.h"
#include "system/info.h"
#include "interactive/terminal.h"

// #########################################################################
// #########################################################################
// #########################################################################

void mkColored(char *str, unsigned int capacity, const char *style) {
// -------------------------------- asserts --------------------------------
// -------------------------------------------------------------------------
    assert(str); assert(capacity);
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    if(style) {
        size_t strLength = stringLength(str),
               styLength = stringLength(style),
               resLength = stringLength(RESET_ALL);
        if( strLength
          + styLength
          + resLength
          + 1 <= capacity ) {
            for(int i = resLength    ; i >= 0; --i) str[i + styLength + strLength] = RESET_ALL[i];
            for(int i = strLength - 1; i >= 0; --i) str[i + styLength] = str[i];
            for(int i = styLength - 1; i >= 0; --i) str[i] = style[i];
        } }
}

//
#define STATIC_BUFFER_SIZE 256

int header(char *dest, size_t destSize) {
// -------------------------------- asserts --------------------------------
// -------------------------------------------------------------------------
    assert(dest
        && destSize >= 32);
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    const int root = isRoot();
    const char *tmp; userInformation(&tmp, NULL, NULL);
    // get and format path;host;username
    char path[STATIC_BUFFER_SIZE] = ""; copyStringWithLength(path, getCurrentDirectory(1), STATIC_BUFFER_SIZE); mkColored(path, STATIC_BUFFER_SIZE,                         SET_FOLDER_STYLE);
    char host[STATIC_BUFFER_SIZE] = ""; copyStringWithLength(host,             hostname(), STATIC_BUFFER_SIZE); mkColored(host, STATIC_BUFFER_SIZE,                         SET_HEADER_STYLE);
    char name[STATIC_BUFFER_SIZE] = ""; copyStringWithLength(name,                    tmp, STATIC_BUFFER_SIZE); mkColored(name, STATIC_BUFFER_SIZE, root ? SET_ROOT_STYLE : SET_HEADER_STYLE);
    int end = snprintf(dest, destSize - 1, "%s@%s:%s", name, host, path);
    if(end < 0) {
        // function failed
        copyStringWithLength(dest, "__unknown__", destSize);
    }
    else if (end < (int)destSize - 1) {
        // function ran successfully
        dest[end    ] = root ? '#' : '$';
        dest[end + 1] = '\0';
    }
    else {
        // the resulting string is too long
        dest[destSize - 2] =  '$';
        dest[destSize - 1] = '\0';
        int resLength = (int)stringLength(RESET_ALL);
        for(int i = 0; i < resLength; ++i)
            dest[destSize - 2 - resLength + i] = RESET_ALL[i];
        dest[destSize - 2 - resLength - 1] =  '.';
        dest[destSize - 2 - resLength - 2] =  '.';
        dest[destSize - 2 - resLength - 3] =  '.';
    }
    return 0;
}
