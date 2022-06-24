/*--------------------------------------------------------------------*/
/* execution.c                                                        */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#define _GNU_SOURCE

#include "execution.h"
#include "dynarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/*--------------------------------------------------------------------*/

/* This function is intended to be a handler for signals of type 
   SIGINT. Exit the process. iSignal is number of the signal that 
   caused the handler to execute. */

static void Execution_sigintHandler(int iSignal)
{
   exit(0);
}

/*--------------------------------------------------------------------*/

/* Check if command is a shell built-in command: setenv, unsetenv, cd, 
   or exit using pcName. If so, check for errors. If error exists in 
   command, print to stderr using pcPgmName and return 0. Otherwise 
   execute the shell built-in command using oArgs and uArgs to access
   the other values and return 0. Return 1 if pcName is not a shell 
   built-in command. */

static int Execution_handleBuiltIns(char *pcName, const char *pcPgmName,
                          DynArray_T oArgs, size_t uArgs)
{
   int iRet;

   assert(pcName != NULL);
   assert(pcPgmName != NULL);
   assert(oArgs != NULL);
   
   /* setenv */
   if (strcmp(pcName, "setenv") == 0)
   {
      /* Cannot have 0 arguments */
      if (uArgs == 0)
      {
         fprintf(stderr, "%s: missing variable\n", pcPgmName);
         return 0;
      }
      /* Cannot have more than two arguments */
      if (uArgs > 2)
      {
         fprintf(stderr, "%s: too many arguments\n", pcPgmName);
         return 0;
      }

      /* Create env variable var if doesn't exist or update value */
      if (uArgs == 2)
      {
         /* Value given (second argument) */
         iRet = setenv(DynArray_get(oArgs, 0), DynArray_get(oArgs, 1)
                       , 1);
         if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
         return 0;
      }
      else if (uArgs == 1)
      {
         /* Value not given ("" instead) */
         iRet = setenv(DynArray_get(oArgs, 0), "", 1);
         if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
         return 0;
      }
   }
   /* unsetenv */
   else if (strcmp(pcName, "unsetenv") == 0)
   {
      /* Cannot have 0 arguments */
      if (uArgs == 0)
      {
         fprintf(stderr, "%s: missing variable\n", pcPgmName);
         return 0;
      }
      /* Cannot have more than one argument */
      if (uArgs > 1)
      {
         fprintf(stderr, "%s: too many arguments\n", pcPgmName);
         return 0;
      }
      iRet = unsetenv(DynArray_get(oArgs, 0));
      if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
      return 0;
   }
   /* cd */
   else if (strcmp(pcName, "cd") == 0)
   {
      char *pcHome;

      /* Cannot have more than one argument */
      if (uArgs > 1)
      {
         fprintf(stderr, "%s: too many arguments\n", pcPgmName);
         return 0;
      }

      /* Get HOME var */
      pcHome = getenv("HOME");    
      if (uArgs == 0)
         /* Cannot have 0 arguments if HOME is not set */
         if (pcHome == NULL)
         {
            fprintf(stderr, "%s: HOME not set\n", pcPgmName);
            return 0;
         }
         /* If HOME is set, cd HOME */
         else
         {
            iRet = chdir(pcHome);
            if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
            return 0;
         }
      else
      {
         /* One argument - cd that location */
         iRet = chdir(DynArray_get(oArgs, 0));
         if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
         return 0;
      }
      
   }
   /* exit */
   else if (strcmp(pcName, "exit") == 0)
   {
      /* Cannot have any arguments */
      if (uArgs != 0)
      {
         fprintf(stderr, "%s: too many arguments\n", pcPgmName);
         return 0;
      }
      exit(0);
   }
   
   return 1;
}

/*--------------------------------------------------------------------*/

