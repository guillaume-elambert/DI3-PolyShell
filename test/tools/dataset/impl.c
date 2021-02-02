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

#include "dataset.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include "misc/string.h"

// #########################################################################
// #########################################################################
// #########################################################################

FsTestingSet* FsTestingSet_new(const char **files, const char **folders, const char **binaries) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    FsTestingSet *nFsTestingSet = malloc(sizeof(FsTestingSet));
    if(nFsTestingSet) {
        if(FsTestingSet_init(nFsTestingSet, files, folders, binaries)) {
            free(nFsTestingSet);
            nFsTestingSet = NULL;
        } }
    return nFsTestingSet;
}

int FsTestingSet_init(FsTestingSet *dataset, const char **files, const char **folders, const char **binaries) {
// -------------------------------- asserts --------------------------------
// -------------------------------------------------------------------------
    assert(dataset);
// ------------------------------- mandatory -------------------------------
// -------------------------------------------------------------------------
    dataset->csum      = 0UL ;
    dataset->tfiles    = NULL;
    dataset->tfolders  = NULL;
    dataset->bin       = NULL;
    dataset->tbinaries = NULL;
    dataset->path = duplicateString("/tmp/units.XXXXXX");
    if(    !dataset->path
        || !mkdtemp(dataset->path) ){
        FsTestingSet_finalize(dataset);
        return 1;
    }
    char *fullPath = concatenateStrings(dataset->path, "/", 0);
    if(!fullPath) {
        FsTestingSet_finalize(dataset);
        return 1;
    }
    free(dataset->path);
    dataset->path = fullPath;
// -------------------------------- folders --------------------------------
// -------------------------------------------------------------------------
    if(     folders
        && *folders )
    {
        size_t nbFolders = 1;
        for(const char **iterator = folders; *iterator; ++iterator) {
            ++nbFolders;
        }
        dataset->tfolders = calloc(nbFolders, sizeof(char*));
        if(!dataset->tfolders) {
            FsTestingSet_finalize(dataset);
            return 1;
        }
        char **ds = dataset->tfolders;
        for(const char **iterator = folders; *iterator; ++iterator, ++ds)
        {
            *ds = concatenateStrings(dataset->path, *iterator, 0);
            if(    !*ds
                || mkdir(*ds,   S_IRWXU
                              | S_IRWXG
                              | S_IROTH
                              | S_IXOTH )) {
                FsTestingSet_finalize(dataset);
                return 1;
            } } }
// --------------------------------- files ---------------------------------
// -------------------------------------------------------------------------
    if(     files
        && *files )
    {
        size_t nbFiles = 1;
        for(const char **iterator = files; *iterator; ++iterator) {
            ++nbFiles;
        }
        dataset->tfiles = calloc(nbFiles, sizeof(char*));
        if(!dataset->tfiles) {
            FsTestingSet_finalize(dataset);
            return 1;
        }
        char **ds = dataset->tfiles;
        for(const char **iterator = files; *iterator; ++iterator, ++ds)
        {
            FILE *f = NULL;
            *ds = concatenateStrings(dataset->path, *iterator, 0);
            if(    !*ds
                || !(f=fopen(*ds, "w+")) ) {
                FsTestingSet_finalize(dataset);
                return 1;
            }
            fprintf(f, "# ============== test file ==============\n");
            fprintf(f, "# =======================================\n");
            for(int r, irand = 0; irand < 5; ++irand) {
                r = rand(); dataset->csum += (__typeof__(dataset->csum))r;
                fprintf(f, "%16d # some comments.\n", r);
            }
            fclose(f);
        } }
// ------------------------------- binaries --------------------------------
// -------------------------------------------------------------------------
    if(     binaries
        && *binaries )
    {
        dataset->bin = concatenateStrings(dataset->path, "bin.XXXXXX", 0);
        if(    !dataset->bin
            || !mkdtemp(dataset->bin) ) {
            FsTestingSet_finalize(dataset);
            return 1;
        }
        char *fullBinPath = concatenateStrings(dataset->bin, "/", 0);
        if(!fullBinPath) {
            FsTestingSet_finalize(dataset);
            return 1;
        }
        free(dataset->bin);
        dataset->bin = fullBinPath;
        size_t nbBinaries = 1;
        for(const char **iterator = binaries; *iterator; ++iterator) {
            ++nbBinaries;
        }
        dataset->tbinaries = calloc(nbBinaries, sizeof(char*));
        if(!dataset->tbinaries) {
            FsTestingSet_finalize(dataset);
            return 1;
        }
        char **ds = dataset->tbinaries;
        for(const char **iterator = binaries; *iterator; ++iterator, ++ds)
        {
            FILE *f = NULL;
            *ds = concatenateStrings(dataset->bin, *iterator, 0);

            if(    !*ds
                || !(f=fopen(*ds, "w+")) ) {
                FsTestingSet_finalize(dataset);
                return 1;
            }
            fprintf(f, "#!/bin/bash\n");
            fprintf(f, "echo \"Un script inutile nomme %s.\"\n", *iterator);
            fclose(f);
        } }
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
    return 0;
}

void FsTestingSet_finalize(FsTestingSet *dataset) {
// -------------------------------- asserts --------------------------------
// -------------------------------------------------------------------------
    assert(dataset);
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    dataset->csum = 0UL;
    if(dataset->path     ) {                                                                   free(dataset->path     ); dataset->path      = NULL; }
    if(dataset->bin      ) {                                                                   free(dataset->bin      ); dataset->bin       = NULL; }
    if(dataset->tfiles   ) { for(char **ptr = dataset->tfiles   ; *ptr; ++ptr) { free(*ptr); } free(dataset->tfiles   ); dataset->tfiles    = NULL; }
    if(dataset->tfolders ) { for(char **ptr = dataset->tfolders ; *ptr; ++ptr) { free(*ptr); } free(dataset->tfolders ); dataset->tfolders  = NULL; }
    if(dataset->tbinaries) { for(char **ptr = dataset->tbinaries; *ptr; ++ptr) { free(*ptr); } free(dataset->tbinaries); dataset->tbinaries = NULL; }
}

void FsTestingSet_delete(FsTestingSet *dataset) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    if(dataset) {
        FsTestingSet_finalize(dataset);
        free(dataset);
    }
}
