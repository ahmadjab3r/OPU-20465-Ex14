#include "initial_run.h"

int initial_run(char *file_name, char *output_file_name, MacroTable *table)
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
          store_mcro(current_line, file, table);
        }
    }
  fclose(file);
  free_table(&table);
  return 0;
}
int store_mcro(char *current_line, FILE *file, struct MacroTable *table)
{
  char *macro_name;
  char *macro_content= NULL;
  int line_num = 0;
  macro_name = strtok(current_line, " ");
  macro_name = strtok(NULL, " ");
  if (macro_name == NULL)
    {
      //TODO HANDLE ERROR
      return -1;
    }

  while (fgets(current_line, LINE_LENGTH, file) != NULL)
    {
      line_num += 1;
      if (strstr(current_line, ENDMCRO) != NULL)
        {
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

  insert_macro(table, macro_name, macro_content);
  return 0;
}
