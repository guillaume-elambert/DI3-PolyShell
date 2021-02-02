#ifndef TEST_SEGFAULT_H

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

#define TEST_SEGFAULT_H

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>

//
extern jmp_buf __env__;

//
extern void __restore__(int sig);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
#define use_segfault_guard                                                                  \
    int sig; struct sigaction sa; { sigemptyset(&sa.sa_mask); sa.sa_handler=__restore__;    \
                                    sa.sa_flags=SA_NODEFER|SA_RESETHAND; }                  \
    sigaction(SIGSEGV,&sa,NULL); /* segmentation fault */                                   \
    sigaction(SIGABRT,&sa,NULL); /* assert             */                                   \
    ck_assert_msg(!(sig=setjmp(__env__)),strsignal(sig));                                   \
    {                                                                                       \

/**
 *
 */
#define end_segfault_guard                                                                  \
    }                                                                                       \
    signal(SIGSEGV,SIG_DFL);                                                                \
    signal(SIGABRT,SIG_DFL);                                                                \

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#endif
