/*--------------------------------------------------------------------*/
/* stra.c                                                             */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

/*--------------------------------------------------------------------*/

size_t Str_getLength(const char pcSrc[])
{
   size_t uLength = 0;
   assert(pcSrc != NULL);
   while (pcSrc[uLength] != '\0')
      uLength++;
   return uLength;
}

/*--------------------------------------------------------------------*/

char *Str_copy(char pcDest[], const char pcSrc[])
{
   size_t i;
   size_t srcLength = Str_getLength(pcSrc);
   assert(pcDest != NULL);
   assert(pcSrc != NULL);
   for (i = 0; i < srcLength; i++) {
      pcDest[i] = pcSrc[i];
   }
   pcDest[i] = '\0'; 
   return pcDest;
}

/*--------------------------------------------------------------------*/

char *Str_concat(char pcDest[], const char pcSrc[])
{
   size_t i;
   size_t srcLength = Str_getLength(pcSrc);
   size_t destLength = Str_getLength(pcDest);
   assert(pcDest != NULL);
   assert(pcSrc != NULL);
   /* copy elements of pcSrc including the last char '\0' */
   for (i = 0; i <= srcLength; i++) {
      pcDest[destLength + i] = pcSrc[i];
   }
   return pcDest;
}

/*--------------------------------------------------------------------*/

int Str_compare(const char pcStr1[], const char pcStr2[])
{
   size_t i;
   assert(pcStr1 != NULL);
   assert(pcStr2 != NULL);
   /* iterate through one string (Str1) */
   for (i = 0; i < Str_getLength(pcStr1); i++) {
      /* less than 0 if Str1 before Str2 lexicographically */
      if (pcStr1[i] < pcStr2[i])
         return -1;
      /* greater than 0 if Str2 before Str1 lexicographically */
      else if (pcStr2[i] < pcStr1[i])
         return 1;
      /* same char */
      else
         continue;
   }
   /* less than 0 if Str1 shorter than Str2, did not return earlier */
   if (Str_getLength(pcStr1) < Str_getLength(pcStr2))
       return -1;
   /* same string */
   return 0;
}

/*--------------------------------------------------------------------*/

char *Str_search(const char pcHaystack[], const char pcNeedle[])
{
   /* hay is index of haystack, need is index of needle */
   size_t hay = 0;
   size_t need = 0;
   assert(pcHaystack != NULL);
   assert(pcNeedle != NULL);

   /* needle is empty */
   if (Str_getLength(pcNeedle) == 0)
      return (char *)pcHaystack;

   /* haystack shorter than needle */
   if (Str_getLength(pcHaystack) < Str_getLength(pcNeedle))
      return NULL;
   
   while (pcHaystack[hay] != '\0') {
      while (pcNeedle[need] != '\0') {
         /* increment as char in haystack = char in needle */
         if (pcHaystack[hay + need] == pcNeedle[need])
            need++; 
         else
            break;
      }
      /* if pointer reached end of needle, found substring */
      if (pcNeedle[need] == '\0') {
         return (char *)(pcHaystack + hay); 
      }
      /* reset need to start of needle, move to next char in haystack */
      need = 0;
      hay++;
   }
   /* substring not found */
   return NULL;
}
