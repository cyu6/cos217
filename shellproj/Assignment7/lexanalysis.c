/*--------------------------------------------------------------------*/
/* lexanalysis.c                                                      */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#include "lexanalysis.h"
#include "token.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*--------------------------------------------------------------------*/

/* Create a token with the type eTokenType and value pcBuffer, ending 
   the string pcBuffer at the index uBufferIndex. Add the token to the 
   array oTokens. If the token cannot be added, exit and print to 
   stderr using pcPgmName. */

static void LexAnalysis_addToArray(enum TokenType eTokenType,
                                   char *pcBuffer, DynArray_T oTokens,
                                   int uBufferIndex,
                                   const char *pcPgmName)
{
   Token_T psToken;
   int iSuccessful;

   assert(pcBuffer != NULL);
   assert(oTokens != NULL);
   assert(pcPgmName != NULL);

   pcBuffer[uBufferIndex] = '\0';
   psToken = Token_new(eTokenType, pcBuffer, pcPgmName);
   iSuccessful = DynArray_add(oTokens, psToken);
   if (!iSuccessful)
   {
      perror(pcPgmName);
      exit(EXIT_FAILURE);
   }   
}

/*--------------------------------------------------------------------*/

DynArray_T LexAnalysis_lexLine(const char *pcLine,
                               const char *pcPgmName)
{
   /* The current state of the DFA. */
   enum LexState eState = NORMAL;

   /* An index into pcLine. */
   size_t uLineIndex = 0;

   /* Pointer to a buffer in which the characters comprising each 
      token are accumulated. */
   char *pcBuffer;

   /* An index into the buffer. */
   int uBufferIndex = 0;

   char c;
   DynArray_T oTokens;

   assert(pcLine != NULL);
   assert(pcPgmName != NULL);

   /* Create an empty token DynArray object. */
   oTokens = DynArray_new(0);
   if (oTokens == NULL)
      {perror(pcPgmName); exit(EXIT_FAILURE);}

   /* Allocate memory for a buffer that is large enough to store the 
      largest token that might appear within pcLine. */
   pcBuffer = (char*)malloc(strlen(pcLine) + 1);
   if (pcBuffer == NULL)
      {perror(pcPgmName); exit(EXIT_FAILURE);}

   /* "Read" the next character from pcLine. */
   while ((c = pcLine[uLineIndex++]) != '\0')
   {
      switch(eState)
      {
         /* Handle the NORMAL state. */
         case NORMAL:
            if (isspace(c))
               eState = NORMAL;
            else if (c == '\"')
               eState = QUOTES;
            else if (c == '>' || c == '<')
            {
               /* If something is left in the buffer, create an ordinary
                  token. */
               if (uBufferIndex != 0)
                  LexAnalysis_addToArray(TOKEN_ORDINARY, pcBuffer,
                                         oTokens, uBufferIndex,
                                         pcPgmName);
               uBufferIndex = 0;
               /* Create a special token. */
               pcBuffer[uBufferIndex++] = c;
               LexAnalysis_addToArray(TOKEN_SPECIAL, pcBuffer, oTokens,
                          uBufferIndex, pcPgmName);
               uBufferIndex = 0;
               eState = IN_TOKEN;
            }
            else
            {
               /* Add to buffer */
               pcBuffer[uBufferIndex++] = c;
               eState = IN_TOKEN;
            }
            break;
            
         /* Handle the IN_TOKEN state. */
         case IN_TOKEN:
            if (c == '>' || c == '<')
            {
               /* Create an ordinary token. */
               LexAnalysis_addToArray(TOKEN_ORDINARY, pcBuffer, oTokens,
                          uBufferIndex, pcPgmName);
               uBufferIndex = 0;
               /* Create a special token. */
               pcBuffer[uBufferIndex++] = c;
               LexAnalysis_addToArray(TOKEN_SPECIAL, pcBuffer, oTokens,
                          uBufferIndex, pcPgmName);
               uBufferIndex = 0;
               eState = IN_TOKEN;
            }
            else if (isspace(c))
            {
               /* If something is left in the buffer, create an 
                  ordinary token. */
               if (uBufferIndex != 0)
                  LexAnalysis_addToArray(TOKEN_ORDINARY, pcBuffer,
                                         oTokens, uBufferIndex,
                                         pcPgmName);
               uBufferIndex = 0;
               eState = NORMAL;
            }
            else if (c == '\"')
            {
               eState = QUOTES;
            }
            else
            {
               /* Add to buffer */
               pcBuffer[uBufferIndex++] = c;
               eState = IN_TOKEN;
            }
            break;

         /* Handle the QUOTES state. */
         case QUOTES:
            if (c == '\"')
               eState = IN_TOKEN;
            else
            {
               /* Add to buffer */
               pcBuffer[uBufferIndex++] = c;
               eState = QUOTES;
            }
            break;
            
         default:
            assert(0);
      }
   }
   
   /* Something is left in the buffer and we are still in the token.. */
   if (uBufferIndex != 0 && eState == IN_TOKEN)
   {
      /* Create an ordinary token. */
      LexAnalysis_addToArray(TOKEN_ORDINARY, pcBuffer, oTokens,
                             uBufferIndex, pcPgmName);  
      uBufferIndex = 0; 
   }
   /* Return NULL if end in a reject state and print to stderror. */
   else if (eState == QUOTES)
   {
      fprintf(stderr, "%s: unmatched quote\n", pcPgmName);
      free(pcBuffer);
      Token_freeTokens(oTokens);
      DynArray_free(oTokens);
      return NULL;
   }
   
   free(pcBuffer);
   return oTokens;
}
