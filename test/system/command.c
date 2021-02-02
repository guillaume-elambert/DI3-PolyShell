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

#include "system/command.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include "tools/macros.h"
#include "tools/dataset/dataset.h"

// #########################################################################
// #########################################################################
// #########################################################################

// -------------------------------------------------------------------------
// --------------------------------- TOOLS ---------------------------------
// -------------------------------------------------------------------------

static CmdMember* __new__(const char *base)
{
    // leak-proof
    CmdMember *mbr = CmdMember_new(base); ck_assert_ptr_ne(mbr, NULL); return mbr;
}

static int __mbr_equals__(const CmdMember *mbr1, const CmdMember *mbr2)
{
    if(mbr1->status              != mbr2->status             ) return 1;
    if(mbr1->base                != mbr2->base               ) return 1;
    if(mbr1->options             != mbr2->options            ) return 1;
    if(mbr1->nbOptions           != mbr2->nbOptions          ) return 1;
    if(mbr1->capacityOption      != mbr2->capacityOption     ) return 1;
    if(mbr1->redirections[0]     != mbr2->redirections[0]    ) return 1;
    if(mbr1->redirections[1]     != mbr2->redirections[1]    ) return 1;
    if(mbr1->redirections[2]     != mbr2->redirections[2]    ) return 1;
    if(mbr1->redirectionTypes[0] != mbr2->redirectionTypes[0]) return 1;
    if(mbr1->redirectionTypes[1] != mbr2->redirectionTypes[1]) return 1;
    if(mbr1->redirectionTypes[2] != mbr2->redirectionTypes[2]) return 1;
    if(mbr1->next                != mbr2->next               ) return 1;
    if(mbr1->prev                != mbr2->prev               ) return 1;
    return 0;
}

static void __check_init__(const char *base)
{
    CmdMember *mbr = __new__(base);
    {
        ck_assert_int_ne (mbr->status              ,       0   );
        ck_assert_ptr_ne (mbr->base                ,      base );
        ck_assert_str_eq (mbr->base                ,      base );
        ck_assert_ptr_ne (mbr->options             ,      NULL );
        ck_assert_uint_eq(mbr->nbOptions           ,         1 );
        ck_assert_uint_ge(mbr->capacityOption      ,         1 );
        ck_assert_ptr_ne (mbr->options[0]          ,      base );
        ck_assert_ptr_ne (mbr->options[0]          , mbr->base );
        ck_assert_str_eq (mbr->options[0]          ,      base );
        ck_assert_ptr_eq (mbr->redirections[0]     ,      NULL );
        ck_assert_ptr_eq (mbr->redirections[1]     ,      NULL );
        ck_assert_ptr_eq (mbr->redirections[1]     ,      NULL );
        ck_assert        (mbr->redirectionTypes[0] == UNDEFINED);
        ck_assert        (mbr->redirectionTypes[1] == UNDEFINED);
        ck_assert        (mbr->redirectionTypes[2] == UNDEFINED);
        ck_assert_ptr_eq (mbr->next                ,      NULL );
        ck_assert_ptr_eq (mbr->prev                ,      NULL );
    }
    CmdMember_flushLivingCmdMembers();
}

static void __check_redirect__( // NORMAL or APPEND or FUSION only
                                RedirectionType type )
{
    if(    type != NORMAL
        && type != APPEND
        && type != FUSION ) ck_abort_msg("type not supported");
    CmdMember *mbr = __new__("solver");
    CmdMember  cpy = *mbr;
    {
        const char *fichiers[2][3] = { { "/home/lara/in.txt", "/home/lara/out.txt", "/home/lara/error.txt" },
                                       { "/home/toto/in.txt", "/home/toto/out.txt", "/home/toto/error.txt" }  };
        for(int it = 0; it < 2; ++it)
        {
            for(int fd = type == NORMAL ? 0 :
                         type == APPEND ? 1 :
                                          2 ; fd < 3; ++fd)
            {
                CmdMember *ret = type == NORMAL ? CmdMember_redirect      (mbr, fd, fichiers[it][fd]) :
                                 type == APPEND ? CmdMember_appendRedirect(mbr, fd, fichiers[it][fd]) :
                                                  CmdMember_mergeOutputs  (mbr                      ) ;
                // mbr status redirectionTypes
                ck_assert_ptr_eq(mbr                       ,  ret );
                ck_assert_int_ne(mbr->status               ,   0  );
                ck_assert       (mbr->redirectionTypes[fd] == type);
                // redirections
                if(    type
                    == FUSION ) { ck_assert_ptr_eq(mbr->redirections[fd],      NULL       ); }
                           else { ck_assert_ptr_ne(mbr->redirections[fd], fichiers[it][fd]);
                                  ck_assert_str_eq(mbr->redirections[fd], fichiers[it][fd]); }
            } }
        // other attributes
        for(int fd = 0; fd < 3; ++fd) { cpy.redirections    [fd] = mbr->redirections    [fd];
                                        cpy.redirectionTypes[fd] = mbr->redirectionTypes[fd]; }
        ck_assert_msg( __mbr_equals__(mbr, &cpy) == 0,
                       "unexpected attributes have been modified by %s", type == NORMAL ? "CmdMember_redirect"       :
                                                                         type == APPEND ? "CmdMember_appendRedirect" :
                                                                                          "CmdMember_mergeOutputs"     );
    }
    CmdMember_flushLivingCmdMembers();
}

