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

#include <getopt.h>
#include "interactive/input.h"
#include "interactive/header.h"
#include "system/command.h"
#include "misc/config.h"
#include "misc/overridable.h"
#include "misc/registry.h"
#include "test.h"

// #########################################################################
// #########################################################################
// #########################################################################

//
#define PROMPT_MAX_SIZE 128

//
extern const char *name1;
extern const char *name2;

// #########################################################################
// #########################################################################
// #########################################################################

static void __autoconfigure__(int unix, int compilation, int projet)
{

// -------------------------------------------------------------------------
// --------------------------------- UNIX ----------------------------------
// -------------------------------------------------------------------------

    if(unix) {
        OverridingOptions.interactive                     = 0;
        OverridingOptions.misc                            = 0;
        OverridingOptions.parser                          = 0;
        OverridingOptions.system_Command_getNbMember      = 0;
        OverridingOptions.system_CmdMember_new            = 0;
        OverridingOptions.system_CmdMember_init           = 0;
        OverridingOptions.system_CmdMember_delete         = 0;
        OverridingOptions.system_CmdMember_finalize       = 0;
        OverridingOptions.system_CmdMember_redirect       = 0;
        OverridingOptions.system_CmdMember_appendRedirect = 0;
        OverridingOptions.system_CmdMember_mergeOutputs   = 0;
        OverridingOptions.system_CmdMember_pipe           = 0;
        OverridingOptions.system_CmdMember_addOption      = 0;
    }

// -------------------------------------------------------------------------
// ------------------------------ COMPILATION ------------------------------
// -------------------------------------------------------------------------

    if(compilation) {
        OverridingOptions.interactive = 0;
        OverridingOptions.misc        = 0;
        OverridingOptions.system      = 0;
    }

// -------------------------------------------------------------------------
// ----------------------------------- C -----------------------------------
// -------------------------------------------------------------------------

    if(projet) {
        OverridingOptions.parser                     = 0;
        OverridingOptions.system_Command_execute     = 0;
        OverridingOptions.system_getCurrentDirectory = 0;
        OverridingOptions.system_getPATHVar          = 0;
        OverridingOptions.system_hostname            = 0;
        OverridingOptions.system_isRoot              = 0;
        OverridingOptions.system_userInformation     = 0;
    }

}

