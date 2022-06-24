/*--------------------------------------------------------------------*/
/* synanalysis.h                                                      */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#ifndef SYNANALYSIS_INCLUDED
#define SYNANALYSIS_INCLUDED

#include "dynarray.h"
#include "command.h"

/*--------------------------------------------------------------------*/

/* Syntactically analyze DynArray oTokens. If pcLine contains a 
   syntactic error, return NULL. Otherwise return a Command object 
   containing the sorted tokens in oTokens. The caller owns the 
   Command object and the strings and DyArray object it contains. 
   Pass in pcPgmName in case of error. */

Command_T SynAnalysis_synArray(DynArray_T oTokens,
                               const char *pcPgmName);

#endif
