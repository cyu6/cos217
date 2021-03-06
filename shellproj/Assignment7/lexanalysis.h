/*--------------------------------------------------------------------*/
/* lexanalysis.h                                                      */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#ifndef LEXANALYSIS_INCLUDED
#define LEXANALYSIS_INCLUDED

#include "dynarray.h"

/* LexAnalysis_lexLine() uses a DFA approach. The DFA has these 
   three states: */
enum LexState {NORMAL, IN_TOKEN, QUOTES};

/*--------------------------------------------------------------------*/

/* Lexically analyze string pcLine. If pcLine contains a lexical
   error, then return NULL. Otherwise return a DynArray object
   containing the tokens in pcLine. The caller owns the DynArray
   object and the tokens that it contains. Pass in pcPgmName in case
   of printing to stderr.  */

DynArray_T LexAnalysis_lexLine(const char *pcLine,
                               const char *pcPgmName);

#endif
