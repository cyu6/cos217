#include <stdio.h>
#include <stdlib.h>

int main(void)
{
   int i;
   int random;
   for (i = 0; i < 50000; i++)
   {
      random = rand() & 0x7F;
      if (random == 0x09 || random == 0x0A ||
          random >= 0x20 || random <= 0x7E)
      {
         printf("%c", random);
      }
   }
   return 0;
}
