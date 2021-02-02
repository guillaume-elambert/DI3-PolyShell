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

char provided_toLowerCase(char c);
char user_toLowerCase(char c);

char provided_toUpperCase(char c);
char user_toUpperCase(char c);

const char* provided_indexOfString(const char *foin, const char *aiguille, int csensitive);
const char* user_indexOfString(const char *foin, const char *aiguille, int csensitive);

size_t provided_stringLength(const char *str);
size_t user_stringLength(const char *str);

char* provided_duplicateString(const char *str);
char* user_duplicateString(const char *str);

const char* provided_findFirst(const char *str, const char *separators);
const char* user_findFirst(const char *str, const char *separators);

char* provided_findLast(char *str, char c);
char* user_findLast(char *str, char c);

int provided_stringCompare(const char *str1, const char *str2);
int user_stringCompare(const char *str1, const char *str2);

char* provided_concatenateStrings(const char *str1, const char *str2, size_t minDestSize);
char* user_concatenateStrings(const char *str1, const char *str2, size_t minDestSize);

void provided_copyStringWithLength(char *dest, const char * src, size_t destSize);
void user_copyStringWithLength(char *dest, const char * src, size_t destSize);

char* provided_mkReverse(char *str);
char* user_mkReverse(char *str);

const char* provided_startWith(const char *str, const char *start, int csensitive);
const char* user_startWith(const char *str, const char *start, int csensitive);

int provided_belongs(char c, const char *str);
int user_belongs(char c, const char *str);

char* provided_subString(const char *start, size_t length);
char* user_subString(const char *start, size_t length);

void provided_mkCommon(char *result, const char *str);
void user_mkCommon(char *result, const char *str);

int provided_isNotEmpty(const char *str);
int user_isNotEmpty(const char *str);

char* provided_getProtString(const char *filename, char c);
char* user_getProtString(const char *filename, char c);

char* provided_getRealString(const char *filename, char c, char **firstNotEscaped);
char* user_getRealString(const char *filename, char c, char **firstNotEscaped);

Tokenizer* provided_Tokenizer_new(const char *str, const char *separators);
Tokenizer* user_Tokenizer_new(const char *str, const char *separators);

int provided_Tokenizer_init(Tokenizer *tokenizer, const char *str, const char *separators);
int user_Tokenizer_init(Tokenizer *tokenizer, const char *str, const char *separators);

void provided_Tokenizer_delete(Tokenizer *tokenizer);
void user_Tokenizer_delete(Tokenizer *tokenizer);

void provided_Tokenizer_finalize(Tokenizer *tokenizer);
void user_Tokenizer_finalize(Tokenizer *tokenizer);

int provided_Tokenizer_isOver(const Tokenizer *tokenizer);
int user_Tokenizer_isOver(const Tokenizer *tokenizer);

char* provided_Tokenizer_get(const Tokenizer *tokenizer);
char* user_Tokenizer_get(const Tokenizer *tokenizer);

void provided_Tokenizer_next(Tokenizer *tokenizer);
void user_Tokenizer_next(Tokenizer *tokenizer);
