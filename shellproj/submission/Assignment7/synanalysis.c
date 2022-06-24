/*--------------------------------------------------------------------*/
/* synanalysis.c                                                      */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#include "synanalysis.h"
#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* Define true and false */
enum {TRUE, FALSE};

/*--------------------------------------------------------------------*/

/* Checks for errors with a stdin special token using psNextToken and 
   iSpecialIn. If error found, free oTokens, oArgs and write to stderr 
   using pcPgmName. Returns TRUE if successful, FALSE if error. */

static int SynAnalysis_errorStdIn(Token_T psNextToken,
                                   DynArray_T oTokens, DynArray_T oArgs,
                                   int iSpecialIn,
                                   const char *pcPgmName)
{
   assert(oTokens != NULL);
   assert(oArgs != NULL);
   assert(pcPgmName != NULL);
   
   /* Must have only one of each special. */
   if (iSpecialIn == TRUE)
   {
      fprintf(stderr, "%s: multiple redirection of standard input\n",
              pcPgmName);
      Token_freeTokens(oTokens);
      DynArray_free(oTokens);
      DynArray_free(oArgs);
      return FALSE;  
   }
   /* If first occurrence of special, must have ordinary token after
      special token. */
   else if (iSpecialIn == FALSE)
   {
      if (psNextToken == NULL ||
          Token_getType(psNextToken) == TOKEN_SPECIAL)
      {
         fprintf(stderr,
                 "%s: standard input redirection without file name\n",
                 pcPgmName);
         Token_freeTokens(oTokens);
         DynArray_free(oTokens);
         DynArray_free(oArgs);
         return FALSE;
      }
   }
   return TRUE;
}

/*--------------------------------------------------------------------*/

/* Checks for errors with a stdout special token using psNextToken and 
   iSpecialOut. If error found, free oTokens, oArgs and write to stderr
   using pcPgmName. Return TRUE if successful, FALSE if error. */

static int SynAnalysis_errorStdOut(Token_T psNextToken,
                                   DynArray_T oTokens, DynArray_T oArgs,
                                   int iSpecialOut,
                                   const char *pcPgmName)
{
   assert(oTokens != NULL);
   assert(oArgs != NULL);
   assert(pcPgmName != NULL);
   
   /* Must have only one of each special. */
   if (iSpecialOut == TRUE)
   {
      fprintf(stderr, "%s: multiple redirection of standard output\n",
              pcPgmName);
      Token_freeTokens(oTokens);
      DynArray_free(oTokens);
      DynArray_free(oArgs);
      return FALSE;
   }
   /* If first occurrence of special, must have ordinary token after
      special token. */
   else if (iSpecialOut == FALSE)
   {
      if (psNextToken == NULL ||
          Token_getType(psNextToken) == TOKEN_SPECIAL)
      {
         fprintf(stderr,
                 "%s: standard output redirection without file name\n",
                 pcPgmName);
         Token_freeTokens(oTokens);
         DynArray_free(oTokens);
         DynArray_free(oArgs);
         return FALSE;
      }
   }
   return TRUE;
}


/*--------------------------------------------------------------------*/

Command_T SynAnalysis_synArray(DynArray_T oTokens,
                               const char *pcPgmName)
{
   /* An index into oTokens. */
   size_t u = 0;
   size_t uLength;

   /* Pointer to current token in array. */
   Token_T psToken;
   /* Pointer to next token in array. */
   Token_T psNextToken = NULL;

   /* psToken's type. */
   enum TokenType eType;
   /* psToken's value. */
   char *pcValue;
   
   int iSuccessful; 
   int iSpecialIn = FALSE; /* For checking for one stdin token. */
   int iSpecialOut = FALSE; /* For checking for one stdout token. */
   char *pcName;
   char *pcStdin = NULL;
   char *pcStdout = NULL;
   DynArray_T oArgs;
   Command_T psCommand;
   
   assert(oTokens != NULL);
   assert(pcPgmName != NULL);

   /* Return NULL if oTokens is empty. */
   if (DynArray_getLength(oTokens) == 0)
   {
      Token_freeTokens(oTokens);
      DynArray_free(oTokens);
      return NULL;
   }
   
   /* Create an empty string DynArray object. */
   oArgs = DynArray_new(0);
   if (oArgs == NULL)
      {perror(pcPgmName); exit(EXIT_FAILURE);}

   psToken = DynArray_get(oTokens, 0);
   eType = Token_getType(psToken);
   pcValue = Token_getValue(psToken);

   /* Must start with ordinary token (name). */
   if (eType != TOKEN_ORDINARY)
   {
      fprintf(stderr, "%s: missing command name\n", pcPgmName);
      DynArray_free(oArgs);
      Token_freeTokens(oTokens);
      DynArray_free(oTokens);
      return NULL;
   }
   pcName = pcValue;
      
   /* Loop through oTokens, checking if the command is accurate and 
      storing right parts into a Command object. */
   uLength = DynArray_getLength(oTokens);
   for (u = 1; u < uLength; u++)
   {
      psToken = DynArray_get(oTokens, u);
      eType = Token_getType(psToken);
      pcValue = Token_getValue(psToken);

      if ((u + 1) != uLength)
         psNextToken = DynArray_get(oTokens, u+1);
      else
         psNextToken = NULL;
      
      /* Check type. If ordinary, add to oArgs. */
      if (eType == TOKEN_ORDINARY)
      {
         iSuccessful = DynArray_add(oArgs, pcValue);
         if (!iSuccessful)
            return NULL;
      }
      /* If special, do error checks and save pointer if correct. */
      else if (eType == TOKEN_SPECIAL)
      {
         if (strcmp(pcValue, "<") == 0)
         {
            iSuccessful = SynAnalysis_errorStdIn(psNextToken, oTokens,
                                                 oArgs, iSpecialIn,
                                                 pcPgmName);
            if (iSuccessful == FALSE)
               return NULL;
            pcStdin = Token_getValue(psNextToken);
            u = u + 1;
            iSpecialIn = TRUE;
         }
         else if (strcmp(pcValue, ">") == 0)
         {
            iSuccessful = SynAnalysis_errorStdOut(psNextToken, oTokens,
                                                  oArgs, iSpecialOut,
                                                  pcPgmName);
            if (iSuccessful == FALSE)
               return NULL;
            pcStdout = Token_getValue(psNextToken);
            u = u + 1;
            iSpecialOut = TRUE;
         }
         
      }
   }

   psCommand = Command_new(pcName, oArgs, pcStdin, pcStdout, pcPgmName);
   return psCommand;


   
}
