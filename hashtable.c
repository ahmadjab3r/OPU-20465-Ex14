#include "hashtable.h"

struct table *create_table(void)
{
  int i;
  table *current_table = malloc(sizeof(struct table));
  if (current_table == NULL)
    {
      //TODO HANDLE ERROR
      exit(EXIT_FAILURE);
    }
  current_table->size = TABLE_SIZE;
  current_table->count = 0;
  current_table->bucket = malloc(
    current_table->size * sizeof(struct table_item));
  if (current_table->bucket == NULL)
    {
      //TODO HANDLE ERROR
      free(current_table);
      exit(EXIT_FAILURE);
    }
  for (i = 0; i < current_table->size; i++)
    {
      current_table->bucket[i] = NULL;
    }
  return current_table;
}

int hash_function(char *key, int table_size)
{
  int current_hash = 3;
  while (*key != '\0')
    {
      current_hash = (current_hash * PRIME_NUMBER) + *key;
      key++;
    }
  return (current_hash % table_size);
}

struct table_item *insert_table_item(table *current_table, char *key,
                                     char *value)
{
  int index;
  struct table_item *new_item, *current_macro;
  index = hash_function(key, current_table->size);
  if (current_table->count >= current_table->size *
      TABLE_UPPER_LIMIT)
    {
      if (!increase_table_size(current_table))
        {
          //TODO HANDLE ERROR
          return NULL;
        }
    }
  if (search_table(current_table, key))
    {
      //TODO HANDLE ERROR macro already exists!?
      printf("Macro already exists!\n");
      return NULL;
    }
  new_item = malloc(sizeof(struct table_item));
  if (new_item == NULL)
    {
      //TODO HANDLE ERROR
      return NULL;
    }
  new_item->key = strdup(key);
  new_item->value = strdup(value);
  new_item->next = NULL;
  new_item->inst_rule = NULL;
  new_item->symbol = NULL;
  if (current_table->bucket[index] == NULL)
    {
      current_table->bucket[index] = new_item;
    }
  else
    {
      current_macro = current_table->bucket[index];
      while (current_macro->next != NULL)
        {
          current_macro = current_macro->next;
        }
      current_macro->next = new_item;
    }
  current_table->count++;
  return new_item;
}

struct table_item *insert_item_with_instructions(table *table,
                                                 char *function_name,
                                                 char *function_content,
                                                 int funct,
                                                 char *source_addressing,
                                                 char *dest_addressing)
{
  struct table_item *item = insert_table_item(
    table, function_name, function_content);
  if (item == NULL)
    {
      return NULL; // Failed to insert the macro
    }
  item->inst_rule = malloc(sizeof(struct instruction_rules));
  item->inst_rule->op_code = atoi(function_content); //TODO
  item->inst_rule->funct = funct;
  item->inst_rule->source_addressing = strdup(source_addressing);
  if (item->inst_rule->source_addressing == NULL)
    {
      //TODO HANDLE ERROR
      return NULL;
    }
  item->inst_rule->dest_addressing = strdup(dest_addressing);
  if (item->inst_rule->dest_addressing == NULL)
    {
      //TODO HANDLE ERROR
      return NULL;
    }

  return item;
}

int increase_table_size(struct table *current_table)
{
  int new_size, i, new_index;
  struct table_item *temp = NULL, *pointer = NULL, *current_macro =
      NULL;
  struct table_item **bucket;
  printf("function called");
  fflush(stdout);
  new_size = current_table->size * TABLE_MULTIPLIER;
  bucket = malloc(new_size * sizeof(struct table_item));
  if (bucket == NULL)
    {
      //TODO HANDLE ERROR
      return 0;
    }
  for (i = 0; i < current_table->size; i++)
    {
      current_macro = current_table->bucket[i];
      while (current_macro != NULL)
        {
          new_index = hash_function(current_macro->key, new_size);
          if (bucket[new_index] == NULL)
            {
              bucket[new_index] = malloc(
                sizeof(struct table_item));
              if (bucket[new_index] == NULL)
                {
                  //TODO HANDLE ERROR
                  free(bucket);
                  return 0;
                }
              bucket[new_index] = current_macro;
              temp = current_macro->next;
              bucket[new_index]->next = NULL;
              current_macro = temp;
            }
          else
            {
              pointer = bucket[new_index]->next;
              while (pointer->next != NULL)
                {
                  pointer = bucket[new_index]->next;
                }
              pointer->next = current_macro;
              temp = current_macro->next;
              current_macro->next = NULL;
              current_macro = temp;
            }
        }
    }
  free(current_table->bucket);
  current_table->bucket = bucket;
  current_table->size = new_size;
  return 1;
}

struct table_item *search_table(struct table *current_table,
                                char *key)
{
  int index;
  struct table_item *current_macro;
  if (!key)return NULL;
  index = hash_function(key, current_table->size);
  current_macro = current_table->bucket[index];
  while (current_macro != NULL)
    {
      if (strcmp(current_macro->key, key) == 0)
        {
          return current_macro;
        }
      current_macro = current_macro->next;
    }
  return NULL;
}

void free_table(struct table **current_table)
{
  int i;
  struct table_item *temp_macro = NULL, *current_macro = NULL;
  struct table *temp_table = *current_table;
  for (i = 0; i < temp_table->size; i++)
    {
      current_macro = temp_table->bucket[i];
      while (current_macro != NULL)
        {
          temp_macro = current_macro;
          current_macro = current_macro->next;
          free(temp_macro->key);
          free(temp_macro->value);
          if (temp_macro->inst_rule != NULL)
            {
              free(temp_macro->inst_rule->source_addressing);
              free(temp_macro->inst_rule->dest_addressing);
              free(temp_macro->inst_rule);
            }
          if (temp_macro->symbol != NULL)
            {
              if (temp_macro->symbol->extern_locations != NULL)
                {
                  free(temp_macro->symbol->extern_locations);
                }
              free(temp_macro->symbol);
            }
          free(temp_macro);
        }
    }
  free(temp_table->bucket);
  free(temp_table);
  *current_table = NULL;
}

struct table_item *insert_item_with_symbol(table *table,
                                           char *symbol_name,
                                           char *symbol_location,
                                           int location,
                                           bool is_data,
                                           bool is_entry,
                                           bool is_extern,
                                           bool is_code)
{
  struct table_item *item = insert_table_item(
    table, symbol_name, symbol_location);
  if (item == NULL)
    {
      return NULL; // Failed to insert the macro
    }
  item->symbol = malloc(sizeof(struct symbol_type));
  if (item->symbol == NULL)
    {
      //TODO HANDLE ERROR
      return NULL;
    }
  item->symbol->location = location;
  item->symbol->is_data = is_data;
  item->symbol->is_entry = is_entry;
  item->symbol->is_extern = is_extern;
  item->symbol->is_code = is_code;
  return item;
}
