/*--------------------------------------------------------------------*/
/* heapmgr5.c                                                         */
/* Author: Joseph Chen and Catherine Yu                               */
/*--------------------------------------------------------------------*/

#define _GNU_SOURCE

#include "heapmgr.h"
#include "checker5.h"
#include "chunk5.h"
#include <stddef.h>
#include <assert.h>
#include <unistd.h>

/* Size of the aoBins array */
#define MAX_BINS 1024

/*--------------------------------------------------------------------*/

/* The state of the HeapMgr. */

/* The address of the start of the heap. */
static Chunk_T oHeapStart = NULL;

/* The address immediately beyond the end of the heap. */
static Chunk_T oHeapEnd = NULL;

/* Array of bins containing free chunks using doubly linked lists. */
static Chunk_T aoBins[MAX_BINS];

/*--------------------------------------------------------------------*/

/* Removes oChunk from the bin corresponding to uUnits and returns 
   oChunk. */

static Chunk_T HeapMgr_removeFromList(Chunk_T oChunk, size_t uUnits)
{
   Chunk_T oNextChunk;
   Chunk_T oPrevChunk;
   Chunk_T oFreeList;
   int index;

   assert(Chunk_isValid(oChunk, oHeapStart, oHeapEnd));

   if (uUnits >= MAX_BINS) index = MAX_BINS - 1;
   else index = (int)uUnits;

   oFreeList = aoBins[index];

   oNextChunk = Chunk_getNextInList(oChunk);
   oPrevChunk = Chunk_getPrevInList(oChunk);

   /* oChunk is start of free list */
   if (oChunk == oFreeList)
   {
      if (oNextChunk != NULL)
      {   
         /* Set footer of next chunk to NULL */
         Chunk_setPrevInList(oNextChunk, NULL);
      }
      /* Set oFreeList to next chunk */
      oFreeList = oNextChunk;
      aoBins[index] = oFreeList;
   }
   else
   {
      if (oPrevChunk != NULL)
      {
         /* Set next of previous to next chunk (could be NULL) */
         Chunk_setNextInList(oPrevChunk, oNextChunk);
      }
      if (oNextChunk != NULL)
      {
         /* Set footer of next chunk to previous chunk */
         Chunk_setPrevInList(oNextChunk, oPrevChunk);
      }
   }

   return oChunk;
}

/*--------------------------------------------------------------------*/

/* Insert oChunk at the front of the bin corresponding to the uUnits in
   oChunk. Return a pointer to the inserted chunk (oChunk). */

static Chunk_T HeapMgr_insert(Chunk_T oChunk, size_t uUnits)
{
   Chunk_T oFreeList;
   size_t index;
   
   assert(Chunk_isValid(oChunk, oHeapStart, oHeapEnd));

   if (uUnits >= MAX_BINS) index = MAX_BINS - 1;
   else index = uUnits;

   oFreeList = aoBins[index];

   /* Nothing in the free list */
   if (oFreeList == NULL)
   {
      /* Next of oChunk is NULL */
      Chunk_setNextInList(oChunk, NULL);;   
   }
   else
   {
      /* Next of oChunk is current front */
      Chunk_setNextInList(oChunk, oFreeList);
      /* Footer of oFreeList is oChunk */
      Chunk_setPrevInList(oFreeList, oChunk);   
   }
   /* Footer of oChunk is NULL */
   Chunk_setPrevInList(oChunk, NULL);   
   /* oFreeList set to oChunk */
   oFreeList = oChunk;

   /* Update aoBins[index] */
   aoBins[index] = oFreeList;
   
   return oChunk;
}

/*--------------------------------------------------------------------*/

/* Request more memory from the operating system -- enough to store
   uUnits units. Create a new chunk, and either append it to the front
   of the free list or increase the size of the previous chunk in
   memory. Return the address of the new (or enlarged) chunk. */

