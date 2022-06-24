/*--------------------------------------------------------------------*/
/* ish.c                                                              */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#define _GNU_SOURCE

#include "execution.h"
#include "token.h"
#include "lexanalysis.h"
#include "synanalysis.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

/* Amount of seconds for alarm to last */
enum {ALARM_DURATION_SECONDS = 3};

/*--------------------------------------------------------------------*/

/* The name of the executable binary file. */
static const char *pcPgmName;

/*--------------------------------------------------------------------*/

/* This function is intended to be a handler for signals of type 
   SIGINT. Handle next SIGINT as default and set alarm for 3 seconds. 
   iSignal is number of the signal that caused this handler to 
   execute. */

static void sigintHandler(int iSignal)
{
   if (signal(SIGINT, SIG_DFL) == SIG_ERR)
      exit(EXIT_FAILURE);
   alarm(ALARM_DURATION_SECONDS);
}

/*--------------------------------------------------------------------*/

/* This function is intended to be a handler for signals of type 
   SIGALRM. Handle next SIGINT using sigintHandler. iSignal is number 
   of the signal that caused this handler to execute. */

static void sigalrmHandler(int iSignal)
{
   if (signal(SIGINT, sigintHandler) == SIG_ERR)
      exit(EXIT_FAILURE);
}

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

/* Read a line from stdin. Write the line to stdout. If the line 
   contains non-characters, then write an error message to stderr and
   reject the line. Otherwise execute the command of the line. Repeat
   until EOF. Return 0 iff successful. Handle SIGINT signals in the 
   parent and child procceses. As always, argc is the command-line 
   argument count and argv is an array of command-line arguments. */

int main(int argc, char *argv[])
{
   char *pcLine;
   DynArray_T oTokens;
   Command_T psCommand;
   int iRet;
   void (*pfRet)(int);
   
   pcPgmName = argv[0];

   printf("%% ");
   
   /* Ignore unless another SIGINT signal received in three seconds */
   pfRet = signal(SIGINT, sigintHandler);
   if (pfRet == SIG_ERR) {perror(pcPgmName); exit(EXIT_FAILURE);}
   pfRet = signal(SIGALRM, sigalrmHandler);
   if (pfRet == SIG_ERR) {perror(pcPgmName); exit(EXIT_FAILURE);}
   
   while ((pcLine = readLine(stdin)) != NULL)
   {
      printf("%s\n", pcLine);
      iRet = fflush(stdout);
      if (iRet == EOF)
         {perror(pcPgmName); exit(EXIT_FAILURE);}
      /* Lexical analysis */
      oTokens = LexAnalysis_lexLine(pcLine, pcPgmName);
      if (oTokens != NULL)
      {
         /* Syntactic analysis */
         psCommand = SynAnalysis_synArray(oTokens, pcPgmName);
         if (psCommand != NULL)
         {
            /* Execute command */
            Execution_execCommand(psCommand, pcPgmName);

            /* Restore SIGINT handling in parent process while parent 
               is not handling a command */
            pfRet = signal(SIGINT, sigintHandler);
            if (pfRet == SIG_ERR) {perror(pcPgmName);
               exit(EXIT_FAILURE);}
            pfRet = signal(SIGALRM, sigalrmHandler);
            if (pfRet == SIG_ERR) {perror(pcPgmName);
               exit(EXIT_FAILURE);}

            /* Free caller-owned memory */
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
