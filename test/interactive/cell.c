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

#include "interactive/cell.h"

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

static void __check__(Cell *cell, Cell *prev, Cell *next, Cell *pprev, Cell *nnext)
{
    ck_assert_ptr_eq(cell->next    , next); if(next) { ck_assert_ptr_eq(next->previous, cell); ck_assert_ptr_eq(next->next    , nnext); }
    ck_assert_ptr_eq(cell->previous, prev); if(prev) { ck_assert_ptr_eq(prev->next    , cell); ck_assert_ptr_eq(prev->previous, pprev); }
}

// -------------------------------------------------------------------------
// --------------------------------- TESTS ---------------------------------
// -------------------------------------------------------------------------

START_UTEST(test_insertAfter)
{
    safeNEW(Cell*, cells, 6, Cell_delete, Cell_new());
    for(unsigned i = 0; i < 6; ++i) ck_assert_ptr_ne(cells[i], NULL);
    {
        // 0 - 1 - 2 - 3 - 4 - 5
        Cell_insertAfter(cells[0], cells[1]); __check__(cells[0], NULL    , cells[1], NULL    , NULL    );
        Cell_insertAfter(cells[1], cells[3]); __check__(cells[1], cells[0], cells[3], NULL    , NULL    );
        Cell_insertAfter(cells[3], cells[5]); __check__(cells[3], cells[1], cells[5], cells[0], NULL    );
        Cell_insertAfter(cells[1], cells[2]); __check__(cells[1], cells[0], cells[2], NULL    , cells[3]);
        Cell_insertAfter(cells[3], cells[4]); __check__(cells[3], cells[2], cells[4], cells[1], cells[5]);
    }
    safeDELETE(cells);
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(cell)
{

    ONE_TEST_CASE(insertAfter, test_insertAfter);

}
END_SUITE_BUILDER