static Chunk_T HeapMgr_getMoreMemory(size_t uUnits)
{
   const size_t MIN_UNITS_FROM_OS = 512;
   Chunk_T oChunk;
   Chunk_T oNewHeapEnd;
   Chunk_T oPrevChunk;
   size_t uBytes;
   size_t uNewUnits;
      
   if (uUnits < MIN_UNITS_FROM_OS)
      uUnits = MIN_UNITS_FROM_OS;

   /* Move the program break. */
   uBytes = Chunk_unitsToBytes(uUnits);
   oNewHeapEnd = (Chunk_T)((char*)oHeapEnd + uBytes);
   if (oNewHeapEnd < oHeapEnd)  /* Check for overflow */
      return NULL;
   if (brk(oNewHeapEnd) == -1)
      return NULL;
   oChunk = oHeapEnd;
   oHeapEnd = oNewHeapEnd;

   /* Set the fields of the new chunk. */
   Chunk_setUnits(oChunk, uUnits);
   Chunk_setNextInList(oChunk, NULL);
   Chunk_setStatus(oChunk, CHUNK_FREE);
   
   /* Add the new chunk to the start of the free list. */
   oChunk = HeapMgr_insert(oChunk, uUnits);

   /* Coalesce the new chunk and the previous one if appropriate. */
   oPrevChunk = Chunk_getPrevInMem(oChunk, oHeapStart);
   if (oPrevChunk != NULL)
   {
      if (Chunk_getStatus(oPrevChunk) == CHUNK_FREE)
      {
         /* Units of the enlarged chunk */
         uNewUnits = Chunk_getUnits(oPrevChunk) + uUnits;
         /* Remove chunks from the list */
         oChunk = HeapMgr_removeFromList(oChunk, uUnits);
         oPrevChunk = HeapMgr_removeFromList(oPrevChunk,
                                             Chunk_getUnits(oPrevChunk));
         /* Coalesce them */
         Chunk_setUnits(oPrevChunk, uNewUnits);
         oChunk = oPrevChunk;
         /* Insert chunk into start of the free list of proper bin */
         oChunk = HeapMgr_insert(oChunk, uNewUnits);
      }
   }

   return oChunk;
}

/*--------------------------------------------------------------------*/

/* If oChunk is close to the right size (as specified by uUnits), then
   remove it from its bin, set its status to INUSE, and return it. If 
   oChunk is too big, then remove oChunk from its bin, split the chunk, 
   insert the tail end of it into the proper bin at the front, set the 
   status of the front end to INUSE, set the status of the tail
   end to FREE, and return the front end of it. */ 

static Chunk_T HeapMgr_useChunk(Chunk_T oChunk, size_t uUnits)
{
   Chunk_T oNewChunk;
   size_t uChunkUnits;
   
   assert(Chunk_isValid(oChunk, oHeapStart, oHeapEnd));

   uChunkUnits = Chunk_getUnits(oChunk);

   /* If oChunk is close to the right size, then use it. */
   if (uChunkUnits < uUnits + MIN_UNITS_PER_CHUNK)
   {
      /* Remove from list */
      oChunk = HeapMgr_removeFromList(oChunk, uChunkUnits);
      /* Set status to INUSE */
      Chunk_setStatus(oChunk, CHUNK_INUSE);
      return oChunk;
   }

   /* oChunk is too big, so use the front end of it. */
   oChunk = HeapMgr_removeFromList(oChunk, uChunkUnits);
   
   /* Split the front end */
   Chunk_setUnits(oChunk, uUnits);
   Chunk_setStatus(oChunk, CHUNK_INUSE);

   /* Set fields and insert tail end into free list at front */
   oNewChunk = Chunk_getNextInMem(oChunk, oHeapEnd);
   Chunk_setUnits(oNewChunk, uChunkUnits - uUnits);
   oNewChunk = HeapMgr_insert(oNewChunk, uChunkUnits - uUnits);
   Chunk_setStatus(oNewChunk, CHUNK_FREE);

   assert(Chunk_isValid(oChunk, oHeapStart, oHeapEnd));

   return oChunk;
}

/*--------------------------------------------------------------------*/

