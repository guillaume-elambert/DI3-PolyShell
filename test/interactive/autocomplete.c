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

#include "interactive/autocomplete.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include "misc/pattern.h"
#include "tools/macros.h"
#include "tools/dataset/dataset.h"

// #########################################################################
// #########################################################################
// #########################################################################

// -------------------------------------------------------------------------
// -------------------------------- PREPEND --------------------------------
// -------------------------------------------------------------------------

START_UTEST(ac_prepend_null)
{
    // NULL ---> NULL
    ck_assert_ptr_eq(prependHomeDir(NULL), NULL);
}
END_UTEST

START_UTEST(ac_prepend_do)
{
    // ~/xxx ---> <HOME>/xxx
    safeMALLOC(char*, p1, 1, strdup("~/fichier.txt"));
    safeMOV(p1, prependHomeDir, b1);
    {
        safeNEW(Pattern*, preg, 1, Pattern_delete, Pattern_new("((/home/.*)|(/root))/fichier.txt"));
        ck_assert_ptr_ne(preg[0], NULL);
        {
            ck_assert_ptr_ne(b1[0], p1[0]);
            ck_assert_int_ne(Pattern_match(preg[0], p1[0]), 0);
        }
        safeDELETE(preg);
    }
    safeDELETE(p1);
    // ~ ---> <HOME>
    safeMALLOC(char*, p2, 1, strdup("~"));
    safeMOV(p2, prependHomeDir, b2);
    {
        safeNEW(Pattern*, preg, 1, Pattern_delete, Pattern_new("(/home/.*)|(/root)"));
        ck_assert_ptr_ne(preg[0], NULL);
        {
            ck_assert_ptr_ne(b2[0], p2[0]);
            ck_assert_int_ne(Pattern_match(preg[0], p2[0]), 0);
        }
        safeDELETE(preg);
    }
    safeDELETE(p2);
}
END_UTEST

START_UTEST(ac_prepend_without)
{
    // xxx ---> xxx
    safeMALLOC(char*, p1, 1, strdup("/home/etudiant/fichier.txt"));
    safeMOV(p1, prependHomeDir, b1);
    {
        ck_assert_ptr_eq(b1[0], p1[0]);
    }
    safeDELETE(p1);
    // ~xxx ---> ~xxx
    safeMALLOC(char*, p2, 1, strdup("~anything"));
    safeMOV(p2, prependHomeDir, b2);
    {
        ck_assert_ptr_eq(b2[0], p2[0]);
    }
    safeDELETE(p2);
    // ~~xxx ---> ~~xxx
    safeMALLOC(char*, p3, 1, strdup("~~else"));
    safeMOV(p3, prependHomeDir, b3);
    {
        ck_assert_ptr_eq(b3[0], p3[0]);
    }
    safeDELETE(p3);
}
END_UTEST

START_UTEST(ac_prepend_escape_character)
{
    // ~~/xxx ---> ~/xxx
    safeMALLOC(char*, p1, 1, strdup("~~/fichier.txt"));
    safeMOV(p1, prependHomeDir, b1);
    {
        ck_assert_ptr_ne(b1[0], p1[0]);
        ck_assert_str_eq(p1[0], "~/fichier.txt");
    }
    safeDELETE(p1);
    // ~~ ---> ~
    safeMALLOC(char*, p2, 1, strdup("~~"));
    safeMOV(p2, prependHomeDir, b2);
    {
        ck_assert_ptr_ne(b2[0], p2[0]);
        ck_assert_str_eq(p2[0], "~");
    }
    safeDELETE(p2);
}
END_UTEST

// -------------------------------------------------------------------------
// ----------------------------- AUTOCOMPLETE ------------------------------
// -------------------------------------------------------------------------

