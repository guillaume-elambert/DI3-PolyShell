#ifndef FILESYSTEM_H

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

#define FILESYSTEM_H

//
#include "new.h"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include <dirent.h>

/**
 *
 */
typedef struct {

    //
    DIR *dir;

    //
    struct dirent *ent;

    //
    int skipSpecials;

} FolderIterator;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
LIB FolderIterator* OVERRIDABLE(FolderIterator_new)(const char *path, int skipSpecials);

/**
 *
 */
LIB int OVERRIDABLE(FolderIterator_init)(FolderIterator *fIterator, const char *path, int skipSpecials);

/**
 *
 */
LIB void OVERRIDABLE(FolderIterator_delete)(FolderIterator *fIterator);

/**
 *
 */
LIB void OVERRIDABLE(FolderIterator_finalize)(FolderIterator *fIterator);

/**
 *
 */
LIB int OVERRIDABLE(FolderIterator_isOver)(const FolderIterator *fIterator);

/**
 *
 */
LIB const char* OVERRIDABLE(FolderIterator_get)(const FolderIterator *fIterator);

/**
 *
 */
LIB int OVERRIDABLE(FolderIterator_isDir)(const FolderIterator *fIterator);

/**
 *
 */
LIB void OVERRIDABLE(FolderIterator_next)(FolderIterator *fIterator);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include <stdio.h>

/**
 *
 */
typedef struct {

    //
    FILE* file;

    //
    char *current;

} FileIterator;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
LIB FileIterator* OVERRIDABLE(FileIterator_new)(FILE *file);

/**
 *
 */
LIB int OVERRIDABLE(FileIterator_init)(FileIterator *fIterator, FILE *file);

/**
 *
 */
LIB void OVERRIDABLE(FileIterator_delete)(FileIterator *fIterator);

/**
 *
 */
LIB void OVERRIDABLE(FileIterator_finalize)(FileIterator *fIterator);

/**
 *
 */
LIB int OVERRIDABLE(FileIterator_isOver)(const FileIterator *fIterator);

/**
 *
 */
LIB const char* OVERRIDABLE(FileIterator_get)(const FileIterator *fIterator);

/**
 *
 */
LIB void OVERRIDABLE(FileIterator_next)(FileIterator *fIterator);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

//
#include "impl/filesystem.h"

#endif
