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

#include "misc/ferror.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include <stdarg.h>
#include <signal.h>
#include <stdlib.h>

void debugFatalError(const char *func, int code, const char *format, ...) {
// -------------------------------- message --------------------------------
// -------------------------------------------------------------------------
    fprintf(stderr, "%s: ", func);
    {
        // custom message
        va_list args; va_start(args, format); vfprintf(stderr, format, args); va_end(args);
    }
    fprintf(stderr, ". Abort.\n");
// ------------------------------ debug-trap -------------------------------
// -------------------------------------------------------------------------
#ifndef NDEBUG
    (void)raise(SIGTRAP);
    __builtin_trap();
#endif
// --------------------------------- exit ----------------------------------
// -------------------------------------------------------------------------
    exit(code);
}
