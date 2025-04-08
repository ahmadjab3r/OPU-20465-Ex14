#include <stdio.h>
#include <stdbool.h>

#include "first_run.h"
#include "initial_run.h"
#include "skeleton.h"


ASFile *initialize_as_file(char *file_name) {
    ASFile *as_file = malloc(sizeof(ASFile));
    if (as_file == NULL) {
        //TODO handle Error
        return NULL;
    }
    as_file->file_name = strdup(file_name);
    as_file->file_name_spaces = NULL;
    as_file->file_name_macros = NULL;
    as_file->file_name_ob = NULL;
    as_file->lines = initialize_linked_list();
    as_file->symbol_table = create_table();
    as_file->data_table = create_table();
    as_file->macro_table = create_table();
    as_file->IC = IC_INITIAL;
    as_file->DC = DC_INITIAL;
    return as_file;
}

void free_as_file(ASFile **as_file, Constants **constants) {
    if (as_file != NULL && *as_file != NULL) {
        free((*as_file)->file_name);
        free((*as_file)->file_name_spaces);
        free((*as_file)->file_name_macros);
        free((*as_file)->file_name_ob);
        free_list(&(*as_file)->lines);
        free_table(&(*as_file)->symbol_table);
        free_table(&(*as_file)->data_table);
        free_table(&(*as_file)->macro_table);
        free(*as_file);
        *as_file = NULL;
    }
    if (constants != NULL && *constants != NULL) {
        free_table(&(*constants)->op_code_table);
        free_table(&(*constants)->registers_table);
        free(*constants);
        *constants = NULL;
    }
}

void print_lines(LinkedList *lines)
{

    Node *current = lines->head;
    while (current != NULL)
        {
            printf("content %s: ,instruction: %d, lines: %d\n", current->content,
            current->instruction,current->line);
            current=  current->next;
        }
}
int main(void) {
    Constants *constants = initialize_constants();
    char *file = "simple_macro_example.as";
    ASFile *as_file = initialize_as_file(file);

    as_file->file_name_spaces = remove_extra_spaces_file(file);

    initial_run(as_file);
    first_run(as_file, constants);
    printf("\nThis is  symbol_table\n");
    print_hash_table(as_file->symbol_table);
    print_lines(as_file->lines);
    // print_hash_table(macro_table);
    free_as_file(&as_file, &constants);
    return 0;
}


// int testing_hash_table()
// {
//   struct MacroTable *m = create_table();
//
//   struct Macro ex1 = {"ex1", "ex1_content", NULL, 0};
//   struct Macro ex2 = {"ex2", "ex2_content", NULL, 0};
//   struct Macro ex3 = {"ex3", "ex3_content", NULL, 0};
//   insert_macro(m, ex1.macro_name, ex1.macro_content);
//   insert_macro(m, ex2.macro_name, ex2.macro_content);
//   printf("\nthis is the old hash %d\n", hash_function(ex2.macro_name, m->size));
//
//   insert_macro(m, ex3.macro_name, ex3.macro_content);
//   struct Macro *found_macro = search_macro(m, "ex2");
//
//   printf("\nthis is the new hash %d\n", hash_function(ex2.macro_name, m->size));
//   if (found_macro != NULL) {
//       printf("Found macro: %s\n", found_macro->macro_name);
//   } else {
//       printf("Macro not found\n");
//   }
//   struct Macro *found_macro2 = search_macro(m, "ex1");
//   printf("testing %s", m->macros[4]->macro_content);
//   if (found_macro != NULL) {
//       printf("Found macro: %s\n", found_macro2->macro_name);
//   } else {
//       printf("Macro not found\n");
//   }
//   struct Macro *found_macro3 = search_macro(m, "ex4");
//   if (found_macro3 != NULL) {
//       printf("Found macro: %s\n", found_macro3->macro_name);
//   } else {
//       printf("Macro not found\n");
//   }
//   return 0;
// }
