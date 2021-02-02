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

#include "interactive/input.h"

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

#define __SPACE__ " "
#define NUMBER_INS 512U
#define NUMBER_DEL 256U

// ---------------------------------- new ----------------------------------
// -------------------------------------------------------------------------

#define W1 "Salut,"    __SPACE__
#define W2 "et"        __SPACE__
#define W3 "encore"    __SPACE__
#define W4 "merci"     __SPACE__
#define W5 "pour"      __SPACE__
#define W6 "le"        __SPACE__ // at least 2 chars
#define W7 "poisson!"            // at least 5 chars
#define SENTENCE W1 W2 W3 W4 W5 W6 W7

#define inputSafeNEW(input,cells,position,length)                                                                                                           \
    size_t position = sizeof(SENTENCE);   --position;                                                                                                       \
    size_t length   = sizeof(SENTENCE);   --length  ;                                                                                                       \
    /* init the inner cells */                                                                                                                              \
    safeNEW(Cell*, __##cells, 7, Cell_delete, Cell_new());                                                                                                  \
                                                  for(const char *str = W1, *ptr = str; *ptr; ++ptr) Bucket_insert(&__##cells[0]->bucket, ptr-str, *ptr);   \
    Cell_insertAfter(__##cells[0], __##cells[1]); for(const char *str = W2, *ptr = str; *ptr; ++ptr) Bucket_insert(&__##cells[1]->bucket, ptr-str, *ptr);   \
    Cell_insertAfter(__##cells[1], __##cells[2]); for(const char *str = W3, *ptr = str; *ptr; ++ptr) Bucket_insert(&__##cells[2]->bucket, ptr-str, *ptr);   \
    Cell_insertAfter(__##cells[2], __##cells[3]); for(const char *str = W4, *ptr = str; *ptr; ++ptr) Bucket_insert(&__##cells[3]->bucket, ptr-str, *ptr);   \
    Cell_insertAfter(__##cells[3], __##cells[4]); for(const char *str = W5, *ptr = str; *ptr; ++ptr) Bucket_insert(&__##cells[4]->bucket, ptr-str, *ptr);   \
    Cell_insertAfter(__##cells[4], __##cells[5]); for(const char *str = W6, *ptr = str; *ptr; ++ptr) Bucket_insert(&__##cells[5]->bucket, ptr-str, *ptr);   \
    Cell_insertAfter(__##cells[5], __##cells[6]); for(const char *str = W7, *ptr = str; *ptr; ++ptr) Bucket_insert(&__##cells[6]->bucket, ptr-str, *ptr);   \
    /* construct an empty input */                                                                                                                          \
    safeNEW(Input*, __##input, 1, Input_delete, Input_new()); ck_assert_ptr_ne(__##input[0], NULL);                                                         \
    /* transfer cells ownership from __##cells to input and copy pointers to cells */                                                                       \
    Cell* cells[7]; for(unsigned iTransfer=0; iTransfer<7; ++iTransfer) { cells[iTransfer] = __##cells[iTransfer]; __##cells[iTransfer] = NULL; }           \
    /* initialize __##input[0] and input */                                                                                                                 \
    Input *input = __##input[0]; input->current = cells[6]; input->pos = sizeof(W7)-1;                                                                      \

#define inputSafeDELETE(input)                                                                                                                              \
    safeDELETE(__##input);                                                                                                                                  \

#define toStringSafeNEW(str,input)                                                                                                                          \
    safeMALLOC(char*, __##str, 1, Input_toString(input)); ck_assert_ptr_ne(__##str[0], NULL); char *str = __##str[0];                                       \

#define toStringSafeDEL(str)                                                                                                                                \
    safeDELETE(__##str);                                                                                                                                    \

// --------------------------------- move ----------------------------------
// -------------------------------------------------------------------------

static void __shift_left__(Input *input, unsigned int nshif)
{
    // ---n--->
    for(unsigned i = 0; i < nshif; ++i) ck_assert_int_eq(Input_moveLeft(input), 0);
}

static void __goto__(Input *input, size_t *position, size_t npos)
{
    // ---->pos
    while(npos>*position) { ck_assert_int_eq(Input_moveRight(input), 0); ++(*position); }
    while(npos<*position) { ck_assert_int_eq(Input_moveLeft (input), 0); --(*position); }
}

// -------------------------------- buckets --------------------------------
// -------------------------------------------------------------------------

void __insert__(char *str, char c, int pos, char *bak);
void __rm__(char *str, int pos, char *bak);

// -------------------------------- insert ---------------------------------
// -------------------------------------------------------------------------

static void __check_insert__(Input *input, char c, char *string, char *bak, size_t capacity, size_t *position, size_t *length)
{
    copyStringWithLength(bak, string, capacity); Input_insert(input, c); __insert__(string, c, *position, bak); (*position)++; (*length)++; toStringSafeNEW(r, input);
    ck_assert_msg(strcmp(r, string) == 0, "insert of '%c' at position=%u in \"%s\" has failed", c, (unsigned)*position, bak); toStringSafeDEL(r);
}

static void __fill__(Input *input, unsigned int ninsertions, char *str, char *bak, size_t capacity, size_t *position, size_t *length)
{
    for(unsigned int i = 1; i <= ninsertions; ++i) { char c = 'a' + i % 26; // move with probability 3/4
                                                     size_t npos = rand() % 4 > 0 ? rand() % (*length+1) : *position;
                                                     __goto__(input, position, npos); __check_insert__(input, c, str, bak, capacity, position, length); }
}

// ---------------------------------- del ----------------------------------
// -------------------------------------------------------------------------

static void __check_rm__(Input *input, char *str, char *bak, size_t capacity, size_t *position, size_t *length)
{
    copyStringWithLength(bak, str, capacity); Input_del(input); __rm__(str, *position, bak); (*length)--; toStringSafeNEW(r, input);
    ck_assert_msg(strcmp(r, str) == 0, "del at position=%u in \"%s\" has failed", (unsigned)*position, bak); toStringSafeDEL(r);
}

static void __clear__(Input *input, unsigned int ndeletions, char *str, char *bak, size_t capacity, size_t *position, size_t *length)
{
    for(unsigned int i = 1; i <= ndeletions; ++i) { size_t npos = rand() % 4 > 0 ? rand() % *length : *position;
                                                    __goto__(input, position, npos); __check_rm__(input, str, bak, capacity, position, length); }
}

// ------------------------------- backspace -------------------------------
// -------------------------------------------------------------------------

static void __check_bs__(Input *input, char *str, char *bak, size_t capacity, size_t *position, size_t *length)
{
    copyStringWithLength(bak, str, capacity); Input_backspace(input); __rm__(str, *position-1, bak); (*position)--; (*length)--; toStringSafeNEW(r, input);
    ck_assert_msg(strcmp(r, str) == 0, "backspace at position=%u in \"%s\" has failed", (unsigned)*position, bak); toStringSafeDEL(r);
}

static void __backspace__(Input *input, unsigned int ndeletions, char *str, char *bak, size_t capacity, size_t *position, size_t *length)
{
    for(unsigned int i = 1; i <= ndeletions; ++i) { size_t npos = !*position || rand() % 4 > 0 ? 1 + rand() % *length : *position;
                                                    __goto__(input, position, npos); __check_bs__(input, str, bak, capacity, position, length); }
}

// -------------------------------------------------------------------------
// --------------------------------- TESTS ---------------------------------
// -------------------------------------------------------------------------

START_UTEST(test_input_equals)
{
    Cell c; Input i; Input j;
    i.current = NULL; i.pos = 5;
    j.current = NULL; j.pos = 5; ck_assert_int_ne(InputIterator_equals(&i, &j), 0);
    j.current = &c  ;            ck_assert_int_eq(InputIterator_equals(&i, &j), 0);
    i.current = &c  ;            ck_assert_int_ne(InputIterator_equals(&i, &j), 0);
                      j.pos = 0; ck_assert_int_eq(InputIterator_equals(&i, &j), 0);
                      i.pos = 0; ck_assert_int_ne(InputIterator_equals(&i, &j), 0);
                                 ck_assert_int_ne(InputIterator_equals(&i, &i), 0);
}
END_UTEST

START_UTEST(test_input_size)
{
    inputSafeNEW(input, cells, position, length);
    {
        ck_assert_uint_eq(Input_size(input), sizeof(SENTENCE)-1);
    }
    inputSafeDELETE(input);
}
END_UTEST

START_UTEST(test_input_iterate)
{
    inputSafeNEW(input, cells, position, length);
    {
        InputIterator iterator; // no alloc
        InputIterator_initIterator(input, &iterator);
        {
            for(const char *str = SENTENCE, *ptr = str; *ptr; ++ptr, InputIterator_next(&iterator)) {
                ck_assert_int_eq(InputIterator_isOver(&iterator), 0   );
                ck_assert_int_eq(InputIterator_get   (&iterator), *ptr);
            } }
    }
    inputSafeDELETE(input);
}
END_UTEST

START_UTEST(test_input_str)
{
    inputSafeNEW(input, cells, position, length);
    {
        toStringSafeNEW(str, input); ck_assert_str_eq(str, SENTENCE); toStringSafeDEL(str);
    }
    inputSafeDELETE(input);
}
END_UTEST

START_UTEST(test_input_clear)
{
    inputSafeNEW(input, cells, position, length);
    {
        Input_clear(input);
                                                         ck_assert_uint_eq(Input_size(input),   0 );
                            toStringSafeNEW(tmp, input); ck_assert_str_eq (tmp              ,  "" ); toStringSafeDEL(tmp);
                                                         ck_assert_ptr_eq (input->current   , NULL);
    }
    inputSafeDELETE(input);
}
END_UTEST

START_UTEST(test_input_move)
{
    inputSafeNEW(input, cells, position, length);
    {
        // ------>|
        ck_assert_int_ne(Input_moveRight(input), 0); ck_assert_ptr_eq(input->current, cells[6]); ck_assert_int_eq(input->pos, sizeof(W7)-1);
        // <-------
        for(unsigned int i = 0; i < sizeof(W7)-1; ++i) { ck_assert_int_eq(Input_moveLeft (input), 0); ck_assert_ptr_eq(input->current, cells[6]); ck_assert_int_eq(input->pos, sizeof(W7)-2-i); }
        for(unsigned int i = 0; i < sizeof(W6)-1; ++i) { ck_assert_int_eq(Input_moveLeft (input), 0); ck_assert_ptr_eq(input->current, cells[5]); ck_assert_int_eq(input->pos, sizeof(W6)-2-i); }
        for(unsigned int i = 0; i < sizeof(W5)-1; ++i) { ck_assert_int_eq(Input_moveLeft (input), 0); ck_assert_ptr_eq(input->current, cells[4]); ck_assert_int_eq(input->pos, sizeof(W5)-2-i); }
        for(unsigned int i = 0; i < sizeof(W4)-1; ++i) { ck_assert_int_eq(Input_moveLeft (input), 0); ck_assert_ptr_eq(input->current, cells[3]); ck_assert_int_eq(input->pos, sizeof(W4)-2-i); }
        for(unsigned int i = 0; i < sizeof(W3)-1; ++i) { ck_assert_int_eq(Input_moveLeft (input), 0); ck_assert_ptr_eq(input->current, cells[2]); ck_assert_int_eq(input->pos, sizeof(W3)-2-i); }
        for(unsigned int i = 0; i < sizeof(W2)-1; ++i) { ck_assert_int_eq(Input_moveLeft (input), 0); ck_assert_ptr_eq(input->current, cells[1]); ck_assert_int_eq(input->pos, sizeof(W2)-2-i); }
        for(unsigned int i = 0; i < sizeof(W1)-1; ++i) { ck_assert_int_eq(Input_moveLeft (input), 0); ck_assert_ptr_eq(input->current, cells[0]); ck_assert_int_eq(input->pos, sizeof(W1)-2-i); }
        // |<------
        ck_assert_int_ne(Input_moveLeft (input), 0); ck_assert_ptr_eq(input->current, cells[0]); ck_assert_int_eq(input->pos,       0     );
        // ------->
        for(unsigned int i = 0; i < sizeof(W1)-2; ++i) { ck_assert_int_eq(Input_moveRight(input), 0); ck_assert_ptr_eq(input->current, cells[0]); ck_assert_int_eq(input->pos, i+1           ); }
        for(unsigned int i = 0; i < sizeof(W2)-1; ++i) { ck_assert_int_eq(Input_moveRight(input), 0); ck_assert_ptr_eq(input->current, cells[1]); ck_assert_int_eq(input->pos, i             ); }
        for(unsigned int i = 0; i < sizeof(W3)-1; ++i) { ck_assert_int_eq(Input_moveRight(input), 0); ck_assert_ptr_eq(input->current, cells[2]); ck_assert_int_eq(input->pos, i             ); }
        for(unsigned int i = 0; i < sizeof(W4)-1; ++i) { ck_assert_int_eq(Input_moveRight(input), 0); ck_assert_ptr_eq(input->current, cells[3]); ck_assert_int_eq(input->pos, i             ); }
        for(unsigned int i = 0; i < sizeof(W5)-1; ++i) { ck_assert_int_eq(Input_moveRight(input), 0); ck_assert_ptr_eq(input->current, cells[4]); ck_assert_int_eq(input->pos, i             ); }
        for(unsigned int i = 0; i < sizeof(W6)-1; ++i) { ck_assert_int_eq(Input_moveRight(input), 0); ck_assert_ptr_eq(input->current, cells[5]); ck_assert_int_eq(input->pos, i             ); }
        for(unsigned int i = 0; i < sizeof(W7)  ; ++i) { ck_assert_int_eq(Input_moveRight(input), 0); ck_assert_ptr_eq(input->current, cells[6]); ck_assert_int_eq(input->pos, i             ); }
        // ------>|
        ck_assert_int_ne(Input_moveRight(input), 0); ck_assert_ptr_eq(input->current, cells[6]); ck_assert_int_eq(input->pos, sizeof(W7)-1);
    }
    inputSafeDELETE(input);
}
END_UTEST

START_UTEST(test_input_get)
{
    inputSafeNEW(input, cells, position, length);
    {
        unsigned int i = sizeof(SENTENCE)-1;
        while(i) { ck_assert_int_eq(Input_get(input), SENTENCE[i]);
                   --i; Input_moveLeft(input); }
    }
    inputSafeDELETE(input);
}
END_UTEST

START_UTEST(test_input_insert)
{
    char str[NUMBER_INS+sizeof(SENTENCE)] = SENTENCE; const size_t capacity = sizeof(SENTENCE)+NUMBER_INS;
    char bak[NUMBER_INS+sizeof(SENTENCE)] = SENTENCE;
    inputSafeNEW(input, cells, position, length);
    {
        // move to a given position or edit the string                                        // insert
        while(position<length) { ck_assert_int_eq(Input_moveRight(input), 0); ++position; }   __check_insert__(input, '$'         , str, bak, capacity, &position, &length);
        while(position       ) { ck_assert_int_eq(Input_moveLeft (input), 0); --position; }   __check_insert__(input, '^'         , str, bak, capacity, &position, &length);
                                                                                              __fill__        (input, NUMBER_INS-2, str, bak, capacity, &position, &length);
        Input_clear(input); position = 0; str[0] = '\0';                                      __check_insert__(input, '.'         , str, bak, capacity, &position, &length);
    }
    inputSafeDELETE(input);
}
END_UTEST

START_UTEST(test_input_del)
{
    char str[2*NUMBER_DEL+sizeof(SENTENCE)] = SENTENCE; const size_t capacity = sizeof(SENTENCE)+2*NUMBER_DEL;
    char bak[2*NUMBER_DEL+sizeof(SENTENCE)] = SENTENCE;
    inputSafeNEW(input, cells, position, length);
    {
        // bad position
        while(position<length) { ck_assert_int_eq(Input_moveRight(input), 0); ++position; }
        ck_assert_int_ne(Input_del(input), 0);
        // random deletions
        __fill__ (input, 2*NUMBER_DEL, str, bak, capacity, &position, &length);
        __clear__(input,   NUMBER_DEL, str, bak, capacity, &position, &length);
        __fill__ (input,   NUMBER_DEL, str, bak, capacity, &position, &length);
        __clear__(input,   NUMBER_DEL, str, bak, capacity, &position, &length);
    }
    inputSafeDELETE(input);
}
END_UTEST

START_UTEST(test_input_backspace)
{
    char str[2*NUMBER_DEL+sizeof(SENTENCE)] = SENTENCE; const size_t capacity = sizeof(SENTENCE)+2*NUMBER_DEL;
    char bak[2*NUMBER_DEL+sizeof(SENTENCE)] = SENTENCE;
    inputSafeNEW(input, cells, position, length);
    {
        // bad position
        while(position) { ck_assert_int_eq(Input_moveLeft(input), 0); --position; }
        ck_assert_int_ne(Input_backspace(input), 0);
        // random deletions
        __fill__     (input, 2*NUMBER_DEL, str, bak, capacity, &position, &length);
        __backspace__(input,   NUMBER_DEL, str, bak, capacity, &position, &length);
        __fill__     (input,   NUMBER_DEL, str, bak, capacity, &position, &length);
        __backspace__(input,   NUMBER_DEL, str, bak, capacity, &position, &length);
    }
    inputSafeDELETE(input);
}
END_UTEST

START_UTEST(test_input_load)
{
    const char *string = "Le Dernier Restaurant avant la fin du monde!";
    inputSafeNEW(input, cells, position, length);
    {
        ck_assert_int_ne(Input_load(input, NULL  ), 0);
        ck_assert_int_eq(Input_load(input, string), 0); toStringSafeNEW(t,input); ck_assert_str_eq(t, string); toStringSafeDEL(t);
                                                        ck_assert_int_ne(Input_moveRight(input), 0); // ------>|
    }
    inputSafeDELETE(input);
}
END_UTEST

START_UTEST(test_input_edited)
{
    char cw1[sizeof(W1)]; copyStringWithLength(cw1, W1, sizeof(W1));
    char cw6[sizeof(W6)]; copyStringWithLength(cw6, W6, sizeof(W6));
    char cw7[sizeof(W7)]; copyStringWithLength(cw7, W7, sizeof(W7));
    inputSafeNEW(input, cells, position, length);
    {
                                                                                         safeMALLOC(char*, tmp1, 1, Input_getEditedWord(input)); ck_assert_ptr_ne(tmp1[0], NULL); ck_assert_str_eq(tmp1[0], cw7); safeDELETE(tmp1);
        __shift_left__(input,              1U               ); cw7[sizeof(W7)-2] = '\0'; safeMALLOC(char*, tmp2, 1, Input_getEditedWord(input)); ck_assert_ptr_ne(tmp2[0], NULL); ck_assert_str_eq(tmp2[0], cw7); safeDELETE(tmp2);
        __shift_left__(input,              2U               ); cw7[sizeof(W7)-4] = '\0'; safeMALLOC(char*, tmp3, 1, Input_getEditedWord(input)); ck_assert_ptr_ne(tmp3[0], NULL); ck_assert_str_eq(tmp3[0], cw7); safeDELETE(tmp3);
        __shift_left__(input,         sizeof(W7)-4U         ); cw7[    0U      ] = '\0'; safeMALLOC(char*, tmp4, 1, Input_getEditedWord(input)); ck_assert_ptr_ne(tmp4[0], NULL); ck_assert_str_eq(tmp4[0], cw7); safeDELETE(tmp4);
        __shift_left__(input,              1U               ); cw6[sizeof(W6)-2] = '\0'; safeMALLOC(char*, tmp5, 1, Input_getEditedWord(input)); ck_assert_ptr_ne(tmp5[0], NULL); ck_assert_str_eq(tmp5[0], cw6); safeDELETE(tmp5);
        __shift_left__(input, sizeof(SENTENCE)-sizeof(W7)-3U); cw1[    2U      ] = '\0'; safeMALLOC(char*, tmp6, 1, Input_getEditedWord(input)); ck_assert_ptr_ne(tmp6[0], NULL); ck_assert_str_eq(tmp6[0], cw1); safeDELETE(tmp6);
        __shift_left__(input,              2U               );                           safeMALLOC(char*, tmp7, 1, Input_getEditedWord(input)); ck_assert_ptr_eq(tmp7[0], NULL);                                 safeDELETE(tmp7);
    }
    inputSafeDELETE(input);
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(input)
{

    ONE_TEST_CASE(input_equals   , test_input_equals   );
    ONE_TEST_CASE(input_size     , test_input_size     );
    ONE_TEST_CASE(input_iterate  , test_input_iterate  );
    ONE_TEST_CASE(input_str      , test_input_str      );
    ONE_TEST_CASE(input_clear    , test_input_clear    );
    ONE_TEST_CASE(input_move     , test_input_move     );
    ONE_TEST_CASE(input_get      , test_input_get      );
    ONE_TEST_CASE(input_insert   , test_input_insert   );
    ONE_TEST_CASE(input_del      , test_input_del      );
    ONE_TEST_CASE(input_backspace, test_input_backspace);
    ONE_TEST_CASE(input_load     , test_input_load     );
    ONE_TEST_CASE(input_edited   , test_input_edited   );

}
END_SUITE_BUILDER
