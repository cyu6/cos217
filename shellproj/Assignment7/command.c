/*--------------------------------------------------------------------*/
/* command.c                                                          */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#include "command.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/*--------------------------------------------------------------------*/

/* A Command has a name and some or all of the following in order: 
   arguments, stdin, stdout. */

struct Command
{
   /* The name of the command. */
   char *pcName;

   /* The arguments of the command. */
   DynArray_T oArgs;

   /* The stdin file. */
   char *pcStdin;

   /* The stdout file. */
   char *pcStdout;
};

/*--------------------------------------------------------------------*/

Command_T Command_new(char *pcName, DynArray_T oArgs,
                      char *pcStdin, char *pcStdout,
                      const char *pcPgmName)
{
   Command_T psCommand;

   assert(pcName != NULL);
   assert(oArgs != NULL);
   assert(pcPgmName != NULL);

   psCommand = (Command_T)malloc(sizeof(struct Command));
   if (psCommand == NULL)
   {
      perror(pcPgmName);
      exit(EXIT_FAILURE);
   }
   
   psCommand->pcName = (char*)malloc(strlen(pcName) + 1);
   if (psCommand->pcName == NULL)
      { perror(pcPgmName); exit(EXIT_FAILURE); }
   strcpy(psCommand->pcName, pcName);
   
   psCommand->oArgs = oArgs;

   /* Only allocate space if pcStdin is not NULL */
   if (pcStdin != NULL)
   {
      psCommand->pcStdin = (char*)malloc(strlen(pcStdin) + 1);
      if (psCommand->pcStdin == NULL)
         { perror(pcPgmName); exit(EXIT_FAILURE); }
      strcpy(psCommand->pcStdin, pcStdin);
   }
   else
      psCommand->pcStdin = pcStdin;
   
   /* Only allocate space if pcStdout is not NULL */
   if (pcStdout != NULL)
   {
      psCommand->pcStdout = (char*)malloc(strlen(pcStdout) + 1);
      if (psCommand->pcStdout == NULL)
         { perror(pcPgmName); exit(EXIT_FAILURE); }
      strcpy(psCommand->pcStdout, pcStdout);
   }
   else
      psCommand->pcStdout = pcStdout;
   
   return psCommand;
}

/*--------------------------------------------------------------------*/

void Command_write(Command_T psCommand)
{
   size_t u;
   size_t uLength;
   char *pcArg;
   
   assert(psCommand != NULL);

   printf("Command name: %s\n", psCommand->pcName);

   uLength = DynArray_getLength(psCommand->oArgs);
   for (u = 0; u < uLength; u++)
   {
      pcArg = DynArray_get(psCommand->oArgs, u);
      printf("Command arg: %s\n", pcArg);
   }

   if (psCommand->pcStdin != NULL)
      printf("Command stdin: %s\n", psCommand->pcStdin);
   if (psCommand->pcStdout != NULL)
      printf("Command stdout: %s\n", psCommand->pcStdout);
}

/*--------------------------------------------------------------------*/

void Command_free(Command_T psCommand)
{
   assert(psCommand != NULL);

   free(psCommand->pcName);

   DynArray_free(psCommand->oArgs);

   if (psCommand->pcStdin != NULL)
      free(psCommand->pcStdin);
   if (psCommand->pcStdout != NULL)
      free(psCommand->pcStdout);
   
   free(psCommand);
}   

/*--------------------------------------------------------------------*/

char *Command_getName(Command_T psCommand)
{
   assert(psCommand != NULL);

   return psCommand->pcName;
}

/*--------------------------------------------------------------------*/

DynArray_T Command_getArgs(Command_T psCommand)
{
   assert(psCommand != NULL);

   return psCommand->oArgs;
}

/*--------------------------------------------------------------------*/

char *Command_getStdin(Command_T psCommand)
{
   assert(psCommand != NULL);

   return psCommand->pcStdin;
}

/*--------------------------------------------------------------------*/

char *Command_getStdout(Command_T psCommand)
{
   assert(psCommand != NULL);

   return psCommand->pcStdout;
}
