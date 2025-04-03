#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 50
#define TABLE_MULTIPLIER 2
#define TABLE_UPPER_LIMIT 0.7

#define PRIME_NUMBER 7

typedef struct table_item {
  char *key;
  char *value;
  struct table_item *next;
  int size;
} Macro;
typedef struct table {
  struct table_item **macros;
  int size;
  int count;
} table;

/* * Creates a new hash table for storing macros.
 * @return A pointer to the newly created MacroTable.
 */
struct table *create_table(void);

/*
 * Hash function to compute the index for a given macro name. by using a prime
 * number in the calculation in order to avoid collisions!
 * @macro_name The name of the macro to hash.
* @return The computed hash index.
 */
int hash_function(char *macro_name, int table_size);
int insert_macro(struct table *table, char *macro_name,
                 char *macro_content);
int increase_table_size(struct table *table);
struct table_item *search_macro(struct table *table, char *macro_name);
void free_table(struct table **table);
