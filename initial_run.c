#include "initial_run.h"

void initial_run(ASFile* current_as_file)
{
  char current_line[LINE_LENGTH];
  FILE *file;
  file = fopen(current_as_file->file_name_spaces, "r");
  if (file == NULL)
    {
      printf(ERROR_INVALID_FILE,current_as_file->file_name_spaces);
       current_as_file->is_valid = false;
      return ;
    }
  while (fgets(current_line, LINE_LENGTH, file) != NULL)
    {
      // printf("%s", current_line);
      if (strstr(current_line, MCRO) != NULL)
        {
          if (store_mcro(current_line, file, current_as_file->macro_table) == -1) {
            current_as_file->is_valid = false;
          }
        }
    //printf("%s\n", current_line);
    }
  fclose(file);
  return;
}
int store_mcro(char *current_line, FILE *file,  table *table)
{
  char *macro_name,*token;
  char *macro_content= NULL;
  token = strtok(current_line, " \n");
  token= strtok(NULL, " \n");

  macro_name = strdup(token);
  if (macro_name == NULL)
    {
      printf(ERROR_MEM_FAILED);
      return -1;
    }

  while (fgets(current_line, LINE_LENGTH, file) != NULL)
    {
      char* result = strstr(current_line, ENDMCRO);
      if (strstr(current_line, ENDMCRO) != NULL)
        {
          //TODO check if there's nothing after the word endmcro
          printf(ERROR_INVALID_MACRO);

        }
      macro_content = copy_line(macro_content, current_line);
      if (macro_content == NULL)
        {
        printf(ERROR_MEM_FAILED);
          //TODO HANDLE ERROR
          return -1;
        }
    }
  if (macro_content == NULL)
    {
    printf(ERROR_INVALID_MACRO);
      //TODO HANDLE ERROR
      return -1;
    }
  printf("-----------MCRO CONTENT START----------- \n");
  printf("%s", macro_content);
  printf("-----------MCRO CONTENT END----------- \n");

  if (insert_table_item(table, macro_name, macro_content) == NULL)
    {
      //TODO handle error
    }
  free(macro_content);
  free(macro_name);

  return 0;
}
