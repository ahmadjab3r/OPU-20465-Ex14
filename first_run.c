#include "first_run.h"
void add_end_of_line(ASFile *as_file, char *symbol_name)
{
  add_node(symbol_name, "END OF LINE", 0, &as_file->IC, false, as_file->lines);
}
bool handle_line(ASFile *as_file, char *str)
{
  bool start = false, end = false;
  /* current_case is true if .string, false if .data*/
  bool current_case;
  char *temp_line = strdup(str);
  char *token = strtok(str, " :\n");
  if (token == NULL)
    {
      return false;
    }
  char *symbol_name = token;
  token = strtok(NULL, " \n");
  if (token == NULL)
    {
      return false;
    }
  char *symbol_type = token;
  struct table_item *item = search_table(as_file->symbol_table, symbol_name);
  if (item)
    {
      if (item->symbol->is_entry)
        {
          item->symbol->is_data = true;
        }
      else
        {
          //TODO handle Error!
          return false;
        }
    }
  insert_item_with_symbol(as_file->symbol_table, symbol_name, symbol_type,
                          as_file->IC,
                          true, false, false, false);

  current_case = strcmp(token, STRING_DEC) == 0;
  token = strtok(NULL, " \t\r\n");
  /* String case */
  int i = 0;
  if (current_case)
    {
      while (strlen(token) != i)
        {
          if (start && end)
            {
              //TODO HANDLE ERROR!
              printf("ERROR");
            }
          else if (start && token[i] == '"')
            {
              end = true;
            }
          else if (token[i] == '"')
            {
              if (i != 0)
                {
                  //TODO handle error! ' " ' was found way later!
                }
              start = true;
            }
          else
            {
              printf("char =%c", token[i]);
              add_node(symbol_name, token, token[i], &as_file->IC,true,
                       as_file->lines);
            }
          //TODO handle character
          i++;
        }
      add_end_of_line(as_file, symbol_name);
      //.data case
    }
  else
    {
      token = strtok(token, ", ");
      do
        {
          int number;
          if (validate_number(token, &number))
            {
              //TODO handle error
            }
          //todo handle number
          add_node(symbol_name, token, number, &as_file->IC,true,
                   as_file->lines);
        }
      while ((token = strtok(NULL, ", ")));
    }

  return true;
}
// First
//
bool immediate_addressing(ASFile *as_file, char *line,
                          struct table_item *command,bool type)
{
  if (type && command->inst_rule->source_addressing[0] !=
      IMMEDIATE_ADDRESSING_SIGN)
    {
      //TODO HANDLE ERROR
      return false;
    }
  if (!type && command->inst_rule->dest_addressing[0] !=
      IMMEDIATE_ADDRESSING_SIGN)
    {
      //TODO HANDLE ERROR
      return false;
    }
  int number;
  line++;
  if (!validate_number(line, &number))
    {
      //TODO handle error
      return false;
    }
  //TODO write to file
  add_node("Immediate", line, number, &as_file->IC,true, as_file->lines);
  return true;
}

//Second
bool direct_addressing(ASFile *as_file, char *line, struct table_item *command,
                       bool type)
{
  return true;
}
//Third

bool relative_addressing(ASFile *as_file, char *line,
                         struct table_item *command,bool type)
{
  if (type && strstr(command->inst_rule->source_addressing,
                     RELATIVE_ADDRESSING_SIGN) == NULL)
    {
      //TODO HANDLE ERROR
      return false;
    }
  if (!type && strstr(command->inst_rule->dest_addressing,
                      RELATIVE_ADDRESSING_SIGN) == NULL)

    {
      //TODO HANDLE ERROR
      return false;
    }
  //TODO if external return false!
  return true;
}
//Fourth
int direct_register_addressing(ASFile *as_file, char *line,
                                struct table_item *command,bool type,
                                Constants *constants)
{
  if (type && strstr(command->inst_rule->source_addressing,
                     REGISTER_ADDRESSING_SIGN) == NULL)
    {
      //TODO HANDLE ERROR
      return -1;
    }
  if (!type && strstr(command->inst_rule->dest_addressing,
                      REGISTER_ADDRESSING_SIGN) == NULL)
    {
      //TODO HANDLE ERROR
      return -1;
    }

  struct table_item *item = search_table(constants->registers_table,line);
  if (item)
    {
      return (int) strtol(item->key,NULL,10);
    }
  return -1;
  // printf()
}
/**
* Checks if the addressing type is valid for the given command.
returns 0 if immediate, 1 if direct, 2 if relative, 3 if direct register
-1 if failed
*/
int check_addressing_type(ASFile *as_file, char *line, struct table_item
                          *command, bool type,
                          instruction *current_instruction,Constants *constants)
{
  /* type = true if source, false if dest */
  if (*line == IMMEDIATE_ADDRESSING_SYMBOL)
    {
      printf("hi");

      if (immediate_addressing(as_file, line, command, type))
        {
          if (type)
            {
              current_instruction->source_addressing =
                IMMEDIATE_ADDRESSING_SIGN;
            }
          else
            {
              current_instruction->destination_addressing =
                IMMEDIATE_ADDRESSING_SIGN;
            }
        }
    }
  if (direct_addressing(as_file, line, command, type))
    {
    }
  if (strstr(line, RELATIVE_ADDRESSING_SYMBOL))
    {
      return relative_addressing(as_file, line, command, type) ? 2 : -1;
    }
  int reg= direct_register_addressing(as_file, line, command, type,
                                     constants);
  if (reg != -1)
    {
      if (type)
        {
          current_instruction->source_reg = reg;
          current_instruction->source_addressing = REGISTER_ADDRESSING_SIGN;
        }  else{
            current_instruction->destination_addressing = reg;
            current_instruction->destination_reg = REGISTER_ADDRESSING_SIGN;
          }
      return 3;
    }
}

