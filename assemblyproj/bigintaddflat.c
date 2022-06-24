#include

enum{FALSE, TRUE};

/* - */

/* comment */

static long BigInt_larger(long lLength1, long lLength2)
{
   long lLarger;
   if (lLength <= lLength2) goto else1;
   lLarger = lLength1;
   goto endif1;
  else1:
   lLarger = lLength2;
  endif1:
   return lLarger;
}

/* - */

/* comment */

int BigInt_add(BigInt_T oAddend1, BigInt_T oAddend2, BigInt_T oSum)
{
   unsigned long ulCarry;
   unsigned long ulSum;
   long lIndex;
   long lSumLength;

   /* determine larger length */
   lSumLength = BigInt_larger(oAddend1->lLength, oAddend2->lLength);

   /* clear oSum's array if necessary */
   if (oSum->lLength <= lSumLength) goto endifclear;
   memset(oSum->aulDigits, 0, MAX_DIGITS * sizeof(unsigned long));
  endifclear:
   /* perform addition */
   ulCarry = 0;
   lIndex = 0;
  addLoop:
   if (lIndex > lSumLength) goto addLoopEnd;
   ulSum = ulCarry;
   ulCarry = 0;
   
   ulSum += oAddend1->aulDigits[lIndex];
   if (ulSum >= oAddend1->aulDigits[lIndex]) goto endifof1; /* Check for overflow */
   ulCarry = 1;
  endifof1:
   ulSum += oAddend2->aulDigits[lIndex];
   if (ulSum >= oAddend2->aulDigits[lIndex]) goto endifof2; /* Check for overflow */
   ulCarry = 1;
  endifof2:
   oSum->aulDigits[lIndex] = ulSum;
  
   lIndex++;
   goto addLoop;

  addLoopEnd:
   /* Check for a carry out of the last "column" of the addition. */
   if (ulCarry != 1) goto endifcarry;
   if (lSumLength != MAX_DIGITS) goto endifdigits;
   return FALSE;
  endifdigits:
   oSum->aulDigits[lSumLength] = 1;
   lSumLength++;
  endifcarry:
   /* Set the length of the sum. */
   oSum->lLength = lSumLength;

   return TRUE;
   
}
