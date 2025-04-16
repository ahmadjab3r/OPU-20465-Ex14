#include "Header Files/utilities.h"
#include "Header Files/hash_table.h"
#include "Header Files/linked_list.h"

/**
 * This function removes all extra unnecessary white spaces from the file
 * @param file_name string of the input file name
 * @return string the name of the new file after white spaces removed
 */
int is_trim_space(int c)
{
  return c == ' ' || c == '\t' || c == '\r';
}

char *remove_spaces(char *file_name, char *output_file_name)
{
  int c, in_word, has_content = 0, has_comment = 0;
  FILE *output_file, *input_file = fopen(file_name, "r");
  if (input_file == NULL)
    {
      perror("Error opening input file");
      return NULL;
    }

  output_file = fopen(output_file_name, "w");
  if (output_file == NULL)
    {
      perror("Error creating output file");

      fclose(input_file);
      return NULL;
    }

  in_word = 0;
  while ((c = fgetc(input_file)) != EOF)
    {
      /*TODO check this*/
      if (c == ';')
        {
          has_comment = 1;
          continue;
        }
      if (has_comment && c == '\n')
        {
          has_comment = 0;
          continue;
        }
      if (has_comment)
        {
          continue;
        }
      if (is_trim_space(c))
        {
          if (in_word)
            {
              /* keep one space */
              fputc(' ', output_file);
              in_word = 0;
            }
        }
      else if (c == ',')
        {
          fputc(',', output_file);
          in_word = 0;
          has_content = 1;
        }
      else if (c == '\n')
        {
          if (has_content)
            {
              fputc('\n', output_file);
            }
          in_word = 0;
          has_content = 0;
          has_comment = 0;
        }
      else
        {
          fputc(c, output_file);
          in_word = 1;
          has_content = 1;
        }
    }
  fclose(input_file);
  fclose(output_file);
  return output_file_name;
}

char *generate_file_name(char *file_name, char *ending)
{
  /* gets file name, finds the '.' and adds the new ending */
  char *c, *new_file_name;
  new_file_name = malloc(LINE_LENGTH * sizeof(char));
  strcpy(new_file_name, file_name);
  /* deleting the file name if a '.' exists and forth */
  if ((c = strchr(new_file_name, '.')) != NULL)
    {
      *c = '\0';
    }
  /* adds the ending of the new file name */
  strcat(new_file_name, ending);
  return new_file_name;
}

char *copy_line(char *macro_content, char *current_line)
{
  int length;
  if (macro_content == NULL)
    {
      macro_content = malloc(strlen(current_line) + 1);
      if (macro_content == NULL)
        {
          printf(ALLOCATION_FAIL);
          exit(EXIT_FAILURE);
        }
      strcpy(macro_content, current_line);
      return macro_content;
    }
  length = strlen(macro_content) + strlen(current_line) + 1;
  macro_content = realloc(macro_content, length);
  if (macro_content == NULL)
    {
      printf(ALLOCATION_FAIL);
      exit(EXIT_FAILURE);
    }
  strcat(macro_content, current_line);
  return macro_content;
}

hash_table *initialize_op_code_table(void)
{
  hash_table *op_code_table = create_table();
  insert_item_with_instructions(op_code_table, "mov", "0", 0, "013",
                                "13");
  insert_item_with_instructions(op_code_table, "cmp", "1", 0, "013",
                                "013");
  insert_item_with_instructions(op_code_table, "add", "2", 1, "013",
                                "13");
  insert_item_with_instructions(op_code_table, "sub", "2", 2, "013",
                                "13");
  insert_item_with_instructions(op_code_table, "lea", "4", 0, "1",
                                "13");
  insert_item_with_instructions(op_code_table, "clr", "5", 1, "",
                                "13");
  insert_item_with_instructions(op_code_table, "not", "5", 2, "",
                                "13");
  insert_item_with_instructions(op_code_table, "inc", "5", 3, "",
                                "13");
  insert_item_with_instructions(op_code_table, "dec", "5", 4, "",
                                "13");
  insert_item_with_instructions(op_code_table, "jmp", "9", 1, "",
                                "12");
  insert_item_with_instructions(op_code_table, "bne", "9", 2, "",
                                "12");
  insert_item_with_instructions(op_code_table, "jsr", "9", 3, "",
                                "12");
  insert_item_with_instructions(op_code_table, "red", "12", 0, "",
                                "13");
  insert_item_with_instructions(op_code_table, "prn", "13", 0, "",
                                "013");
  insert_item_with_instructions(op_code_table, "rts", "14", 0, "",
                                "");
  insert_item_with_instructions(op_code_table, "stop", "15", 0, "",
                                "");
  return op_code_table;
}

