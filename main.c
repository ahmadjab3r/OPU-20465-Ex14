#include <stdio.h>
#include <stdbool.h>

#include "first_run.h"
#include "initial_run.h"
#include "second_run.h"
#include "skeleton.h"

ASFile *initialize_as_file(char *file_name)
{
  ASFile *as_file = malloc(sizeof(ASFile));
  if (as_file == NULL)
    {
      //TODO handle Error
      return NULL;
    }
  as_file->file_name = generate_file_name(file_name, AS_FILE_ENDING);
  as_file->file_name_spaces = generate_file_name(
    file_name, REMOVED_SPACES_FILE_ENDING);
  as_file->file_name_macros = generate_file_name(file_name, MACRO_FILE_ENDING);
  as_file->file_name_ob = generate_file_name(file_name, OB_FILE_ENDING);
  as_file->file_name_entries = NULL;
  as_file->file_name_externals = NULL;
  as_file->lines = initialize_linked_list();
  as_file->symbol_table = create_table();
  as_file->macro_table = create_table();
  as_file->IC = IC_INITIAL;
  as_file->DC = DC_INITIAL;
  return as_file;
}

void free_as_file(ASFile **as_file, Constants **constants)
{
  if (as_file != NULL && *as_file != NULL)
    {
      free((*as_file)->file_name);
      free((*as_file)->file_name_spaces);
      free((*as_file)->file_name_macros);
      free((*as_file)->file_name_ob);
      free_list(&(*as_file)->lines);
      free_table(&(*as_file)->symbol_table);
      free_table(&(*as_file)->macro_table);
      if ((*as_file)->file_name_entries != NULL)
        {
          free((*as_file)->file_name_entries);
        }
      if ((*as_file)->file_name_externals != NULL)
        {
          free((*as_file)->file_name_externals);
        }
      free(*as_file);
      *as_file = NULL;
    }
  if (constants != NULL && *constants != NULL)
    {
      free_table(&(*constants)->op_code_table);
      free_table(&(*constants)->registers_table);
      free(*constants);
      *constants = NULL;
    }
}

/** function that receives an int which is 32 bits, prints the first 24 bits
 * and the value of the first 21 bits **/
void print_binary(int value)
{
  int i;
  for (i = 23; i >= 0; i--)
    {
      printf("%d", (value >> i) & 1);
    }
  printf("\n");

  /**prints the value of the first 21 bits**/
  int mask = 0xFFFFF8; // 21 bits mask
  int result = value & mask;
  printf("first 21 bits: %d\n", result);
  // printf("first 21 bits: ");
  // for (i = 31; i >= 11; i--)
  //     {
  //         printf("%d", (value >> i) & 1);
  //     }
  // printf("\n");
}

void print_lines(LinkedList *lines)
{
  Node *current = lines->head;
  while (current != NULL)
    {
      printf("key: %s, content: %s, instruction: %d, lines: %d\n",
             current->name, current->content,
             current->instruction, current->line);
      print_binary(current->instruction);

      current = current->next;
    }
}

int main(int argc, char **argv)
{
  if (argc != 2)
    {
      printf("ERROR: At least one file is required.\n");
      return 1;
    }
  Constants *constants = initialize_constants();
  for (int i = 1; i < argc; i++)
    {
      ASFile *as_file = initialize_as_file(argv[i]);
      as_file->file_name_spaces = remove_spaces(as_file->file_name,as_file->file_name_spaces);

      initial_run(as_file);
      first_run(as_file, constants);
      printf("#########\nThis is  symbol_table\n############");
      print_hash_table(as_file->symbol_table);
      second_run(as_file);
      print_lines(as_file->lines);
      // print_hash_table(macro_table);
      free_as_file(&as_file, &constants);
    }

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