int main(int argc, char **argv)
{

// -------------------------------------------------------------------------
// ---------------------------- READ ARGUMENTS -----------------------------
// -------------------------------------------------------------------------

    Configuration.pid  = getpid();
    Configuration.exec = realpath(argv[0], NULL);

    if(!Configuration.exec) { fprintf(stderr, "realpath has failed.\n");
                              return 1; }

    int result = 0, banner = 0, unitTests = 0, printNames = 0, unix = 0, compilation = 0, projet = 0;
    const char *arg = NULL; int seed = 1;

    const struct option options[] = {
        { "discreet", no_argument, NULL, 'd' },
        { "quiet", no_argument, NULL, 'q' },
        { "verbose", no_argument, NULL, 'v' },
        { "banner", no_argument, NULL, 'b' },
        { "help", no_argument, NULL, 'h' },
        { "interactive", no_argument, NULL, 'i' },
        { "pipe", no_argument, NULL, 's' },
        { "script", required_argument, NULL, 'f' },
        { "command", required_argument, NULL, 'c' },
        { "unit-tests", no_argument, NULL, 'u' },
        { "output", required_argument, NULL, 'o' },
        { "seed", required_argument, NULL, 'r' },
        { "names", no_argument, &printNames, 1 },
        { "unsecured-tests", no_argument, &Configuration.unsecuredTests, 1 },
        { "config-unix", no_argument, &unix, 1 },
        { "config-compilation", no_argument, &compilation, 1 },
        { "config-projet-c", no_argument, &projet, 1 },
        { "provided-interactive", no_argument, &OverridingOptions.interactive, 0 },
        { "provided-interactive-autocomplete", no_argument, &OverridingOptions.interactive_autocomplete, 0 },
        { "provided-interactive-prependHomeDir", no_argument, &OverridingOptions.interactive_prependHomeDir, 0 },
        { "provided-interactive-Bucket_new", no_argument, &OverridingOptions.interactive_Bucket_new, 0 },
        { "provided-interactive-Bucket_init", no_argument, &OverridingOptions.interactive_Bucket_init, 0 },
        { "provided-interactive-Bucket_delete", no_argument, &OverridingOptions.interactive_Bucket_delete, 0 },
        { "provided-interactive-Bucket_finalize", no_argument, &OverridingOptions.interactive_Bucket_finalize, 0 },
        { "provided-interactive-Bucket_size", no_argument, &OverridingOptions.interactive_Bucket_size, 0 },
        { "provided-interactive-Bucket_remove", no_argument, &OverridingOptions.interactive_Bucket_remove, 0 },
        { "provided-interactive-Bucket_insert", no_argument, &OverridingOptions.interactive_Bucket_insert, 0 },
        { "provided-interactive-Bucket_move", no_argument, &OverridingOptions.interactive_Bucket_move, 0 },
        { "provided-interactive-Bucket_empty", no_argument, &OverridingOptions.interactive_Bucket_empty, 0 },
        { "provided-interactive-Bucket_full", no_argument, &OverridingOptions.interactive_Bucket_full, 0 },
        { "provided-interactive-Cell_new", no_argument, &OverridingOptions.interactive_Cell_new, 0 },
        { "provided-interactive-Cell_init", no_argument, &OverridingOptions.interactive_Cell_init, 0 },
        { "provided-interactive-Cell_delete", no_argument, &OverridingOptions.interactive_Cell_delete, 0 },
        { "provided-interactive-Cell_finalize", no_argument, &OverridingOptions.interactive_Cell_finalize, 0 },
        { "provided-interactive-Cell_insertAfter", no_argument, &OverridingOptions.interactive_Cell_insertAfter, 0 },
        { "provided-interactive-History_new", no_argument, &OverridingOptions.interactive_History_new, 0 },
        { "provided-interactive-History_init", no_argument, &OverridingOptions.interactive_History_init, 0 },
        { "provided-interactive-History_delete", no_argument, &OverridingOptions.interactive_History_delete, 0 },
        { "provided-interactive-History_finalize", no_argument, &OverridingOptions.interactive_History_finalize, 0 },
        { "provided-interactive-History_clear", no_argument, &OverridingOptions.interactive_History_clear, 0 },
        { "provided-interactive-History_add", no_argument, &OverridingOptions.interactive_History_add, 0 },
        { "provided-interactive-History_up", no_argument, &OverridingOptions.interactive_History_up, 0 },
        { "provided-interactive-History_down", no_argument, &OverridingOptions.interactive_History_down, 0 },
        { "provided-interactive-Input_new", no_argument, &OverridingOptions.interactive_Input_new, 0 },
        { "provided-interactive-Input_init", no_argument, &OverridingOptions.interactive_Input_init, 0 },
        { "provided-interactive-Input_delete", no_argument, &OverridingOptions.interactive_Input_delete, 0 },
        { "provided-interactive-Input_finalize", no_argument, &OverridingOptions.interactive_Input_finalize, 0 },
        { "provided-interactive-Input_clear", no_argument, &OverridingOptions.interactive_Input_clear, 0 },
        { "provided-interactive-Input_size", no_argument, &OverridingOptions.interactive_Input_size, 0 },
        { "provided-interactive-Input_get", no_argument, &OverridingOptions.interactive_Input_get, 0 },
        { "provided-interactive-Input_insert", no_argument, &OverridingOptions.interactive_Input_insert, 0 },
        { "provided-interactive-Input_backspace", no_argument, &OverridingOptions.interactive_Input_backspace, 0 },
        { "provided-interactive-Input_del", no_argument, &OverridingOptions.interactive_Input_del, 0 },
        { "provided-interactive-Input_moveLeft", no_argument, &OverridingOptions.interactive_Input_moveLeft, 0 },
        { "provided-interactive-Input_moveRight", no_argument, &OverridingOptions.interactive_Input_moveRight, 0 },
        { "provided-interactive-Input_toString", no_argument, &OverridingOptions.interactive_Input_toString, 0 },
        { "provided-interactive-Input_load", no_argument, &OverridingOptions.interactive_Input_load, 0 },
        { "provided-interactive-Input_getEditedWord", no_argument, &OverridingOptions.interactive_Input_getEditedWord, 0 },
        { "provided-interactive-InputIterator_initIterator", no_argument, &OverridingOptions.interactive_InputIterator_initIterator, 0 },
        { "provided-interactive-InputIterator_equals", no_argument, &OverridingOptions.interactive_InputIterator_equals, 0 },
        { "provided-interactive-InputIterator_next", no_argument, &OverridingOptions.interactive_InputIterator_next, 0 },
        { "provided-interactive-InputIterator_isOver", no_argument, &OverridingOptions.interactive_InputIterator_isOver, 0 },
        { "provided-interactive-InputIterator_get", no_argument, &OverridingOptions.interactive_InputIterator_get, 0 },
        { "provided-parser", no_argument, &OverridingOptions.parser, 0 },
        { "provided-system", no_argument, &OverridingOptions.system, 0 },
        { "provided-system-CmdMember_new", no_argument, &OverridingOptions.system_CmdMember_new, 0 },
        { "provided-system-CmdMember_init", no_argument, &OverridingOptions.system_CmdMember_init, 0 },
        { "provided-system-CmdMember_delete", no_argument, &OverridingOptions.system_CmdMember_delete, 0 },
        { "provided-system-CmdMember_finalize", no_argument, &OverridingOptions.system_CmdMember_finalize, 0 },
        { "provided-system-CmdMember_redirect", no_argument, &OverridingOptions.system_CmdMember_redirect, 0 },
        { "provided-system-CmdMember_appendRedirect", no_argument, &OverridingOptions.system_CmdMember_appendRedirect, 0 },
        { "provided-system-CmdMember_mergeOutputs", no_argument, &OverridingOptions.system_CmdMember_mergeOutputs, 0 },
        { "provided-system-CmdMember_pipe", no_argument, &OverridingOptions.system_CmdMember_pipe, 0 },
        { "provided-system-CmdMember_addOption", no_argument, &OverridingOptions.system_CmdMember_addOption, 0 },
        { "provided-system-Command_execute", no_argument, &OverridingOptions.system_Command_execute, 0 },
        { "provided-system-Command_getNbMember", no_argument, &OverridingOptions.system_Command_getNbMember, 0 },
        { "provided-system-isRoot", no_argument, &OverridingOptions.system_isRoot, 0 },
        { "provided-system-getPATHVar", no_argument, &OverridingOptions.system_getPATHVar, 0 },
        { "provided-system-hostname", no_argument, &OverridingOptions.system_hostname, 0 },
        { "provided-system-userInformation", no_argument, &OverridingOptions.system_userInformation, 0 },
        { "provided-system-getCurrentDirectory", no_argument, &OverridingOptions.system_getCurrentDirectory, 0 },
        { "provided-misc", no_argument, &OverridingOptions.misc, 0 },
        { "provided-misc-Fifo_new", no_argument, &OverridingOptions.misc_Fifo_new, 0 },
        { "provided-misc-Fifo_init", no_argument, &OverridingOptions.misc_Fifo_init, 0 },
        { "provided-misc-Fifo_delete", no_argument, &OverridingOptions.misc_Fifo_delete, 0 },
        { "provided-misc-Fifo_finalize", no_argument, &OverridingOptions.misc_Fifo_finalize, 0 },
        { "provided-misc-Fifo_clear", no_argument, &OverridingOptions.misc_Fifo_clear, 0 },
        { "provided-misc-Fifo_front", no_argument, &OverridingOptions.misc_Fifo_front, 0 },
        { "provided-misc-Fifo_pop", no_argument, &OverridingOptions.misc_Fifo_pop, 0 },
        { "provided-misc-Fifo_push", no_argument, &OverridingOptions.misc_Fifo_push, 0 },
        { "provided-misc-Fifo_full", no_argument, &OverridingOptions.misc_Fifo_full, 0 },
        { "provided-misc-Fifo_empty", no_argument, &OverridingOptions.misc_Fifo_empty, 0 },
        { "provided-misc-FolderIterator_new", no_argument, &OverridingOptions.misc_FolderIterator_new, 0 },
        { "provided-misc-FolderIterator_init", no_argument, &OverridingOptions.misc_FolderIterator_init, 0 },
        { "provided-misc-FolderIterator_delete", no_argument, &OverridingOptions.misc_FolderIterator_delete, 0 },
        { "provided-misc-FolderIterator_finalize", no_argument, &OverridingOptions.misc_FolderIterator_finalize, 0 },
        { "provided-misc-FolderIterator_isOver", no_argument, &OverridingOptions.misc_FolderIterator_isOver, 0 },
        { "provided-misc-FolderIterator_get", no_argument, &OverridingOptions.misc_FolderIterator_get, 0 },
        { "provided-misc-FolderIterator_isDir", no_argument, &OverridingOptions.misc_FolderIterator_isDir, 0 },
        { "provided-misc-FolderIterator_next", no_argument, &OverridingOptions.misc_FolderIterator_next, 0 },
        { "provided-misc-FileIterator_new", no_argument, &OverridingOptions.misc_FileIterator_new, 0 },
        { "provided-misc-FileIterator_init", no_argument, &OverridingOptions.misc_FileIterator_init, 0 },
        { "provided-misc-FileIterator_delete", no_argument, &OverridingOptions.misc_FileIterator_delete, 0 },
        { "provided-misc-FileIterator_finalize", no_argument, &OverridingOptions.misc_FileIterator_finalize, 0 },
        { "provided-misc-FileIterator_isOver", no_argument, &OverridingOptions.misc_FileIterator_isOver, 0 },
        { "provided-misc-FileIterator_get", no_argument, &OverridingOptions.misc_FileIterator_get, 0 },
        { "provided-misc-FileIterator_next", no_argument, &OverridingOptions.misc_FileIterator_next, 0 },
        { "provided-misc-Pattern_new", no_argument, &OverridingOptions.misc_Pattern_new, 0 },
        { "provided-misc-Pattern_init", no_argument, &OverridingOptions.misc_Pattern_init, 0 },
        { "provided-misc-Pattern_delete", no_argument, &OverridingOptions.misc_Pattern_delete, 0 },
        { "provided-misc-Pattern_finalize", no_argument, &OverridingOptions.misc_Pattern_finalize, 0 },
        { "provided-misc-Pattern_match", no_argument, &OverridingOptions.misc_Pattern_match, 0 },
        { "provided-misc-toLowerCase", no_argument, &OverridingOptions.misc_toLowerCase, 0 },
        { "provided-misc-toUpperCase", no_argument, &OverridingOptions.misc_toUpperCase, 0 },
        { "provided-misc-indexOfString", no_argument, &OverridingOptions.misc_indexOfString, 0 },
        { "provided-misc-stringLength", no_argument, &OverridingOptions.misc_stringLength, 0 },
        { "provided-misc-duplicateString", no_argument, &OverridingOptions.misc_duplicateString, 0 },
        { "provided-misc-findFirst", no_argument, &OverridingOptions.misc_findFirst, 0 },
        { "provided-misc-findLast", no_argument, &OverridingOptions.misc_findLast, 0 },
        { "provided-misc-stringCompare", no_argument, &OverridingOptions.misc_stringCompare, 0 },
        { "provided-misc-concatenateStrings", no_argument, &OverridingOptions.misc_concatenateStrings, 0 },
        { "provided-misc-copyStringWithLength", no_argument, &OverridingOptions.misc_copyStringWithLength, 0 },
        { "provided-misc-mkReverse", no_argument, &OverridingOptions.misc_mkReverse, 0 },
        { "provided-misc-startWith", no_argument, &OverridingOptions.misc_startWith, 0 },
        { "provided-misc-belongs", no_argument, &OverridingOptions.misc_belongs, 0 },
        { "provided-misc-subString", no_argument, &OverridingOptions.misc_subString, 0 },
        { "provided-misc-mkCommon", no_argument, &OverridingOptions.misc_mkCommon, 0 },
        { "provided-misc-isNotEmpty", no_argument, &OverridingOptions.misc_isNotEmpty, 0 },
        { "provided-misc-getProtString", no_argument, &OverridingOptions.misc_getProtString, 0 },
        { "provided-misc-getRealString", no_argument, &OverridingOptions.misc_getRealString, 0 },
        { "provided-misc-Tokenizer_new", no_argument, &OverridingOptions.misc_Tokenizer_new, 0 },
        { "provided-misc-Tokenizer_init", no_argument, &OverridingOptions.misc_Tokenizer_init, 0 },
        { "provided-misc-Tokenizer_delete", no_argument, &OverridingOptions.misc_Tokenizer_delete, 0 },
        { "provided-misc-Tokenizer_finalize", no_argument, &OverridingOptions.misc_Tokenizer_finalize, 0 },
        { "provided-misc-Tokenizer_isOver", no_argument, &OverridingOptions.misc_Tokenizer_isOver, 0 },
        { "provided-misc-Tokenizer_get", no_argument, &OverridingOptions.misc_Tokenizer_get, 0 },
        { "provided-misc-Tokenizer_next", no_argument, &OverridingOptions.misc_Tokenizer_next, 0 },
        { 0, 0, 0, 0 }
    };

    int loopIsOver = 0; while(!loopIsOver) {

        int c = getopt_long(argc, argv, "qdvbhisc:f:uo:r:", options, NULL);
        switch(c) {

            case -1:
                loopIsOver = 1;
                break;

            case 'q':
                Configuration.verbosity = QUIET;
                break;

            case 'd':
                Configuration.verbosity = DISCREET;
                break;

            case 'v':
                Configuration.verbosity = VERBOSE;
                break;

            case 'b':
                banner = 1;
                break;

            case 'h':
                Configuration.mode = MANPAGE;
                loopIsOver = 1;
                break;

            case 'i':
                Configuration.mode = INTERACTIVE;
                break;

            case 's':
                Configuration.mode = PIPE;
                break;

            case 'f':
                Configuration.mode = SCRIPT;
                arg = optarg;
                break;

            case 'c':
                Configuration.mode = COMMAND;
                arg = optarg;
                break;

            case 'u':
                unitTests = 1;
                break;

            case 'o':
                setOutput(optarg);
                break;

            case 'r':
                seed = atoi(optarg);
                break;

            case 0:
                break;

            case '?':
                Configuration.mode = MANPAGE;
                loopIsOver = result = 1;
                break;

            default:
                fprintf(stderr, "getopt_long has failed.\n");
                return 1;

        } }

    //
    srand((unsigned)seed);

    //
    __autoconfigure__(unix, compilation, projet);

    //
    initOverridable(); initRegistry();

// -------------------------------------------------------------------------
// --------------------------------- NAMES ---------------------------------
// -------------------------------------------------------------------------

    if( printNames
        == 1 )
    {

        printf("%s&%s", name1, name2);
        return 0;

    }

// -------------------------------------------------------------------------
// -------------------------------- MANPAGE --------------------------------
// -------------------------------------------------------------------------

    if( Configuration.mode
        == MANPAGE )
    {

        printf("%s", Configuration.manpage);
        return result;

    }

// -------------------------------------------------------------------------
// --------------------------------- TESTS ---------------------------------
// -------------------------------------------------------------------------

    if( unitTests
        == 1 )
    {

        test();

    }

// -------------------------------------------------------------------------
// -------------------------------- SCRIPT ---------------------------------
// -------------------------------------------------------------------------

    if( Configuration.mode
        == SCRIPT )
    {

        FILE *file = fopen(arg, "r");
        if(!file) { fprintf(stderr, "main: fopen: %s\n", optarg);
                    result = 1;                                     }
        else      { result |= script(file);
                    fclose(file);                                   }

    }

// -------------------------------------------------------------------------
// -------------------------------- COMMAND --------------------------------
// -------------------------------------------------------------------------

    if( Configuration.mode
        == COMMAND )
    {

        result |= run(arg);

    }

// -------------------------------------------------------------------------
// --------------------------------- GUESS ---------------------------------
// -------------------------------------------------------------------------

    if( Configuration.mode
        == GUESS )
    {

        Configuration.mode = isatty(0) && isatty(1) ? INTERACTIVE
                                                    : PIPE;

    }

// -------------------------------------------------------------------------
// ------------------------------ INTERACTIVE ------------------------------
// -------------------------------------------------------------------------

    if( Configuration.mode
        == INTERACTIVE )
    {

        char prompt[PROMPT_MAX_SIZE];

        if(prepareInteractiveMode()) {
            fprintf(stderr, "main: prepareInteractiveMode failed.\n");
            return 1;
        }

        if(banner) {
            printf("%s", Configuration.banner);
        }

        while(1) {

            if(header(prompt, PROMPT_MAX_SIZE)) {
                fprintf(stderr, "main: prompHeader failed.\n");
                return 1;
            }

            char *input = readLine(prompt);
            run(input); free(input);

        } }

// -------------------------------------------------------------------------
// --------------------------------- PIPE ----------------------------------
// -------------------------------------------------------------------------

    if( Configuration.mode
        == PIPE )
    {

        result |= script(stdin);

    }

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

    return result;

}

// #########################################################################
// #########################################################################
// #########################################################################

/**
 * \mainpage About the project
 *
 * See the [dokuwiki](https://teaching.bocqfamily.fr/c/wiki/) for more details.
 *****/

// #########################################################################
// #########################################################################
// #########################################################################

/**
 * \page    Iterator
 * \brief   About the Iterator pattern.
 *
 * An iterator is any data structure that, referencing some element in a range of elements (such as an
 * array or a container), has the ability to iterate through the elements of that range using a set of operators.
 *
 * This concept originates from the well-known software design paradigm called object-oriented design.
 *
 * See [https://en.wikipedia.org/](https://en.wikipedia.org/wiki/Iterator_pattern)
 * and [http://www.cplusplus.com/](http://www.cplusplus.com/reference/iterator/) for more details.
 *****/
