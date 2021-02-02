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

#include "system/command.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include <assert.h>
#include <string.h>
#include "interactive/terminal.h"
#include "misc/config.h"
#include "misc/string.h"
#include "misc/filesystem.h"
#include "parser/parser.h"

// #########################################################################
// #########################################################################
// #########################################################################

//
static int __prepare_add_living_command__(void);

//
CmdMember** livingCmdMembers = NULL;

//
unsigned int livingCmdMembersSize = 0;

//
unsigned int livingCmdMembersCapacity = 0;

// #########################################################################
// #########################################################################
// #########################################################################

static int __prepare_add_living_command__(void) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    if(    livingCmdMembersSize
        >= livingCmdMembersCapacity )
    {
        // the new capacity is doubled
        unsigned int ncapacity = livingCmdMembersCapacity ? livingCmdMembersCapacity * 2
                                                          : 16; // should quite always be enough
        // a new array is allocated
        CmdMember **narray = malloc(   sizeof(CmdMember*)
                                     * ncapacity );
        if (!narray) return 1;
        // the content of the old array is copied to the new one
        for(unsigned i = 0; i < livingCmdMembersSize; i += 1)
                narray[i] = livingCmdMembers[i];
        // the modifications are applied
        if (livingCmdMembers) free(livingCmdMembers);
        livingCmdMembers = narray;
        livingCmdMembersCapacity = ncapacity;
    }
    return 0;
}

// #########################################################################
// #########################################################################
// #########################################################################

void CmdMember_flushLivingCmdMembers(void) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    for(unsigned int i = 0; i < livingCmdMembersSize; i += 1)
            CmdMember_delete(livingCmdMembers[i]);
    livingCmdMembersSize = 0;
}

int CmdMember_addLivingCmdMember(CmdMember *ncommand) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    if(__prepare_add_living_command__())
            return 1;
    livingCmdMembers[livingCmdMembersSize++] = ncommand;
    return 0;
}

int Command_unparse(const Command *cmd) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    while(cmd)
    {
        // pipes
        if (cmd->prev) printf(SPACE""SET_OPERATOR_STYLE"|"RESET_ALL""SPACE);
        // command
        printf(SET_EXECUTABLE_STYLE"%s"RESET_ALL, cmd->base);
        // options
        for(unsigned int iOpt = 1; iOpt < cmd->nbOptions; ++iOpt)
            printf(SPACE""SET_ARGS_STYLE"%s"RESET_ALL, cmd->options[iOpt]);
        // redirections
        if(cmd->redirectionTypes[0] == NORMAL) printf(SPACE""SET_OPERATOR_STYLE""REDIRECT_NORMAL_0""RESET_ALL""SPACE""SET_FILE_STYLE"%s"RESET_ALL, cmd->redirections[0]);
        if(cmd->redirectionTypes[1] == NORMAL) printf(SPACE""SET_OPERATOR_STYLE""REDIRECT_NORMAL_1""RESET_ALL""SPACE""SET_FILE_STYLE"%s"RESET_ALL, cmd->redirections[1]);
        if(cmd->redirectionTypes[2] == NORMAL) printf(SPACE""SET_OPERATOR_STYLE""REDIRECT_NORMAL_2""RESET_ALL""SPACE""SET_FILE_STYLE"%s"RESET_ALL, cmd->redirections[2]);
        if(cmd->redirectionTypes[1] == APPEND) printf(SPACE""SET_OPERATOR_STYLE""REDIRECT_APPEND_1""RESET_ALL""SPACE""SET_FILE_STYLE"%s"RESET_ALL, cmd->redirections[1]);
        if(cmd->redirectionTypes[2] == APPEND) printf(SPACE""SET_OPERATOR_STYLE""REDIRECT_APPEND_2""RESET_ALL""SPACE""SET_FILE_STYLE"%s"RESET_ALL, cmd->redirections[2]);
        if(cmd->redirectionTypes[2] == FUSION) printf(SPACE""SET_OPERATOR_STYLE""REDIRECT_FUSION_2""RESET_ALL                                                          );
        // iterate
        cmd = cmd->next;
    }
    printf("\n");
    return 0;
}

int run(const char *input) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    int r = 0;
    if( input && strcmp(input,"__nope__")
              && isNotEmpty(input) )
    {
        Command *cmd = NULL;
        r =    ( Command_parse(input, &cmd)                                 )
            || ( Configuration.verbosity == VERBOSE && Command_unparse(cmd) )
            || ( Command_execute(cmd)                                       );
        CmdMember_flushLivingCmdMembers();
    }
    return r;
}

int script(FILE *file) {
// -------------------------------- asserts --------------------------------
// -------------------------------------------------------------------------
    assert(file);
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    FileIterator it; if(FileIterator_init(&it, file)) {
        fprintf(stderr, "command: FileIterator_init failed.\n");
        return 1;
    }
    int result = 0;
    while(!FileIterator_isOver(&it)) {
        result |= run(FileIterator_get(&it));
        FileIterator_next(&it);
    }
    return result;
}
