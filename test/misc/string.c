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

#include <misc/string.h>

// #########################################################################
// #########################################################################
// #########################################################################

#include "tools/macros.h"

// #########################################################################
// #########################################################################
// #########################################################################

// -------------------------------------------------------------------------
// -------------------------------- STRING ---------------------------------
// -------------------------------------------------------------------------

START_UTEST(test_toLowerCase)
{
    ck_assert_uint_eq(toLowerCase('A'), 'a');
    ck_assert_uint_eq(toLowerCase('b'), 'b');
    ck_assert_uint_eq(toLowerCase('Z'), 'z');
    ck_assert_uint_eq(toLowerCase('!'), '!');
}
END_UTEST

START_UTEST(test_toUpperCase)
{
    ck_assert_uint_eq(toUpperCase('a'), 'A');
    ck_assert_uint_eq(toUpperCase('B'), 'B');
    ck_assert_uint_eq(toUpperCase('z'), 'Z');
    ck_assert_uint_eq(toUpperCase('!'), '!');
}
END_UTEST

START_UTEST(test_indexOfString)
{
    char src[] = "abcdefabfd";
    ck_assert_ptr_eq(indexOfString(src, "abc"       , 1), src    ); ck_assert_ptr_eq(indexOfString(src, "ABC"       , 0), src    );
    ck_assert_ptr_eq(indexOfString(src, "bcd"       , 1), src + 1); ck_assert_ptr_eq(indexOfString(src, "bCd"       , 0), src + 1);
    ck_assert_ptr_eq(indexOfString(src, "ab"        , 1), src    ); ck_assert_ptr_eq(indexOfString(src, "aB"        , 0), src    );
    ck_assert_ptr_eq(indexOfString(src, "abfd"      , 1), src + 6); ck_assert_ptr_eq(indexOfString(src, "abFD"      , 0), src + 6);
    ck_assert_ptr_eq(indexOfString(src, "xyz"       , 1), NULL   ); ck_assert_ptr_eq(indexOfString(src, "xYZ"       , 0), NULL   );
    ck_assert_ptr_eq(indexOfString(src, "abcdefabfd", 1), src    ); ck_assert_ptr_eq(indexOfString(src, "abCdeFAbfD", 0), src    );
}
END_UTEST

START_UTEST(test_stringLength)
{
    ck_assert_uint_eq(stringLength(""       ), 0);
    ck_assert_uint_eq(stringLength("abc"    ), 3);
    ck_assert_uint_eq(stringLength("abcdefg"), 7);
    ck_assert_uint_eq(stringLength("abcde"  ), 5);
}
END_UTEST

START_UTEST(test_copyStringWithLength)
{
    char dst[64];
    char src[] = "Le petit prince.";
    copyStringWithLength(dst, src, 64); ck_assert_str_eq(dst, src             );
    copyStringWithLength(dst, src, 17); ck_assert_str_eq(dst, src             );
    copyStringWithLength(dst, src, 15); ck_assert_str_eq(dst, "Le petit princ");
    copyStringWithLength(dst, src,  9); ck_assert_str_eq(dst, "Le petit"      );
    copyStringWithLength(dst, src,  1); ck_assert_str_eq(dst, ""              );
}
END_UTEST

static void __test_duplicateString__(const char *src)
{
    safeMALLOC(char*, t, 1, duplicateString(src));
    {
        ck_assert_ptr_ne(NULL, t[0]);
        ck_assert_ptr_ne( src, t[0]);
        ck_assert_str_eq( src, t[0]);
    }
    safeDELETE(t);
}

START_UTEST(test_duplicateString)
{
    char buffer[1024];
    for(unsigned int i = 0; i < 1024U; ++i)
    {
        for(unsigned j = 0; j < i; ++j) { buffer[j] = 'a' + j % 26; }
        buffer[i] = '\0'; __test_duplicateString__(buffer);
    }
    __test_duplicateString__("abc");
    __test_duplicateString__("cba");
}
END_UTEST

START_UTEST(test_findFirst)
{
    char src[] = "Le petit prince.";
    ck_assert_ptr_eq(findFirst(src, "!tib"   ), src  +  5);
    ck_assert_ptr_eq(findFirst(src, "xy.z"   ), src  + 15);
    ck_assert_ptr_eq(findFirst(src, "l pLpsq"), src      );
    ck_assert_ptr_eq(findFirst(src, "azvm;*s"), NULL     );
}
END_UTEST

