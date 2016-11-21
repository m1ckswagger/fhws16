#include <stdio.h>
#include <stdlib.h>

// compile with `gcc -Wall -std=c99 list.c`

typedef struct list
{
   struct list *next;
   int a;
} list_t;


int add_int(int a, list_t **dl)
{
   list_t *ne;

   if ((ne = malloc(sizeof(*ne))) == NULL)
      return -1;

   ne->a = a;
   ne->next = *dl;
   *dl = ne;
   return 1;
}

void print_int(const list_t *dl)
{
   for (int i = 0; dl != NULL; dl = dl->next, i++)
      printf("%d: %d\n", i, dl->a);
}

void free_list(list_t *dl)
{
   list_t * tmp;

   for (; dl != NULL; dl = tmp)
   {
      tmp = dl->next;
      free(dl);
   }
}


int main()
{
   list_t *root = NULL; // initialize pointer to first element with NULL!

   add_int(17, &root);
   add_int(7, &root);
   add_int(5, &root);
   add_int(-800, &root);
   add_int(22, &root);
   
   print_int(root);

   free_list(root);

   return 0;
}

