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

Input* provided_Input_new(void);
Input* user_Input_new(void);

int provided_Input_init(Input *input);
int user_Input_init(Input *input);

void provided_Input_delete(Input *input);
void user_Input_delete(Input *input);

void provided_Input_finalize(Input *input);
void user_Input_finalize(Input *input);

void provided_Input_clear(Input *input);
void user_Input_clear(Input *input);

size_t provided_Input_size(const Input *input);
size_t user_Input_size(const Input *input);

char provided_Input_get(const Input *input);
char user_Input_get(const Input *input);

int provided_Input_insert(Input *input, char c);
int user_Input_insert(Input *input, char c);

int provided_Input_backspace(Input *input);
int user_Input_backspace(Input *input);

int provided_Input_del(Input *input);
int user_Input_del(Input *input);

int provided_Input_moveLeft(Input *input);
int user_Input_moveLeft(Input *input);

int provided_Input_moveRight(Input *input);
int user_Input_moveRight(Input *input);

char* provided_Input_toString(const Input *input);
char* user_Input_toString(const Input *input);

int provided_Input_load(Input *input, const char *command);
int user_Input_load(Input *input, const char *command);

char* provided_Input_getEditedWord(const Input *input);
char* user_Input_getEditedWord(const Input *input);

void provided_InputIterator_initIterator(const Input *input, InputIterator *inputIterator);
void user_InputIterator_initIterator(const Input *input, InputIterator *inputIterator);

int provided_InputIterator_equals(const InputIterator *x, const InputIterator *y);
int user_InputIterator_equals(const InputIterator *x, const InputIterator *y);

void provided_InputIterator_next(InputIterator *inputIterator);
void user_InputIterator_next(InputIterator *inputIterator);

int provided_InputIterator_isOver(const InputIterator *inputIterator);
int user_InputIterator_isOver(const InputIterator *inputIterator);

char provided_InputIterator_get(const InputIterator *inputIterator);
char user_InputIterator_get(const InputIterator *inputIterator);