hash_table *initialize_registers_table(void)
{
  hash_table *registers_table = create_table();
  insert_table_item(registers_table, "r0", "0");
  insert_table_item(registers_table, "r1", "1");
  insert_table_item(registers_table, "r2", "2");
  insert_table_item(registers_table, "r3", "3");
  insert_table_item(registers_table, "r4", "4");
  insert_table_item(registers_table, "r5", "5");
  insert_table_item(registers_table, "r6", "6");
  insert_table_item(registers_table, "r7", "7");
  return registers_table;
}

void print_hash_table(hash_table *table)
{
  table_item *current_macro;
  int i = 0;

  for (; i < table->size; i++)
    {
      if (table->bucket[i] != NULL)
        {
          printf("Index %d, ", i);
          printf("Key: %s Value: %d\n", table->bucket[i]->key,
                 table->bucket[i]->symbol->location);

          if (table->bucket[i]->next != NULL)
            {
              current_macro =
                  table->bucket[i]->next;
              while (current_macro != NULL)
                {
                  if (current_macro->symbol)
                    {
                      printf("          Key: %s Value: %d\n",
                             current_macro->key,
                             current_macro->symbol->location);
                    }
                  current_macro = current_macro->next;
                }
            }
        }
    }
}

constants *initialize_constants(void)
{
  constants *globals = malloc(sizeof(constants));
  if (globals == NULL)
    {
      printf(ALLOCATION_FAIL);
      exit(EXIT_FAILURE);
    }
  globals->op_code_table = initialize_op_code_table();
  if (globals->op_code_table == NULL)
    {
      printf(ALLOCATION_FAIL);
      exit(EXIT_FAILURE);
    }
  globals->registers_table = initialize_registers_table();
  if (globals->registers_table == NULL)
    {
      printf(ALLOCATION_FAIL);
      exit(EXIT_FAILURE);
    }
  return globals;
}

int validate_number(char *str, int *number)
{
  int i;
  char *letter = str;
  if (strlen(str) == 0)return false;
  i = 0;
  while (*letter != '\0')
    {
      if (*letter != '-' && !isdigit(*letter))
        {
          return false;
        }
      if (*letter == '-' && i != 0)
        {
          return false;
        }
      letter++;
      i++;
    }
  *number = (int) strtol(str,NULL, 10);
  return true;
}

int translate_address(int value, int A, int R, int E)
{
  int result = value << 1;
  result += A;
  result = result << 1;
  result += R;
  result = result << 1;
  result += E;
  return result;
}

int translate_instruction_address(instruction *instruction)
{
  int result = instruction->op_code;
  result = result << 2;
  result += instruction->source_addressing;
  result = result << 3;
  result += instruction->source_reg;
  result = result << 2;
  result += instruction->destination_addressing;
  result = result << 3;
  result += instruction->destination_reg;
  result = result << 5;
  result += instruction->funct;
  result = result << 1;
  result += instruction->A;
  result = result << 1;
  result += instruction->R;
  result = result << 1;
  result += instruction->E;
  return result;
}

/* function that receives an int which is 32 bits, prints the first 24 bits
 * and the value of the first 21 bits */
void print_binary(int value)
{
  int i,mask,result;
  for (i = 23; i >= 0; i--)
    {
      printf("%d", (value >> i) & 1);
    }
  printf("\n");

  /**prints the value of the first 21 bits**/
  mask = 0xFFFFF8; /* 21 bits mask*/
  result = value & mask;
  printf("first 21 bits: %d\n", result);
}

void print_lines(linked_list *lines)
{
  struct node *current = lines->head;
  while (current != NULL)
    {
      printf("key: %s, content: %s, instruction: %d, lines: %d\n",
             current->name, current->content,
             current->instruction, current->line);
      print_binary(current->instruction);
      current = current->next;
    }
}
char *strdup(const char *str)
{
  int n = strlen(str) + 1;
  char *dup = malloc(n);
  if(dup)
  {
    strcpy(dup, str);
  }
  return dup;
}
/*int testing_hash_table(void)
{
  struct MacroTable *m = create_table();

  struct Macro ex1 = {"ex1", "ex1_content", NULL, 0};
  struct Macro ex2 = {"ex2", "ex2_content", NULL, 0};
  struct Macro ex3 = {"ex3", "ex3_content", NULL, 0};
  insert_macro(m, ex1.macro_name, ex1.macro_content);
  insert_macro(m, ex2.macro_name, ex2.macro_content);
  printf("\nthis is the old hash %d\n", hash_function(ex2.macro_name, m->size));

  insert_macro(m, ex3.macro_name, ex3.macro_content);
  struct Macro *found_macro = search_macro(m, "ex2");

  printf("\nthis is the new hash %d\n", hash_function(ex2.macro_name, m->size));
  if (found_macro != NULL) {
      printf("Found macro: %s\n", found_macro->macro_name);
  } else {
      printf("Macro not found\n");
  }
  struct Macro *found_macro2 = search_macro(m, "ex1");
  printf("testing %s", m->macros[4]->macro_content);
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
}*/