void initialize_instruction(instruction *current_instruction, int op_code,
                            int funct)
{
  current_instruction->op_code = op_code;
  current_instruction->source_addressing = 0;
  current_instruction->source_reg = 0;
  current_instruction->destination_addressing = 0;
  current_instruction->destination_reg = 0;
  current_instruction->funct = funct;
  current_instruction->A = 1;
  current_instruction->R = 0;
  current_instruction->E = 0;
}
void handle_instruction(char *full_line, char *line, ASFile *as_file,
                        Constants *constants)
{
  instruction current_instruction;
  //TODO change return
  printf("this is token! %s\n", line);
  struct table_item *command = search_table(constants->op_code_table, line);
  if (command == NULL)
    {
      //TODO HANDLE ERROR
      printf("ERROR");
      return;
    }
  Node *node = add_node(line, line, 0, &as_file->IC,false,
           as_file->lines);
  initialize_instruction(&current_instruction, command->inst_rule->op_code,
                         command->inst_rule->funct);
  bool source = strlen(command->inst_rule->source_addressing);
  bool dest = strlen(command->inst_rule->dest_addressing);
  char *token = strtok(NULL, ", \n");
  /*this is also equal to !dest && !source */
  if (strcmp(line, "stop") == 0 || strcmp(line, "rts") == 0)
    {
      if (token != NULL)
        {
          //TODO handle error!
          return;
        }
      return;
    }
  if (token == NULL && source)
    {
      //TODO HANDLE ERROR
      printf("ERROR");
      return;
    }
  if (dest && !source)
    {
      current_instruction.destination_addressing = check_addressing_type(
        as_file,
        token, command, false, &current_instruction,constants);
      token = strtok(NULL, ", \n");
      if (token != NULL)
        {
          printf("ERROR! operand cant take more than one argument\n");
          //TODO handle error
          return;
        }
    }
  else
    {
      current_instruction.source_addressing = check_addressing_type(
        as_file, token, command, true, &current_instruction,constants);
      if (current_instruction.source_addressing == -1)
        {
          //TODO HANDLE ERROR
          printf("ERROR");
          return;
        }
      token = strtok(NULL, ", \n");
      if (dest && token == NULL)
        {
          //TODO HANDLE ERROR
          printf("ERROR");
          return;
        }
      current_instruction.destination_addressing = check_addressing_type(
        as_file, token, command, false, &current_instruction,constants);
      if (current_instruction.destination_addressing == -1)
        {
          //TODO HANDLE ERROR
          printf("ERROR");
          return;
        }
    }
}

int first_run(ASFile *current_as_file, Constants *constants)
{
  FILE *input_file, *output_file;
  struct table_item *item;
  char *temp = NULL;
  char *token = NULL;
  input_file = fopen(current_as_file->file_name_spaces, "r");
  if (input_file == NULL)
    {
      printf("Error opening file\n");
      return 1;
    }
  char line[LINE_LENGTH];
  int line_number = 0;
  while (fgets(line, LINE_LENGTH, input_file) != NULL)
    {
      if (temp)free(temp);
      temp = strdup(line);
      //TODO check label dec
      token = strtok(line, " \n");
      if (token == NULL)
        {
          //TODO HANDLE ERROR
          continue;
        }
      printf("token: %s\n", token);
      item = search_table(constants->op_code_table, token);
      //TODO instruction case
      if (item != NULL)
        {
          handle_instruction(temp, token, current_as_file, constants);
          //TODO HANDLE INSTRUCTION
          printf("found INSTRUCTION\n");
          continue;
        }
      if (strstr(line, LABEL_DEC))
        {
          //String/data case
          if (strstr(temp, STRING_DEC) || strstr(temp, DATA_DEC))
            {
              handle_line(current_as_file, temp);
            }
          if (strstr(temp,EXTERN_DEC))
            {
              continue;
              //TODO handle extern
            }
          if (strstr(temp,ENTRY_DEC))
            {
              continue;
              //TODO handle entry
            }
          //Checks if the rest is an instruction!
          token = strtok(NULL, " \n");

          item = search_table(constants->op_code_table, token);
          if (item != NULL)
            {
              //TODO HANDLE MACRO
              handle_instruction(temp, token, current_as_file, constants);
              continue;
            }
          printf("label token %s\n", token);
        }
    }
}
