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

#include "misc/pattern.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include "tools/macros.h"

// #########################################################################
// #########################################################################
// #########################################################################

START_UTEST(test_no_pattern)
{
    safeNEW(Pattern*, preg, 1, Pattern_delete, Pattern_new(NULL)); ck_assert_ptr_eq(preg[0], NULL); safeDELETE(preg);
}
END_UTEST

static void __test_pattern__(const char *pattern, const char *valid, const char *fail)
{
    safeNEW(Pattern*, preg, 1, Pattern_delete, Pattern_new(pattern)); ck_assert_ptr_ne(preg[0], NULL);
    {
        ck_assert_int_ne(Pattern_match(preg[0], valid), 0);
        ck_assert_int_eq(Pattern_match(preg[0], fail ), 0);
    }
    safeDELETE(preg);
}

START_UTEST(test_valid_pattern)
{
    __test_pattern__("^[a-z]+\\.txt$", "tropfacile.txt", "it.s.a.trap.txt");
    __test_pattern__("(moi)|(toi?)|(sam) ", "to", "fred");
    __test_pattern__("^[a-zA-Z0-9_-]+(\\.[a-zA-Z0-9_-]+)*@[a-zA-Z0-9_-]+(\\.[a-zA-Z0-9_-]+)*\\.[a-zA-Z]{2,4}$", "lara.croft@univ-tours.fr", "adresse@non@valide@univ-tours.fr");
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(regex)
{

    ONE_TEST_CASE(no_pattern   , test_no_pattern   );
    ONE_TEST_CASE(valid_pattern, test_valid_pattern);

}
END_SUITE_BUILDER
