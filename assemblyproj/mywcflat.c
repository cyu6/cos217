#include <stdio.h>
#include <ctype.h>

/* data section */
static long lLineCount = 0;
static long lWordCount = 0;
static long iCharCount = 0;
static int iInWord = FALSE; /* false = 0 */

/* bss section */
static int iChar;

/* text section */
/* comment */
int main(void)
{
#define EOF = -1; /* .equ */
  countLoop:
   /* check loop conditions */
   if (iChar == EOF) goto countLoopEnd;
   iChar = getchar();
   lCharCount++;
   if(!isspace(iChar)) goto else1;
      if(!iInWord) goto endif1;
      lWordCount++;
      iInWord = 0;
      goto endif1;
     else1:
      if(iInWord) goto endif1;
      iInWord = 1;
  endif1:
      if(iChar != '\n') goto endif2;
      lLineCount++;
  endif2:
      goto countLoop;
  countLoopEnd:
      if(!inWord) goto endif3;
      lWordCount++;
  endif3:
      printf("format string", lLineCount, lWordCount, lCharCount);
      return 0;
   
}
