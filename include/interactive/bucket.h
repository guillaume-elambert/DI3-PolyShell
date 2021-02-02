#ifndef BUCKET_H

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

#define BUCKET_H

//
#include "misc/overridable.h"

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
#define BUCKET_SIZE 16

/**
 *
 */
typedef struct {

    //
    char content[BUCKET_SIZE];

    //
    int top;

} Bucket;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include <stdlib.h>

/**
 *
 */
LIB Bucket* OVERRIDABLE(Bucket_new)(void);

/**
 *
 */
LIB int OVERRIDABLE(Bucket_init)(Bucket *bucket);

/**
 *
 */
LIB void OVERRIDABLE(Bucket_delete)(Bucket *bucket);

/**
 *
 */
LIB void OVERRIDABLE(Bucket_finalize)(Bucket *bucket);

/**
 *
 */
LIB size_t OVERRIDABLE(Bucket_size)(const Bucket *bucket);

/**
 *
 */
LIB void OVERRIDABLE(Bucket_remove)(Bucket *bucket, int position);

/**
 *
 */
LIB void OVERRIDABLE(Bucket_insert)(Bucket *bucket, int position, char c);

/**
 *
 */
LIB void OVERRIDABLE(Bucket_move)(Bucket *from, int position, Bucket *to);

/**
 *
 */
LIB int OVERRIDABLE(Bucket_empty)(const Bucket *bucket);

/**
 *
 */
LIB int OVERRIDABLE(Bucket_full)(const Bucket *bucket);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

//
#include "impl/bucket.h"

#endif
