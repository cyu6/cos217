/*--------------------------------------------------------------------*/
/* execution.h                                                        */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#include "command.h"

/*--------------------------------------------------------------------*/

/* Executes the command psCommand using shell built-in functions or
   system-level functions. Handles SIGINT signals in parent and child
   processes. Pass in pcPgmName in case of error. Return 0 from parent 
   process. */

int Execution_execCommand(Command_T psCommand, const char *pcPgmName);
