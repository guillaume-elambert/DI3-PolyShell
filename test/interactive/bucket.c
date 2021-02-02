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

#include "interactive/bucket.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include <stdarg.h>
#include "tools/macros.h"

// #########################################################################
// #########################################################################
// #########################################################################

// -------------------------------------------------------------------------
// --------------------------------- TOOLS ---------------------------------
// -------------------------------------------------------------------------

void __insert__(char *str, char c, int pos, char *bak)
{
    strcpy(bak, str); memmove(str+pos+1, str+pos, strlen(str+pos)+1); str[pos] = c;
}

void __rm__(char *str, int pos, char *bak)
{
    strcpy(bak, str); memmove(str+pos, str+pos+1, strlen(str+pos+1)+1);
}

static void __move__(char *str, char *dst, int pos, char *bak)
{
    strcpy(bak, str); strcpy(dst, str+pos); str[pos] = '\0';
}

static void __check__(const Bucket *bucket, const char *str, const char *msg, ...)
{
// ----------------------------- message error -----------------------------
// -------------------------------------------------------------------------
    char message[256]; message[0] = '\0';
    va_list args; va_start(args, msg); vsnprintf(message, 256, msg, args); va_end(args);
// -------------------------------- content --------------------------------
// -------------------------------------------------------------------------
    size_t length = strlen(str);
    ck_assert_msg(memcmp(bucket->content, str, length) == 0, message);
    ck_assert_msg(Bucket_size(bucket) == length, message);
// ---------------------------- empty and full  ----------------------------
// -------------------------------------------------------------------------
    ck_assert_msg(Bucket_empty(bucket) == (length == 0          ), message);
    ck_assert_msg(Bucket_full (bucket) == (length == BUCKET_SIZE), message);
}

static void __fill__(Bucket *b, char *str, char *bak)
{
    for(int i = 0; i < BUCKET_SIZE; ++i) { int position = rand() % (i+1); int c = 'a' + i%26;
                                           Bucket_insert(b, position, c); __insert__(str, c, position, bak); __check__(b, str, "insert '%c' at position=%d in \"%s\" has failed", c, position, bak); }
}

// -------------------------------------------------------------------------
// --------------------------------- TESTS ---------------------------------
// -------------------------------------------------------------------------

START_UTEST(test_insert)
{
    char str[BUCKET_SIZE+1] = "",
         bak[BUCKET_SIZE+1] = "";
    safeNEW(Bucket*, b, 1, Bucket_delete, Bucket_new()); ck_assert_ptr_ne(b[0], NULL);
    __check__(b[0], "", "test: bucket: buckets are not correctly initialised");
    {
        __fill__(b[0], str, bak);
    }
    safeDELETE(b);
}
END_UTEST

START_UTEST(test_rm)
{
    char str[BUCKET_SIZE+1] = ""; size_t length = 0;
    char bak[BUCKET_SIZE+1] = "";
    safeNEW(Bucket*, b, 1, Bucket_delete, Bucket_new()); ck_assert_ptr_ne(b[0], NULL);
    __check__(b[0], "", "test: bucket: buckets are not correctly initialised");
    {
        for(int i = 0; i < 4096; ++i)
        {
            if( length == 0
                || ( length != BUCKET_SIZE-1 && rand()%2 ) )
            {
                // random insert::
                int position = rand() % (length+1); int c = 'a' + i%26;
                Bucket_insert(b[0], position, c); __insert__(str, c, position, bak); __check__(b[0], str, "insert '%c' at position=%d in \"%s\" has failed", c, position, bak); ++length;
            }
            else
            {
                // random remove::
                int position = rand() % length;
                Bucket_remove(b[0], position); __rm__(str, position, bak); __check__(b[0], str, "remove at position=%d in \"%s\" has failed", position, bak); --length;
            } }
    }
    safeDELETE(b);
}
END_UTEST

START_UTEST(test_move)
{
    char str[BUCKET_SIZE+1] = "",
         dst[BUCKET_SIZE+1] = "",
         bak[BUCKET_SIZE+1] = "";
    safeNEW(Bucket*, b, 1, Bucket_delete, Bucket_new()); ck_assert_ptr_ne(b[0], NULL);
    __check__(b[0], "", "test: bucket: buckets are not correctly initialised");
    {
        __fill__(b[0], str, bak);
        for(int position = BUCKET_SIZE-1; position > 0; position -= 2)
        {
            safeNEW(Bucket*, d, 1, Bucket_delete, Bucket_new()); ck_assert_ptr_ne(d[0], NULL);
            __check__(d[0], "", "test: bucket: buckets are not correctly initialised");
            {
                Bucket_move(b[0], position, d[0]); __move__(str, dst, position, bak);
                __check__(b[0], str, "move from \"%s\" et pos=%d has failed", bak, position);
                __check__(d[0], dst, "move from \"%s\" et pos=%d has failed", bak, position);
            }
            safeDELETE(d);
        }
    }
    safeDELETE(b);
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(bucket)
{

    ONE_TEST_CASE(insert, test_insert);
    ONE_TEST_CASE(rm    , test_rm    );
    ONE_TEST_CASE(move  , test_move  );

}
END_SUITE_BUILDER
