/*--------------------------------------------------------------------*/
/* ishsyn.c                                                           */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#include "dynarray.h"
#include "token.h"
#include "command.h"
#include "lexanalysis.h"
#include "synanalysis.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*--------------------------------------------------------------------*/

/* The name of the executable binary file. */
static const char *pcPgmName;

/*--------------------------------------------------------------------*/

/* If no lines remain in psFile, then return NULL. Otherwise read a line
   of psFile and return it as a string. The string does not contain a
   terminating newline character. The caller owns the string. */

static char *readLine(FILE *psFile)
{
   const size_t INITIAL_LINE_LENGTH = 2;
   const size_t GROWTH_FACTOR = 2;

   size_t uLineLength = 0;
   size_t uPhysLineLength = INITIAL_LINE_LENGTH;
   char *pcLine;
   int iChar;

   assert(psFile != NULL);

   /* If no lines remain, return NULL. */
   if (feof(psFile))
      return NULL;
   iChar = fgetc(psFile);
   if (iChar == EOF)
      return NULL;

   /* Allocate memory for the string. */
   pcLine = (char*)malloc(uPhysLineLength);
   if (pcLine == NULL)
      {perror(pcPgmName); exit(EXIT_FAILURE);}

   /* Read characters into the string. */
   while ((iChar != '\n') && (iChar != EOF))
   {
      if (uLineLength == uPhysLineLength)
      {
         uPhysLineLength *= GROWTH_FACTOR;
         pcLine = (char*)realloc(pcLine, uPhysLineLength);
         if (pcLine == NULL)
            {perror(pcPgmName); exit(EXIT_FAILURE);}
      }
      pcLine[uLineLength] = (char)iChar;
      uLineLength++;
      iChar = fgetc(psFile);
   }

   /* Append a null character to the string. */
   if (uLineLength == uPhysLineLength)
   {
      uPhysLineLength++;
      pcLine = (char*)realloc(pcLine, uPhysLineLength);
      if (pcLine == NULL)
         {perror(pcPgmName); exit(EXIT_FAILURE);}
   }
   pcLine[uLineLength] = '\0';

   return pcLine;
}

/*--------------------------------------------------------------------*/

/* Read a line from stdin.  Write the line to stdout. If the line 
   contains non-characters, then write an error message to stderr 
   and reject the line. Otherwise write to stdout each part of a 
   Command that the line contains and their respective category. 
   Repeat until EOF. Return 0 iff successful. As always, argc is the 
   command-line argument count and argv is an array of 
   command-line arguments. */

int main(int argc, char *argv[])
{
   char *pcLine;
   DynArray_T oTokens;
   Command_T psCommand;
   int iRet;

   pcPgmName = argv[0];

   printf("%% ");

   while ((pcLine = readLine(stdin)) != NULL)
   {
      printf("%s\n", pcLine);
      iRet = fflush(stdout);
      if (iRet == EOF)
         {perror(pcPgmName); exit(EXIT_FAILURE);}
      oTokens = LexAnalysis_lexLine(pcLine, pcPgmName);
      if (oTokens != NULL)
      {
         psCommand = SynAnalysis_synArray(oTokens, pcPgmName);
         if (psCommand != NULL)
         {
            Command_write(psCommand);
            Command_free(psCommand);
            Token_freeTokens(oTokens);
            DynArray_free(oTokens);
         }
      }

      printf("%% ");
      free(pcLine);
   }
   return 0;
}
