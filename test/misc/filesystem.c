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

#include "misc/filesystem.h"

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

static void __iterate_file__(const char *filename, unsigned long *read)
{
    char buffer[128]; char *endptr;
    __s_new__(FILE*, file, 1, fclose, int, FILE*, fopen(filename, "r"));
    {
        ck_assert_ptr_ne(file[0], NULL);
        // the first line is read manually
        ck_assert_ptr_ne(fgets(buffer, 128, file[0]), NULL);
        *read += strtoul(buffer, NULL, 10);
        // other lines are read with the iterator
        safeNEW(FileIterator*, fileIterator, 1, FileIterator_delete, FileIterator_new(file[0]));
        {
            ck_assert_ptr_ne(fileIterator[0], NULL);
            while(!FileIterator_isOver(fileIterator[0])) { *read += strtoul(FileIterator_get(fileIterator[0]), &endptr, 10);
                                                            ck_assert_msg(    *endptr ==  0
                                                                           || *endptr == ' ', "comments are not correctly removed");
                                                            FileIterator_next(fileIterator[0]); }
        }
        safeDELETE(fileIterator);
    }
    safeDELETE(file);

}

static void __iterate_folder__(int skip, int inspectFiles)
{

    const char *files  [] = { "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", NULL },
               *folders[] = { "d1", "d2"                                          , NULL };

    safeNEW(FsTestingSet*, dataset, 1, FsTestingSet_delete, FsTestingSet_new(files, folders, NULL));
    if(!dataset[0]) fatalError("FsTestingSet_new has failed");

    safeNEW(FolderIterator*, fIterator, 1, FolderIterator_delete, FolderIterator_new(dataset[0]->path, skip))
    ck_assert_ptr_ne(fIterator[0], NULL);

    int exists[13] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    if(skip) { exists[9] = 0; exists[10] = 0; }

    unsigned long read     = 0,
                  expected = dataset[0]->csum;

    // explore dataset.path
    while(!FolderIterator_isOver(fIterator[0]))
    {

        const char *name = FolderIterator_get(fIterator[0]);
        ck_assert_ptr_ne(name, NULL);

             if (stringCompare(name, "f0") == 0) { ck_assert_msg(exists[0 ], "f0 file is discovered twice"); exists[0 ] = 0; ck_assert_int_eq(FolderIterator_isDir(fIterator[0]), 0); if(inspectFiles) __iterate_file__(dataset[0]->tfiles[0], &read); }
        else if (stringCompare(name, "f1") == 0) { ck_assert_msg(exists[1 ], "f1 file is discovered twice"); exists[1 ] = 0; ck_assert_int_eq(FolderIterator_isDir(fIterator[0]), 0); if(inspectFiles) __iterate_file__(dataset[0]->tfiles[1], &read); }
        else if (stringCompare(name, "f2") == 0) { ck_assert_msg(exists[2 ], "f2 file is discovered twice"); exists[2 ] = 0; ck_assert_int_eq(FolderIterator_isDir(fIterator[0]), 0); if(inspectFiles) __iterate_file__(dataset[0]->tfiles[2], &read); }
        else if (stringCompare(name, "f3") == 0) { ck_assert_msg(exists[3 ], "f3 file is discovered twice"); exists[3 ] = 0; ck_assert_int_eq(FolderIterator_isDir(fIterator[0]), 0); if(inspectFiles) __iterate_file__(dataset[0]->tfiles[3], &read); }
        else if (stringCompare(name, "f4") == 0) { ck_assert_msg(exists[4 ], "f4 file is discovered twice"); exists[4 ] = 0; ck_assert_int_eq(FolderIterator_isDir(fIterator[0]), 0); if(inspectFiles) __iterate_file__(dataset[0]->tfiles[4], &read); }
        else if (stringCompare(name, "f5") == 0) { ck_assert_msg(exists[5 ], "f5 file is discovered twice"); exists[5 ] = 0; ck_assert_int_eq(FolderIterator_isDir(fIterator[0]), 0); if(inspectFiles) __iterate_file__(dataset[0]->tfiles[5], &read); }
        else if (stringCompare(name, "f6") == 0) { ck_assert_msg(exists[6 ], "f6 file is discovered twice"); exists[6 ] = 0; ck_assert_int_eq(FolderIterator_isDir(fIterator[0]), 0); if(inspectFiles) __iterate_file__(dataset[0]->tfiles[6], &read); }
        else if (stringCompare(name, "f7") == 0) { ck_assert_msg(exists[7 ], "f7 file is discovered twice"); exists[7 ] = 0; ck_assert_int_eq(FolderIterator_isDir(fIterator[0]), 0); if(inspectFiles) __iterate_file__(dataset[0]->tfiles[7], &read); }
        else if (stringCompare(name, "f8") == 0) { ck_assert_msg(exists[8 ], "f8 file is discovered twice"); exists[8 ] = 0; ck_assert_int_eq(FolderIterator_isDir(fIterator[0]), 0); if(inspectFiles) __iterate_file__(dataset[0]->tfiles[8], &read); }
        else if (stringCompare(name,  ".") == 0) { ck_assert_msg(exists[9 ],  ". file is discovered twice"); exists[9 ] = 0; ck_assert_int_ne(FolderIterator_isDir(fIterator[0]), 0);                                                                  }
        else if (stringCompare(name, "..") == 0) { ck_assert_msg(exists[10], ".. file is discovered twice"); exists[10] = 0; ck_assert_int_ne(FolderIterator_isDir(fIterator[0]), 0);                                                                  }
        else if (stringCompare(name, "d1") == 0) { ck_assert_msg(exists[11], "d1 file is discovered twice"); exists[11] = 0; ck_assert_int_ne(FolderIterator_isDir(fIterator[0]), 0);                                                                  }
        else if (stringCompare(name, "d2") == 0) { ck_assert_msg(exists[12], "d2 file is discovered twice"); exists[12] = 0; ck_assert_int_ne(FolderIterator_isDir(fIterator[0]), 0);                                                                  }
        else                                     { /* --- unknwon file --- */ ck_abort_msg("unknown file discovered: %s", name); /* --- unknwon file --- */                                                                                            }

        FolderIterator_next(fIterator[0]);

    }

    safeDELETE(fIterator);

    // check that all files have been discovered
    ck_assert_msg(!exists[0 ], "f0 not discovered");
    ck_assert_msg(!exists[1 ], "f1 not discovered");
    ck_assert_msg(!exists[2 ], "f2 not discovered");
    ck_assert_msg(!exists[3 ], "f3 not discovered");
    ck_assert_msg(!exists[4 ], "f4 not discovered");
    ck_assert_msg(!exists[5 ], "f5 not discovered");
    ck_assert_msg(!exists[6 ], "f6 not discovered");
    ck_assert_msg(!exists[7 ], "f7 not discovered");
    ck_assert_msg(!exists[8 ], "f8 not discovered");
    ck_assert_msg(!exists[9 ],  ". not discovered");
    ck_assert_msg(!exists[10], ".. not discovered");
    ck_assert_msg(!exists[11], "d1 not discovered");
    ck_assert_msg(!exists[12], "d2 not discovered");

    // check that all files are read correctly
    ck_assert_msg(    inspectFiles == 0
                   || expected     == read, "files are not read correctly");

    safeDELETE(dataset);

}

