#include "initial_run.h"

void initial_run(ASFile* current_as_file)
{
  char current_line[LINE_LENGTH];
  char *token, *temp;
  FILE *file,*file_copy, *file_output;
  table_item *mcro;
  bool mcro_found = false;

  file = fopen(current_as_file->file_name_spaces, "r");
  file_copy = fopen(current_as_file->file_name_spaces, "r");
  file_output = fopen(current_as_file->file_name_macros, "w");
  temp = NULL;
  if (file == NULL)
    {
      printf(ERROR_INVALID_FILE,current_as_file->file_name_spaces);
       current_as_file->is_valid = false;
      return ;
    }
  while (fgets(current_line, LINE_LENGTH, file) != NULL)
    {
      if (strstr(current_line, MCRO) != NULL)
        {
          if (store_mcro(current_line, file, current_as_file->macro_table) == -1) {
            current_as_file->is_valid = false;
            return;
          }
        }
    }

  while (fgets(current_line, LINE_LENGTH, file_copy) != NULL)
  {

    if(strstr(current_line, ENDMCRO) != NULL)
    {
      mcro_found = false;
      continue;
    }
    if(mcro_found) {
      continue;
    }
    if (strstr(current_line, MCRO) != NULL)
    {
      mcro_found= true;
      continue;
      }
    if(temp)free(temp);
   temp = strdup(current_line);
    if(current_as_file->macro_table->size !=0) {
      token= strtok(temp, " \n");
    mcro = search_table(current_as_file->macro_table, token);
    if(mcro!=NULL) {
      fprintf(file_output, "%s", mcro->value);
      continue;
    }
    }

    fprintf(file_output,"%s", current_line);
    }
  if(temp)free(temp);
  fclose(file_copy);
  fclose(file_output);
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
      if (strstr(current_line, ENDMCRO) != NULL)
        {
         break;
        }
      macro_content = copy_line(macro_content, current_line);
      if (macro_content == NULL)
        {
          printf(ERROR_MEM_FAILED);
          return -1;
        }
    }
  if (macro_content == NULL)
    {
      return -1;
    }
  printf("-----------MCRO CONTENT START----------- \n");
  printf("%s", macro_content);
  printf("-----------MCRO CONTENT END----------- \n");

  if (insert_table_item(table, macro_name, macro_content) == NULL)
    {
    printf(ERROR_MEM_FAILED);
    }
  free(macro_content);
  free(macro_name);
  return 0;
}
