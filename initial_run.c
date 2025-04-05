#include "initial_run.h"

int initial_run(char *file_name, char *output_file_name, table *macro_table,
table* symbol_table)
{
  char current_line[LINE_LENGTH];
  FILE *file;
  //TODO check file name ?
  file = fopen(file_name, "r");
  if (file == NULL)
    {
      //TODO HANDLE ERROR
      return -1;
    }

  while (fgets(current_line, LINE_LENGTH, file) != NULL)
    {
      printf("%s", current_line);
      if (strstr(current_line, MCRO) != NULL)
        {
          store_mcro(current_line, file, macro_table);
        }
    }
  fclose(file);
  return 0;
}
int store_mcro(char *current_line, FILE *file, struct table *table)
{
  char *macro_name,*token;
  char *macro_content= NULL;
  token = strtok(current_line, " \n");
  token= strtok(NULL, " \n");

  macro_name = strdup(token);
  if (macro_name == NULL)
    {
      //TODO HANDLE ERROR
      return -1;
    }

  while (fgets(current_line, LINE_LENGTH, file) != NULL)
    {
      if (strstr(current_line, ENDMCRO) != NULL)
        {
          //TODO check if there's nothing after the word endmcro
          break;
        }
      macro_content = copy_text(macro_content, current_line);
      if (macro_content == NULL)
        {
          //TODO HANDLE ERROR
          return -1;
        }
    }
  if (macro_content == NULL)
    {
      //TODO HANDLE ERROR
      return -1;
    }
  printf("-----------MCRO CONTENT START----------- \n");
  printf("%s", macro_content);
  printf("-----------MCRO CONTENT END----------- \n");

  if (insert_macro(table, macro_name, macro_content))
    {
      //TODO handle error
    }
  free(macro_content);
  free(macro_name);

  return 0;
}
