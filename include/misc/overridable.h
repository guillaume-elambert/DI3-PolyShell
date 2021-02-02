#ifndef OVERRIDABLE_H

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

#define OVERRIDABLE_H

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#define OVERRIDABLE(fname) (*fname)
#define LIB extern
#define IMPLEMENT(fname) user_ ## fname

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
struct __overriding_options__ {

    //
    int interactive;

    //
    int interactive_autocomplete;
    int interactive_prependHomeDir;

    //
    int interactive_Bucket_new;
    int interactive_Bucket_init;
    int interactive_Bucket_delete;
    int interactive_Bucket_finalize;
    int interactive_Bucket_size;
    int interactive_Bucket_remove;
    int interactive_Bucket_insert;
    int interactive_Bucket_move;
    int interactive_Bucket_empty;
    int interactive_Bucket_full;

    //
    int interactive_Cell_new;
    int interactive_Cell_init;
    int interactive_Cell_delete;
    int interactive_Cell_finalize;
    int interactive_Cell_insertAfter;

    //
    int interactive_History_new;
    int interactive_History_init;
    int interactive_History_delete;
    int interactive_History_finalize;
    int interactive_History_clear;
    int interactive_History_add;
    int interactive_History_up;
    int interactive_History_down;

    //
    int interactive_Input_new;
    int interactive_Input_init;
    int interactive_Input_delete;
    int interactive_Input_finalize;
    int interactive_Input_clear;
    int interactive_Input_size;
    int interactive_Input_get;
    int interactive_Input_insert;
    int interactive_Input_backspace;
    int interactive_Input_del;
    int interactive_Input_moveLeft;
    int interactive_Input_moveRight;
    int interactive_Input_toString;
    int interactive_Input_load;
    int interactive_Input_getEditedWord;

    //
    int interactive_InputIterator_initIterator;
    int interactive_InputIterator_equals;
    int interactive_InputIterator_next;
    int interactive_InputIterator_isOver;
    int interactive_InputIterator_get;

    //
    int parser;

    //
    int system;

    //
    int system_CmdMember_new;
    int system_CmdMember_init;
    int system_CmdMember_delete;
    int system_CmdMember_finalize;
    int system_CmdMember_redirect;
    int system_CmdMember_appendRedirect;
    int system_CmdMember_mergeOutputs;
    int system_CmdMember_pipe;
    int system_CmdMember_addOption;

    //
    int system_Command_execute;
    int system_Command_getNbMember;

    //
    int system_isRoot;
    int system_getPATHVar;
    int system_hostname;
    int system_userInformation;
    int system_getCurrentDirectory;

    //
    int misc;

    //
    int misc_Fifo_new;
    int misc_Fifo_init;
    int misc_Fifo_delete;
    int misc_Fifo_finalize;
    int misc_Fifo_clear;
    int misc_Fifo_front;
    int misc_Fifo_pop;
    int misc_Fifo_push;
    int misc_Fifo_full;
    int misc_Fifo_empty;

    //
    int misc_FolderIterator_new;
    int misc_FolderIterator_init;
    int misc_FolderIterator_delete;
    int misc_FolderIterator_finalize;
    int misc_FolderIterator_isOver;
    int misc_FolderIterator_get;
    int misc_FolderIterator_isDir;
    int misc_FolderIterator_next;

    //
    int misc_FileIterator_new;
    int misc_FileIterator_init;
    int misc_FileIterator_delete;
    int misc_FileIterator_finalize;
    int misc_FileIterator_isOver;
    int misc_FileIterator_get;
    int misc_FileIterator_next;

    //
    int misc_Pattern_new;
    int misc_Pattern_init;
    int misc_Pattern_delete;
    int misc_Pattern_finalize;
    int misc_Pattern_match;

    //
    int misc_toLowerCase;
    int misc_toUpperCase;
    int misc_indexOfString;
    int misc_stringLength;
    int misc_duplicateString;
    int misc_findFirst;
    int misc_findLast;
    int misc_stringCompare;
    int misc_concatenateStrings;
    int misc_copyStringWithLength;
    int misc_mkReverse;
    int misc_startWith;
    int misc_belongs;
    int misc_subString;
    int misc_mkCommon;
    int misc_isNotEmpty;
    int misc_getProtString;
    int misc_getRealString;

    //
    int misc_Tokenizer_new;
    int misc_Tokenizer_init;
    int misc_Tokenizer_delete;
    int misc_Tokenizer_finalize;
    int misc_Tokenizer_isOver;
    int misc_Tokenizer_get;
    int misc_Tokenizer_next;

};

/**
 *
 */
extern struct __overriding_options__ OverridingOptions;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 *
 */
void initOverridable(void);

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#endif
