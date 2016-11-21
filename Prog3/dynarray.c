#include <stdio.h>
#include <stdlib.h>

// compile with `gcc -Wall -std=c99 dynarray.c`

typedef struct dyn_array
{
   int *ar;
   int size;   // number of elements in teh arry
} dyn_array_t;

/*! Add integer to my array da. */
int add_int(int a, dyn_array_t *da)
{
   int *tmp;

   if ((tmp = realloc(da->ar, sizeof(int) * (da->size + 1))) == NULL)
      return -1;

   da->ar = tmp;

   da->ar[da->size] = a;
   da->size++;

   return da->size;
}

void print_array(const dyn_array_t *da)
{
   for (int i = 0; i < da->size; i++)
      printf("da[%d] = %d\n", i, da->ar[i]);
}

int main()
{
   dyn_array_t da;

   da.size = 0;
   da.ar = NULL; // initialize to NULL of realloc(3) is called directly!

   add_int(13, &da);
   add_int(27, &da);
   add_int(-800, &da);

   print_array(&da);

   free(da.ar);
}


