/*--------------------------------------------------------------------*/
/* checker4.c                                                         */
/* Author: Joseph Chen and Catherine Yu                               */
/*--------------------------------------------------------------------*/

#include "checker4.h"
#include <stdio.h>

/* In lieu of a boolean data type. */
enum {FALSE, TRUE};

/*--------------------------------------------------------------------*/

int Checker_isValid(Chunk_T oHeapStart, Chunk_T oHeapEnd,
   Chunk_T oFreeList)
{
   Chunk_T oChunk;
   Chunk_T oPrevChunk;
   Chunk_T oTortoiseChunk;
   Chunk_T oHareChunk;

   size_t freeCount = 0;
   size_t memCount = 0;

   /* Do oHeapStart and oHeapEnd have non-NULL values? */
   if (oHeapStart == NULL)
   {
      fprintf(stderr, "The heap start is uninitialized\n"); 
      return FALSE; 
   }
   if (oHeapEnd == NULL)
   {
      fprintf(stderr, "The heap end is uninitialized\n");
      return FALSE;
   }

   /* If the heap is empty, is the free list empty too? */
   if (oHeapStart == oHeapEnd)
   {
      if (oFreeList == NULL)
         return TRUE;
      else
      {
         fprintf(stderr, "The heap is empty, but the list is not\n");
         return FALSE;
      }
   }
   
   /* Is the list devoid of cycles? Use Floyd's algorithm to find out.
      See the Wikipedia "Cycle detection" page for a description. */

   oTortoiseChunk = oFreeList;
   oHareChunk = oFreeList;
   if (oHareChunk != NULL)
      oHareChunk = Chunk_getNextInList(oHareChunk);
   while (oHareChunk != NULL)
   {
      if (oTortoiseChunk == oHareChunk)
      {
         fprintf(stderr, "The list has a cycle\n");  
         return FALSE;
      }
      /* Move oTortoiseChunk one step. */
      oTortoiseChunk = Chunk_getNextInList(oTortoiseChunk);
      /* Move oHareChunk two steps, if possible. */
      oHareChunk = Chunk_getNextInList(oHareChunk);
      if (oHareChunk != NULL)
         oHareChunk = Chunk_getNextInList(oHareChunk);
   }

   /* Does the first node footer point to NULL? */
   if (oFreeList != NULL && Chunk_getPrevInList(oFreeList) != NULL)
   {
      fprintf(stderr, "First node footer does not point to NULL\n");
      return FALSE;
   }

   /* Traverse the free list. */

   oPrevChunk = NULL;
   for (oChunk = oFreeList;
        oChunk != NULL;
        oChunk = Chunk_getNextInList(oChunk))
   {
      /* Is the chunk valid? */
      if (! Chunk_isValid(oChunk, oHeapStart, oHeapEnd))
      {
         fprintf(stderr, "Traversing the list detected a bad chunk\n");
         return FALSE;
      }

      /* Is the status bit correct for free chunks? */
      if (Chunk_getStatus(oChunk) != CHUNK_FREE)
      {
         fprintf(stderr, "The status bit is not 0 for free chunk\n");
         return FALSE;
      }

      /* Does end of the current chuck point to the previous chunk? */
      if ((oPrevChunk != NULL) &&
          (Chunk_getPrevInList(oChunk) != oPrevChunk))
      {
         fprintf(stderr, "The chunk points to wrong previous chunk\n");
         return FALSE;
      }
      
      /* Is the previous chunk in memory in use? */
      if ((Chunk_getPrevInMem(oChunk, oHeapStart) != NULL) &&
          Chunk_getStatus(Chunk_getPrevInMem(oChunk,
                                             oHeapStart)) == CHUNK_FREE)
      {
         fprintf(stderr, "The heap contains contiguous free chunks\n");
         return FALSE;
      }

      oPrevChunk = oChunk;
      /* Update freeCount */
      freeCount++;
   }

   /* Traverse memory. */

   for (oChunk = oHeapStart;
        oChunk != NULL;
        oChunk = Chunk_getNextInMem(oChunk, oHeapEnd))
   {
      /* Is the chunk valid? */
      if (! Chunk_isValid(oChunk, oHeapStart, oHeapEnd))
      {
         fprintf(stderr, "Traversing memory detected a bad chunk\n");
         return FALSE;
      }
      
      /* Update memCount if chunk is free */
      if (Chunk_getStatus(oChunk) == CHUNK_FREE)
         memCount++;  
   }

   /* Are there free chunks that are not in the free list? */
   if (memCount != freeCount)
   {
      fprintf(stderr, "There are free chunks not in the free list\n");
      return FALSE;
   }

   return TRUE;
}