static void __check__(const char *__prefix__, unsigned int limit, int usePath, const char **files, const char **folders, const char **binaries, int code, unsigned nbItems, const char *extend, const char **results)
{

// ---------------------------- testing dataset ----------------------------
// -------------------------------------------------------------------------

    // dataset
    safeNEW(FsTestingSet*, dataset, 1, FsTestingSet_delete, FsTestingSet_new(files, folders, binaries));
    if(!dataset[0])
          fatalError("FsTestingSet_new has failed");

    // chdir to dataset.path
    if(chdir(dataset[0]->path))
          fatalError("chdir has failed");

    // $PATH is set to dataset.bin
    maskPath( usePath && binaries ? dataset[0]->bin
                                  : "" );

    // prefix
    if(!__prefix__)
          fatalError("prefix is not set");

    // build prefix
    const char *t; safeMALLOC(char*, prefix, 1, (t=startWith(__prefix__, "$bin$", 1)) ? concatenateStrings(dataset[0]->bin, t, 0)
                                                                                      : duplicateString(__prefix__) );

    // check that prefix has been set correctly
    if(!prefix[0])
          fatalError("duplicateString or concatenateStrings has failed");

// ----------------------------- run the test ------------------------------
// -------------------------------------------------------------------------

    // outputs
    unsigned _nbItems = 0;
    safeMALLOC(char*, _extend , 1,              NULL);
    safeNEW   (Fifo*, _results, 1, Fifo_delete, NULL);

    // run autocomplete
    int _code = autocomplete(prefix[0], limit, &_nbItems, &_extend[0], &_results[0]);

    // error code
    ck_assert_msg( (_code==0) == (code==0),
                   "invalid error code (expected=%d;returned=%d)", _code, code );

    if(!code)
    {

        // nbItems is always accurate
        ck_assert_msg( nbItems == _nbItems,
                       "nbItems is not correctly set" );

        if(nbItems)
        {

            // extend xor results has to be set
            ck_assert_msg(    (  _extend[0] && !_results[0] )
                           || ( !_extend[0] &&  _results[0] ), "both or none of extend and results are set" );

// -------------------------- extend must be set ---------------------------
// -------------------------------------------------------------------------

            if(extend)
            {
                // extend is correct
                ck_assert_msg( _extend[0] && strcmp(extend, _extend[0]) == 0,
                               "extend is not correctly set" );
            }

            else
            {

                ck_assert_msg( _results[0],
                               "results is not set" );

// --------------------- exhaustive list of solutions ----------------------
// -------------------------------------------------------------------------

                if(     results
                    && *results )
                {

                    // compute the size of results
                    unsigned int remaining = 0; for(const char **ptr = results; *ptr; ++ptr, ++remaining);
                    unsigned int size      = remaining;

                    // check that all results are correct
                    for( ; !Fifo_empty(_results[0])
                         ;  Fifo_pop  (_results[0]) )
                    {
                        int found = 0; const char *elem = Fifo_front(_results[0]);
                        for(unsigned int i = 0; i < size; ++i)
                        {
                            if( results[i] && strcmp(results[i], elem) == 0 ) { results[i] = NULL; --remaining;
                                                                                found = 1; break; }
                        }
                        ck_assert_msg( found,
                                       "%s is not an expected output or has been suggested twice", elem );
                    }

                    // the list must be complete
                    if (remaining) { for(const char **ptr = results; *ptr; ++ptr) { if(*ptr)
                                                                                    ck_abort_msg("%s has not been suggested"); } }

                }

// ---------------------------- some solutions -----------------------------
// -------------------------------------------------------------------------

                else
                {

                    // suggestions must at least start with prefix
                    size_t size = 0;
                    for( ; !Fifo_empty(_results[0])
                         ;  Fifo_pop  (_results[0]), ++size )
                    {
                         const char *elem = Fifo_front(_results[0]);
                         ck_assert_msg( startWith(elem, prefix[0], 1) != NULL,
                                        "%s does not start with %s", elem, prefix[0] );
                    }

                    // the number of results is equal to max(nbItems,limit)
                    ck_assert_msg( size == ( _nbItems > limit ? limit : _nbItems ) ,
                                   "the number of results is neither equal to limit nor to nbItems" );

                } } }

// ----------------------------- no suggestion -----------------------------
// -------------------------------------------------------------------------

        else
        {

            ck_assert_ptr_eq(_extend [0], NULL);
            ck_assert_ptr_eq(_results[0], NULL);

        } }

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

    safeDELETE( _extend);
    safeDELETE(  prefix);
    safeDELETE(_results);
    safeDELETE( dataset);

}

// -------------------------------------------------------------------------
// ----------------------------- AUTOCOMPLETE ------------------------------
// -------------------------------------------------------------------------

START_UTEST(autocomplete_no_proposition)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "try", *extend = NULL, *r[] = { NULL };
    // -------------------------------
    __check__( prefix, 4U, 0, f, d, b,
               0, 0, extend, r );
}
END_UTEST

START_UTEST(autocomplete_one)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "res", *extend = "ults.txt", *r[] = { NULL };
    // -------------------------------
    __check__( prefix, 4U, 0, f, d, b,
               0, 1, extend, r );
}
END_UTEST

START_UTEST(autocomplete_several)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "data_", *extend = NULL, *r[] = { "data_1.txt", "data_2.txt", NULL };
    // -------------------------------
    __check__( prefix, 4U, 0, f, d, b,
               0, 2, extend, r );
}
END_UTEST

START_UTEST(autocomplete_in_subfolder)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "$bin$/mkfi", *extend = "fo", *r[] = { NULL };
    // -------------------------------
    __check__( prefix, 4U, 0, f, d, b,
               0, 1, extend, r );
}
END_UTEST

