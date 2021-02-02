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

// (useless)
// #include "system/info.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include "tools/macros.h"

// #########################################################################
// #########################################################################
// #########################################################################

START_UTEST(test_isRoot)
{
    ck_assert( // boolean test
               provided_isRoot() ?  isRoot()
                                 : !isRoot() );
}
END_UTEST

START_UTEST(test_getPATHVar)
{
    if(!maskIsActive())
    {
        const char *t1 = getPATHVar(),
                   *t2 = getPATHVar(),
                   *t3 = provided_getPATHVar();
        ck_assert_ptr_eq(t1, t2); // stored in a static array
        ck_assert_str_eq(t1, t3); // check the results
    }
    else
    {
        ck_abort_msg( // mask should not be active
                      "mask: active");
    }
}
END_UTEST

START_UTEST(test_hostname)
{
    const char *t1 = hostname(),
               *t2 = hostname(),
               *t3 = provided_hostname();
    ck_assert_ptr_eq(t1, t2);
    ck_assert_str_eq(t1, t3);
}
END_UTEST

START_UTEST(test_getCurrentDirectory)
{
    const char *t1 = getCurrentDirectory(1),
               *t2 = getCurrentDirectory(1),
               *t3 = provided_getCurrentDirectory(1);
    ck_assert_ptr_eq(t1, t2);
    ck_assert_str_eq(t1, t3);
                t1 = getCurrentDirectory(0);
                t2 = getCurrentDirectory(0);
                t3 = provided_getCurrentDirectory(0);
    ck_assert_ptr_eq(t1, t2);
    ck_assert_str_eq(t1, t3);
}
END_UTEST

START_UTEST(test_userInformation)
{
    const char *t1 = NULL, *t2 = NULL, *t3 = NULL;
    userInformation(&t1 , NULL, NULL); userInformation(&t2 , NULL, NULL); provided_userInformation(&t3 , NULL, NULL); ck_assert_ptr_eq(t1, t2); ck_assert_str_eq(t1, t3);
    userInformation(NULL, &t1 , NULL); userInformation(NULL, &t2 , NULL); provided_userInformation(NULL, &t3 , NULL); ck_assert_ptr_eq(t1, t2); ck_assert_str_eq(t1, t3);
    userInformation(NULL, NULL, &t1 ); userInformation(NULL, NULL, &t2 ); provided_userInformation(NULL, NULL, &t3 ); ck_assert_ptr_eq(t1, t2); ck_assert_str_eq(t1, t3);
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(info)
{

    ONE_TEST_CASE(isRoot             , test_isRoot             );
    ONE_TEST_CASE(getPATHVar         , test_getPATHVar         );
    ONE_TEST_CASE(hostname           , test_hostname           );
    ONE_TEST_CASE(userInformation    , test_userInformation    );
    ONE_TEST_CASE(getCurrentDirectory, test_getCurrentDirectory);

}
END_SUITE_BUILDER
