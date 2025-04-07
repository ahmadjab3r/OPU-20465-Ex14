#include "hashtable.h"

struct table *create_table(void) {
    int i;
    table *current_table = malloc(sizeof(struct table));
    if (current_table == NULL) {
        //TODO HANDLE ERROR
        exit(EXIT_FAILURE);
    }
    current_table->size = TABLE_SIZE;
    current_table->count = 0;
    current_table->macros = malloc(
        current_table->size * sizeof(struct table_item));
    if (current_table->macros == NULL) {
        //TODO HANDLE ERROR
        free(current_table);
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < current_table->size; i++) {
        current_table->macros[i] = NULL;
    }
    return current_table;
}

int hash_function(char *key, int table_size) {
    int current_hash = 3;
    while (*key != '\0') {
        current_hash = (current_hash * PRIME_NUMBER) + *key;
        key++;
    }
    return (current_hash % table_size);
}

struct table_item *insert_macro(table *current_table, char *key,
                                char *macro_content) {
    int index;
    struct table_item *new_macro, *current_macro;
    index = hash_function(key, current_table->size);
    if (current_table->count >= current_table->size *
        TABLE_UPPER_LIMIT) {
        if (!increase_table_size(current_table)) {
            //TODO HANDLE ERROR
            return NULL;
        }
    }
    if (search_macro(current_table, key)) {
        //TODO HANDLE ERROR macro already exists!?
        printf("Macro already exists!\n");
        return NULL;
    }
    new_macro = malloc(sizeof(struct table_item));
    if (new_macro == NULL) {
        //TODO HANDLE ERROR
        return NULL;
    }
    new_macro->key = strdup(key);
    new_macro->value = strdup(macro_content);
    new_macro->next = NULL;
    new_macro->inst_rule = NULL;
    if (current_table->macros[index] == NULL) {
        current_table->macros[index] = new_macro;
    } else {
        current_macro = current_table->macros[index];
        while (current_macro->next != NULL) {
            current_macro = current_macro->next;
        }
        current_macro->next = new_macro;
    }
    current_table->count++;
    return new_macro;
}

struct table_item *insert_macro_with_instruction(table *table,
    char *function_name,
    char *function_content,
    int funct, char *source_addressing, char *dest_addressing) {
    struct table_item *item = insert_macro(
        table, function_name, function_content);
    if (item == NULL) {
        return NULL; // Failed to insert the macro
    }
    item->inst_rule = malloc(sizeof(struct instruction_rules));
    item->inst_rule->op_code = atoi(function_content); //TODO
    item->inst_rule->funct = funct;
    item->inst_rule->source_addressing = strdup(source_addressing);
    if (item->inst_rule->source_addressing == NULL) {
        //TODO HANDLE ERROR
        return NULL;
    }
    item->inst_rule->dest_addressing = strdup(dest_addressing);
    if (item->inst_rule->dest_addressing == NULL) {
        //TODO HANDLE ERROR
        return NULL;
    }

    return item;
}

int increase_table_size(struct table *current_table) {
    int new_size, i, new_index;
    struct table_item *temp = NULL, *pointer = NULL, *current_macro =
            NULL;
    struct table_item **new_macros;
    printf("function called");
    fflush(stdout);
    new_size = current_table->size * TABLE_MULTIPLIER;
    new_macros = malloc(new_size * sizeof(struct table_item));
    if (new_macros == NULL) {
        //TODO HANDLE ERROR
        return 0;
    }
    for (i = 0; i < current_table->size; i++) {
        current_macro = current_table->macros[i];
        while (current_macro != NULL) {
            new_index = hash_function(current_macro->key, new_size);
            if (new_macros[new_index] == NULL) {
                new_macros[new_index] = malloc(
                    sizeof(struct table_item));
                if (new_macros[new_index] == NULL) {
                    //TODO HANDLE ERROR
                    free(new_macros);
                    return 0;
                }
                new_macros[new_index] = current_macro;
                temp = current_macro->next;
                new_macros[new_index]->next = NULL;
                current_macro = temp;
            } else {
                pointer = new_macros[new_index]->next;
                while (pointer->next != NULL) {
                    pointer = new_macros[new_index]->next;
                }
                pointer->next = current_macro;
                temp = current_macro->next;
                current_macro->next = NULL;
                current_macro = temp;
            }
        }
    }
    free(current_table->macros);
    current_table->macros = new_macros;
    current_table->size = new_size;
    return 1;
}

struct table_item *search_macro(struct table *current_table,
                                char *key) {
    int index;
    struct table_item *current_macro;
    index = hash_function(key, current_table->size);
    current_macro = current_table->macros[index];
    while (current_macro != NULL) {
        if (strcmp(current_macro->key, key) == 0) {
            return current_macro;
        }
        current_macro = current_macro->next;
    }
    return NULL;
}

void free_table(struct table **current_table) {
    int i;
    struct table_item *temp_macro = NULL, *current_macro = NULL;
    struct table *temp_table = *current_table;
    for (i = 0; i < temp_table->size; i++) {
        current_macro = temp_table->macros[i];
        while (current_macro != NULL) {
            temp_macro = current_macro;
            current_macro = current_macro->next;
            free(temp_macro->key);
            free(temp_macro->value);
            if (temp_macro->inst_rule != NULL) {
                free(temp_macro->inst_rule->source_addressing);
                free(temp_macro->inst_rule->dest_addressing);
                free(temp_macro->inst_rule);
            }
            free(temp_macro);
        }
    }
    free(temp_table->macros);
    free(temp_table);
    *current_table = NULL;
}
