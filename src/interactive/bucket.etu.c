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

#include "interactive/bucket.h"
#include "misc/new.h"
#include <stdio.h>

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_0(Bucket)
MAKE_DEL_0(Bucket)

//Créé grâce aux macros (CF. include/misc/new.etu.c)
/*Bucket* IMPLEMENT(Bucket_new)(void)
{
	Bucket *myBucket = (Bucket*)malloc(sizeof(Bucket));
	if(myBucket != NULL){
		if(Bucket_init(myBucket)){
			free(myBucket);
			myBucket=NULL;
		}
	}
	return myBucket;
    //return provided_Bucket_new();
}*/

int IMPLEMENT(Bucket_init)(Bucket *bucket)
{
	bucket->top = -1;
	return 0;
    //return provided_Bucket_init(bucket);
}

//Créé grâce aux macros (CF. include/misc/new.etu.c)
/*void IMPLEMENT(Bucket_delete)(Bucket *bucket)
{
	if(bucket){
		Bucket_finalize(bucket);
		free(bucket);
	}
    //provided_Bucket_delete(bucket);
}*/

void IMPLEMENT(Bucket_finalize)(Bucket *bucket)
{
	//Ne fait rien, évite juste les warnings
	(void)bucket;
	//provided_Bucket_finalize(bucket);
}

size_t IMPLEMENT(Bucket_size)(const Bucket *bucket)
{
	return bucket?bucket->top+1:0;
    //return provided_Bucket_size(bucket);
}

void IMPLEMENT(Bucket_remove)(Bucket *bucket, int position)
{
	size_t i = position, size = Bucket_size(bucket);
	
	while(i<size){
		bucket->content[i] = bucket->content[i+1];
		i++;
	}
	--bucket->top;
    //provided_Bucket_remove(bucket, position);
}

void IMPLEMENT(Bucket_insert)(Bucket *bucket, int position, char c)
{
	int i = bucket->top+1;
	
	//Parcours inversé du bucket jusqu'à la position voulue
	while(i>position){
		bucket->content[i] = bucket->content[i-1];
		--i;
	}
	bucket->content[i]=c;
	++bucket->top;
	
    //provided_Bucket_insert(bucket, position, c);
}

void IMPLEMENT(Bucket_move)(Bucket *from, int position, Bucket *to)
{
	if(from && to && position<=from->top){
		int i=0;
		while(position<=from->top){
			Bucket_insert(to, i, from->content[position]);
			Bucket_remove(from,position);
			++i;
		}
	}
	
    //provided_Bucket_move(from, position, to);
}

int IMPLEMENT(Bucket_empty)(const Bucket *bucket)
{
	return Bucket_size(bucket)>0?0:1;
    //return provided_Bucket_empty(bucket);
}

int IMPLEMENT(Bucket_full)(const Bucket *bucket)
{
	return Bucket_size(bucket)==BUCKET_SIZE?1:0;
    //return provided_Bucket_full(bucket);
}
