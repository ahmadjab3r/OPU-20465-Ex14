#include "hashtable.h"

struct MacroTable *create_table(void){

    struct MacroTable *table = malloc(sizeof(struct MacroTable));
    if (table == NULL) {
      //TODO HANDLE ERROR
        exit(EXIT_FAILURE);
    }
    table->size = TABLE_SIZE;
    table->count = 0;
    table->macros = malloc(table->size * sizeof(struct Macro));
    if (table->macros == NULL) {
      //TODO HANDLE ERROR
        free(table);
        exit(EXIT_FAILURE);
    }
  for ( int i = 0; i < table->size; i++) {
        table->macros[i] = NULL;
    }
    return table;
  }
int hash_function(char *macro_name, int table_size){
  int i;
  int current_hash= 3;
  while (*macro_name != '\0'){
    current_hash = (current_hash * PRIME_NUMBER) + *macro_name;
    macro_name++;
  }
  return (current_hash % table_size);
  }
int insert_macro(struct MacroTable *table, char *macro_name, char *macro_content){

    int index = hash_function(macro_name, table->size);
    if (table->count >= table->size * TABLE_UPPER_LIMIT) {
        if (!increase_table_size(table)) {
            //TODO HANDLE ERROR
            return 0;
        }
    }
    if (search_macro(table, macro_name)) {
        //TODO HANDLE ERROR macro already exists!?
        printf("Macro already exists!\n");
        return 0;
    }
    struct Macro *new_macro = malloc(sizeof(struct Macro));
    if (new_macro == NULL) {
      //TODO HANDLE ERROR
        return 0;
    }
    new_macro->macro_name = strdup(macro_name);
    new_macro->macro_content = strdup(macro_content);
    new_macro->next = NULL;
    if (table->macros[index] == NULL) {
        table->macros[index] = new_macro;
    } else
      {
        struct Macro *current_macro = table->macros[index];
        while (current_macro->next != NULL) {
            current_macro = current_macro->next;
        }
        current_macro->next = new_macro;
      }
    table->count++;
    return 1;

  }
int increase_table_size(struct MacroTable *table) {
    struct Macro *temp;
  struct Macro *pointer = NULL;
    int new_size = table->size * TABLE_MULTIPLIER;
    struct Macro **new_macros = malloc(new_size * sizeof(struct Macro));
    if (new_macros == NULL) {
      //TODO HANDLE ERROR
        return 0;
    }
    for (int i = 0; i < table->size; i++) {
        struct Macro *current_macro = table->macros[i]->next;
        while (current_macro != NULL) {
            int new_index = hash_function(current_macro->macro_name,new_size);
            if (new_macros[new_index] == NULL) {
                new_macros[new_index] = malloc(sizeof(struct Macro));
                if (new_macros[new_index] == NULL) {
                  //TODO HANDLE ERROR
                    free(new_macros);
                    return 0;
                }
                new_macros[new_index]= current_macro;
                temp= current_macro->next;
                new_macros[new_index]->next = NULL;
                current_macro = temp;

            } else
              {
                pointer = new_macros[new_index]->next;
                  while ( pointer->next != NULL) {
                      pointer = new_macros[new_index]->next;
                  }
                pointer->next = current_macro;
                temp = current_macro->next;
                current_macro->next = NULL;
                current_macro = temp;
              }
        }
    }
    free(table->macros);
    table->macros = new_macros;
    table->size = new_size;
    return 1;

    }

struct Macro *search_macro(struct MacroTable *table, char *macro_name){
    int index = hash_function(macro_name, table->size);
    struct Macro *current_macro = table->macros[index];
    while (current_macro != NULL) {
        if (strcmp(current_macro->macro_name, macro_name) == 0) {
            return current_macro;
        }
        current_macro = current_macro->next;
    }
    return NULL;
  }
void free_table(struct MacroTable **table)
{
    struct MacroTable *temp_table = *table;
    for (int i = 0; i < temp_table->size; i++) {
        struct Macro *current_macro = temp_table->macros[i];
        while (current_macro != NULL) {
            struct Macro *temp_macro = current_macro;
            current_macro = current_macro->next;
            free(temp_macro->macro_name);
            free(temp_macro->macro_content);
            free(temp_macro);
        }

    }
  free(temp_table->macros);
    free(temp_table);
   *table = NULL;
}
