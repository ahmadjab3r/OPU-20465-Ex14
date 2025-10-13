#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ALLOCATION_FAIL "ERROR: MEMORY ALLOCATION FAILED\n"
#define true 1
#define false 0
#define TABLE_SIZE 50
#define TABLE_MULTIPLIER 2
#define TABLE_UPPER_LIMIT 0.7
#define PRIME_NUMBER 5381

typedef struct instruction_rules {
  int op_code;
  int funct;
  char *source_addressing;
  char *dest_addressing;
} instruction_rules;

typedef struct symbol_type {
  int *extern_locations;
  /*only relevant for .extern probably gonna change it*/
  int extern_locations_size;
  /*only relevant for .extern probably gonna change it*/
  int location;
  int is_data;
  int is_entry;
  int is_extern;
  int is_code;
} symbol_type;

/*
  * A structure representing a single item in the hash table.
  * Each item contains a key-value pair, a pointer to the next item,
  * and additional information for instructions and symbols.
  * I would have done it with void* if we learned that, but I didnt incase
  * of using something we didn't learn in the current course.
  */
typedef struct table_item {
  char *key;
  char *value;
  struct table_item *next;
  instruction_rules *inst_rule; /*only relevant for instructions!*/
  symbol_type *symbol; /*only relevant for symbols*/
  int size;
} table_item;

/*
 * A hashtable structure that contains an array of table_item pointers,
 * the size of the table, and the count of items in the table.
 * The table is used to store mcros, symbols and instructions.
 */
typedef struct hash_table {
  table_item **bucket;
  int size;
  int count;
} hash_table;

/* * Creates a new hash table for storing macros.
 * @return A pointer to the newly created MacroTable.
 */
hash_table *create_table(void);

/*
 * Hash function to compute the index for a given macro name. by using a prime
 * number in the calculation in order to avoid collisions!
 * @macro_name The name of the macro to hash.
* @return The computed hash index.
 */
int hash_function(char *macro_name, int table_size);


table_item *insert_table_item(hash_table *table, char *item_name,
                              char *item_content);

int increase_table_size(hash_table *table);

table_item *
search_table(hash_table *table, char *key);

void free_table(hash_table **table);

table_item *insert_item_with_instructions(hash_table *table,
                                          char *function_name,
                                          char *function_content,
                                          int funct,
                                          char *source_addressing,
                                          char *dest_addressing);

table_item *insert_item_with_symbol(hash_table *table,
                                     char *symbol_name,
                                     char *symbol_location,
                                     int location,
                                     int is_data,
                                     int is_entry,
                                     int is_extern,
                                     int is_code
);
int insert_external(hash_table *table, char *symbol_name,
                     int location);