START_UTEST(test_findLast)
{
    char src[] = "Le petit prince.";
    ck_assert_ptr_eq(findLast(src, ' '), src  + 8 );
    ck_assert_ptr_eq(findLast(src, 'L'), src      );
    ck_assert_ptr_eq(findLast(src, '.'), src  + 15);
    ck_assert_ptr_eq(findLast(src, '?'), NULL     );
    ck_assert_ptr_eq(findLast(src, 'x'), NULL     );
}
END_UTEST

START_UTEST(test_stringCompare)
{
    ck_assert_int_ne(stringCompare("abc" , "def" ), 0);
    ck_assert_int_ne(stringCompare("abc" , "abcd"), 0);
    ck_assert_int_ne(stringCompare("abcd", "abc" ), 0);
    ck_assert_int_eq(stringCompare("abc" , "abc" ), 0);
    ck_assert_int_eq(stringCompare(""    , ""    ), 0);
    ck_assert_int_ne(stringCompare(""    , "abcd"), 0);
    ck_assert_int_lt(stringCompare("abc" , "abcd"), 0);
    ck_assert_int_gt(stringCompare("abcd", "abc" ), 0);
    ck_assert_int_lt(stringCompare(""    , "abcd"), 0);
    ck_assert_int_ge(stringCompare("abc" , ""    ), 0);
}
END_UTEST

START_UTEST(test_concatenateStrings)
{
    char src1[] = "abc";
    char src2[] = "def";
    safeMALLOC(char*, r1, 1, concatenateStrings(src1, src2, 0));
    {
        ck_assert_ptr_ne(r1[0],   NULL  );
        ck_assert_ptr_ne(r1[0],   src1  );
        ck_assert_ptr_ne(r1[0],   src2  );
        ck_assert_str_eq(r1[0], "abcdef");
    }
    safeDELETE(r1);
    safeMALLOC(char*, r2, 1, concatenateStrings("unAutreExemple", "", 0));
    {
        ck_assert_ptr_ne(r2[0],      NULL       );
        ck_assert_str_eq(r2[0], "unAutreExemple");
    }
    safeDELETE(r2);
    const size_t destsize = 8192;
    safeMALLOC(char*, r3, 1, concatenateStrings("", "oh", destsize));
    {
        ck_assert_ptr_ne(r3[0], NULL);
        ck_assert_str_eq(r3[0], "oh");
        for(size_t iwrite = 0; iwrite < destsize; ++iwrite) { // try to check minDestSize
                                                              r3[0][iwrite] = 'a'; }
    }
    safeDELETE(r3);
}
END_UTEST

START_UTEST(test_mkReverse)
{
    char src[] = "abc";
    ck_assert_ptr_eq(mkReverse(src), src); ck_assert_str_eq(src, "cba");
    ck_assert_ptr_eq(mkReverse(src), src); ck_assert_str_eq(src, "abc");
}
END_UTEST

START_UTEST(test_startWith)
{
    const char* src = "abcdef";
    ck_assert_ptr_eq(startWith(src, "abc"    , 1), src + 3);
    ck_assert_ptr_eq(startWith(src, "abcdef" , 1), src + 6);
    ck_assert_ptr_eq(startWith(src, "Abc"    , 1), NULL   );
    ck_assert_ptr_eq(startWith(src, "xyu"    , 1), NULL   );
    ck_assert_ptr_eq(startWith(src, "abcdefg", 1), NULL   );
    ck_assert_ptr_eq(startWith(src, "aBC"    , 0), src + 3);
    ck_assert_ptr_eq(startWith(src, "aBcDEF" , 0), src + 6);
    ck_assert_ptr_eq(startWith(src, "aBcdEFG", 0), NULL   );
}
END_UTEST

START_UTEST(test_belongs)
{
    ck_assert_int_ne(belongs('f', "abcdef"   ), 0);
    ck_assert_int_eq(belongs('B', "abcdef"   ), 0);
    ck_assert_int_ne(belongs('?', "quoi?qui!"), 0);
}
END_UTEST

