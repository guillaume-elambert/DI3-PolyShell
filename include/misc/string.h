#ifndef STRING_H

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

#define STRING_H

// for curious persons only
#include "new.h"

/**
 * \file
 * \brief This header defines various functions for easily manipulating characters and arrays of characters.
 *
 * See the detailed description of \ref string and \ref Tokenizer for more details.
 *****/

/**
 * \addtogroup misc
 * \{
 *****/

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include <stdlib.h>

/**
 * \defgroup string
 * \brief This module gathers functions for easily manipulating characters and arrays of characters.
 * \{
 *****/

/**
 * \brief   This function converts uppercase letters to lowercase.
 * \param   c The letter to be converted to lowercase.
 * \return  This function returns the lowercase equivalent to \a c if such value exists. Otherwise it returns \a c unchanged.
 *
 * See [tolower](http://man7.org/linux/man-pages/man3/tolower.3.html) for more details.
 */
LIB char OVERRIDABLE(toLowerCase)(char c);

/**
 * \brief   This function converts lowercase letters to uppercase.
 * \param   c The letter to be converted to uppercase.
 * \return  This function returns the uppercase equivalent to \a c if such value exists. Otherwise it returns \a c unchanged.
 *
 * See [toupper](http://man7.org/linux/man-pages/man3/toupper.3.html) for more details.
 */
LIB char OVERRIDABLE(toUpperCase)(char c);

/**
 * \brief   This function finds the first occurrence of a substring in a string.
 * \param   foin The string to be scanned.
 * \param   aiguille The substring to be found.
 * \param   csentive A boolean indicating whether or not this search is case sensitive.
 * \return  This function returns a pointer to the first occurrence in \a foin of the entire sequence of characters specified in \a aiguille,
            or NULL if the sequence is not present in \a foin.
 *
 * See [strstr and strcasestr](http://man7.org/linux/man-pages/man3/strstr.3.html) for more details.
 */
LIB const char* OVERRIDABLE(indexOfString)(const char *foin, const char *aiguille, int csensitive);

/**
 * \brief   This function calculates the length of a given string.
 * \param   str The string.
 * \return  This functions returns the length of \a str.
 *
 * See [strlen](http://man7.org/linux/man-pages/man3/strlen.3.html) for more details.
 */
LIB size_t OVERRIDABLE(stringLength)(const char *str);

/**
 * \brief   This function returns a pointer to a new string which is a duplicate of the input string.
 * \param   str The string to be duplicated.
 * \return  On success, this function returns a pointer to the duplicated string. It returns NULL if insufficient memory was available.
 *
 * See [strdup](http://man7.org/linux/man-pages/man3/strdup.3.html) for more details.
 */
LIB char* OVERRIDABLE(duplicateString)(const char *str);

/**
 * \brief   This function locates the first occurrence in a string of any of the characters in another string.
 * \param   str The string to be scanned.
 * \param   separators The characters to be found.
 * \return  This function returns a pointer to the first character in \a str that matches one of the characters in \a separators,
 *          or NULL if no such character exists.
 *
 * See [strpbrk](http://man7.org/linux/man-pages/man3/strpbrk.3.html) for more details.
 */
LIB const char* OVERRIDABLE(findFirst)(const char *str, const char *separators);

/**
 * \brief   This function locates the last occurrence in a string of a given character.
 * \param   str The string to be scanned.
 * \param   c The character to be found.
 * \return  This function returns a pointer to the last occurence of \a c in \a str, or NULL if character \a c is not present in \a str.
 */
LIB char* OVERRIDABLE(findLast)(char *str, char c);

/**
 * \brief   This function compares two strings.
 * \param   str1 The first  string.
 * \param   str2 The second string.
 * \return  This function returns an integer less than, equal to, or greater than zero if \a str1 is found, respectively,
 *          to be less than, to match, or be greater than \a str2.
 *
 * See [strcmp](http://man7.org/linux/man-pages/man3/strcmp.3.html) for more details.
 */
LIB int OVERRIDABLE(stringCompare)(const char *str1, const char *str2);

/**
 * \brief   This function returns a pointer to a new string which is the concatenation of two given strings.
 * \param   str1 The first  string.
 * \param   str2 The second string.
 * \param   minDestSize The minimal size of the array that the function is expected to allocate in order to store the resulting string
            (more memory will be allocated if necessary, so 0 is a valid value).
 * \return  On success, this function returns a new string which contains the characters in \a str1 followed by
 *          those in \a str2. It returns NULL if insufficient memory was available.
 */
LIB char* OVERRIDABLE(concatenateStrings)(const char *str1, const char *str2, size_t minDestSize);

