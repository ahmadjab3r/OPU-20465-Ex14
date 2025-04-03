#include "utilities.h"

void allocate_new_file_name(char *file_name, char **edited_file_name,
                            int additional_size,
                            int removal_size, char *ending)
{
  *edited_file_name = malloc(strlen(file_name) + additional_size);
  if (*edited_file_name == NULL)
    {
      perror("Error allocating memory");
      exit(EXIT_FAILURE);
    }
  *edited_file_name = strcpy(*edited_file_name, file_name);
  (*edited_file_name)[strlen(*edited_file_name) - removal_size] = '\0'; //TODO
  strcat(*edited_file_name, ending);
}
char *generate_file_name(char *file_name, int stage)
{
  char *edited_file_name;
  /* First stage, the current file name ends in .as */
  if (stage == PRE_MACRO_STAGE)
    {
      allocate_new_file_name(file_name, &edited_file_name, 2, 3,
                             PREC_FILE_ENDING);
    }
  /* Second stage, the current file name ends in .prec */
  if (stage == MACRO_STAGE)
    {
      allocate_new_file_name(file_name, &edited_file_name, 1, 5,
                             MCRO_FILE_ENDING);
    }

  return edited_file_name;
}
/**
 * This function removes all extra unnecessary white spaces from the file
 * @param file_name string of the input file name
 * @return string the name of the new file after white spaces removed
 */
char *remove_spaces(char *file_name, char *output_file_name)
{
  FILE *input_file = fopen(file_name, "r");
  if (input_file == NULL)
    {
      //TODO HANDLE ERROR
      return NULL;
    }

  FILE *output_file = fopen(output_file_name, "w");
  if (output_file == NULL)
    {
      perror("Error creating file");
      fclose(input_file);
      free(output_file_name);
      return NULL;
    }

  int c;
  int last_char = ' ';
  while ((c = fgetc(input_file)) != EOF)
    {
      if (!isspace(c) || (isspace(c) && !isspace(last_char)))
        {
          fputc(c, output_file);
        }
      last_char = c;
    }

  fclose(input_file);
  fclose(output_file);

  return output_file_name;
}

char *copy_text(char *macro_content, char *current_line)
{
  int length;
  /** appends current_line to macro_content **/
  if (macro_content == NULL)
    {
      macro_content = malloc(strlen(current_line) + 1);
    }
  else
    {
      length = strlen(macro_content) + strlen(current_line) + 1;
      macro_content = realloc(macro_content, length);
    }
  if (macro_content == NULL)
    {
      //TODO HANDLE ERROR
      return NULL;
    }
  strcat(macro_content, current_line);
  return macro_content;
}

table *initialize_op_code_table(void)
{
  table *op_code_table = create_table();
  if (op_code_table == NULL)
    {
      //TODO HANDLE ERROR
      return NULL;
    }
  insert_macro(op_code_table, "mov", "2");
  insert_macro(op_code_table, "cmp", "2");
  insert_macro(op_code_table, "add", "2");
  insert_macro(op_code_table, "sub", "2");
  insert_macro(op_code_table, "not", "1");
  insert_macro(op_code_table, "clr", "1");
  insert_macro(op_code_table, "lea", "2");
  insert_macro(op_code_table, "inc", "1");
  insert_macro(op_code_table, "dec", "1");
  insert_macro(op_code_table, "jmp", "1");
  insert_macro(op_code_table, "bne", "1");
  insert_macro(op_code_table, "red", "1");
  insert_macro(op_code_table, "prn", "1");
  insert_macro(op_code_table, "jsr", "1");
  insert_macro(op_code_table, "rts", "0");
  insert_macro(op_code_table, "stop", "0");
  return op_code_table;
}
void print_hash_table(table *table)
{
  for (int i = 0; i < table->size; i++)
    {
      if (table->macros[i] != NULL)
        {
          printf("Index %d, ", i);
          printf("Key: %s Value: %s\n", table->macros[i]->key,
                 table->macros[i]->value);
          if (table->macros[i]->next != NULL)
            {
              struct table_item *current_macro =
                table->macros[i]->next;
              while (current_macro != NULL)
                {
                  printf("          Key: %s Value: %s\n", current_macro->key,
                         current_macro->value);
                  current_macro = current_macro->next;
                }
            }
        }
    }
}