int Execution_execCommand(Command_T psCommand, const char *pcPgmName)
{
   /* The Unix permissions that a newly created file should have. */
   enum {PERMISSIONS = 0600};
   
   pid_t iPid;
   int iRet;
   void (*pfRet)(int);
   char *pcName;
   DynArray_T oArgs;

   /* Number of arguments for execvp */
   size_t uArgs = 0;
   /* Array of arguments for execvp */
   char **apcArgv;

   assert(psCommand != NULL);
   assert(pcPgmName != NULL);

   pcName = Command_getName(psCommand);
   oArgs = Command_getArgs(psCommand);
   uArgs = DynArray_getLength(oArgs);

   /* Ignore SIGINT signal as parent process is handling a command */
   pfRet = signal(SIGINT, SIG_IGN);
   if (pfRet == SIG_ERR) {perror(pcPgmName); exit(EXIT_FAILURE);}
   
   /* Handle built-in commands */
   iRet = Execution_handleBuiltIns(pcName, pcPgmName, oArgs, uArgs);
   if (iRet == 0) {return 0;}

   /* Allocate memory for execvp array */
   apcArgv = (char**)malloc(uArgs + 2);
   if (apcArgv == NULL)
      {perror(pcPgmName); exit(EXIT_FAILURE);}
   
   iRet = fflush(stdin);
   if (iRet == EOF) {perror(pcPgmName); exit(EXIT_FAILURE); }
   iRet = fflush(stdout);
   if (iRet == EOF) {perror(pcPgmName); exit(EXIT_FAILURE); }

   iPid = fork();
   if (iPid == -1) {perror(pcPgmName); exit(EXIT_FAILURE); }

   if (iPid == 0)
   {
      /* This code is executed by the child process only. */

      /* Index pointer for traversing oArgs array */
      size_t u = 1;
   
      int iFd;
      int iRet;

      /* Pointer to Stdin file */
      char *pcStdin = Command_getStdin(psCommand);
      /* Pointer to Stdout file */
      char *pcStdout = Command_getStdout(psCommand);

      pfRet = signal(SIGINT, Execution_sigintHandler);
      if (pfRet == SIG_ERR) {perror(pcPgmName); exit(EXIT_FAILURE);}

      /* Redirect stdin if exists */
      if (pcStdin != NULL)
      {
         iFd = open(pcStdin, O_RDONLY);
         /* Must be redirected to a file that exists */
         if (iFd == -1)
         {
            fprintf(stderr, "%s: No such file or directory\n",
                    pcPgmName);
            exit(EXIT_FAILURE);
         }

         iRet = close(0);
         if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
         
         iRet = dup(iFd);
         if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
         
         iRet = close(iFd);
         if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
      }
      
      /* Redirect stdout if exists */
      if (pcStdout != NULL)
      {
         /* File must have a valid name */
         if (strcmp(pcStdout, "/") == 0 || strcmp(pcStdout, ".") == 0)
         {
            fprintf(stderr, "%s: Is a directory", pcPgmName);
            exit(EXIT_FAILURE);
         }
         /* Check if file exists */
         iFd = open(pcStdout, O_RDONLY);
         /* Create file if file doesn't exist */
         if (iFd == -1)
         {
            iFd = creat(pcStdout, PERMISSIONS);
            if (iFd == -1)
            {
               perror(pcPgmName);
               exit(EXIT_FAILURE);
            }
         }
         /* If file exists, destroy contents and rewrite from scratch */
         else
         {
            iRet = close(iFd);
            if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
            iFd = open(pcStdout, O_WRONLY);
            if (iFd == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
         }

         iRet = close(1);
         if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}

         iRet = dup(iFd);
         if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
         
         iRet = close(iFd);
         if (iRet == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}
      }
      
      apcArgv[0] = pcName;

      /* loop through oArgs */
      for (u = 1; u < uArgs+1; u++)
      {
         apcArgv[u] = DynArray_get(oArgs, u - 1);
      }
      
      apcArgv[u] = NULL;

      /* Execute the command */
      execvp(pcName, apcArgv);
      
      perror(pcPgmName);
      free(apcArgv);
      exit(EXIT_FAILURE);
   }

   /* This code is executed by the parent process only. */
   iPid = wait(NULL);
   free(apcArgv);
   if (iPid == -1) {perror(pcPgmName); exit(EXIT_FAILURE); }

   return 0;
}