START_UTEST(autocomplete_unknown_subfolder)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "foo/mkfi", *extend = NULL, *r[] = { NULL };
    // -------------------------------
    __check__( prefix, 4U, 0, f, d, b,
               1, 0, extend, r );
}
END_UTEST

START_UTEST(autocomplete_partial_extend)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "dat", *extend = "a_", *r[] = { NULL };
    // -------------------------------
    __check__( prefix, 4U, 0, f, d, b,
               0, 2, extend, r );
}
END_UTEST

START_UTEST(autocomplete_one_dir)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "doc", *extend = "uments/", *r[] = { NULL };
    // -------------------------------
    __check__( prefix, 4U, 0, f, d, b,
               0, 1, extend, r );
}
END_UTEST

START_UTEST(autocomplete_several_dirs)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "d", *extend = NULL, *r[] = { "data_1.txt", "data_2.txt", "documents/", "desktop/", NULL };
    // -------------------------------
    __check__( prefix, 4U, 0, f, d, b,
               0, 4, extend, r );
}
END_UTEST

START_UTEST(autocomplete_special_directories)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *extend = "/", *r[] = { NULL };
    // -------------------------------
    __check__(    ".", 4U, 0, f, d, b,
               0, 1, extend, r );
    // -------------------------------
    __check__(    "~", 4U, 0, f, d, b,
               0, 1, extend, r );
    // -------------------------------
    __check__(   "..", 4U, 0, f, d, b,
               0, 1, extend, r );
}
END_UTEST

START_UTEST(autocomplete_exceed_lim)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "data_", *extend = NULL, *r[] = { NULL };
    // -------------------------------
    __check__( prefix, 1U, 0, f, d, b,
               0, 2, extend, r );
}
END_UTEST

START_UTEST(autocomplete_path_variable)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "mk", *extend = NULL, *r[] = { "mkmyjob.sh", "mkfifo", "mkdir", "mkfontdir", NULL };
    // -------------------------------
    __check__( prefix, 4U, 1, f, d, b,
               0, 4, extend, r );
}
END_UTEST

START_UTEST(autocomplete_double_star)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "bla**u", *extend = "es.doc", *r[] = { NULL };
    // -------------------------------
    __check__( prefix, 4U, 0, f, d, b,
               0, 1, extend, r );
}
END_UTEST

START_UTEST(autocomplete_single_star)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "bla*ues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL },
               *prefix = "bla*u", *extend = NULL, *r[] = { NULL };
    // -------------------------------
    __check__( prefix, 4U, 0, f, d, b,
               1, 0, extend, r );
}
END_UTEST

START_UTEST(autocomplete_invalid_arguments)
{
    unsigned int nbItems = 0; char *extend = NULL; Fifo *results = NULL;
    ck_assert_int_ne(autocomplete(   NULL   , 1, &nbItems, &extend, &results), 0);
    ck_assert_int_ne(autocomplete("anything", 0, &nbItems, &extend, &results), 0);
    ck_assert_int_ne(autocomplete("anything", 1,   NULL  , &extend, &results), 0);
    ck_assert_int_ne(autocomplete("anything", 1, &nbItems,   NULL , &results), 0);
    ck_assert_int_ne(autocomplete("anything", 1, &nbItems, &extend,   NULL  ), 0);
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(autocomplete)
{

    START_CASE(prepend)
    {
        ADD_TEST(ac_prepend_null);
        ADD_TEST(ac_prepend_do);
        ADD_TEST(ac_prepend_without);
        ADD_TEST(ac_prepend_escape_character);
    }
    END_CASE

    START_CASE(level1)
    {
        ADD_TEST(autocomplete_invalid_arguments);
        ADD_TEST(autocomplete_exceed_lim);
        ADD_TEST(autocomplete_no_proposition);
        ADD_TEST(autocomplete_one);
        ADD_TEST(autocomplete_several);
    }
    END_CASE

    START_CASE(level2)
    {
        ADD_TEST(autocomplete_one_dir);
        ADD_TEST(autocomplete_several_dirs);
    }
    END_CASE

    START_CASE(level3)
    {
        ADD_TEST(autocomplete_in_subfolder);
        ADD_TEST(autocomplete_unknown_subfolder);
        ADD_TEST(autocomplete_partial_extend);
    }
    END_CASE

    START_CASE(level4)
    {
        ADD_TEST(autocomplete_special_directories);
        ADD_TEST(autocomplete_single_star);
        ADD_TEST(autocomplete_double_star);
    }
    END_CASE

    START_CASE(level5)
    {
        ADD_TEST(autocomplete_path_variable);
    }
    END_CASE

}
END_SUITE_BUILDER