START_UTEST(test_subString)
{
    const char * str = "abcdef";
    safeMALLOC(char*, s1, 1, subString(  str, 0)); ck_assert_ptr_ne(NULL, s1[0]); ck_assert_ptr_ne(str, s1[0]); ck_assert_str_eq(s1[0], ""      ); safeDELETE(s1);
    safeMALLOC(char*, s2, 1, subString(  str, 6)); ck_assert_ptr_ne(NULL, s2[0]); ck_assert_ptr_ne(str, s2[0]); ck_assert_str_eq(s2[0], "abcdef"); safeDELETE(s2);
    safeMALLOC(char*, s3, 1, subString(++str, 1)); ck_assert_ptr_ne(NULL, s3[0]); ck_assert_ptr_ne(str, s3[0]); ck_assert_str_eq(s3[0], "b"     ); safeDELETE(s3);
}
END_UTEST

START_UTEST(test_mkCommon)
{
    char result[] = "/un/super/dossier/de/la/mort/qui/tue/fichier.txt";
    mkCommon(result, /* autocomplete */ "/un/super/dossier/"); ck_assert_str_eq(result, "/un/super/dossier/");
    mkCommon(result, /* autocomplete */ "/un/super/"        ); ck_assert_str_eq(result, "/un/super/"        );
    mkCommon(result, /* autocomplete */ "/un/super/dossier/"); ck_assert_str_eq(result, "/un/super/"        );
    mkCommon(result, /* autocomplete */ "/un/"              ); ck_assert_str_eq(result, "/un/"              );
}
END_UTEST

START_UTEST(test_isNotEmpty)
{
    ck_assert_int_ne(isNotEmpty("abc_"), 0);
    ck_assert_int_ne(isNotEmpty(" abc"), 0);
    ck_assert_int_ne(isNotEmpty("abc "), 0);
    ck_assert_int_ne(isNotEmpty(" !  "), 0);
    ck_assert_int_ne(isNotEmpty("  v "), 0);
    ck_assert_int_eq(isNotEmpty("    "), 0);
    ck_assert_int_eq(isNotEmpty(""    ), 0);
}
END_UTEST

START_UTEST(test_getProtString)
{
    char s1[] = "~/fichier.txt";              safeMALLOC(char*, r1, 1, getProtString(s1, '~')); ck_assert_ptr_ne(r1[0], NULL); ck_assert_ptr_ne(s1, r1[0]); ck_assert_str_eq(r1[0], "~~/fichier.txt"             ); safeDELETE(r1);
    char s2[] = "une commande # commentaire"; safeMALLOC(char*, r2, 1, getProtString(s2, '#')); ck_assert_ptr_ne(r2[0], NULL); ck_assert_ptr_ne(s2, r2[0]); ck_assert_str_eq(r2[0], "une commande ## commentaire"); safeDELETE(r2);
    char s3[] = "une # etrange ## cmd";       safeMALLOC(char*, r3, 1, getProtString(s3, '#')); ck_assert_ptr_ne(r3[0], NULL); ck_assert_ptr_ne(s3, r3[0]); ck_assert_str_eq(r3[0], "une ## etrange #### cmd"    ); safeDELETE(r3);
    char s4[] = "absolutely nothing to do";   safeMALLOC(char*, r4, 1, getProtString(s4, '!')); ck_assert_ptr_ne(r4[0], NULL); ck_assert_ptr_ne(s4, r4[0]); ck_assert_str_eq(r4[0], "absolutely nothing to do"   ); safeDELETE(r4);
}
END_UTEST

