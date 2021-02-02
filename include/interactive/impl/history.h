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

History* provided_History_new(const char *filename, unsigned int length);
History* user_History_new(const char *filename, unsigned int length);

int provided_History_init(History *history, const char *filename, unsigned int length);
int user_History_init(History *history, const char *filename, unsigned int length);

void provided_History_delete(History *history, const char *filename);
void user_History_delete(History *history, const char *filename);

void provided_History_finalize(History *history, const char *filename);
void user_History_finalize(History *history, const char *filename);

void provided_History_clear(History *history);
void user_History_clear(History *history);

void provided_History_add(History *history, const char *command);
void user_History_add(History *history, const char *command);

const char* provided_History_up(History *history);
const char* user_History_up(History *history);

const char* provided_History_down(History *history);
const char* user_History_down(History *history);
