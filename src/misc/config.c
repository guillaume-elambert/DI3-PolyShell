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

#include "misc/config.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include <errno.h>
#include <assert.h>
#include <sys/stat.h>
#include "misc/registry.h"
#include "misc/string.h"
#include "misc/ferror.h"
#include "interactive/terminal.h"

// #########################################################################
// #########################################################################
// #########################################################################

//
#define MKLOGO(str) SET_POLYTECH_STYLE str RESET_ALL

struct __config__ Configuration = {

    // shell pid
    -1,

    // shell exec
    NULL,

    // banner
    "         "MKLOGO(",ppKKhPPbKKpw,")"\n"
    "     "MKLOGO(",pKPPPPPPPPPPPPPPPPKw")"\n"
    "   "MKLOGO(",0PPPPPPPPPPPPPPPPPPPPPPb")"\n"
    "  "MKLOGO("#PPPPPPPM^")"       "MKLOGO("`\"TPPPPPPPp")"\n"
    " "MKLOGO("pPPPPPP\"")"              "MKLOGO("TPPPPPPb")"\n"
    MKLOGO("iPPPPPK")"                 "MKLOGO("`PPPPPPL")"\n"
    "                         "MKLOGO("!PPPPPP")"\n"
    "                          "MKLOGO("PPPPPP")"\n"
    "                         "MKLOGO("{PPPPPK")"\n"
    MKLOGO("PPPPPPPp")"                "MKLOGO("/PPPPPP")"\n"
    MKLOGO("PPPPPPPPb")"             "MKLOGO(",0PPPPPP\"")"  _          _ _\n"
    MKLOGO("PPPPPPPPPPKpw,")"   "MKLOGO(",,wpbPPPPPPK`")"  | |        | | |\n"
    MKLOGO("PPPPPPPPPPPPPPPPPPPPPPPPPPK\"")" ___| |__   ___| | |\n"
    MKLOGO("TPPPPP`*KPPPPPPPPPPPPPPK*`")"  / __| '_ \\ / _ \\ | |\n"
    " "MKLOGO("TPPPP")"    "MKLOGO("`\"\"*PPPP*\"^`")"      \\__ \\ | | |  __/ | |\n"
    "   "MKLOGO("`\"\"")"                      |___/_| |_|\\___|_|_|    (v"VERSION")\n\n",

    // manpage
    "Usage: polyshell [OPTIONS]...\n"
    "Run your dear home-made Unix shell.\n"
    "---------------------------------- options ----------------------------------\n"
    "-----------------------------------------------------------------------------\n"
    "      --names              display your names   and exit\n"
    "  -h  --help               display this message and exit\n"
    "  -q  --quiet              set the verbosity level to quiet\n"
    "  -d  --discreet           set the verbosity level to discreet\n"
    "  -v  --verbose            set the verbosity level to verbose\n"
    "      --provided-<M>\n"
    "      --provided-<M>-<FN>  enforce the usage of all or a given provided\n"
    "                           function <FN> in module <M>; with:\n"
    "                           <M> =    interactive\n"
    "                                 or misc\n"
    "                                 or parser\n"
    "                                 or system\n"
    "      --config-<P>         enforce the usage of all the provided functions\n"
    "                           that are NOT part of <P>; with:\n"
    "                           <P> =    projet-c\n"
    "                                 or unix\n"
    "                                 or compilation\n"
    "  -r  --seed=SEED          set the SEED for random generators\n"
    "  -u  --unit-tests         run the unit tests before executing any command\n"
    "      --unsecured-tests    do not run these tests in a child process\n"
    "  -o  --output=DIRECTORY   set the DIRECTORY where outputs will be written\n"
    "                           to (this will be created if necessary)\n"
    "--------------------------------- interactive -------------------------------\n"
    "-----------------------------------------------------------------------------\n"
    "  -i  --interactive        enforce that the shell is interactive\n"
    "  -b  --banner             print a nice banner before displaying the prompt\n"
    "------------------------------- non-interactive -----------------------------\n"
    "-----------------------------------------------------------------------------\n"
    "  -c  --command=STRING     execute the command read from STRING and exit\n"
    "  -s  --pipe               execute the script  read from  STDIN and exit\n"
    "  -f  --script=FILE        execute the script  read from   FILE and exit\n"
    "-----------------------------------------------------------------------------\n"
    "-----------------------------------------------------------------------------\n"
    "If neither -i nor -c, -s or -f is set, the shell is interactive if both\n"
    "STDIN and STDOUT refer to a terminal. The commands will be read from STDIN\n"
    "otherwise (--pipe).\n",

    // history
    NULL,

    // verbosity
    DISCREET,

    // mode
    GUESS,

    // unsecuredTests
    0,

    // watchdog
    0

};

// #########################################################################
// #########################################################################
// #########################################################################

void oHistory(const char *filename) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    History_delete(Configuration.history, NULL);
    Configuration.history = History_new(filename, HISTORY_LENGTH);
}

void cHistory(const char *filename) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    History_delete(Configuration.history, filename);
    Configuration.history = NULL;
}

static void __quit__(void) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    if(    getpid()
        == Configuration.pid ) { showRegister();
                                 cHistory(HISTORY_FILE); }
}

int prepareInteractiveMode(void) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    // check that the input and the output are both tty
    if(isatty(0)  ==  0
    || isatty(1)  ==  0) {
        fprintf(stderr, "prepareInteractiveMode: interactive mode with a non-tty input or a non-tty output.\n");
        return 1;
    }
    // clean exit
    if(atexit(__quit__)) {
        fprintf(stderr, "prepareInteractiveMode: atexit failed.\n");
        return 1;
    }
    // history
    oHistory(HISTORY_FILE);
    // prepareInteractiveMode has run successfully
    return 0;
}

// #########################################################################
// ################## shall not use overridable functions ##################
// #########################################################################

//
static const char *__dir__ = NULL;

void setOutput(const char *directory) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    __dir__ = directory;
}

const char* getOutput() {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    return __dir__;
}

char* getOutputPath(const char *filename) {
// -------------------------------- asserts --------------------------------
// -------------------------------------------------------------------------
    assert(filename);
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    if(__dir__)
    {
        if( !mkdir(__dir__,   S_IRWXU
                            | S_IRWXG
                            | S_IROTH
                            | S_IXOTH ) || errno == EEXIST )
        {
            char *result = malloc(   stringLength(filename)
                                   + stringLength(__dir__ ) + 2U );
            if(result)
            {
                size_t k = 0U; for(const char *copy =  __dir__; *copy; ++copy) { result[k++] = *copy; }
                                                                               { result[k++] =  '/' ; }
                               for(const char *copy = filename; *copy; ++copy) { result[k++] = *copy; }
                                                                               { result[k++] =   0  ; } return result;
            } }
        fatalError("getOutputPath has failed");
    }
    return NULL;
}