static void __check_add_option__(const char *arg, int expend, const char **files, const char **folders, const char **binaries, const char **result)
{

    CmdMember *mbr = __new__("notsu"),
               cpy = *mbr;

// ------------------------------- easy case -------------------------------
// -------------------------------------------------------------------------

    if(    !expend
        || !arg )
    {
        // add option
        CmdMember *ret = CmdMember_addOption(mbr, arg, expend);
        ck_assert_ptr_eq(mbr, ret);
        // check the result
                  ck_assert_uint_eq(mbr->nbOptions     ,  2  );
                  ck_assert_uint_ge(mbr->capacityOption,  2  );
        if(arg) { ck_assert_ptr_ne (mbr->options[1]    , arg );
                  ck_assert_str_eq (mbr->options[1]    , arg ); }
           else { ck_assert_ptr_eq (mbr->options[1]    , NULL); }
    }

// ------------------------------- hard case -------------------------------
// -------------------------------------------------------------------------

    else
    {

        // arguments
        if( !files    || !folders
         || !binaries || !result  )
              fatalError("invalid argument");

        // dataset
        safeNEW(FsTestingSet*, dataset, 1, FsTestingSet_delete, FsTestingSet_new(files, folders, binaries));
        if(!dataset[0])
              fatalError("FsTestingSet_new has failed");

        // chdir to dataset.path
        if(chdir(dataset[0]->path))
              fatalError("chdir has failed");

        // nbOptions
        unsigned int size = 0;
        for(const char **ptr = result; *ptr; ++ptr, ++size);

        // add option
        CmdMember *ret = CmdMember_addOption(mbr, arg, expend);
        ck_assert_ptr_eq(mbr, ret);

        // number of options
        ck_assert_uint_eq(mbr->nbOptions     , 1U+size);
        ck_assert_uint_ge(mbr->capacityOption, 1U+size);

        // duplicate bin
        safeMALLOC(char*, bin, 1, duplicateString( dataset[0]->bin
                                                   // remove parent directory             // remove trailing /
                                                   + stringLength(dataset[0]->path) ));   bin[0][stringLength(bin[0])-1] = '\0';

        // results
        for(unsigned int ires = 0; ires < size; ++ires)
        {

            int found = 0; const char *tmp;

            // replace $bin$ by dataset.bin if necessary
            safeMALLOC(char*, elem, 1, (tmp=startWith(result[ires], "$bin$", 1)) ? concatenateStrings(bin[0], tmp, 0)
                                                                                 : duplicateString(result[ires] ));

            // check that elem has been set correctly
            if(!elem[0])
                  fatalError("duplicateString or concatenateStrings has failed");

            for(unsigned int iopt = 1; iopt < mbr->nbOptions; ++iopt)
            {

                // remove ./ if necessary
                const char *opt = (tmp=startWith(mbr->options[iopt], "./", 1)) ? tmp
                                                                               : mbr->options[iopt];

                // search each elem of result in mbr->options
                if( !strcmp(elem[0],
                            opt       )) ++found;

            }

            ck_assert_msg( found == 1U,
                           "%s has not been found or has been found twice", elem );

            safeDELETE(elem);

        }

        // deallocate memory
        safeDELETE(bin);
        safeDELETE(dataset);

    }

// --------------------------------- both ----------------------------------
// -------------------------------------------------------------------------

    cpy.nbOptions      = mbr->nbOptions     ;
    cpy.capacityOption = mbr->capacityOption;
    cpy.options        = mbr->options       ;
    ck_assert_msg( __mbr_equals__(mbr, &cpy) == 0,
                   "unexpected attributes have been modified by CmdMember_addOption" );

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

    CmdMember_flushLivingCmdMembers();

}