/**
 * \brief   This function copies up to \a destSize - 1 characters from the null-terminated string \a src to \a dst,
            null-terminating the result.
 * \param   dest The destination buffer (its size must be greater than or equal to \a destSize).
 * \param   src The string to be copied.
 * \param   destSize The maximum number of characters to be copied.
 *
 * See [strlcpy](https://www.commandlinux.com/man-page/man3/strlcpy.3.html) for more details.
 */
LIB void OVERRIDABLE(copyStringWithLength)(char *dest, const char * src, size_t destSize);

/**
 * \brief   This function reverses a given string.
 * \param   str The string to be reversed.
 * \return  This function first reverses the entire sequence of characters specified in \a str, e.g. "abc" becomes "cba",
 *          and then returns a pointer to \a str for convenience.
 */
LIB char* OVERRIDABLE(mkReverse)(char *str);

/**
 * \brief   This function tests whether or not a substring is a prefix of a string.
 * \param   str The string to be scanned.
 * \param   start The substring to be tested.
 * \param   csentive A boolean indicating whether or not this search is case sensitive.
 * \return  This function returns a pointer to the character following the substring in \a str if the test succeeds,
 *          or NULL if \a start is not a prefix of \a str.
 */
LIB const char* OVERRIDABLE(startWith)(const char *str, const char *start, int csensitive);

/**
 * \brief   This function tests whether or not a string contains a given character.
 * \param   c The character to be found.
 * \param   str The string to be scanned (the terminating null-character is ignored).
 * \return  This function returns 1 if \a str contains \a c. Otherwise it returns 0.
 */
LIB int OVERRIDABLE(belongs)(char c, const char *str);

/**
 * \brief   This function returns a pointer to a new string which contains the first \a length characters of a given string.
 * \param   start The string to be copied.
 * \param   length The maximum number of characters to be copied.
 * \return  On success, this function returns a new string which contains the first \a length characters of \a start (the whole string
 *          may be copied if the length of \a start is less than \a length). It returns NULL if insufficient memory was available.
 */
LIB char* OVERRIDABLE(subString)(const char *start, size_t length);

/**
 * \brief   This function edits a string so that it becomes equal to the longest common prefix string between the original string and another one.
 * \param   result The string to be edited.
 * \param   str The other string.
 *
 * Example:
 * \code
 * int main()
 * {
 *     char example[PATH_MAX] = "path/to/my/file.txt";
 *     mkCommon(example, "path/to/my/file"); printf("%s\n", example) // --- path/to/my/file
 *     mkCommon(example, "path/to"        ); printf("%s\n", example) // --- path/to
 *     mkCommon(example, "path/to/my/file"); printf("%s\n", example) // --- path/to
 *     mkCommon(example, "another"        ); printf("%s\n", example) // ---
 *     return 0;
 * }
 * \endcode
 * The function has no effect if \a result is a prefix of \a str.
 */
LIB void OVERRIDABLE(mkCommon)(char *result, const char *str);

/**
 * \brief   This function tests whether or not a given string contains a non-space character.
 * \param   str The string to be scanned.
 * \return  This function returns 1 if \a str contains a non-space character. Otherwise it returns 0.
 */
LIB int OVERRIDABLE(isNotEmpty)(const char *str);

/**
 * \brief   This function returns a pointer to a new string which is obtained by escaping (doubling) all occurences of a given character in a string.
 * \param   str The input string.
 * \param   c The character to be escaped (doubled).
 * \return  On success, this function returns a pointer to a new string which is obtained by escaping (doubling)
 *          all occurences of \a c in \a str. It returns NULL if insufficient memory was available.
 *
 * Example:
 * \code
 * int main()
 * {
 *     char *prot = getProtString("$Vincent$Vega$",'$');
 *     if(prot) {
 *         printf("%s\n",prot); // ------------------------ $$Vincent$$Vega$$
 *         free(prot); // --------------------------------- You must free the string returned.
 *     }
 *     return 0;
 * }
 * \endcode
 * See its reverse function, \ref getRealString, for more details.
 */
LIB char* OVERRIDABLE(getProtString)(const char *str, char c);

