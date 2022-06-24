/*--------------------------------------------------------------------*/
/* strp.c                                                             */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

/*--------------------------------------------------------------------*/

size_t Str_getLength(const char *pcSrc)
{
   const char *pcEnd;
   assert(pcSrc != NULL);
   pcEnd = pcSrc;
   while (*pcEnd != '\0')
      pcEnd++;
   return (size_t)(pcEnd - pcSrc);
}

/*--------------------------------------------------------------------*/

char *Str_copy(char *pcDest, const char *pcSrc)
{
   char *pointer;
   assert(pcDest != NULL);
   assert(pcSrc != NULL);
   pointer = pcDest;
   while (*pcSrc) {
      *pointer = *pcSrc;
      pointer++;
      pcSrc++;
   }
   *pointer = '\0';
   return pcDest;
}

/*--------------------------------------------------------------------*/

char *Str_concat(char *pcDest, const char *pcSrc)
{
   char *pointer;
   assert(pcDest != NULL);
   assert(pcSrc != NULL);
   pointer = pcDest;
   /* find end of pcDest */
   while (*pointer)
      pointer++;
   /* copy elements of pcSrc */
   while (*pcSrc) {
      *pointer = *pcSrc;
      pointer++;
      pcSrc++;
   }
   *pointer = '\0';
   return pcDest;
}

/*--------------------------------------------------------------------*/

int Str_compare(const char *pcStr1, const char *pcStr2)
{
   assert(pcStr1 != NULL);
   assert(pcStr2 != NULL);
   while (*pcStr1) {
      /* increment as long as chars are the same */
      if (*pcStr1 == *pcStr2) {
         pcStr1++;
         pcStr2++;
      }
      else {
         if (*pcStr1 < *pcStr2)
            /* less than 0 if Str1 before Str2 lexicographically */
            return -1;
         else
            /* greater than 0 if Str2 before Str1 lexicographically */
            return 1;
      }
   }
   /* less than 0 if Str1 shorter than Str2, did not return earlier */
   if (Str_getLength(pcStr1) < Str_getLength(pcStr2))
      return -1;
   /* same string */
   return 0;
}

/*--------------------------------------------------------------------*/

char *Str_search(const char *pcHaystack, const char *pcNeedle)
{
   const char *pHay = pcHaystack;
   const char *pNeed = pcNeedle;
   assert(pcHaystack != NULL);
   assert(pcNeedle != NULL);

   /* needle is empty */
   if (Str_getLength(pcNeedle) == 0)
      return (char *)pcHaystack;

   /* haystack shorter than needle */
   if (Str_getLength(pcHaystack) < Str_getLength(pcNeedle))
      return NULL;
   
   while(*pcHaystack != '\0') {
      while (*pNeed != '\0') {
         /* break if chars mismatch, otherwise increment pointers */ 
         if (*pHay != *pNeed) {
            break;
         }
         pHay++;
         pNeed++;
      }
      /* if pointer reached the end of needle, substring found */
      if (*pNeed == '\0') {
         return (char*)pHay - Str_getLength(pcNeedle);
      }
      /* reset values to start search from next char in haystack */
      pNeed = pcNeedle;
      pcHaystack++;
      pHay = pcHaystack;
   }
   /* substring not found */
   return NULL;
}
