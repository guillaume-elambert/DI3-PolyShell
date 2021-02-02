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

// #########################################################################
// ###################### DO NEVER INCLUDE THIS FILE #######################
// #########################################################################

FolderIterator* provided_FolderIterator_new(const char *path, int skipSpecials);
FolderIterator* user_FolderIterator_new(const char *path, int skipSpecials);

int provided_FolderIterator_init(FolderIterator *fIterator, const char *path, int skipSpecials);
int user_FolderIterator_init(FolderIterator *fIterator, const char *path, int skipSpecials);

void provided_FolderIterator_delete(FolderIterator *fIterator);
void user_FolderIterator_delete(FolderIterator *fIterator);

void provided_FolderIterator_finalize(FolderIterator *fIterator);
void user_FolderIterator_finalize(FolderIterator *fIterator);

int provided_FolderIterator_isOver(const FolderIterator *fIterator);
int user_FolderIterator_isOver(const FolderIterator *fIterator);

const char* provided_FolderIterator_get(const FolderIterator *fIterator);
const char* user_FolderIterator_get(const FolderIterator *fIterator);

int provided_FolderIterator_isDir(const FolderIterator *fIterator);
int user_FolderIterator_isDir(const FolderIterator *fIterator);

void provided_FolderIterator_next(FolderIterator *fIterator);
void user_FolderIterator_next(FolderIterator *fIterator);

FileIterator* provided_FileIterator_new(FILE *file);
FileIterator* user_FileIterator_new(FILE *file);

int provided_FileIterator_init(FileIterator *fIterator, FILE *file);
int user_FileIterator_init(FileIterator *fIterator, FILE *file);

void provided_FileIterator_delete(FileIterator *fIterator);
void user_FileIterator_delete(FileIterator *fIterator);

void provided_FileIterator_finalize(FileIterator *fIterator);
void user_FileIterator_finalize(FileIterator *fIterator);

int provided_FileIterator_isOver(const FileIterator *fIterator);
int user_FileIterator_isOver(const FileIterator *fIterator);

const char* provided_FileIterator_get(const FileIterator *fIterator);
const char* user_FileIterator_get(const FileIterator *fIterator);

void provided_FileIterator_next(FileIterator *fIterator);
void user_FileIterator_next(FileIterator *fIterator);