START_UTEST(test_getRealString)
{
    char *first = NULL;
    char s1[] = "~/fichier.txt";              safeMALLOC(char*, r1, 1, getRealString(s1, '~', &first)); ck_assert_ptr_ne(r1[0], NULL); ck_assert_ptr_ne(s1, r1[0]); ck_assert_ptr_eq(first, r1[0]     ); ck_assert_str_eq(r1[0], "~/fichier.txt"             ); safeDELETE(r1);
    char s2[] = "une commande # commentaire"; safeMALLOC(char*, r2, 1, getRealString(s2, '#', &first)); ck_assert_ptr_ne(r2[0], NULL); ck_assert_ptr_ne(s2, r2[0]); ck_assert_ptr_eq(first, r2[0] + 13); ck_assert_str_eq(r2[0], "une commande # commentaire"); safeDELETE(r2);
    char s3[] = "une # etrange ## cmd";       safeMALLOC(char*, r3, 1, getRealString(s3, '#', &first)); ck_assert_ptr_ne(r3[0], NULL); ck_assert_ptr_ne(s3, r3[0]); ck_assert_ptr_eq(first, r3[0] + 4 ); ck_assert_str_eq(r3[0], "une # etrange # cmd"       ); safeDELETE(r3);
    char s4[] = "absolutely nothing to do";   safeMALLOC(char*, r4, 1, getRealString(s4, '!', &first)); ck_assert_ptr_ne(r4[0], NULL); ck_assert_ptr_ne(s4, r4[0]); ck_assert_ptr_eq(first,    NULL   ); ck_assert_str_eq(r4[0], "absolutely nothing to do"  ); safeDELETE(r4);
    char s5[] = "qqc de ## parfait";          safeMALLOC(char*, r5, 1, getRealString(s5, '#', &first)); ck_assert_ptr_ne(r5[0], NULL); ck_assert_ptr_ne(s5, r5[0]); ck_assert_ptr_eq(first,    NULL   ); ck_assert_str_eq(r5[0], "qqc de # parfait"          ); safeDELETE(r5);
}
END_UTEST

// -------------------------------------------------------------------------
// ------------------------------- TOKENIZER -------------------------------
// -------------------------------------------------------------------------

START_UTEST(tokenize_un_seul_seprateur)
{
    char str[] = "cas;vraiment;facile";
    safeNEW(Tokenizer*, tokenizer, 1, Tokenizer_delete, Tokenizer_new(str, ";"));
    {
        ck_assert_ptr_ne(tokenizer, NULL);
        ck_assert_int_eq(Tokenizer_isOver(tokenizer[0]), 0); safeMALLOC(char*, e1, 1, Tokenizer_get(tokenizer[0])); ck_assert_ptr_ne(e1[0], NULL); ck_assert_ptr_ne(e1[0], str + 0 ); ck_assert_str_eq(e1[0], "cas"     ); safeDELETE(e1); Tokenizer_next(tokenizer[0]);
        ck_assert_int_eq(Tokenizer_isOver(tokenizer[0]), 0); safeMALLOC(char*, e2, 1, Tokenizer_get(tokenizer[0])); ck_assert_ptr_ne(e2[0], NULL); ck_assert_ptr_ne(e2[0], str + 4 ); ck_assert_str_eq(e2[0], "vraiment"); safeDELETE(e2); Tokenizer_next(tokenizer[0]);
        ck_assert_int_eq(Tokenizer_isOver(tokenizer[0]), 0); safeMALLOC(char*, e3, 1, Tokenizer_get(tokenizer[0])); ck_assert_ptr_ne(e3[0], NULL); ck_assert_ptr_ne(e3[0], str + 13); ck_assert_str_eq(e3[0], "facile"  ); safeDELETE(e3); Tokenizer_next(tokenizer[0]);
        ck_assert_int_ne(Tokenizer_isOver(tokenizer[0]), 0);
    }
    safeDELETE(tokenizer);
}
END_UTEST

START_UTEST(tokenize_normal)
{
    char str[] = "((un-cas(assez)standard))";
    safeNEW(Tokenizer*, tokenizer, 1, Tokenizer_delete, Tokenizer_new(str, "(-)"));
    {
        ck_assert_ptr_ne(tokenizer, NULL);
        ck_assert_int_eq(Tokenizer_isOver(tokenizer[0]), 0); safeMALLOC(char*, e1, 1, Tokenizer_get(tokenizer[0])); ck_assert_ptr_ne(e1[0], NULL); ck_assert_ptr_ne(e1[0], str + 0 ); ck_assert_str_eq(e1[0], "un"      ); safeDELETE(e1); Tokenizer_next(tokenizer[0]);
        ck_assert_int_eq(Tokenizer_isOver(tokenizer[0]), 0); safeMALLOC(char*, e2, 1, Tokenizer_get(tokenizer[0])); ck_assert_ptr_ne(e2[0], NULL); ck_assert_ptr_ne(e2[0], str + 3 ); ck_assert_str_eq(e2[0], "cas"     ); safeDELETE(e2); Tokenizer_next(tokenizer[0]);
        ck_assert_int_eq(Tokenizer_isOver(tokenizer[0]), 0); safeMALLOC(char*, e3, 1, Tokenizer_get(tokenizer[0])); ck_assert_ptr_ne(e3[0], NULL); ck_assert_ptr_ne(e3[0], str + 7 ); ck_assert_str_eq(e3[0], "assez"   ); safeDELETE(e3); Tokenizer_next(tokenizer[0]);
        ck_assert_int_eq(Tokenizer_isOver(tokenizer[0]), 0); safeMALLOC(char*, e4, 1, Tokenizer_get(tokenizer[0])); ck_assert_ptr_ne(e4[0], NULL); ck_assert_ptr_ne(e4[0], str + 13); ck_assert_str_eq(e4[0], "standard"); safeDELETE(e4); Tokenizer_next(tokenizer[0]);
        ck_assert_int_ne(Tokenizer_isOver(tokenizer[0]), 0);
    }
    safeDELETE(tokenizer);
}
END_UTEST

