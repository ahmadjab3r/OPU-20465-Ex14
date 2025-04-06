#include <stdio.h>
#include <stdbool.h>

#include "first_run.h"
#include "initial_run.h"
#include "skeleton.h"




int main(void) {
  char *pre_macro, *macro, *label, *second;
  int IC=IC_INITIAL,DC=DC_INITIAL;
  table *macro_table = create_table();
  table *symbol_table = create_table();
  table *data_table = create_table();
  Constants *constants= initialize_constants();
  if (macro_table == NULL)
    {
      //TODO HANDLE ERROR
      return -1;
    }


  char * file = "simple_macro_example.as";
  pre_macro = generate_file_name(file, PRE_MACRO_STAGE);
  macro = generate_file_name(file, MACRO_STAGE);
  label = generate_file_name(file, LABEL_STAGE);
  second = generate_file_name(file, SECOND_STAGE);

  char * temp = remove_extra_spaces_file(file);
  initial_run(temp, macro,macro_table,symbol_table);
  first_run(temp,"test",symbol_table,data_table,constants);
  printf("\nThis is  macro_table\n");
  print_hash_table(macro_table);
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
