#ifndef FIFO_H

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

#define FIFO_H

//
#include "new.h"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
typedef enum {

    //
    AGGREGATE,

    //
    COMPOSE

} FifoMode;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
typedef struct {

    //
    char** storage;

    //
    unsigned int head;

    //
    unsigned int tail;

    //
    unsigned int capacity;

    //
    FifoMode mode;

} Fifo;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
LIB Fifo* OVERRIDABLE(Fifo_new)(unsigned int capacity, FifoMode mode);

/**
 *
 */
LIB int OVERRIDABLE(Fifo_init)(Fifo *fifo, unsigned int capacity, FifoMode mode);

/**
 *
 */
LIB void OVERRIDABLE(Fifo_delete)(Fifo *fifo);

/**
 *
 */
LIB void OVERRIDABLE(Fifo_finalize)(Fifo *fifo);

/**
 *
 */
LIB void OVERRIDABLE(Fifo_clear)(Fifo *fifo);

/**
 *
 */
LIB const char* OVERRIDABLE(Fifo_front)(const Fifo *fifo);

/**
 *
 */
LIB int OVERRIDABLE(Fifo_pop)(Fifo *fifo);

/**
 *
 */
LIB int OVERRIDABLE(Fifo_push)(Fifo *fifo, const char *str);

/**
 *
 */
LIB int OVERRIDABLE(Fifo_full)(const Fifo *fifo);

/**
 *
 */
LIB int OVERRIDABLE(Fifo_empty)(const Fifo *fifo);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

//
#include "impl/fifo.h"

#endif