// -------------------------------------------------------------------------
// -------------------------------- FOLDER ---------------------------------
// -------------------------------------------------------------------------

START_UTEST(test_folder_does_not_exist)
{
    safeNEW(FolderIterator*, f1, 1, FolderIterator_delete, FolderIterator_new("/foo/", 1)); ck_assert_ptr_eq(f1[0], NULL); safeDELETE(f1);
    safeNEW(FolderIterator*, f2, 1, FolderIterator_delete, FolderIterator_new("/foo/", 0)); ck_assert_ptr_eq(f2[0], NULL); safeDELETE(f2);
    safeNEW(FolderIterator*, f3, 1, FolderIterator_delete, FolderIterator_new(  NULL , 1)); ck_assert_ptr_eq(f3[0], NULL); safeDELETE(f3);
    safeNEW(FolderIterator*, f4, 1, FolderIterator_delete, FolderIterator_new(  NULL , 0)); ck_assert_ptr_eq(f4[0], NULL); safeDELETE(f4);
}
END_UTEST

START_UTEST(test_iterate_over_folder_normally)
{
    __iterate_folder__(0, 0);
}
END_UTEST

START_UTEST(test_iterate_over_folder_skip)
{
    __iterate_folder__(1, 0);
}
END_UTEST

START_UTEST(test_iterate_over_folder_empty)
{
    // an empty dataset
    safeNEW(FsTestingSet*, dataset, 1, FsTestingSet_delete, FsTestingSet_new(NULL, NULL, NULL));
    if (!dataset[0]) fatalError("FsTestingSet_new has failed");
    // an iterator over this empty folder
    safeNEW(FolderIterator*, fIterator, 1, FolderIterator_delete, FolderIterator_new(dataset[0]->path, 1))
    ck_assert_ptr_ne(fIterator[0], NULL);
    ck_assert_int_ne(FolderIterator_isOver(fIterator[0]), 0);
}
END_UTEST

// -------------------------------------------------------------------------
// --------------------------------- FILE ----------------------------------
// -------------------------------------------------------------------------

START_UTEST(test_file_does_not_exist)
{
    safeNEW(FileIterator*, it, 1, FileIterator_delete, FileIterator_new(NULL)); ck_assert_ptr_eq(it[0], NULL); safeDELETE(it);
}
END_UTEST

START_UTEST(test_read_all_files)
{
    __iterate_folder__(0, 1);
    __iterate_folder__(1, 1);
}
END_UTEST

// #########################################################################
// #########################################################################
// #########################################################################

START_SUITE_BUILDER(filesystem)
{

    ONE_TEST_CASE(folder_does_not_exist       , test_folder_does_not_exist       );
    ONE_TEST_CASE(iterate_over_folder_normally, test_iterate_over_folder_normally);
    ONE_TEST_CASE(iterate_over_folder_skip    , test_iterate_over_folder_skip    );
    ONE_TEST_CASE(iterate_over_folder_empty   , test_iterate_over_folder_empty   );
    ONE_TEST_CASE(file_does_not_exist         , test_file_does_not_exist         );
    ONE_TEST_CASE(read_all_files              , test_read_all_files              );

}
END_SUITE_BUILDER
