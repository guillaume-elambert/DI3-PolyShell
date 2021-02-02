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

Pattern* provided_Pattern_new(const char *pattern);
Pattern* user_Pattern_new(const char *pattern);

int provided_Pattern_init(Pattern *preg, const char *pattern);
int user_Pattern_init(Pattern *preg, const char *pattern);

void provided_Pattern_delete(Pattern *preg);
void user_Pattern_delete(Pattern *preg);

void provided_Pattern_finalize(Pattern *preg);
void user_Pattern_finalize(Pattern *preg);

int provided_Pattern_match(const Pattern *preg, const char *str);
int user_Pattern_match(const Pattern *preg, const char *str);
