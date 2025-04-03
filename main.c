#include <stdio.h>
#include <stdbool.h>
#include "initial_run.h"
#include "skeleton.h"
#include "hashtable.h"
#define LINE_LENGTH 80 /* Maybe add one more for /n doouble check that*/



int main() {
  char *pre_macro, *macro, *label, *second;

  char * file = "../tests/simple_macro_example.as";
  pre_macro = generate_file_name(file, PRE_MACRO_STAGE);
  macro = generate_file_name(file, MACRO_STAGE);
  label = generate_file_name(file, LABEL_STAGE);
  second = generate_file_name(file, SECOND_STAGE);

  // remove_spaces(file, pre_macro);
  // initial_run(pre_macro, macro);
  struct MacroTable *m = create_table();
  struct Macro ex1 = {"ex1", "ex1_content", NULL, 0};
  struct Macro ex2 = {"ex2", "ex2_content", NULL, 0};
  struct Macro ex3 = {"ex3", "ex3_content", NULL, 0};
  insert_macro(m, ex1.macro_name, ex1.macro_content);
  insert_macro(m, ex2.macro_name, ex2.macro_content);
  insert_macro(m, ex3.macro_name, ex3.macro_content);
  struct Macro *found_macro = search_macro(m, "ex2");
  if (found_macro != NULL) {
    printf("Found macro: %s\n", found_macro->macro_name);
  } else {
    printf("Macro not found\n");
  }
  struct Macro *found_macro2 = search_macro(m, "ex1");
  if (found_macro != NULL) {
      printf("Found macro: %s\n", found_macro2->macro_name);
  } else {
      printf("Macro not found\n");
  }
  struct Macro *found_macro3 = search_macro(m, "ex4");
  if (found_macro3 != NULL) {
      printf("Found macro: %s\n", found_macro3->macro_name);
  } else {
      printf("Macro not found\n");
  }


  return 0;
}