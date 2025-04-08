#include "first_run.h"

bool handle_line(char *str, int *line_number, table *data_table)
{
  bool start = false, end = false;
  /* current_case is true if .string, false if .data*/
  bool current_case;
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
  struct table_item *item = search_table(data_table, symbol_name);
  if (item){
      if (item->symbol->is_entry)
        {
          item->symbol->is_data = true;
          // item->value = itoa(line_number) ;
        } else
          {
            //TODO handle Error!
          }


    }

  current_case = strcmp(token, STRING_DEC) == 0;
  token = strtok(NULL, "\n");
  char *c = token;
  /* String case */
  int i =0;
  if (current_case)
    {
      while (strlen(c) != i)
        {
          if (start && end)
            {
              //TODO HANDLE ERROR!
              printf("ERROR");
            }
          else if (start && c[i] == '"')
            {
              end = true;
            }
          else if (c[i] == '"')
            {
              if (i != 0)
                {
                  //TODO handle error! ' " ' was found way later!
                }
              start = true;
            }
          else
            {
              printf("%c", c[i]);
            }
          //TODO handle character
          i++;
          line_number++;
        }
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
          line_number++;
        }
      while ((token = strtok(NULL, ", ")));
    }

  return true;
}
// First
//
bool immediate_addressing(ASFile *as_file,char *line, struct table_item *command,bool type)
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
  line++;
  int number;
  if (validate_number(line, &number))
    {
      //TODO handle error
      return false;
    }
  //TODO write to file

  return true;
}

//Second
bool direct_addressing(ASFile *as_file,char *line, struct table_item *command,bool type)
{
return true;
}
//Third

bool relative_addressing(ASFile *as_file,char *line, struct table_item *command,bool type)
{
  if (type && command->inst_rule->source_addressing[0] !=
      RELATIVE_ADDRESSING_SIGN)
    {
      //TODO HANDLE ERROR
      return false;
    }
  if (!type && command->inst_rule->dest_addressing[0] !=
      RELATIVE_ADDRESSING_SIGN)
    {
      //TODO HANDLE ERROR
      return false;
    }
  //TODO if external return false!
  return true;

}
//Fourth
bool direct_register_addressing(char *line);
/**
* Checks if the addressing type is valid for the given command.
returns 0 if immediate, 1 if direct, 2 if relative, 3 if direct register
-1 if failed
*/
int check_addressing_type(ASFile *as_file, char *line, struct table_item
                          *command, bool type, instruction *current_instruction)
{
  /* type = true if source, false if dest */
  if (*line == IMMEDIATE_ADDRESSING_SYMBOL)
    {
      printf("hi");
      return immediate_addressing(as_file,line, command, type)? 0 : -1;
    }
  if (direct_addressing(as_file,line, command, type))
    {

    }
  if (strstr(line, RELATIVE_ADDRESSING_SYMBOL))
    {
      return relative_addressing(as_file,line,command,type)? 2: -1;
    }
}

void initialize_instruction(instruction *current_instruction, int op_code, int funct)
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
  initialize_instruction(&current_instruction,command->inst_rule->op_code,
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
      current_instruction.destination_addressing = check_addressing_type(as_file,
        token, command, false, &current_instruction);
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
        as_file, token, command, true,  &current_instruction);
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
        as_file, token, command, false , &current_instruction);
      if (current_instruction.destination_addressing == -1)
        {
          //TODO HANDLE ERROR
          printf("ERROR");
          return;
        }
    }
  // save_instruction(command->inst_rule->op_code, command->inst_rule->funct,
  //                  source_addressing_type, dest_addressing_type,
  //                  command->inst_rule->source_addressing,
  //                  command->inst_rule->dest_addressing, A, R, E, as_file);
  // Instruction cases
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
          //TODO HANDLE INSTRUCTION
          printf("found INSTRUCTION\n");
          continue;
        }
      if (strstr(line, LABEL_DEC))
        {
          //String/data case
          if (strstr(temp, STRING_DEC) || strstr(temp, DATA_DEC))
            {
              handle_line(temp, &line_number, current_as_file->data_table);
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