START_UTEST(tokenize_aucun_seprateur)
{
    char str[] = "au(cas)ou";
    safeNEW(Tokenizer*, tokenizer, 1, Tokenizer_delete, Tokenizer_new(str, ""));
    {
        ck_assert_ptr_ne(tokenizer, NULL);
        ck_assert_int_eq(Tokenizer_isOver(tokenizer[0]), 0); safeMALLOC(char*, e1, 1, Tokenizer_get(tokenizer[0])); ck_assert_ptr_ne(e1[0], NULL); ck_assert_ptr_ne(e1[0], str); ck_assert_str_eq(e1[0], str); safeDELETE(e1); Tokenizer_next(tokenizer[0]);
        ck_assert_int_ne(Tokenizer_isOver(tokenizer[0]), 0);
    }
    safeDELETE(tokenizer);
}
END_UTEST

START_UTEST(tokenize_str_vide_ou_arg_null)
{
    safeNEW(Tokenizer*, t1, 1, Tokenizer_delete, Tokenizer_new(   NULL   , "anything")); ck_assert_ptr_eq(t1[0], NULL);                                               safeDELETE(t1);
    safeNEW(Tokenizer*, t2, 1, Tokenizer_delete, Tokenizer_new(    ""    , "anything")); ck_assert_ptr_ne(t2[0], NULL); ck_assert_int_ne(Tokenizer_isOver(t2[0]), 0); safeDELETE(t2);
    safeNEW(Tokenizer*, t3, 1, Tokenizer_delete, Tokenizer_new("anything",    NULL   )); ck_assert_ptr_eq(t3[0], NULL);                                               safeDELETE(t3);
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(string)
{

    ONE_TEST_CASE(toLowerCase         , test_toLowerCase         );
    ONE_TEST_CASE(toUpperCase         , test_toUpperCase         );
    ONE_TEST_CASE(stringLength        , test_stringLength        );
    ONE_TEST_CASE(copyStringWithLength, test_copyStringWithLength);
    ONE_TEST_CASE(duplicateString     , test_duplicateString     );
    ONE_TEST_CASE(stringCompare       , test_stringCompare       );
    ONE_TEST_CASE(concatenateStrings  , test_concatenateStrings  );
    ONE_TEST_CASE(belongs             , test_belongs             );
    ONE_TEST_CASE(mkCommon            , test_mkCommon            );
    ONE_TEST_CASE(findLast            , test_findLast            );
    ONE_TEST_CASE(mkReverse           , test_mkReverse           );
    ONE_TEST_CASE(startWith           , test_startWith           );
    ONE_TEST_CASE(subString           , test_subString           );
    ONE_TEST_CASE(findFirst           , test_findFirst           );
    ONE_TEST_CASE(isNotEmpty          , test_isNotEmpty          );
    ONE_TEST_CASE(indexOfString       , test_indexOfString       );
    ONE_TEST_CASE(getProtString       , test_getProtString       );
    ONE_TEST_CASE(getRealString       , test_getRealString       );

    START_CASE(tokenize)
    {
        ADD_TEST(tokenize_un_seul_seprateur);
        ADD_TEST(tokenize_normal);
    }
    END_CASE

    START_CASE(tokenize_special_cases)
    {
        ADD_TEST(tokenize_aucun_seprateur);
        ADD_TEST(tokenize_str_vide_ou_arg_null);
    }
    END_CASE

}
END_SUITE_BUILDER
