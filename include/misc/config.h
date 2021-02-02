#ifndef CONFIG_H

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

#define CONFIG_H

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
#define VERSION "0.9.1"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
typedef enum {

    //
    QUIET,

    //
    DISCREET,

    //
    VERBOSE

} Verbosity;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
typedef enum {

    //
    COMMAND,

    //
    SCRIPT,

    //
    GUESS,

    //
    INTERACTIVE,

    //
    PIPE,

    //
    MANPAGE

} Mode;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include <unistd.h>
#include "interactive/history.h"

/**
 *
 */
struct __config__ {

    //
    pid_t pid;

    //
    const char* exec;

    //
    const char* banner;

    //
    const char *manpage;

    //
    History *history;

    //
    Verbosity verbosity;

    //
    Mode mode;

    //
    int unsecuredTests;

    //
    unsigned int watchdog;

};

/**
 *
 */
extern struct __config__ Configuration;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
#define HISTORY_FILE "~/.polyshell_history"

/**
 *
 */
#define HISTORY_LENGTH 32

/**
 *
 */
void oHistory(const char *filename);

/**
 *
 */
void cHistory(const char *filename);

/**
 *
 */
int prepareInteractiveMode(void);

/**
 *
 */
void setOutput(const char *directory);

/**
 *
 */
const char* getOutput();

/**
 *
 */
char* getOutputPath(const char *filename);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#endif
