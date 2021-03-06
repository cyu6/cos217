/*--------------------------------------------------------------------*/
/* token.c                                                            */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/*--------------------------------------------------------------------*/

/* A Token is either ordinary or special, expressed as a string. */

struct Token
{
   /* The type of the token. */
   enum TokenType eType;

   /* The string which is the token's value. */
   char *pcValue;
};

/*--------------------------------------------------------------------*/

Token_T Token_new(enum TokenType eTokenType, char *pcValue,
                  const char *pcPgmName)
{
   Token_T psToken;

   assert(pcValue != NULL);
   assert(pcPgmName != NULL);

   psToken = (Token_T)malloc(sizeof(struct Token));
   if (psToken == NULL)
   {
      perror(pcPgmName);
      exit(EXIT_FAILURE);
   }
   psToken->eType = eTokenType;
   psToken->pcValue = (char*)malloc(strlen(pcValue) + 1);
   if (psToken->pcValue == NULL)
   {
      perror(pcPgmName);
      exit(EXIT_FAILURE);
   }
   strcpy(psToken->pcValue, pcValue);

   return psToken;
}

/*--------------------------------------------------------------------*/

void Token_writeTokens(DynArray_T oTokens)
{
   size_t u;
   size_t uLength;
   Token_T psToken;

   assert(oTokens != NULL);

   uLength = DynArray_getLength(oTokens);

   for (u = 0; u < uLength; u++)
   {
      psToken = DynArray_get(oTokens, u);
      if (psToken->eType == TOKEN_ORDINARY)
         printf("Token: %s (ordinary)\n", psToken->pcValue);
      else if (psToken->eType == TOKEN_SPECIAL)
         printf("Token: %s (special)\n", psToken->pcValue);
   }
}

/*--------------------------------------------------------------------*/

void Token_freeTokens(DynArray_T oTokens)
{
   size_t u;
   size_t uLength;
   Token_T psToken;

   assert(oTokens != NULL);

   uLength = DynArray_getLength(oTokens);

   for (u = 0; u < uLength; u++)
   {
      psToken = DynArray_get(oTokens, u);
      free(psToken->pcValue);
      free(psToken);
   }
}

/*--------------------------------------------------------------------*/

enum TokenType Token_getType(Token_T psToken)
{
   assert(psToken != NULL);
   return psToken->eType;
}

/*--------------------------------------------------------------------*/

char *Token_getValue(Token_T psToken)
{
   assert(psToken != NULL);
   return psToken->pcValue;
}
