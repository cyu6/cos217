/*--------------------------------------------------------------------*/
/* command.h                                                          */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#ifndef COMMAND_INCLUDED
#define COMMAND_INCLUDED

#include "dynarray.h"
#include "token.h"

/* A Command_T object is a data structure that stores tokens
   of a command-line command as strings. */

typedef struct Command *Command_T;

/*--------------------------------------------------------------------*/

/* Create and return a command whose name is the string pcName, whose 
   arguments are stored in a DynArray oArgs, and whose stdin and 
   stdout values are respectively the strings pcStdin and pcStdout. 
   Pass in pcPgmName in case of error. */

Command_T Command_new(char *pcName, DynArray_T oArgs, char *pcStdin,
                      char *pcStdout, const char* pcPgmName);

/*--------------------------------------------------------------------*/

/* Write all non-NULL parts of psCommand to stdout in order of: name, 
   arguments, stdin, stdout. */

void Command_write(Command_T psCommand);

/*--------------------------------------------------------------------*/

/* Free all of the tokens in psCommand. */

void Command_free(Command_T psCommand);

/*--------------------------------------------------------------------*/

/* Return name of psCommand. */

char *Command_getName(Command_T psCommand);

/*--------------------------------------------------------------------*/

/* Return arguments of psCommand. */

DynArray_T Command_getArgs(Command_T psCommand);

/*--------------------------------------------------------------------*/

/* Return stdin file of psCommand. */

char *Command_getStdin(Command_T psCommand);

/*--------------------------------------------------------------------*/

/* Return stdout file of psCommand. */

char *Command_getStdout(Command_T psCommand);

#endif
   
