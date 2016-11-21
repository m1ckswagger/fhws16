/*! Example program of lab 5th of October 2016.
 * This is the combined program of both lab groups.
 * compile with `gcc -Wall -std=c99 dnsrr.c`
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXBYTES 256

// number of elements for the hash table
// (which depends on the size of the type of the hash value)
#define HT_SIZE (1 << sizeof(hash_t) * 8)

// typdef own type for hash value (can be changed easier in future)
typedef uint8_t hash_t;

// excercise 1.b
typedef struct dns_rr dns_rr_t;
struct dns_rr
{
   char name[MAXBYTES];
   int rrtype;
   int ttl;
   char rrdata[MAXBYTES];
   // excercise 2.a
   dns_rr_t * next;
};


// excercise 1.a
hash_t hash(const char *str)
{
   hash_t h;
   int i;

   // safety check
   if (str == NULL)
      return 0;

   for (h = 0, i = 1; *str; h ^= i++ * *str++);

   /* alternatively, e.g.
   for (i = 0, h = 0; *str; *str++, i++)
      h = (i + 1) * *str;
   */
   /* alternatively, e.g.
   for (i = 0, h = 0; i < strlen(str); i++)
      h ^= str[i];
   */
   /* alternatively, e.g. 
   int c;
   h = 33;
   while((c=*str++)) {
      h = ((h << 5) + h) + (hash_t)c;
   }
   */
   /* alternatively, e.g. 
   for(i = 0, h = 0; i < strlen(str); i++) {
      h ^= tolower(str[i] & i);
   }
   */

   return h;
}

// excercise 1.c.2
void init_htable(dns_rr_t **arg)
{
   // safety check
   if (arg == NULL)
      return;
   memset(arg, 0, HT_SIZE * sizeof(*arg));
   // alternatively, e.g.
   //memset(arg, 0, 256 * sizeof(struct dns_rr*));
   /* alternatively, e.g.
   int i;
   for (i=0; i < 256; i++)
   {
      arg[i] = NULL;
   }*/
}


// excercise 1.d
dns_rr_t* new_dns_rr(const char *name, int rrtype, int ttl)
{
   //excercise 1.d.1
   dns_rr_t* addr;

   // safety check
   if (name == NULL)
      return NULL;

   //if ( (addr = malloc(sizeof(dns_rr_t))) == NULL )
   if ((addr = malloc(sizeof(*addr))) == NULL)
   {
      printf("Error: dsh\n");
      return NULL;
   }
   // excercise 1.d.2
   strncpy(addr->name, name, MAXBYTES);
   addr->name[MAXBYTES-1] = '\0';
   addr->rrtype = rrtype;
   addr->ttl = ttl;
   memset(addr->rrdata, '\0', MAXBYTES);
   
   // excersie 2.b.1
   addr->next = NULL;
   return addr;
}


// excercise 1.e
dns_rr_t *add_rr(dns_rr_t **table, dns_rr_t *elem)
//dns_rr_t *add_rr(dns_rr_t *table[], dns_rr_t *elem)
{
   hash_t h;
   dns_rr_t *tmp;

   // safety check
   if (elem == NULL || table == NULL)
      return NULL;

   // excercise 1.e.1
   h = hash(elem->name);

   // excercise 1.e.2
   tmp = table[h];
   table[h] = elem;

   // excercise 2.c.2
   elem->next = tmp;

   return tmp;
}


// excercise 1.f
dns_rr_t * get_rr(dns_rr_t **table, const char *name)
{
   if (table == NULL)
      return NULL;
   return table[hash(name)];
}


// excercise 1.g
void print_rr(const dns_rr_t *rr)
{
   // safety check
   if (rr == NULL)
   {
      printf("rr does not exist\n");
      return;
   }
   printf("name: %s (hash = 0x%02x), ttl: %d, type: %d\n",
         rr->name, hash(rr->name), rr->ttl, rr->rrtype);
   /*printf("Domain name: %s\n", rr->name);
   printf("TTL: %d\n", rr->ttl);
   printf("Type: %d\n", rr->rrtype);*/
}


void print_htable(dns_rr_t **table)
{
   dns_rr_t *tmp;

   // safety check
   if (table == NULL)
      return;

   for(int i = 0; i < HT_SIZE; i++)
      for(tmp = table[i]; tmp != NULL; tmp = tmp->next)
         print_rr(tmp);
}


int main()
{
   // excercise 1.c.1
   dns_rr_t* hashtable[HT_SIZE];
   // struct dns_rr* hashtable[256];

   dns_rr_t *ne;

   // excercise 1.c.2
   init_htable(hashtable);

   ne = new_dns_rr("www.google.com", 1, 500);
   add_rr(hashtable, ne);
   ne = new_dns_rr("www.google.com", 2, 500);
   add_rr(hashtable, ne);
   ne = new_dns_rr("www.fhstp.ac.at", 1, 600);
   add_rr(hashtable, ne);
   ne = new_dns_rr("www.videos.xxx", 1, 1000);
   add_rr(hashtable, ne);
   ne = new_dns_rr("www.univie.ac.at", 1, 1000);
   add_rr(hashtable, ne);
   ne = new_dns_rr("www.univie.ac.at", 1, 2200);
   add_rr(hashtable, ne);
   ne = new_dns_rr("www.univie.ac.at", 1, 3300);
   add_rr(hashtable, ne);
   ne = new_dns_rr("www.univie.at.ac", 1, 4400);
   add_rr(hashtable, ne);


   print_htable(hashtable);

   return 0;
}

