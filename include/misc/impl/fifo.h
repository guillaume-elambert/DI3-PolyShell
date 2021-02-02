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

Fifo* provided_Fifo_new(unsigned int capacity, FifoMode mode);
Fifo* user_Fifo_new(unsigned int capacity, FifoMode mode);

int provided_Fifo_init(Fifo *fifo, unsigned int capacity, FifoMode mode);
int user_Fifo_init(Fifo *fifo, unsigned int capacity, FifoMode mode);

void provided_Fifo_delete(Fifo *fifo);
void user_Fifo_delete(Fifo *fifo);

void provided_Fifo_finalize(Fifo *fifo);
void user_Fifo_finalize(Fifo *fifo);

void provided_Fifo_clear(Fifo *fifo);
void user_Fifo_clear(Fifo *fifo);

const char* provided_Fifo_front(const Fifo *fifo);
const char* user_Fifo_front(const Fifo *fifo);

int provided_Fifo_pop(Fifo *fifo);
int user_Fifo_pop(Fifo *fifo);

int provided_Fifo_push(Fifo *fifo, const char *str);
int user_Fifo_push(Fifo *fifo, const char *str);

int provided_Fifo_full(const Fifo *fifo);
int user_Fifo_full(const Fifo *fifo);

int provided_Fifo_empty(const Fifo *fifo);
int user_Fifo_empty(const Fifo *fifo);
