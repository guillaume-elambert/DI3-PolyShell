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

#include "interactive/terminal.h"

// #########################################################################
// #########################################################################
// #########################################################################

#include <termios.h>

// #########################################################################
// #########################################################################
// #########################################################################

void initTerminal(void)  {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    struct termios opt; tcgetattr(0, &opt); opt.c_lflag &= ~ICANON;
    opt.c_lflag &= ~ECHO; tcsetattr(0, TCSANOW, &opt); printf(HIDE_CURSOR);
}

void restoreTerminal(void) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    struct termios options; tcgetattr(0, &options); options.c_lflag |= ICANON;
    options.c_lflag |= ECHO; tcsetattr(0, TCSANOW, &options); printf(SHOW_CURSOR);
}

int readKeyboard(void) {
// --------------------------------- body ----------------------------------
// -------------------------------------------------------------------------
    int kb1 = getchar();
    switch(kb1) {
        case 10:
            return KB_ENTER;
        case 27:
            switch(getchar()) {
                case 27:
                    return KB_ESCAPE;
                case 91:
                    switch(getchar()) {
                        case 65:
                            return KB_UP;
                        case 66:
                            return KB_DOWN;
                        case 67:
                            return KB_RIGHT;
                        case 68:
                            return KB_LEFT;
                        case 51:
                            return getchar() == 126 ? KB_DELETE
                                                    : KB_NOPE;
                        case 53:
                        case 54:
                            getchar();
                            return KB_NOPE;
                        default:
                            return KB_NOPE;

                    };
                default:
                    return KB_NOPE;
            };
        default:
            return kb1;
    };
}