// -------------------------------------------------------------------------
// --------------------------------- TESTS ---------------------------------
// -------------------------------------------------------------------------

START_UTEST(test_init)
{
    __check_init__("ls");
    __check_init__("mkdir");
}
END_UTEST

START_UTEST(test_living_mbrs)
{
    CmdMember* mbrs[1024];
    {
        for(unsigned int i = 0; i < 1024; ++i) { mbrs[i] = __new__("any"); ck_assert_uint_eq(livingCmdMembersSize    ,  i+1   );
                                                                           ck_assert_uint_ge(livingCmdMembersCapacity,  i+1   );
                                                                           ck_assert_ptr_eq (livingCmdMembers[i]     , mbrs[i]); }
    }
    CmdMember_flushLivingCmdMembers();
    ck_assert_uint_eq(livingCmdMembersSize, 0);
}
END_UTEST

START_UTEST(test_redirect)
{
    __check_redirect__(NORMAL);
}
END_UTEST

START_UTEST(test_appendRedirect)
{
    __check_redirect__(APPEND);
}
END_UTEST

START_UTEST(test_mergeOutputs)
{
    __check_redirect__(FUSION);
}
END_UTEST

START_UTEST(test_pipe)
{
    CmdMember *mbr1 = __new__("base1"),
              *mbr2 = __new__("base2");
    {
        close(STDERR_FILENO);
        CmdMember *ret = CmdMember_pipe(mbr1, mbr2); ck_assert_uint_ne(mbr1->status, 0); ck_assert_ptr_eq(mbr1->next, mbr2);
                                                     ck_assert_uint_ne(mbr2->status, 0); ck_assert_ptr_eq(mbr2->prev, mbr1); ck_assert_ptr_eq(ret, mbr2);
                   ret = CmdMember_pipe(mbr1, mbr2); ck_assert_uint_eq(mbr1->status, 0);
                                                     ck_assert_uint_eq(mbr2->status, 0);                                     ck_assert_ptr_eq(ret, mbr2);
    }
    CmdMember_flushLivingCmdMembers();
}
END_UTEST

START_UTEST(test_nb_members)
{
    Command *cmd1 = __new__("base1"),
            *cmd2 = __new__("base2"),
            *cmd3 = __new__("base3"),
            *cmd4 = __new__("base4"),
            *cmd5 = __new__("base5");
    {
        CmdMember_pipe(CmdMember_pipe(CmdMember_pipe(CmdMember_pipe( // cmd1 | cmd2 | ... | cmd5
                                                                     cmd1, cmd2), cmd3), cmd4), cmd5);
        ck_assert_uint_eq(Command_getNbMember(cmd1), 5);
        ck_assert_uint_eq(Command_getNbMember(cmd2), 4);
        ck_assert_uint_eq(Command_getNbMember(cmd3), 3);
        ck_assert_uint_eq(Command_getNbMember(cmd4), 2);
        ck_assert_uint_eq(Command_getNbMember(cmd5), 1);
    }
    CmdMember_flushLivingCmdMembers();
}
END_UTEST

START_UTEST(test_add_opt_l1)
{
    __check_add_option__("argument", 0, NULL, NULL, NULL, NULL);
    __check_add_option__("arg*me*t", 0, NULL, NULL, NULL, NULL);
    __check_add_option__(NULL      , 0, NULL, NULL, NULL, NULL);
    __check_add_option__(NULL      , 1, NULL, NULL, NULL, NULL);
}
END_UTEST

START_UTEST(test_add_opt_l2)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "blagues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL };
    // 1
    const char *arg1 = "*", *r1[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "blagues.doc", "documents", "desktop", "$bin$", ".secret", ".ssh", NULL };
    __check_add_option__(arg1, 1, f, d, b, r1);
    // 2
    const char *arg2 = "d*", *r2[] = { "data_1.txt", "data_2.txt", "documents", "desktop", NULL };
    __check_add_option__(arg2, 1, f, d, b, r2);
    // 3
    const char *arg3 = "*.txt", *r3[] = { "data_1.txt", "data_2.txt", "results.txt", NULL };
    __check_add_option__(arg3, 1, f, d, b, r3);
}
END_UTEST

