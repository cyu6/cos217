/*--------------------------------------------------------------------*/
/* token.h                                                            */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#ifndef TOKEN_INCLUDED
#define TOKEN_INCLUDED

#include "dynarray.h"

/* A Token object can be either ordinary or special. */
enum TokenType {TOKEN_ORDINARY, TOKEN_SPECIAL};

/*--------------------------------------------------------------------*/

/* A Token_T object is a data structure that has a type and a value
   expressed as a string. */
typedef struct Token *Token_T;

/*--------------------------------------------------------------------*/

/* Create and return a token whose type is eTokenType and whose
   value consists of string pcValue.  The caller owns the token. 
   Pass in pcPgmName in case of error. */

Token_T Token_new(enum TokenType eTokenType, char *pcValue,
                  const char *pcPgmName);

/*--------------------------------------------------------------------*/

/* Write all tokens in oTokens to stdout in order of input into array 
   with their type (ordinary or special). */

void Token_writeTokens(DynArray_T oTokens);

/*--------------------------------------------------------------------*/

/* Free all of the tokens in oTokens. */

void Token_freeTokens(DynArray_T oTokens);

/*--------------------------------------------------------------------*/

/* Return type of psToken. */

enum TokenType Token_getType(Token_T psToken);

/*--------------------------------------------------------------------*/

/* Return value of psToken. */

char *Token_getValue(Token_T psToken);

#endif
