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

#include "parser/parser.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include "tools/macros.h"

// #########################################################################
// #########################################################################
// #########################################################################

START_UTEST(test_parse_level01)
{
    // ...
    // ...
}
END_UTEST

START_UTEST(test_parse_level02)
{
    // ...
    // ...
}
END_UTEST

START_UTEST(test_parse_level03)
{
    // ...
    // ...
}
END_UTEST

START_UTEST(test_parse_level04)
{
    // ...
    // ...
}
END_UTEST

START_UTEST(test_parse_level05)
{
    // ...
    // ...
}
END_UTEST

START_UTEST(test_parse_level06)
{
    // ...
    // ...
}
END_UTEST

START_UTEST(test_parse_level07)
{
    // ...
    // ...
}
END_UTEST

START_UTEST(test_parse_level08)
{
    // ...
    // ...
}
END_UTEST

START_UTEST(test_parse_level09)
{
    // ...
    // ...
}
END_UTEST

START_UTEST(test_parse_level10)
{
    // ...
    // ...
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(parser)
{

    ONE_TEST_CASE(parse_level01, test_parse_level01);
    ONE_TEST_CASE(parse_level02, test_parse_level02);
    ONE_TEST_CASE(parse_level03, test_parse_level03);
    ONE_TEST_CASE(parse_level04, test_parse_level04);
    ONE_TEST_CASE(parse_level05, test_parse_level05);
    ONE_TEST_CASE(parse_level06, test_parse_level06);
    ONE_TEST_CASE(parse_level07, test_parse_level07);
    ONE_TEST_CASE(parse_level08, test_parse_level08);
    ONE_TEST_CASE(parse_level09, test_parse_level09);
    ONE_TEST_CASE(parse_level10, test_parse_level10);

}
END_SUITE_BUILDER
