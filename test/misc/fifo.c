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

#include "misc/fifo.h"

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

#define __s1__ "s1"
#define __s2__ "s2"
#define __s3__ "s3"
#define __s4__ "s4"
#define __s5__ "s5"

static const char* __select_str__(unsigned int i)
{
    switch(i%5) { case 0: return __s1__; case 1: return __s2__;   case 2: return __s3__;
                  case 3: return __s4__; case 4: return __s5__;  default: return NULL  ; }
}

static void __check__(Fifo *fifo, unsigned int capacity, FifoMode mode, unsigned int size, const char *expected)
{
    if(size)
    {
        // empty = NO
        ck_assert_int_eq(Fifo_empty(fifo), 0);
        // full  = YES/NO
        if   (size == capacity) { ck_assert_int_ne(Fifo_full(fifo), 0); ck_assert_int_ne(Fifo_push(fifo, __s1__),  0); }
        else                    { ck_assert_int_eq(Fifo_full(fifo), 0);                                                }
        // front
        ck_assert_str_eq(Fifo_front(fifo)            , expected         );
        ck_assert_int_eq(Fifo_front(fifo) == expected, mode == AGGREGATE);
    }
    else
    {
        // empty = YES
        ck_assert_int_eq(Fifo_full (fifo),  0  );
        ck_assert_int_ne(Fifo_empty(fifo),  0  );
        ck_assert_int_ne(Fifo_pop  (fifo),  0  );
        ck_assert_ptr_eq(Fifo_front(fifo), NULL);
    }
}

#define __new__(fifo,capacity,mode)                                                                 \
    safeNEW(Fifo*, __##fifo, 1, Fifo_delete, Fifo_new(capacity, mode)); Fifo *fifo = __##fifo[0];   \
    ck_assert_ptr_ne(__##fifo[0], NULL); __check__(__##fifo[0], capacity, mode, 0, NULL);           \

#define __del__(fifo)                                                                               \
    safeDELETE(__##fifo)                                                                            \

#define __use__(operation)                                                                          \
    ck_assert_int_eq(operation, 0);                                                                 \

static void __5add__(Fifo *fifo, unsigned int capacity, FifoMode mode)
{
    __use__(Fifo_push(fifo, __s1__)); __check__(fifo, capacity, mode, 1, __s1__);
    __use__(Fifo_push(fifo, __s2__)); __check__(fifo, capacity, mode, 2, __s1__);
    __use__(Fifo_push(fifo, __s3__)); __check__(fifo, capacity, mode, 3, __s1__);
    __use__(Fifo_push(fifo, __s4__)); __check__(fifo, capacity, mode, 4, __s1__);
    __use__(Fifo_push(fifo, __s5__)); __check__(fifo, capacity, mode, 5, __s1__);
}

static void __5pop__(Fifo *fifo, unsigned int capacity, FifoMode mode)
{
    __use__(Fifo_pop(fifo)); __check__(fifo, capacity, mode, 4, __s2__);
    __use__(Fifo_pop(fifo)); __check__(fifo, capacity, mode, 3, __s3__);
    __use__(Fifo_pop(fifo)); __check__(fifo, capacity, mode, 2, __s4__);
    __use__(Fifo_pop(fifo)); __check__(fifo, capacity, mode, 1, __s5__);
    __use__(Fifo_pop(fifo)); __check__(fifo, capacity, mode, 0, NULL  );
}

static void __mixt__(Fifo *fifo, unsigned int capacity, FifoMode mode)
{
    const char *str = __select_str__(1); unsigned i = 1;
    __use__(Fifo_push(fifo, str)); const char *front = str; __check__(fifo, capacity, mode, 1, front);
    for(unsigned int j = 0; j < 64; ++j)
    {
        str = __select_str__(++i); __use__(Fifo_push(fifo, str)); __check__(fifo, capacity, mode, 2, front);
        str = __select_str__(++i); __use__(Fifo_push(fifo, str)); __check__(fifo, capacity, mode, 3, front);
        __use__(Fifo_pop(fifo)); front = __select_str__(i-1); __check__(fifo, capacity, mode, 2, front);
        __use__(Fifo_pop(fifo)); front = __select_str__(i  ); __check__(fifo, capacity, mode, 1, front);
    }
    __use__(Fifo_pop(fifo)); __check__(fifo, capacity, mode, 0, NULL);
}

// -------------------------------------------------------------------------
// --------------------------------- TESTS ---------------------------------
// -------------------------------------------------------------------------

START_UTEST(test_fifo_capacity_0)
{
    safeNEW(Fifo*, f1, 1, Fifo_delete, Fifo_new(0, COMPOSE  )); ck_assert_ptr_eq(f1[0], NULL); safeDELETE(f1);
    safeNEW(Fifo*, f2, 1, Fifo_delete, Fifo_new(0, AGGREGATE)); ck_assert_ptr_eq(f2[0], NULL); safeDELETE(f2);
}
END_UTEST

START_UTEST(test_fifo_push)
{
    __new__(fifo1, 5, AGGREGATE); __5add__(fifo1, 5, AGGREGATE); __del__(fifo1);
    __new__(fifo2, 5, COMPOSE  ); __5add__(fifo2, 5, COMPOSE  ); __del__(fifo2);
}
END_UTEST

START_UTEST(test_fifo_pop)
{
    __new__(fifo1, 5, AGGREGATE); __5add__(fifo1, 5, AGGREGATE); __5pop__(fifo1, 5, AGGREGATE); __5add__(fifo1, 5, AGGREGATE); __5pop__(fifo1, 5, AGGREGATE); __mixt__(fifo1, 5, AGGREGATE); __del__(fifo1);
    __new__(fifo2, 5, COMPOSE  ); __5add__(fifo2, 5, COMPOSE  ); __5pop__(fifo2, 5, COMPOSE  ); __5add__(fifo2, 5, COMPOSE  ); __5pop__(fifo2, 5, COMPOSE  ); __mixt__(fifo2, 5, COMPOSE  ); __del__(fifo2);
}
END_UTEST

START_UTEST(test_fifo_clear)
{
    __new__(fifo1, 5, AGGREGATE); __5add__(fifo1, 5, AGGREGATE); Fifo_clear(fifo1); __check__(fifo1, 5, AGGREGATE, 0, NULL); __mixt__(fifo1, 5, AGGREGATE); __del__(fifo1);
    __new__(fifo2, 5, COMPOSE  ); __5add__(fifo2, 5, COMPOSE  ); Fifo_clear(fifo2); __check__(fifo2, 5, COMPOSE  , 0, NULL); __mixt__(fifo2, 5, COMPOSE  ); __del__(fifo2);
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(fifo)
{

    ONE_TEST_CASE(fifo_capacity_0, test_fifo_capacity_0);
    ONE_TEST_CASE(fifo_push      , test_fifo_push      );
    ONE_TEST_CASE(fifo_pop       , test_fifo_pop       );
    ONE_TEST_CASE(fifo_clear     , test_fifo_clear     );

}
END_SUITE_BUILDER
