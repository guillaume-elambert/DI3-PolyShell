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

#include "interactive/history.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include "tools/macros.h"

// #########################################################################
// #########################################################################
// #########################################################################

// -------------------------------------------------------------------------
// --------------------------------- TOOLS ---------------------------------
// -------------------------------------------------------------------------

#define HISTORY_UNIT_TESTS "/tmp/.polyshell_utests"
#define __s1__ "pwd"
#define __s2__ "cat fichier.txt | grep -E pattern > output.txt"
#define __s3__ "ls -l -a >> output.txt"
#define __s4__ "su -u etudiant # some useless comments."
#define __s5__ "apt-get update && apt-get upgrade"

static void __fill__(History *history)
{
    History_add(history, __s1__); const char *front = Fifo_front(&history->storage); ck_assert_ptr_ne(front,  __s1__ ); ck_assert_str_eq(front,  __s1__ ); const char *expected = front;
    History_add(history, __s2__);             front = Fifo_front(&history->storage); ck_assert_ptr_eq(front, expected);
    History_add(history, __s3__);             front = Fifo_front(&history->storage); ck_assert_ptr_eq(front, expected);
    History_add(history, __s4__);             front = Fifo_front(&history->storage); ck_assert_ptr_ne(front,  __s2__ ); ck_assert_str_eq(front,  __s2__ );
    History_add(history, __s5__);             front = Fifo_front(&history->storage); ck_assert_ptr_ne(front,  __s3__ ); ck_assert_str_eq(front,  __s3__ );             expected = front;
    History_add(history,   ""  );             front = Fifo_front(&history->storage); ck_assert_ptr_eq(front, expected);
    History_add(history,   ""  );             front = Fifo_front(&history->storage); ck_assert_ptr_eq(front, expected);
}

static void __check_up__(History *history)
{
    const char *r = History_up(history); ck_assert_ptr_ne(r, NULL); ck_assert_str_eq(r, __s5__);
                r = History_up(history); ck_assert_ptr_ne(r, NULL); ck_assert_str_eq(r, __s4__);
                r = History_up(history); ck_assert_ptr_ne(r, NULL); ck_assert_str_eq(r, __s3__);
                r = History_up(history); ck_assert_ptr_eq(r, NULL);
}

// -------------------------------------------------------------------------
// --------------------------- LEAKS (OVERRIDE) ----------------------------
// -------------------------------------------------------------------------

#undef  safeNEW
#define safeNEW(name,filename,length) static History *name = NULL; safeDELETE(name, NULL); name = History_new(filename, length);

#undef  safeDELETE
#define safeDELETE(name,filename) if(name) { History_delete(name, filename); name = NULL; }

// -------------------------------------------------------------------------
// --------------------------------- TESTS ---------------------------------
// -------------------------------------------------------------------------

START_UTEST(test_zero_length)
{
    safeNEW(h1, NULL              , 0); ck_assert_ptr_eq(h1, NULL); safeDELETE(h1, NULL);
    safeNEW(h2, HISTORY_UNIT_TESTS, 0); ck_assert_ptr_eq(h2, NULL); safeDELETE(h2, NULL);
}
END_UTEST

START_UTEST(test_push)
{
    safeNEW(history, NULL, 3); ck_assert_ptr_ne(history, NULL); __fill__(history); safeDELETE(history, NULL);
}
END_UTEST

START_UTEST(test_up_down)
{
    safeNEW(history, NULL, 3); ck_assert_ptr_ne(history, NULL);
    {
        __fill__(history);             // up
                                       __check_up__(history);
                                       // down
                           const char *r = History_down(history); ck_assert_ptr_ne(r, NULL); ck_assert_str_eq(r, __s4__);
                                       r = History_down(history); ck_assert_ptr_ne(r, NULL); ck_assert_str_eq(r, __s5__);
                                       r = History_down(history); ck_assert_ptr_ne(r, NULL); ck_assert_str_eq(r,   ""  );
                                       r = History_down(history); ck_assert_ptr_eq(r, NULL);
                                       // up
                                       __check_up__(history);
                                       // down
                                       r = History_down(history); ck_assert_ptr_ne(r, NULL); ck_assert_str_eq(r, __s4__);
                                       r = History_down(history); ck_assert_ptr_ne(r, NULL); ck_assert_str_eq(r, __s5__);
                                       r = History_down(history); ck_assert_ptr_ne(r, NULL); ck_assert_str_eq(r,   ""  );
                                       r = History_down(history); ck_assert_ptr_eq(r, NULL);

    }
    safeDELETE(history, NULL);
}
END_UTEST

START_UTEST(test_load_save)
{
    unlink(HISTORY_UNIT_TESTS); safeNEW(h1, NULL              , 3); ck_assert_ptr_ne(h1, NULL); __fill__    (h1); safeDELETE(h1, HISTORY_UNIT_TESTS);
                                safeNEW(h2, HISTORY_UNIT_TESTS, 3); ck_assert_ptr_ne(h2, NULL); __check_up__(h2); safeDELETE(h2, NULL              );
}
END_UTEST

START_UTEST(test_clear)
{
    safeNEW(history, NULL, 3); ck_assert_ptr_ne(history, NULL);
    {
        __fill__(history);
        History_clear(history); ck_assert_ptr_eq(History_up  (history), NULL);
                                ck_assert_ptr_eq(History_down(history), NULL);
    }
    safeDELETE(history, NULL);
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(history)
{

    ONE_TEST_CASE(push       , test_push       );
    ONE_TEST_CASE(up_down    , test_up_down    );
    ONE_TEST_CASE(load_save  , test_load_save  );
    ONE_TEST_CASE(zero_length, test_zero_length);
    ONE_TEST_CASE(clear      , test_clear      );

}
END_SUITE_BUILDER
