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

Bucket* provided_Bucket_new(void);
Bucket* user_Bucket_new(void);

int provided_Bucket_init(Bucket *bucket);
int user_Bucket_init(Bucket *bucket);

void provided_Bucket_delete(Bucket *bucket);
void user_Bucket_delete(Bucket *bucket);

void provided_Bucket_finalize(Bucket *bucket);
void user_Bucket_finalize(Bucket *bucket);

size_t provided_Bucket_size(const Bucket *bucket);
size_t user_Bucket_size(const Bucket *bucket);

void provided_Bucket_remove(Bucket *bucket, int position);
void user_Bucket_remove(Bucket *bucket, int position);

void provided_Bucket_insert(Bucket *bucket, int position, char c);
void user_Bucket_insert(Bucket *bucket, int position, char c);

void provided_Bucket_move(Bucket *from, int position, Bucket *to);
void user_Bucket_move(Bucket *from, int position, Bucket *to);

int provided_Bucket_empty(const Bucket *bucket);
int user_Bucket_empty(const Bucket *bucket);

int provided_Bucket_full(const Bucket *bucket);
int user_Bucket_full(const Bucket *bucket);
