#include <stdio.h>
#include "Header Files/first_run.h"
#include "Header Files/initial_run.h"
#include "Header Files/second_run.h"
#include "Header Files/skeleton.h"

as_file *initialize_as_file(char *file_name)
{
  as_file *current_as_file = malloc(sizeof(as_file));
  if (current_as_file == NULL)
    {
      printf(ALLOCATION_FAIL);
      exit(EXIT_FAILURE);
    }
  current_as_file->file_name =
      generate_file_name(file_name, AS_FILE_ENDING);
  current_as_file->file_name_spaces = generate_file_name(
    file_name, REMOVED_SPACES_FILE_ENDING);
  current_as_file->file_name_macros = generate_file_name(
    file_name, MACRO_FILE_ENDING);
  current_as_file->file_name_ob = generate_file_name(
    file_name, OB_FILE_ENDING);
  current_as_file->file_name_entries = NULL;
  current_as_file->file_name_externals = NULL;
  current_as_file->lines = initialize_linked_list();
  current_as_file->symbol_table = create_table();
  current_as_file->macro_table = create_table();
  current_as_file->IC = IC_INITIAL;
  current_as_file->DC = DC_INITIAL;
  current_as_file->is_valid = true;
  return current_as_file;
}

void free_as_file(as_file **current_as_file)
{
  if (current_as_file != NULL && *current_as_file != NULL)
    {
      free((*current_as_file)->file_name);
      free((*current_as_file)->file_name_spaces);
      free((*current_as_file)->file_name_macros);
      free((*current_as_file)->file_name_ob);
      free_list(&(*current_as_file)->lines);
      free_table(&(*current_as_file)->symbol_table);
      free_table(&(*current_as_file)->macro_table);
      if ((*current_as_file)->file_name_entries != NULL)
        {
          free((*current_as_file)->file_name_entries);
        }
      if ((*current_as_file)->file_name_externals != NULL)
        {
          free((*current_as_file)->file_name_externals);
        }
      free(*current_as_file);
      *current_as_file = NULL;
    }
}

void free_constants(constants **globals)
{
  if (globals != NULL && *globals != NULL)
    {
      free_table(&(*globals)->op_code_table);
      free_table(&(*globals)->registers_table);
      free(*globals);
      *globals = NULL;
    }
}

int main(int argc, char **argv)
{
  as_file *current_as_file;
  constants *globals;
  int i;
  if (argc < 2)
    {
      printf("ERROR: At least one file is required.\n");
      return 1;
    }
  globals = initialize_constants();
  for (i = 1; i < argc; i++)
    {
      printf(PROCESSING_FILE, argv[i]);
      current_as_file = initialize_as_file(argv[i]);
      current_as_file->file_name_spaces = remove_spaces(
        current_as_file->file_name, current_as_file->file_name_spaces);
      initial_run(current_as_file);
      first_run(current_as_file, globals);
      /*printf("\n\n######### This is  symbol_table ############\n");
      print_hash_table(current_as_file->symbol_table);
      printf("#########################################\n\n");*/
      second_run(current_as_file);
      /*printf("######### This is lines ############\n");
      print_lines(current_as_file->lines);
      printf("#########################################\n\n");*/
      free_as_file(&current_as_file);
    }
  free_constants(&globals);

  return 0;
}
