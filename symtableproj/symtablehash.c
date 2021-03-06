/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "symtable.h"

/* Declare bucket counts for hash table expansion. */
static const size_t auBucketCounts[] = {509, 1021, 2039, 4093, 8191,
                                        16381, 32749, 65521};

/*--------------------------------------------------------------------*/

/* Each binding of the symbol table is stored in a TableNode. TableNodes
   are placed in buckets of the hash table and linked to form lists in 
   each bucket. A TableNode contains a key, value, and link to the 
   next TableNode. */

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

/* A SymTable is a hash table structure that points to the first bucket
   in the hash table, which is represented by an array of the first 
   TableNode in each bucket. A SymTable also contains the logical and 
   physical lengths of the hash table. */

struct SymTable
{
   /* The hash table, represented as an array of linked lists. */
   struct TableNode **hashTable;

   /* Length, or number of TableNodes, of the SymTable. */
   size_t length;

   /* Number of buckets in the hash table. */
   const size_t *uBucketCount;
};

/*--------------------------------------------------------------------*/

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */

static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}

/*--------------------------------------------------------------------*/

/* Expands oSymTable when number of bindings becomes too large. 
   Return 1 (TRUE) if successful or 0 (FALSE) if insufficient memory is
   available or if table already has max number of buckets (65521). */

static int SymTable_grow(SymTable_T oSymTable)
{
   const size_t *uNewBucketCount;
   struct TableNode **newHashTable;
   struct TableNode **oldHashTable;
   struct TableNode *psCurrentNode;
   struct TableNode *psNextNode;
   size_t index;
   size_t bucket;

   assert(oSymTable != NULL);

   /* already have max buckets */
   if (*oSymTable->uBucketCount == 65521)
      return 0;
 
   /* increment array index */
   uNewBucketCount = oSymTable->uBucketCount + 1;
   newHashTable = (struct TableNode**)calloc(*uNewBucketCount,
                                             sizeof(struct TableNode*));
   if (newHashTable == NULL)
      return 0;

   oldHashTable = oSymTable->hashTable;

   /* iterate through the smaller hash table and rehash each binding,
      then add each binding into the larger hash table */
   for(index = 0; index < *oSymTable->uBucketCount; index++)
   {
      for(psCurrentNode = oldHashTable[index];
          psCurrentNode != NULL;
          psCurrentNode = psNextNode)
      {
         psNextNode = psCurrentNode->psNextNode;
         bucket = SymTable_hash(psCurrentNode->pcKey, *uNewBucketCount);
         psCurrentNode->psNextNode = newHashTable[bucket];
         newHashTable[bucket] = psCurrentNode;
      }
   }
   free(oldHashTable);
   oSymTable->hashTable = newHashTable;
   oSymTable->uBucketCount = uNewBucketCount;
   return 1;
}

/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void)
{
   SymTable_T oSymTable;

   oSymTable = (struct SymTable*)malloc(sizeof(struct SymTable));
   if (oSymTable == NULL)
      return NULL;

   oSymTable->hashTable = (struct TableNode**)
      calloc(auBucketCounts[0], sizeof(struct TableNode*));
   oSymTable->length = 0;
   oSymTable->uBucketCount = auBucketCounts;
   return oSymTable;
}

/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable)
{
   size_t index;
   struct TableNode *psCurrentNode;
   struct TableNode *psNextNode;

   assert(oSymTable != NULL);

   for(index = 0; index < *oSymTable->uBucketCount; index++)
   {
      for(psCurrentNode = oSymTable->hashTable[index];
          psCurrentNode != NULL;
          psCurrentNode = psNextNode)
      {
         psNextNode = psCurrentNode->psNextNode;
         free((void*)psCurrentNode->pcKey);
         free(psCurrentNode);
      }
   }
   free(oSymTable->hashTable);
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
   size_t index;
   struct TableNode *psNewNode;
   const char *pcKeyCopy;
   
   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   /* if pcKey already in table, return */
   if (SymTable_contains(oSymTable, pcKey))
      return 0;

   /* expand hash table if necessary, continues with put even if there 
      is insufficient memory */
   if (oSymTable->length == *oSymTable->uBucketCount)
      SymTable_grow(oSymTable); 

   /* determine bucket to put pcKey into */
   index = SymTable_hash(pcKey, *oSymTable->uBucketCount);
   
   psNewNode = (struct TableNode*)malloc(sizeof(struct TableNode));
   if (psNewNode == NULL)
      return 0;
   
   /* make a defensive copy of the key */
   pcKeyCopy = (const char*)malloc(strlen(pcKey)+1);
   strcpy((char*)pcKeyCopy, pcKey);

   /* add to front of list in bucket and update length of table */
   psNewNode->pcKey = pcKeyCopy;
   psNewNode->pvValue = pvValue;
   psNewNode->psNextNode = oSymTable->hashTable[index];
   oSymTable->hashTable[index] = psNewNode;
   oSymTable->length++;
   return 1;
}

/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
                       const void *pvValue)
{
   const void *pvOldValue;
   size_t index;
   struct TableNode *psCurrentNode;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   /* determine bucket of pcKey */
   index = SymTable_hash(pcKey, *oSymTable->uBucketCount);

   /* search for pcKey */
   psCurrentNode = oSymTable->hashTable[index];
   while (psCurrentNode != NULL)
   {
      if (strcmp(psCurrentNode->pcKey, pcKey) == 0)
         break;
      psCurrentNode = psCurrentNode->psNextNode;
   }
   
   /* pcKey not in table */
   if (psCurrentNode == NULL)
      return NULL;

   /* pcKey in table */
   pvOldValue = psCurrentNode->pvValue;
   psCurrentNode->pvValue = pvValue;
   return (void*)pvOldValue;
}

/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
{
   size_t index;
   struct TableNode *psCurrentNode;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   /* bucket of pcKey */
   index = SymTable_hash(pcKey, *oSymTable->uBucketCount);

   /* search for pcKey */
   psCurrentNode = oSymTable->hashTable[index];
   while (psCurrentNode != NULL)
   {
      /* return if found */
      if (strcmp(psCurrentNode->pcKey, pcKey) == 0)
         return 1;
      psCurrentNode = psCurrentNode->psNextNode;
   }
   /* pcKey not in the table */
   return 0;
}

/*--------------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
{
   size_t index;
   struct TableNode *psCurrentNode;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   /* determine bucket of pcKey */
   index = SymTable_hash(pcKey, *oSymTable->uBucketCount);
   
   /* search for pcKey */
   psCurrentNode = oSymTable->hashTable[index];
   while (psCurrentNode != NULL)
   {
      /* if found return value of pcKey */
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
   size_t index;
   struct TableNode *psCurrentNode;
   struct TableNode *psPreviousNode;
   const void *pvOldValue;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   /* determine bucket of pcKey */
   index = SymTable_hash(pcKey, *oSymTable->uBucketCount);

   /* search for pcKey */
   psCurrentNode = oSymTable->hashTable[index];
   psPreviousNode = NULL;
   while (psCurrentNode != NULL)
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
      oSymTable->hashTable[index] = psCurrentNode->psNextNode;
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
   size_t index;
   struct TableNode *psCurrentNode;

   assert(oSymTable != NULL);
   assert(pfApply != NULL);
   
   for (index = 0; index < *oSymTable->uBucketCount; index++)
      for (psCurrentNode = oSymTable->hashTable[index];
           psCurrentNode != NULL;
           psCurrentNode = psCurrentNode->psNextNode)
         (*pfApply) (psCurrentNode->pcKey,
                     (void*)psCurrentNode->pvValue, (void*)pvExtra);
}
