#ifndef COMMAND_H

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

#define COMMAND_H

//
#include "misc/new.h"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include <stdio.h>

/**
 *
 */
int run(const char *command);

/**
 *
 */
int script(FILE *file);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
#define INTERNAL_WATCH "watchdog"

/**
 *
 */
#define INTERNAL_CHDIR "cd"

/**
 *
 */
#define INTERNAL_NEXIT "exit"

/**
 *
 */
#define INTERNAL_CEXIT "clear_exit"

/**
 *
 */
#define INTERNAL_FHIST "history"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

typedef enum {

    //
    NORMAL,

    //
    APPEND,

    //
    FUSION,

    //
    UNDEFINED

} RedirectionType;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
typedef struct __CmdMember__ {

    //
    unsigned int status;

    //
    char* base;

    //
    char** options;

    //
    unsigned int nbOptions;

    //
    unsigned int capacityOption;

    //
    char* redirections[3];

    //
    RedirectionType redirectionTypes[3];

    //
    struct __CmdMember__ *next;

    //
    struct __CmdMember__ *prev;

} CmdMember;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
extern CmdMember** livingCmdMembers;

/**
 *
 */
extern unsigned int livingCmdMembersSize;

/**
 *
 */
extern unsigned int livingCmdMembersCapacity;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
LIB CmdMember* OVERRIDABLE(CmdMember_new)(const char *base);

/**
 *
 */
LIB int OVERRIDABLE(CmdMember_init)(CmdMember *mbr, const char *base);

/**
 *
 */
LIB void OVERRIDABLE(CmdMember_delete)(CmdMember *mbr);

/**
 *
 */
LIB void OVERRIDABLE(CmdMember_finalize)(CmdMember *mbr);

/**
 *
 */
int CmdMember_addLivingCmdMember(CmdMember *mbr);

/**
 *
 */
void CmdMember_flushLivingCmdMembers(void);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
LIB CmdMember* OVERRIDABLE(CmdMember_redirect)(CmdMember *mbr, int fd, const char *filename);

/**
 *
 */
LIB CmdMember* OVERRIDABLE(CmdMember_appendRedirect)(CmdMember *mbr, int fd, const char *filename);

/**
 *
 */
LIB CmdMember* OVERRIDABLE(CmdMember_mergeOutputs)(CmdMember *mbr);

/**
 *
 */
LIB CmdMember* OVERRIDABLE(CmdMember_pipe)(CmdMember *m1, CmdMember *m2);

/**
 *
 */
LIB CmdMember* OVERRIDABLE(CmdMember_addOption)(CmdMember *mbr, const char *arg, int expend);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
typedef CmdMember Command;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
LIB int OVERRIDABLE(Command_execute)(Command *cmd);

/**
 *
 */
int Command_unparse(const Command *cmd);

/**
 *
 */
LIB size_t OVERRIDABLE(Command_getNbMember)(const Command *cmd);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

//
#include "impl/command.h"

#endif
