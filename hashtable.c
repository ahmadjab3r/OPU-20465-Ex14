#include "hashtable.h"


table *create_table(void) {
    int i;
    table *current_table = malloc(sizeof(struct table));
    if (current_table == NULL) {
        printf(ALLOCATION_FAIL);
        exit(EXIT_FAILURE);
    }
    current_table->size = TABLE_SIZE;
    current_table->count = 0;
    current_table->bucket = malloc(
        current_table->size * sizeof(struct table_item));
    if (current_table->bucket == NULL) {
        printf(ALLOCATION_FAIL);
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < current_table->size; i++) {
        current_table->bucket[i] = NULL;
    }
    return current_table;
}

int hash_function(char *key, int table_size) {
    unsigned long hash = 5381;
    int c;

    while (c = *key++)
        hash = ((hash << 5) + hash) + c;

    return hash % table_size;
}

struct table_item *insert_table_item(table *current_table, char *key,
                                     char *value) {
    int index;
    struct table_item *new_item, *current_macro;
    index = hash_function(key, current_table->size);
    if (current_table->count >= current_table->size *
        TABLE_UPPER_LIMIT) {
        if (!increase_table_size(current_table)) {
            printf(ALLOCATION_FAIL);
            exit(EXIT_FAILURE);
            return NULL;
        }
    }
    if (search_table(current_table, key)) {
        return NULL;
    }
    new_item = malloc(sizeof(struct table_item));
    if (new_item == NULL) {
        printf(ALLOCATION_FAIL);
        exit(EXIT_FAILURE);
        return NULL;
    }
    new_item->key = strdup(key);
    new_item->value = strdup(value);
    new_item->next = NULL;
    new_item->inst_rule = NULL;
    new_item->symbol = NULL;
    if (current_table->bucket[index] == NULL) {
        current_table->bucket[index] = new_item;
    } else {
        current_macro = current_table->bucket[index];
        while (current_macro->next != NULL) {
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
    char *dest_addressing) {
    struct table_item *item = insert_table_item(
        table, function_name, function_content);
    if (item == NULL) {
        printf(ALLOCATION_FAIL);
        exit(EXIT_FAILURE);
    }
    item->inst_rule = malloc(sizeof(instruction_rules));
    item->inst_rule->op_code = atoi(function_content); //TODO
    item->inst_rule->funct = funct;
    item->inst_rule->source_addressing = strdup(source_addressing);
    if (item->inst_rule->source_addressing == NULL) {
        printf(ALLOCATION_FAIL);
        exit(EXIT_FAILURE);
    }
    item->inst_rule->dest_addressing = strdup(dest_addressing);
    if (item->inst_rule->dest_addressing == NULL) {
        printf(ALLOCATION_FAIL);
        exit(EXIT_FAILURE);
    }
    return item;
}

int increase_table_size(table *current_table) {
    int new_size, i, new_index;
    table_item *temp = NULL, *pointer = NULL, *current_macro =
            NULL;
    table_item **bucket;
    new_size = current_table->size * TABLE_MULTIPLIER;
    bucket = malloc(new_size * sizeof(table_item));
    if (bucket == NULL) {
        printf(ALLOCATION_FAIL);
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < current_table->size; i++) {
        current_macro = current_table->bucket[i];
        while (current_macro != NULL) {
            new_index = hash_function(current_macro->key, new_size);
            if (bucket[new_index] == NULL) {
                bucket[new_index] = malloc(
                    sizeof(struct table_item));
                if (bucket[new_index] == NULL) {
                    printf(ALLOCATION_FAIL);
                    exit(EXIT_FAILURE);

                }
                bucket[new_index] = current_macro;
                temp = current_macro->next;
                bucket[new_index]->next = NULL;
                current_macro = temp;
            } else {
                pointer = bucket[new_index]->next;
                while (pointer->next != NULL) {
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

table_item *search_table(table *current_table,
                                char *key) {
    int index;
    table_item *current_macro;
    if (!key)return NULL;
    index = hash_function(key, current_table->size);
    current_macro = current_table->bucket[index];
    while (current_macro != NULL) {
        if (strcmp(current_macro->key, key) == 0) {
            return current_macro;
        }
        current_macro = current_macro->next;
    }
    return NULL;
}

void free_table(table **current_table) {
    int i;
    table_item *temp_macro = NULL, *current_macro = NULL;
    table *temp_table = *current_table;
    for (i = 0; i < temp_table->size; i++) {
        current_macro = temp_table->bucket[i];
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
            if (temp_macro->symbol != NULL) {
                if (temp_macro->symbol->extern_locations != NULL) {
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

table_item *insert_item_with_symbol(table *table,
                                           char *symbol_name,
                                           char *symbol_location,
                                           int location,
                                           bool is_data,
                                           bool is_entry,
                                           bool is_extern,
                                           bool is_code) {
    table_item *item = insert_table_item(
        table, symbol_name, symbol_location);
    if (item == NULL) {
        return NULL; // Failed to insert the macro
    }
    item->symbol = malloc(sizeof(symbol_type));
    if (item->symbol == NULL) {
        printf(ALLOCATION_FAIL);
        exit(EXIT_FAILURE);
    }
    item->symbol->extern_locations_size = 0;
    item->symbol->extern_locations = NULL;
    item->symbol->location = location;
    item->symbol->is_data = is_data;
    item->symbol->is_entry = is_entry;
    item->symbol->is_extern = is_extern;
    item->symbol->is_code = is_code;
    return item;
}

bool insert_external(table *table, char *symbol_name,
                     int location) {
    table_item *item = search_table(
        table, symbol_name);
    if (!item) {
        return false;
    }
    if (!item->symbol->is_extern) {
        return false;
    }
    if (item->symbol->extern_locations == NULL) {
        item->symbol->extern_locations = malloc(
            sizeof(int));
        item->symbol->extern_locations[0] = location;
        item->symbol->extern_locations_size++;
    } else {
        item->symbol->extern_locations = realloc(
            item->symbol->extern_locations,
            (item->symbol->extern_locations_size + 1) * sizeof(int));
        if (item->symbol->extern_locations == NULL) {
            printf(ALLOCATION_FAIL);
            exit(EXIT_FAILURE);
        }
        item->symbol->extern_locations[item->symbol->
            extern_locations_size] = location;
        item->symbol->extern_locations_size++;
    }
    return true;
}
