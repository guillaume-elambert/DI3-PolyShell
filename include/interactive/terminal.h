#ifndef TERMINAL_H

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

#define TERMINAL_H

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 *
 *
 */
#define RING "\a"
#define SAVE_CURSOR "\x1B[s"
#define RESTORE_CURSOR "\x1B[u"
#define SET_CURSOR_STYLE "\x1B[97m\x1B[41m"
#define SET_EXECUTABLE_STYLE "\x1B[30m\x1B[106m"
#define SET_ARGS_STYLE "\x1B[97m\x1B[100m"
#define SET_POLYTECH_STYLE "\x1B[96m"
#define SET_ESC_CHARACTER_STYLE "\x1B[37m"
#define SET_OPERATOR_STYLE "\x1B[30m\x1B[105m"
#define SET_FILE_STYLE "\x1B[30m\x1B[102m"
#define SET_INFO_STYLE "\x1B[30m\x1B[47m"
#define SET_ROOT_STYLE "\x1B[31m"
#define SET_HEADER_STYLE "\x1B[32m"
#define SET_FOLDER_STYLE "\x1B[34m"
#define RESET_ALL "\x1B[0m"
#define SPACE "\x20"
#define ERASE_AFTER_CURSOR "\033[K"
#define LEFT_SHIFT_1 "\b"
#define LEFT_SHIFT_2 "\b\b"
#define LEFT_SHIFT_N "\r"
#define HIDE_CURSOR "\x1B[?25l"
#define SHOW_CURSOR "\x1B[?25h"
#define ERASE_1_CHAR "\x20\b"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 *
 *
 */
#define KB_NOPE 0
#define KB_UP -1
#define KB_DOWN -2
#define KB_LEFT -3
#define KB_RIGHT -4
#define KB_ESCAPE -5
#define KB_ENTER -6
#define KB_DELETE -7
#define KB_TAB 9
#define KB_BACKSPACE 127

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include <stdio.h>

/**
 *
 */
#define RUN_OR_RING(expr) if(expr) printf(RING);

/**
 *
 */
#define IF_NOT_THEN_RING(expr) if(!(expr)) printf(RING);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 *
 *
 */
#define REDIRECT_NORMAL_0 "<"
#define REDIRECT_NORMAL_1 ">"
#define REDIRECT_NORMAL_2 "2>"
#define REDIRECT_APPEND_1 ">>"
#define REDIRECT_APPEND_2 "2>>"
#define REDIRECT_FUSION_2 "2>&1"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
void initTerminal(void);

/**
 *
 */
void restoreTerminal(void);

/**
 *
 */
int readKeyboard(void);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#endif