/**
 * \brief       This function returns a pointer to a new string which is obtained by replacing every two consecutive occurrences of a given
 *              character in a string with a single copy of that character.
 * \param[in]   str The input string.
 * \param[in]   c The character to be found.
 * \param[out]  notEscaped On success, \a *notEscaped points to the first occurence of \a c in the returned string that does not
 *              correspond to two consecutive occurences of \a c in \a str. If such character does not exist, \a *notEscaped is set to NULL.
 * \return      On success, this function returns a pointer to a new string which is obtained by replacing every two consecutive
 *              occurrences of \a c in \a str with a single copy of \c c. It returns NULL if insufficient memory was available.
 *
 * Example:
 * \code
 * int main()
 * {
 *     char *comment = NULL;
 *     char *cmd = getRealString("echo Leeroy Jenkins ##ftw # If you understand this, you're just a nerd!", '#', &comment);
 *     // ------------------------------------------- ^^ these two # will be replaced by a single #
 *     if(cmd) {
 *         printf("%s\n", cmd); // ----------- echo Leeroy Jenkins #ftw # If you understand this, you're just a nerd!
 *         // ------------------------------------------------- comment ^
 *         *comment = '\O'; // --------------- This statement removes the comment but not the command itself.
 *         printf("%s\n", cmd); // ----------- echo Leeroy Jenkins #ftw
 *         free(cmd); // --------------------- You must free the string returned.
 *     }
 *     return 0;
 * }
 * \endcode
 * See its reverse function, \ref getProtString, for more details.
 */
LIB char* OVERRIDABLE(getRealString)(const char *str, char c, char **notEscaped);

/**
 * \}
 *****/

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 * \defgroup Tokenizer
 * \brief This \ref Iterator traverses a serialized list of strings seperated by arbitrary seperators.
 *
 * Example:
 * \code
 * int main() {
 *     Tokenizer *tokenizer = Tokenizer_new("salut:les?amis!", ":?");
 *     if(tokenizer) {
 *         while(!Tokenizer_isOver(tokenizer)) {            // 1: salut
 *             printf("%s\n", Tokenizer_get(tokenizer));    // 2: les
 *             Tokenizer_next(tokenizer);                   // 3: amis!
 *     } }
 *     Tokenizer_delete(tokenizer);
 *     return 0;
 * }
 * \endcode
 * It may be used, for example, to iterate through the directories listed in the PATH environment variable.
 * \{
 *****/

/**
 * \brief This data structure enables a serialized list of strings seperated by arbitrary seperators to be traversed.
 *
 * See [this page](\ref Tokenizer) for more details.
 */
typedef struct {

    //! The string to be parsed.
    const char *str;

    //! The list of separators based on which the string is split.
    const char *separators;

    //! Do with this pointer what you wish!
    const char *next;

} Tokenizer;

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**
 * \brief   This function allocates and initialises a new Tokenizer.
 * \param   str The string to be parsed.
 * \param   separators The list of separators based on which the string is split.
 * \return  This function returns a pointer to a new Tokenizer, or NULL if an error occured. The iterator references, if any,
 *          the first element of the list.
 */
LIB Tokenizer* OVERRIDABLE(Tokenizer_new)(const char *str, const char *separators);

/**
 * \brief   This function initialises a Tokenizer.
 * \param   tokenizer The Tokenizer to be initialised.
 * \param   str The string to be parsed.
 * \param   separators The list of separators based on which the string is split.
 * \return  This function returns 1 if the procedure ran successfully. Otherwise it return 0. On success, the iterator references,
 *          if any, the first element of the list.
 */
LIB int OVERRIDABLE(Tokenizer_init)(Tokenizer *tokenizer, const char *str, const char *separators);

/**
 * \brief   This function finalizes and free a Tokenizer.
 * \param   tokenizer The Tokenizer to be deleted.
 */
LIB void OVERRIDABLE(Tokenizer_delete)(Tokenizer *tokenizer);

/**
 * \brief   This function finalizes a Tokenizer.
 * \param   tokenizer The Tokenizer to be finalized.
 */
LIB void OVERRIDABLE(Tokenizer_finalize)(Tokenizer *tokenizer);

/**
 * \brief   This function tests whether or not a given Tokenizer is over.
 * \param   tokenizer The Tokenizer.
 * \return  This function returns 1 if the Tokenizer is over (has reached the end of the list). Otherwise it returns 0.
 */
LIB int OVERRIDABLE(Tokenizer_isOver)(const Tokenizer *tokenizer);

/**
 * \brief   This function returns the substring referenced by a Tokenizer in a new string.
 * \param   tokenizer The Tokenizer.
 * \return  On success, this function returns the substring referenced by a Tokenizer in a new string. It returns NULL if insufficient
 *          memory was available.
 *
 * If the Tokenizer is over (see \ref Tokenizer_isOver for more details), the behavior is undefined.
 */
LIB char* OVERRIDABLE(Tokenizer_get)(const Tokenizer *tokenizer);

/**
 * \brief   This function increments a Tokenizer.
 * \param   tokenizer The Tokenizer.
 *
 * If the Tokenizer is over (see \ref Tokenizer_isOver for more details), the behavior is undefined.
 */
LIB void OVERRIDABLE(Tokenizer_next)(Tokenizer *tokenizer);

/**
 * \}
 * \}
 *****/

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

// for curious persons only
#include "impl/string.h"

#endif
