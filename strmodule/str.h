/*--------------------------------------------------------------------*/
/* str.h                                                              */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#ifndef STR_H
#define STR_H
#include <stddef.h>

/* Return the number of characters in the string pointed to by pcSrc. */

size_t Str_getLength(const char *pcSrc);

/*--------------------------------------------------------------------*/

/* Copy string pcSrc into the string pcDest. Return the new pcDest. */

char *Str_copy(char *pcDest, const char *pcSrc);

/*--------------------------------------------------------------------*/

/* Appends contents of string pcSrc to the end of string pcDest. Return
   the new pcDest. */

char *Str_concat(char *pcDest, const char *pcSrc);

/*--------------------------------------------------------------------*/

/* Compares the strings pcStr1 and pcStr2. Return a value less than, 
   equal to, or greater than 0 depending on whether pcStr1 is less than,
   equal to, or greater than pcStr2. */

int Str_compare(const char *pcStr1, const char *pcStr2);

/*--------------------------------------------------------------------*/

/* Searches the string pcHaystack for the string pcNeedle. If pcNeedle
   is empty, return pcHaystack. If pcNeedle is not in pcHaystack, return
   NULL. Otherwise, return the address of the first character within 
   pcHaystack at which pcNeedle occurs. */

char *Str_search(const char *pcHaystack, const char *pcNeedle);

#endif
