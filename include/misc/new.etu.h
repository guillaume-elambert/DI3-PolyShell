#ifndef NEW_ETU_H

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

#define NEW_ETU_H

// an ugly tweak to remove the following warning:
// ISO C99 requires at least one argument for the ... in a variadic macro
#pragma GCC system_header

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
// LOOK AT new.h FOR MORE DETAILS!
//

#include <stdlib.h>

/**
 * Utilise des macros pour créer l'ensemble
 * des fonctions 'new' du projet
 */
#define BODY_NEW(DStructure, ...)                           	\
	DStructure *ptr = (DStructure*)malloc(sizeof(DStructure));	\
	if(ptr!=NULL){												\
		if(DStructure##_init(ptr, ##__VA_ARGS__)){				\
			free(ptr);											\
			ptr=NULL;											\
		}														\
	}															\
	return ptr;													\
    //return provided_##DStructure##_new(__VA_ARGS__);

/**
 * Utilise des macros pour créer l'ensemble
 * des fonctions 'delete' du projet
 */
#define BODY_DEL(DStructure,  ptr, ...)                     \
	if(ptr){												\
		DStructure##_finalize(ptr, ##__VA_ARGS__);			\
		free(ptr);											\
	}														\
    //provided_##DStructure##_delete(ptr, ##__VA_ARGS__);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#endif
