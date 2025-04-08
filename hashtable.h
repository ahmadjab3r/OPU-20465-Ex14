#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 50
#define TABLE_MULTIPLIER 2
#define TABLE_UPPER_LIMIT 0.7

#define PRIME_NUMBER 7

typedef struct instruction_rules {
  int op_code;
  int funct;
  char *source_addressing;
  char *dest_addressing;
} instruction_rules;

typedef struct symbol_type {
  int *extern_locations; //only relevant for .extern probably gonna change it //TODO
  int location;
  bool is_data;
  bool is_entry;
  bool is_extern;
  bool is_code;
} symbol_type;
typedef struct table_item {
  char *key;
  char *value;
  struct table_item *next;
  instruction_rules *inst_rule; //only relevant for instructions!
  symbol_type *symbol; //only relevant for symbols
  int size;
} Macro;

typedef struct table {
  struct table_item **bucket;
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

struct table_item *insert_table_item(table *table, char *item_name,
                                char *item_content);

int increase_table_size(table *table);

struct table_item *
search_table(table *table, char *key);

void free_table(table **table);

struct table_item *insert_item_with_instructions(table *table,
                                                 char *function_name,
                                                 char *function_content,
                                                 int funct,
                                                 char *source_addressing,
                                                 char *dest_addressing);
struct table_item *insert_item_with_symbol(table *table,
                                                char *symbol_name,
                                                char *symbol_location,
                                                int location,
                                                bool is_data,
                                                bool is_entry,
                                                bool is_extern,
                                                bool is_code
                                                );
