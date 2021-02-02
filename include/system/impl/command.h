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

CmdMember* provided_CmdMember_new(const char *base);
CmdMember* user_CmdMember_new(const char *base);

int provided_CmdMember_init(CmdMember *mbr, const char *base);
int user_CmdMember_init(CmdMember *mbr, const char *base);

void provided_CmdMember_delete(CmdMember *mbr);
void user_CmdMember_delete(CmdMember *mbr);

void provided_CmdMember_finalize(CmdMember *mbr);
void user_CmdMember_finalize(CmdMember *mbr);

CmdMember* provided_CmdMember_redirect(CmdMember *mbr, int fd, const char *filename);
CmdMember* user_CmdMember_redirect(CmdMember *mbr, int fd, const char *filename);

CmdMember* provided_CmdMember_appendRedirect(CmdMember *mbr, int fd, const char *filename);
CmdMember* user_CmdMember_appendRedirect(CmdMember *mbr, int fd, const char *filename);

CmdMember* provided_CmdMember_mergeOutputs(CmdMember *mbr);
CmdMember* user_CmdMember_mergeOutputs(CmdMember *mbr);

CmdMember* provided_CmdMember_pipe(CmdMember *m1, CmdMember *m2);
CmdMember* user_CmdMember_pipe(CmdMember *m1, CmdMember *m2);

CmdMember* provided_CmdMember_addOption(CmdMember *mbr, const char *arg, int expend);
CmdMember* user_CmdMember_addOption(CmdMember *mbr, const char *arg, int expend);

int provided_Command_execute(Command *cmd);
int user_Command_execute(Command *cmd);

size_t provided_Command_getNbMember(const Command *cmd);
size_t user_Command_getNbMember(const Command *cmd);
