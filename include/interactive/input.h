#ifndef INPUT_H

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

#define INPUT_H

//
#include "misc/new.h"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
char* readLine(const char *header);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include "cell.h"

/**
 *
 */
typedef struct {

    //
    Cell *current;

    //
    int pos;

} Input;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
LIB Input* OVERRIDABLE(Input_new)(void);

/**
 *
 */
LIB int OVERRIDABLE(Input_init)(Input *input);

/**
 *
 */
LIB void OVERRIDABLE(Input_delete)(Input *input);

/**
 *
 */
LIB void OVERRIDABLE(Input_finalize)(Input *input);

/**
 *
 */
LIB void OVERRIDABLE(Input_clear)(Input *input);

/**
 *
 */
LIB int OVERRIDABLE(Input_load)(Input *input, const char *command);

/**
 *
 */
LIB size_t OVERRIDABLE(Input_size)(const Input *input);

/**
 *
 */
LIB char OVERRIDABLE(Input_get)(const Input *input);

/**
 *
 */
LIB int OVERRIDABLE(Input_insert)(Input *input, char c);

/**
 *
 */
LIB int OVERRIDABLE(Input_backspace)(Input *input);

/**
 *
 */
LIB int OVERRIDABLE(Input_del)(Input *input);

/**
 *
 */
LIB int OVERRIDABLE(Input_moveLeft)(Input *input);

/**
 *
 */
LIB int OVERRIDABLE(Input_moveRight)(Input *input);

/**
 *
 */
LIB char* OVERRIDABLE(Input_toString)(const Input *input);

/**
 *
 */
LIB char* OVERRIDABLE(Input_getEditedWord)(const Input *input);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
void Input_interactiveClear(Input *input);

/**
 *
 */
int Input_interactiveLoad(Input *input, const char *command);

/**
 *
 */
int Input_interactiveAutocomplete(Input *input, const char *header);

/**
 *
 */
void Input_refresh(const Input *input, const char *cursor);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
typedef Input InputIterator;

/**
 *
 */
LIB void OVERRIDABLE(InputIterator_initIterator)(const Input *input, InputIterator *inputIterator);

/**
 *
 */
LIB int OVERRIDABLE(InputIterator_equals)(const InputIterator *x, const InputIterator *y);

/**
 *
 */
LIB void OVERRIDABLE(InputIterator_next)(InputIterator *inputIterator);

/**
 *
 */
LIB int OVERRIDABLE(InputIterator_isOver)(const InputIterator *inputIterator);

/**
 *
 */
LIB char OVERRIDABLE(InputIterator_get)(const InputIterator *inputIterator);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

//
#include "impl/input.h"

#endif
