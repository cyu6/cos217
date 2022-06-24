/*--------------------------------------------------------------------*/
/* symtable.h                                                         */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stddef.h>

/* A SymTable_T object is a symbol table that stores keys as strings and
   their associated values. */

typedef struct SymTable *SymTable_T;

/*--------------------------------------------------------------------*/

/* Return a new SymTable_T object, or NULL if insufficient memory is
   available. */

SymTable_T SymTable_new(void);

/*--------------------------------------------------------------------*/

/* Free oSymTable. */

void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Find and return the number of bindings in oSymTable. */

size_t SymTable_getLength(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Add pcKey with pvValue into oSymTable if oSymTable does not already 
   contain a binding with key pcKey and return 1 (TRUE). Otherwise, 
   leave oSymTable unchanged and return 0 (FALSE). Return 0 (FALSE) if 
   insufficient memory is available. */

int SymTable_put(SymTable_T oSymTable, const char *pcKey,
                 const void *pvValue);

/*--------------------------------------------------------------------*/

/* If oSymTable contains a binding with key pcKey, replace the value 
   stored in pcKey with pvValue and return the old value. Otherwise, 
   leave oSymTable unchanged and return NULL. */

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
                       const void *pvValue);

/*--------------------------------------------------------------------*/

/* Return 1 (TRUE) if oSymTable contains a binding whose key is pcKey 
   and 0 (FALSE) otherwise. */

int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Return the value of the binding in oSymTable whose key is pcKey or 
   NULL if this binding does not exist. */

void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Remove pcKey from oSymTable if it exists in oSymTable and return the
   binding's value. Otherwise leave oSymTable unchanged and return 
   NULL. */

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Apply function *pfApply to each binding in oSymTable, passing pvExtra
   as an extra parameter. */

void SymTable_map(SymTable_T oSymTable,
                  void (*pfApply)(const char *pcKey, void *pvValue,
                                  void *pvExtra),
                  const void *pvExtra);

#endif
