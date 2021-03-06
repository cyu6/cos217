/*--------------------------------------------------------------------*/
/* heapmgr4.c                                                         */
/* Author: Joseph Chen and Catherine Yu                               */
/*--------------------------------------------------------------------*/

#define _GNU_SOURCE

#include "heapmgr.h"
#include "checker4.h"
#include "chunk4.h"
#include <stddef.h>
#include <assert.h>
#include <unistd.h>

/*--------------------------------------------------------------------*/

/* The state of the HeapMgr. */

/* The address of the start of the heap. */
static Chunk_T oHeapStart = NULL;

/* The address immediately beyond the end of the heap. */
static Chunk_T oHeapEnd = NULL;

/* The free list is a list of all free Chunks. It is not kept in any
   particular order. */
static Chunk_T oFreeList = NULL;

/*--------------------------------------------------------------------*/

/* Remove oChunk from the free list and return oChunk. */

static Chunk_T HeapMgr_removeFromList(Chunk_T oChunk)
{
   Chunk_T oNextChunk;
   Chunk_T oPrevChunk;

   assert(Chunk_isValid(oChunk, oHeapStart, oHeapEnd));

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

/* Insert oChunk at the front of the free list and return oChunk. */

static Chunk_T HeapMgr_insert(Chunk_T oChunk)
{
   assert(Chunk_isValid(oChunk, oHeapStart, oHeapEnd));

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
   oChunk = HeapMgr_insert(oChunk);

   /* Coalesce the new chunk and the previous one if appropriate. */
   oPrevChunk = Chunk_getPrevInMem(oChunk, oHeapStart);
   if (oPrevChunk != NULL)
   {
      if (Chunk_getStatus(oPrevChunk) == CHUNK_FREE)
      {
         /* Remove the chunks from the free list */
         oChunk = HeapMgr_removeFromList(oChunk);
         oPrevChunk = HeapMgr_removeFromList(oPrevChunk);
         /* Coalesce the chunks */
         Chunk_setUnits(oPrevChunk, Chunk_getUnits(oPrevChunk) + uUnits);
         oChunk = oPrevChunk;
         /* Insert enlarged chunk to the start of the free list */
         oChunk = HeapMgr_insert(oChunk);
      }
   }

   return oChunk;
}

/*--------------------------------------------------------------------*/

/* If oChunk is close to the right size (as specified by uUnits), then
   remove it from the free list, set its status to INUSE, and return
   it. If oChunk is too big, then remove oChunk from the free list,
   split the chunk, insert the tail end at the front of the free list,
   set the status of the front end to INUSE, set the status of the tail
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
      /* Remove from free list */
      oChunk = HeapMgr_removeFromList(oChunk);
      /* Set status to INUSE */
      Chunk_setStatus(oChunk, CHUNK_INUSE);
      return oChunk;
   }

   /* oChunk is too big, so use the front end of it. */
   oChunk = HeapMgr_removeFromList(oChunk);

   /* Split the front end */
   Chunk_setUnits(oChunk, uUnits);
   Chunk_setStatus(oChunk, CHUNK_INUSE);

   /* Set fields and insert tail end into free list at front */
   oNewChunk = Chunk_getNextInMem(oChunk, oHeapEnd);
   Chunk_setUnits(oNewChunk, uChunkUnits - uUnits);
   oNewChunk = HeapMgr_insert(oNewChunk);
   Chunk_setStatus(oNewChunk, CHUNK_FREE);

   assert(Chunk_isValid(oChunk, oHeapStart, oHeapEnd));

   return oChunk;
}

/*--------------------------------------------------------------------*/

void *HeapMgr_malloc(size_t uBytes)
{
   Chunk_T oChunk;
   size_t uUnits;

   if (uBytes == 0)
      return NULL;

   /* Step 1: Initialize the heap manager if this is the first call. */
   if (oHeapStart == NULL)
   {
      oHeapStart = (Chunk_T)sbrk(0);
      oHeapEnd = oHeapStart;
   }

   assert(Checker_isValid(oHeapStart, oHeapEnd, oFreeList));

   /* Step 2: Determine the number of units the new chunk should
      contain. */
   uUnits = Chunk_bytesToUnits(uBytes);

   /* Step 3: For each chunk in the free list... */
   for (oChunk = oFreeList;
        oChunk != NULL;
        oChunk = Chunk_getNextInList(oChunk))
   {
      /* If oChunk is big enough, then use it. */
      if (Chunk_getUnits(oChunk) >= uUnits)
      {
         oChunk = HeapMgr_useChunk(oChunk, uUnits);
         assert(Checker_isValid(oHeapStart, oHeapEnd, oFreeList));
         return Chunk_toPayload(oChunk);
      }
   }

   /* Step 4: Ask the OS for more memory, and create a new chunk (or
      expand the existing chunk) at the start of the free list. */
   oChunk =  HeapMgr_getMoreMemory(uUnits);
   if (oChunk == NULL)
   {
      assert(Checker_isValid(oHeapStart, oHeapEnd, oFreeList));
      return NULL;
   }

   /* Step 5: oChunk is big enough, so use it. */
   oChunk = HeapMgr_useChunk(oChunk, uUnits);

   assert(Checker_isValid(oHeapStart, oHeapEnd, oFreeList));

   return Chunk_toPayload(oChunk);
}

/*--------------------------------------------------------------------*/

void HeapMgr_free(void *pv)
{
   Chunk_T oChunk;
   Chunk_T oNextChunk;
   Chunk_T oPrevChunk;

   assert(Checker_isValid(oHeapStart, oHeapEnd, oFreeList));

   if (pv == NULL)
      return;

   oChunk = Chunk_fromPayload(pv);

   assert(Chunk_isValid(oChunk, oHeapStart, oHeapEnd));

   /* Step 1: Set the status of the given chunk to FREE */
   Chunk_setStatus(oChunk, CHUNK_FREE);
   
   /* Step 2: Insert oChunk into the free list at the front. */
   oChunk = HeapMgr_insert(oChunk);

   /* Step 3: If appropriate, coalesce the given chunk and the next
      one. */
   oNextChunk = Chunk_getNextInMem(oChunk, oHeapEnd);
   if (oNextChunk != NULL)
   {
      if (Chunk_getStatus(oNextChunk) == CHUNK_FREE)
      {
         /* Remove chunks from the list */
         oChunk = HeapMgr_removeFromList(oChunk);
         oNextChunk = HeapMgr_removeFromList(oNextChunk);
         /* Coalesce them */
         Chunk_setUnits(oChunk, Chunk_getUnits(oChunk) +
                        Chunk_getUnits(oNextChunk));
         /* Insert enlarged chunk into front of free list */
         oChunk = HeapMgr_insert(oChunk);
      }
   }

   /* Step 4: If appropriate, coalesce the given chunk and the previous
      one. */
   oPrevChunk = Chunk_getPrevInMem(oChunk, oHeapStart);
   if (oPrevChunk != NULL)
   {
      if (Chunk_getStatus(oPrevChunk) == CHUNK_FREE)
      {
         /* Remove chunks from the list */
         oChunk = HeapMgr_removeFromList(oChunk);
         oPrevChunk = HeapMgr_removeFromList(oPrevChunk);
         /* Coalesce them */
         Chunk_setUnits(oPrevChunk, Chunk_getUnits(oPrevChunk) +
                        Chunk_getUnits(oChunk));
         oChunk = oPrevChunk;
         /* Insert enlarged chunk into front of free list */
         oChunk = HeapMgr_insert(oChunk);
      }
   }

   assert(Checker_isValid(oHeapStart, oHeapEnd, oFreeList));
}
