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

#include "interactive/all.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include <ctype.h>
#include <assert.h>
#include "misc/string.h"
#include "misc/config.h"
#include "system/command.h"

// #########################################################################
// #########################################################################
// #########################################################################

void Input_refresh(const Input *input, const char *cursor) {
// -------------------------------- asserts --------------------------------
// -------------------------------------------------------------------------
    assert(input); assert(cursor);
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    printf(RESTORE_CURSOR);
    InputIterator it;
    for(InputIterator_initIterator(input, &it); !InputIterator_isOver(&it); InputIterator_next(&it))
    {
        InputIterator_equals(input, &it) ? printf("%s%c"RESET_ALL, cursor, InputIterator_get(&it))
                                         : printf("%c", InputIterator_get(&it));
    }
    input->current == NULL || input->pos == input->current->bucket.top + 1
        ? printf("%s"SPACE""RESET_ALL""SPACE""LEFT_SHIFT_2, cursor)
        : printf(ERASE_1_CHAR);
}

void Input_interactiveClear(Input *input) {
// -------------------------------- asserts --------------------------------
// -------------------------------------------------------------------------
    assert(input);
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    printf(RESTORE_CURSOR);
    {
        for(size_t c = 0, e = Input_size(input) + 1;
            c < e; ++c) printf(SPACE);
    }
    printf(RESTORE_CURSOR);
    Input_clear(input);
}

int Input_interactiveLoad(Input *input, const char *command) {
// -------------------------------- asserts --------------------------------
// -------------------------------------------------------------------------
    assert(input);
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    if(command) { Input_interactiveClear(input);
                  return Input_load(input, command); } else { return 1; }
}

static int __autocomplete_return__(char *edited, char *extend, Fifo *sugges, int code)
{
    if(edited) { free(edited);        }
    if(extend) { free(extend);        }
    if(sugges) { Fifo_delete(sugges); } return code;
}

int Input_interactiveAutocomplete(Input *input, const char *header) {
// -------------------------------- asserts --------------------------------
// -------------------------------------------------------------------------
    assert(input);
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    const unsigned max = 16U; unsigned numberItems = 0; char *extend = NULL; Fifo *suggestions = NULL;
    char *edited = Input_getEditedWord(input); int code = 1;
    if(edited)
    {
        if(!autocomplete(edited, max, &numberItems, &extend, &suggestions))
        {
            if(numberItems)
            {
                // one suggestion
                if(extend)
                {
                    char *prot = getProtString(extend, '*');
                    if (prot) { for(const char *ptr = prot; *ptr; ++ptr) { Input_insert(input, *ptr); }
                                free(prot); code = 0; }
                }
                // several suggestions
                else
                if(suggestions)
                {
                    for(Input_refresh(input, ""); !Fifo_empty(suggestions); Fifo_pop(suggestions))
                    {
                        printf("\n");
                        for(const char *str = Fifo_front(suggestions); *str; ++str)
                        {
                            if   (*str == '*') printf(SET_ESC_CHARACTER_STYLE"*"RESET_ALL"*");
                            else               printf("%c", *str);
                        } }
                    // too many suggestions
                    if(   numberItems
                        > max )
                    {
                        printf("\n+ %u elements!", numberItems-max);
                    }
                    // new line
                    printf("\n%s"SPACE""SAVE_CURSOR, header);
                    code = 0;
                } }
        } }
    return __autocomplete_return__(edited, extend, suggestions, code);;
}

static void __try_add_history__(const char *rLine) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    const char *pointer = rLine;
    if (pointer) { // ignore INTERNAL_FHIST
                   while (*pointer&&isspace(*pointer)) ++pointer;
                   if (!startWith(pointer, INTERNAL_FHIST, 1)) History_add(Configuration.history, rLine); }
}

static char* __remove_comments__(char *rLine) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    if(rLine)
    {
        char *comment, *tmp = getRealString(rLine, '#', &comment);
        if(tmp) {
            // remove comments
            if (comment) { *comment = '\0'; }
            // keep the simplified string
            free(rLine); rLine = tmp;
        } }
    return rLine;
}

char* readLine(const char *header) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    initTerminal(); printf("%s"SPACE""SAVE_CURSOR, header);
    char* rLine = NULL; Input i; Input *input = &i; Input_init(input);
    while(!rLine)
    {
        Input_refresh(input, SET_CURSOR_STYLE);
        int key = readKeyboard();
        switch(key)
        {
            case KB_ESCAPE:
                break;
            case KB_ENTER:
                Input_refresh(input, ""); rLine = Input_toString(input); __try_add_history__(rLine); rLine = __remove_comments__(rLine);
                break;
            case KB_UP:
                RUN_OR_RING(Input_interactiveLoad(input, History_up(Configuration.history)));
                break;
            case KB_DOWN:
                RUN_OR_RING(Input_interactiveLoad(input, History_down(Configuration.history)));
                break;
            case KB_TAB:
                RUN_OR_RING(Input_interactiveAutocomplete(input, header));
                break;
            case KB_LEFT:
                RUN_OR_RING(Input_moveLeft(input));
                break;
            case KB_RIGHT:
                RUN_OR_RING(Input_moveRight(input));
                break;
            case KB_BACKSPACE:
                RUN_OR_RING(Input_backspace(input));
                break;
            case KB_DELETE:
                RUN_OR_RING(Input_del(input));
                break;
            default:
                RUN_OR_RING(Input_insert(input, key));
        };
    }
    printf(SPACE""LEFT_SHIFT_1"\n");
    Input_finalize(input); restoreTerminal();
    return rLine;
}
