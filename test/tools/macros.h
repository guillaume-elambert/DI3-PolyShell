#ifndef TEST_MACROS_H

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

#define TEST_MACROS_H

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

//
#include <check.h>

//
#include <linux/limits.h>

//
#include "misc/ferror.h"
#include "misc/string.h"
#include "misc/config.h"

//
#include "path/path.h"
#include "segfault/segfault.h"

// -------------------------------------------------------------------------
// ------------------------------- UNIT TEST -------------------------------
// -------------------------------------------------------------------------

#define START_UTEST(tname)                                              \
    START_TEST(tname)                                                   \
    {                                                                   \
        use_segfault_guard                                              \

#define END_UTEST                                                       \
        end_segfault_guard                                              \
    }                                                                   \
    END_TEST                                                            \

// -------------------------------------------------------------------------
// ------------------------------- TEST CASE -------------------------------
// -------------------------------------------------------------------------

#define START_CASE(cname)                                               \
    {                                                                   \
        TCase *c = tcase_create(#cname);                                \

#define END_CASE                                                        \
        suite_add_tcase(s,c);                                           \
    }                                                                   \

#define ADD_TEST(tname)                                                 \
    tcase_add_test(c,tname);                                            \

#define ONE_TEST_CASE(cname,tname)                                      \
    START_CASE(cname)                                                   \
    {                                                                   \
        ADD_TEST(tname)                                                 \
    }                                                                   \
    END_CASE                                                            \

// -------------------------------------------------------------------------
// --------------------------------- SUITE ---------------------------------
// -------------------------------------------------------------------------

#define DECLARE_SUITE_BUILDER(sname)                                    \
    Suite* b_ ## sname (void)                                           \

#define CALL_SUITE_BUILDER(sname)                                       \
    b_ ## sname ()                                                      \

#define START_SUITE_BUILDER(sname)                                      \
    Suite* b_ ## sname (void)                                           \
    {                                                                   \
        Suite *s = suite_create(#sname);                                \

#define END_SUITE_BUILDER                                               \
        return s;                                                       \
    }                                                                   \

// -------------------------------------------------------------------------
// ------------------------------- PROCEDURE -------------------------------
// -------------------------------------------------------------------------

#define DECLARE_TEST_PROCEDURE(name)                                    \
    int t_ ## name (enum print_output verbosity)                        \

#define CALL_TEST_PROCEDURE(name,verbosity)                             \
    t_ ## name (verbosity)                                              \

#define START_TEST_PROCEDURE(name)                                      \
    int t_ ## name (enum print_output verbosity)                        \
    {                                                                   \
        SRunner *sr = srunner_create(NULL);                             \
        static const char *lfile = NULL;                                \
        if (!lfile) lfile = getOutputPath(#name".log");                 \
        if ( lfile) srunner_set_log(sr,lfile);                          \
        static const char *xfile = NULL;                                \
        if (!xfile) xfile = getOutputPath(#name".xml");                 \
        if ( xfile) srunner_set_xml(sr,xfile);                          \
        if (Configuration.unsecuredTests)                               \
                        srunner_set_fork_status(sr,CK_NOFORK);          \

#define END_TEST_PROCEDURE                                              \
        char dir[PATH_MAX];                                             \
        if (getcwd(dir,PATH_MAX)==NULL) {                               \
            fatalError("duplicateString has failed");                   \
        }                                                               \
        useMask();                                                      \
        srunner_run_all(sr,verbosity);                                  \
        int nerr = srunner_ntests_failed(sr);                           \
        endMask();                                                      \
        if (chdir(dir)) {                                               \
            fatalError("chdir has failed");                             \
        }                                                               \
        srunner_free(sr);                                               \
        return nerr;                                                    \
    }                                                                   \

#define ADD_SUITE(sname)                                                \
    srunner_add_suite(sr,CALL_SUITE_BUILDER(sname));                    \

// -------------------------------------------------------------------------
// --------------------------------- LEAKS ---------------------------------
// -------------------------------------------------------------------------
//
// Theses macros ensure that the test procedures will not generate
// memory leaks --- even if CK_FORK=yes and some test cases fail.
//

#define __s_update__(name,expr)                                         \
    for(unsigned i=0; i<s##name; ++i) {                                 \
        if (name[i]) { d##name(name[i]); name[i] = NULL; }              \
        name[i] = expr;                                                 \
    }                                                                   \

#define __s_new__(type,name,size,destruct,rtype,argtype,construct)      \
    static type name[size] = { NULL };                                  \
    unsigned int s##name = size;                                        \
    rtype (*d##name)(argtype) = destruct;                               \
    __s_update__(name, construct);                                      \

#define safeMALLOC(type,name,size,expr)                                 \
    __s_new__(type, name, size, free, void, void*, expr);               \

#define safeNEW(type,name,size,destruct,expr)                           \
    __s_new__(type, name, size, destruct, void, type, expr);            \

#define safeDELETE(name)                                                \
    __s_update__(name, NULL);                                           \

#define safeMOV(name,move,destination)                                  \
    __typeof__(name) destination = { NULL };                            \
    for(unsigned i=0; i<s##name; ++i) {                                 \
        destination[i] = name[i];                                       \
        if (name[i]) name[i] = move(name[i]);                           \
    }                                                                   \

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#endif