START_UTEST(test_add_opt_l3)
{
    const char *f[] = { "data_1.txt", "data_2.txt", "results.txt", "mkmyjob.sh", "blagues.doc", ".secret", NULL },
               *d[] = { "documents", "desktop", ".ssh", NULL }, *b[] = { "find", "mkfifo", "mkdir", "mkfontdir", NULL };
    // 1
    const char *arg1 = "bin.*/m*", *r1[] = { "$bin$/mkfifo", "$bin$/mkdir", "$bin$/mkfontdir", NULL };
    __check_add_option__(arg1, 1, f, d, b, r1);
    // 2
    const char *arg2 = ".*", *r2[] = { ".secret", ".ssh", NULL };
    __check_add_option__(arg2, 1, f, d, b, r2);
    // 3
    const char *arg3 = "bin.*/*fi*", *r3[] = { "$bin$/find", "$bin$/mkfifo", NULL };
    __check_add_option__(arg3, 1, f, d, b, r3);
}
END_UTEST

START_UTEST(test_status)
{
    CmdMember *mbr1 = __new__("base1"),
              *mbr2 = __new__("base2"),
               cpy1 = *mbr1          ;
    // mbr2 is dangerous
    mbr2->status = cpy1.status = 0;
    {
        CmdMember_pipe          (mbr1, mbr2         ); ck_assert_msg(__mbr_equals__(mbr1, &cpy1) == 0, "status is not correctly propagated in CmdMember_pipe");
        CmdMember_redirect      (mbr1, 0, "anything"); ck_assert_msg(__mbr_equals__(mbr1, &cpy1) == 0, "status is ignored by CmdMember_redirect"             );
        CmdMember_redirect      (mbr1, 1, "anything"); ck_assert_msg(__mbr_equals__(mbr1, &cpy1) == 0, "status is ignored by CmdMember_redirect"             );
        CmdMember_redirect      (mbr1, 2, "anything"); ck_assert_msg(__mbr_equals__(mbr1, &cpy1) == 0, "status is ignored by CmdMember_redirect"             );
        CmdMember_appendRedirect(mbr1, 1, "anything"); ck_assert_msg(__mbr_equals__(mbr1, &cpy1) == 0, "status is ignored by CmdMember_appendRedirect"       );
        CmdMember_appendRedirect(mbr1, 2, "anything"); ck_assert_msg(__mbr_equals__(mbr1, &cpy1) == 0, "status is ignored by CmdMember_appendRedirect"       );
        CmdMember_mergeOutputs  (mbr1               ); ck_assert_msg(__mbr_equals__(mbr1, &cpy1) == 0, "status is ignored by CmdMember_mergeOutputs"         );
        CmdMember_addOption     (mbr1, "args", 0    ); ck_assert_msg(__mbr_equals__(mbr1, &cpy1) == 0, "status is ignored by CmdMember_addOption"            );
        CmdMember_addOption     (mbr1, "args", 1    ); ck_assert_msg(__mbr_equals__(mbr1, &cpy1) == 0, "status is ignored by CmdMember_addOption"            );
        CmdMember_addOption     (mbr1, "ar*s", 1    ); ck_assert_msg(__mbr_equals__(mbr1, &cpy1) == 0, "status is ignored by CmdMember_addOption"            );
    }
    CmdMember_flushLivingCmdMembers();
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(command)
{

    ONE_TEST_CASE(init          , test_init          );
    ONE_TEST_CASE(living_mbrs   , test_living_mbrs   );
    ONE_TEST_CASE(redirect      , test_redirect      );
    ONE_TEST_CASE(appendRedirect, test_appendRedirect);
    ONE_TEST_CASE(mergeOutputs  , test_mergeOutputs  );
    ONE_TEST_CASE(pipe          , test_pipe          );
    ONE_TEST_CASE(nb_members    , test_nb_members    );
    ONE_TEST_CASE(add_opt_l1    , test_add_opt_l1    );
    ONE_TEST_CASE(add_opt_l2    , test_add_opt_l2    );
    ONE_TEST_CASE(add_opt_l3    , test_add_opt_l3    );
    ONE_TEST_CASE(status        , test_status        );

}
END_SUITE_BUILDER
