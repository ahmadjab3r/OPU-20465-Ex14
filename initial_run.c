#include "Header Files/initial_run.h"

void initial_run(as_file *current_as_file)
{
  char current_line[LINE_LENGTH];
  FILE *file;
  file = fopen(current_as_file->file_name_spaces, "r");
  if (file == NULL)
    {
      printf(ERROR_INVALID_FILE, current_as_file->file_name_spaces);
      current_as_file->is_valid = false;
      return;
    }
  while (fgets(current_line, LINE_LENGTH, file) != NULL)
    {
      if (strstr(current_line, MCRO) != NULL)
        {
          if (store_mcro(current_as_file, current_line, file) == -1)
            {
              current_as_file->is_valid = false;
            }
        }
    }
  if (write_mcro_file(current_as_file) == -1)
    {
      current_as_file->is_valid = false;
      return;
    };
  fclose(file);
  return;
}
int write_mcro_file(as_file *current_as_file)
{
  char current_line[LINE_LENGTH];
  char *token, *temp;
  FILE *file, *file_output;
  table_item *mcro;
  int mcro_found = false;
  file = fopen(current_as_file->file_name_spaces, "r");
  file_output = fopen(current_as_file->file_name_macros, "w");
  temp = NULL;
  if (file == NULL || file_output == NULL)
    {
      printf(ERROR_INVALID_FILE, current_as_file->file_name_macros);
      current_as_file->is_valid = false;
      return -1;
    }
  while (fgets(current_line, LINE_LENGTH, file) != NULL)
    {
      if (strstr(current_line, ENDMCRO) != NULL)
        {
          mcro_found = false;
          continue;
        }
      if (mcro_found)
        {
          continue;
        }
      if (strstr(current_line, MCRO) != NULL)
        {
          mcro_found = true;
          continue;
        }
      if (temp)free(temp);
      temp = strdup(current_line);
      if (current_as_file->macro_table->size != 0)
        {
          token = strtok(temp, " \n");
          mcro = search_table(current_as_file->macro_table, token);
          if (mcro != NULL)
            {
              fprintf(file_output, "%s", mcro->value);
              continue;
            }
        }
      fprintf(file_output, "%s", current_line);
    }
  if (temp)free(temp);
  fclose(file);
  fclose(file_output);
  return 0;
}
int store_mcro(as_file *current_as_file, char *current_line, FILE *file)
{
  char *macro_name, *token;
  char *macro_content = NULL;
  token = strtok(current_line, " \n");
  token = strtok(NULL, " \n");
  if (token == NULL)
    {
      current_as_file->is_valid = false;
      printf(ERROR_INVALID_MACRO);
      return -1;
    }

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
        }
    }
  if (macro_content == NULL)
    {
      exit(-1);
    }
  if (insert_table_item(current_as_file->macro_table, macro_name,
                        macro_content) == NULL)
    {
      printf(ERROR_MEM_FAILED);
      exit(-1);
    }
  printf("-----------MCRO CONTENT START----------- \n");
  printf("%s", macro_content);
  printf("-----------MCRO CONTENT END----------- \n");
  free(macro_content);
  free(macro_name);
  return 0;
}
