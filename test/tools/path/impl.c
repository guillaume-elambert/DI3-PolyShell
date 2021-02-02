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

#include "path.h"

// #########################################################################
// #########################################################################
// #########################################################################

//
#include <string.h>
#include <stdlib.h>

//
#include "misc/ferror.h"

// #########################################################################
// #########################################################################
// #########################################################################

//
const char* (*__old__getPATHVar__)(void) = NULL;

//
static char *__substitute__ = NULL;

//
static const char *__test_getPATHVar__(void) { return __substitute__; }

// #########################################################################
// #########################################################################
// #########################################################################

void useMask()
{
    if (__old__getPATHVar__) fatalError("rerouting has failed");
    __old__getPATHVar__ = getPATHVar;
}

void endMask()
{
    getPATHVar = __old__getPATHVar__; __old__getPATHVar__ = NULL;
}

void maskPath(const char *newpath)
{
    if (__old__getPATHVar__ == NULL) { fatalError("rerouting has failed");                                      }
    if (     __substitute__ != NULL) { free(__substitute__);                                                    }
                                     { getPATHVar = __test_getPATHVar__; __substitute__ = strdup(newpath?newpath:""); }
    if (     __substitute__ == NULL) { fatalError("rerouting has failed");                                      }
}

int maskIsActive()
{
    return getPATHVar == __test_getPATHVar__;
}
