#include <stdio.h>
#include <unistd.h>


int main()
{
   char c;
   int i;
   int num;

   num = 0;
   for (i = 0;;i++)
   {
      read(0, &c, sizeof(c));

      if (c == '\n')
         break;

      c -= '0';
      //c -= 0x30;

      num *= 10;
      num += c;

   }

   printf("Number has %d digits\nThe number is %d", i, num);
   return 0;
}


