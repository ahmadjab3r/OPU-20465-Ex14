#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 4
#define TABLE_MULTIPLIER 2
#define TABLE_UPPER_LIMIT 0.5

#define PRIME_NUMBER 7

struct Macro {
    char *macro_name;
    char *macro_content;
    struct Macro *next;
    int size;

};
struct MacroTable{
 struct Macro **macros;
    int size;
    int count;
  };

/* * Creates a new hash table for storing macros.
 * @return A pointer to the newly created MacroTable.
 */
struct MacroTable *create_table();

/*
 * Hash function to compute the index for a given macro name. by using a prime
 * number in the calculation in order to avoid collisions!
 * @macro_name The name of the macro to hash.
* @return The computed hash index.
 */
int hash_function(char *macro_name, int table_size);
int insert_macro(struct MacroTable *table, char *macro_name, char *macro_content);
int increase_table_size(struct MacroTable *table);
struct Macro *search_macro(struct MacroTable *table, char *macro_name);
void free_table(struct MacroTable **table);