void *HeapMgr_malloc(size_t uBytes)
{
   Chunk_T oChunk;
   Chunk_T oFreeList;
   
   size_t uUnits;
   int count;
   int countInitial; /* starting value of first for loop in Step 3 */
   
   if (uBytes == 0)
      return NULL;

   /* Step 1: Initialize the heap manager if this is the first call. */
   if (oHeapStart == NULL)
   {
      oHeapStart = (Chunk_T)sbrk(0);
      oHeapEnd = oHeapStart;
   }

   assert(Checker_isValid(oHeapStart, oHeapEnd, aoBins, MAX_BINS));

   /* Step 2: Determine the number of units the new chunk should
      contain. */
   uUnits = Chunk_bytesToUnits(uBytes);

   /* Only loop through the free list of the last bin */
   if (uUnits >= MAX_BINS) countInitial = MAX_BINS - 1;
   else countInitial = (int)uUnits;

   /* Step 3: For each bin from the proper start bin to the last bin.. */
   for (count = countInitial; count < MAX_BINS; count++)
   {
      oFreeList = aoBins[count];
      
      /* For each chunk in the current bin.. */
      for (oChunk = oFreeList;
           oChunk != NULL;
           oChunk = Chunk_getNextInList(oChunk))
      {
         /* If oChunk is big enough, then use it. */
         if (Chunk_getUnits(oChunk) >= uUnits)
         {
            oChunk = HeapMgr_useChunk(oChunk, uUnits);
            assert(Checker_isValid(oHeapStart, oHeapEnd, aoBins, MAX_BINS));
            return Chunk_toPayload(oChunk);
         }
      } 
   }

   /* Step 4: Ask the OS for more memory, and create a new chunk (or
      expand the existing chunk) at the start of the free list. */
   oChunk =  HeapMgr_getMoreMemory(uUnits);
   if (oChunk == NULL)
   {
      assert(Checker_isValid(oHeapStart, oHeapEnd, aoBins, MAX_BINS));
      return NULL;
   }

   /* Step 5: oChunk is big enough, so use it. */
   oChunk = HeapMgr_useChunk(oChunk, uUnits);
   assert(Checker_isValid(oHeapStart, oHeapEnd, aoBins, MAX_BINS));
   return Chunk_toPayload(oChunk);
}

/*--------------------------------------------------------------------*/

void HeapMgr_free(void *pv)
{
   Chunk_T oChunk;
   Chunk_T oNextChunk;
   Chunk_T oPrevChunk;
   size_t uUnits; /* size of current oChunk */
   size_t uNewUnits; /* size of coalesced bin */

   assert(Checker_isValid(oHeapStart, oHeapEnd, aoBins, MAX_BINS));

   if (pv == NULL)
      return;

   oChunk = Chunk_fromPayload(pv);
   uUnits = Chunk_getUnits(oChunk); 

   assert(Chunk_isValid(oChunk, oHeapStart, oHeapEnd));

   /* Step 1: Set the status of the given chunk to FREE */
   Chunk_setStatus(oChunk, CHUNK_FREE);
   
   /* Step 2: Insert oChunk into the free list at the front. */
   oChunk = HeapMgr_insert(oChunk, uUnits);

   /* Step 3: If appropriate, coalesce the given chunk and the next
      one. */
   oNextChunk = Chunk_getNextInMem(oChunk, oHeapEnd);
   if (oNextChunk != NULL)
   {
      if (Chunk_getStatus(oNextChunk) == CHUNK_FREE)
      {
         /* Units of enlarged chunk */
         uNewUnits = Chunk_getUnits(oChunk) + Chunk_getUnits(oNextChunk);
         /* Remove chunks from the list */
         oChunk = HeapMgr_removeFromList(oChunk, uUnits);
         oNextChunk = HeapMgr_removeFromList(oNextChunk,
                                             Chunk_getUnits(oNextChunk));
         /* Coalesce them */
         Chunk_setUnits(oChunk, uNewUnits);
         /* Insert chunk into front of free list of proper bin */
         oChunk = HeapMgr_insert(oChunk, uNewUnits);

      }
   }

   /* Step 4: If appropriate, coalesce the given chunk and the previous
      one. */
   oPrevChunk = Chunk_getPrevInMem(oChunk, oHeapStart);
   if (oPrevChunk != NULL)
   {
      if (Chunk_getStatus(oPrevChunk) == CHUNK_FREE)
      {
         /* Units of enlarged chunk */
         uNewUnits = Chunk_getUnits(oChunk) + Chunk_getUnits(oPrevChunk);
         /* Remove chunks from the list */
         oChunk = HeapMgr_removeFromList(oChunk, Chunk_getUnits(oChunk));
         oPrevChunk = HeapMgr_removeFromList(oPrevChunk,
                                             Chunk_getUnits(oPrevChunk));
         /* Coalesce them */
         Chunk_setUnits(oPrevChunk, uNewUnits);
         oChunk = oPrevChunk;
         /* Insert chunk into front of free list of proper bin */
         oChunk = HeapMgr_insert(oChunk, uNewUnits);
      }
   }

   assert(Checker_isValid(oHeapStart, oHeapEnd, aoBins, MAX_BINS));
}
