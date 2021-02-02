#ifndef NEW_H

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

#define NEW_H

//
#include "new.etu.h"

//
#include "overridable.h"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
#define MAKE_NEW_0(DStructure)                                      \
    DStructure* IMPLEMENT(DStructure##_new)(void)                   \
    {                                                               \
        BODY_NEW(DStructure);                                       \
    }                                                               \

/**
 *
 */
#define MAKE_NEW_1(DStructure, T1)                                  \
    DStructure* IMPLEMENT(DStructure##_new)(T1 a1)                  \
    {                                                               \
        BODY_NEW(DStructure, a1);                                   \
    }                                                               \

/**
 *
 */
#define MAKE_NEW_2(DStructure, T1, T2)                              \
    DStructure* IMPLEMENT(DStructure##_new)(T1 a1, T2 a2)           \
    {                                                               \
        BODY_NEW(DStructure, a1, a2);                               \
    }                                                               \

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
#define MAKE_DEL_0(DStructure)                                      \
    void IMPLEMENT(DStructure##_delete)(DStructure *ptr)            \
    {                                                               \
        BODY_DEL(DStructure, ptr);                                  \
    }                                                               \

/**
 *
 */
#define MAKE_DEL_1(DStructure, T1)                                  \
    void IMPLEMENT(DStructure##_delete)(DStructure *ptr, T1 a1)     \
    {                                                               \
        BODY_DEL(DStructure, ptr, a1);                              \
    }                                                               \

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#endif
