/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "symtable.h"

/*--------------------------------------------------------------------*/

/* Each binding of the symbol table is stored in a TableNode. TableNodes
   are linked to form a list. A TableNode contains a key, value, and a
   link to the next TableNode. */

struct TableNode
{
   /* The key (a string). */
   const char *pcKey;

   /* The value. */
   const void *pvValue;

   /* The address of the next TableNode. */
   struct TableNode *psNextNode;
};

/*--------------------------------------------------------------------*/

/* A SymTable is a linked list structure that points to the first 
   TableNode in the list, along with the length of the list. */

struct SymTable
{
    /* The address of the first TableNode. */
   struct TableNode *psFirstNode;

   /* Length, or number of TableNodes, of the SymTable. */
   size_t length;
};

/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void)
{
   SymTable_T oSymTable;

   oSymTable = (struct SymTable*)malloc(sizeof(struct SymTable));
   if (oSymTable == NULL)
      return NULL;

   oSymTable->psFirstNode = NULL;
   oSymTable->length = 0;
   return oSymTable;
}

/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable)
{
   struct TableNode *psCurrentNode;
   struct TableNode *psNextNode;
   
   assert(oSymTable != NULL);

   for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psNextNode)
   {
      psNextNode = psCurrentNode->psNextNode;
      free((void*)psCurrentNode->pcKey);
      free(psCurrentNode);
   }

   free(oSymTable);
}

/*--------------------------------------------------------------------*/

size_t SymTable_getLength(SymTable_T oSymTable)
{
   assert(oSymTable != NULL);
   return oSymTable->length;
}

/*--------------------------------------------------------------------*/

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void
                 *pvValue)
{
   struct TableNode *psNewNode;
   const char *pcKeyCopy;
   
   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   /* if in table already, return */
   if (SymTable_contains(oSymTable, pcKey))
      return 0;
   
   psNewNode = (struct TableNode*)malloc(sizeof(struct TableNode));
   if (psNewNode == NULL)
      return 0;

   /* make a defensive copy of the key */
   pcKeyCopy = (const char*)malloc(strlen(pcKey)+1);
   strcpy((char*)pcKeyCopy, pcKey);

   /* add to front of table and update length of table */
   psNewNode->pcKey = pcKeyCopy;
   psNewNode->pvValue = pvValue;
   psNewNode->psNextNode = oSymTable->psFirstNode;
   oSymTable->psFirstNode = psNewNode;
   oSymTable->length++;
   return 1;
}

/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
                       const void *pvValue)
{
   struct TableNode *psCurrentNode;
   const void *pvOldValue;
   
   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   psCurrentNode = oSymTable->psFirstNode;
   /* check if pcKey in table */
   while (psCurrentNode != NULL)
   {
      /* found pcKey */
      if (strcmp(psCurrentNode->pcKey, pcKey) == 0)
         break;
      psCurrentNode = psCurrentNode->psNextNode;
   }

   /* pcKey not in the table */
   if (psCurrentNode == NULL)
      return NULL;

   /* pcKey in the table */
   pvOldValue = psCurrentNode->pvValue;
   psCurrentNode->pvValue = pvValue;
   return (void*)pvOldValue;
}

/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
{
   struct TableNode *psCurrentNode;
   
   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   psCurrentNode = oSymTable->psFirstNode;
   /* search for pcKey */
   while (psCurrentNode != NULL)
   {
      /* if found pcKey then return */
      if (strcmp(psCurrentNode->pcKey, pcKey) == 0)
         return 1;
      psCurrentNode = psCurrentNode->psNextNode;
   }
   /* does not contain pcKey */
   return 0;
}

/*--------------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
{
   struct TableNode *psCurrentNode;
   
   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   psCurrentNode = oSymTable->psFirstNode;
   /* search for pcKey */
   while (psCurrentNode != NULL)
   {
      /* if found pcKey then return pointer to value */
      if (strcmp(psCurrentNode->pcKey, pcKey) == 0)
         return (void*)psCurrentNode->pvValue;
      psCurrentNode = psCurrentNode->psNextNode;
   }
   /* pcKey not in table */
   return NULL;
}

/*--------------------------------------------------------------------*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey)
{
   struct TableNode *psCurrentNode;
   struct TableNode *psPreviousNode;
   const void *pvOldValue;
   
   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   psCurrentNode = oSymTable->psFirstNode;
   psPreviousNode = NULL;
   /* search for pcKey */
   while(psCurrentNode != NULL)
   {
      /* found pcKey */
      if (strcmp(psCurrentNode->pcKey, pcKey) == 0)
         break;
      psPreviousNode = psCurrentNode;
      psCurrentNode = psCurrentNode->psNextNode;
   }

   /* not in table */
   if (psCurrentNode == NULL)
      return NULL;

   /* in table */
   pvOldValue = psCurrentNode->pvValue;
   
   /* first node */
   if (psPreviousNode == NULL)
      oSymTable->psFirstNode = psCurrentNode->psNextNode;
   /* other nodes */
   else
      psPreviousNode->psNextNode = psCurrentNode->psNextNode;

   /* free memory of pcKey and TableNode, decrement length */
   free((void*)psCurrentNode->pcKey);
   free(psCurrentNode);
   oSymTable->length--;
   return (void*)pvOldValue;
}

/*--------------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable,
                  void (*pfApply)(const char *pcKey, void *pvValue,
                  void *pvExtra),
                  const void *pvExtra)
{
   struct TableNode *psCurrentNode;
   
   assert(oSymTable != NULL);
   assert(pfApply != NULL);

   for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode)
      (*pfApply) (psCurrentNode->pcKey, (void*)psCurrentNode->pvValue,
                  (void*)pvExtra);
}
