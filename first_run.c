#include "Header Files/first_run.h"
#include "Header Files/hash_table.h"
#include "Header Files/linked_list.h"

int handle_entry_extern(as_file *current_as_file, char *line)
{
  /* type == true if entry, false if extern */
  char *symbol;
  table_item *item;
  int type = strstr(line,ENTRY_DEC) != NULL;
  symbol = strtok(NULL, " \r\n");
  item = search_table(current_as_file->symbol_table, symbol);
  if (item)
    {
      if (!type)
        {
          /* extern and exists already */
          printf(ERROR_DUPLICATE_SYMBOL_LABEL, symbol);
          current_as_file->is_valid = false;
          return false;
        }

      if (item->symbol->is_entry)
        {
          /*entry and exists already*/
          printf(ERROR_DUPLICATE_SYMBOL_LABEL, symbol);
          current_as_file->is_valid = false;
          return false;
        }
      item->symbol->is_entry = true;
      return true;
    }
  /*This will allow us to create entry and external files only if they exist!  */
  if (type)
    {
      if (current_as_file->file_name_entries == NULL)
        {
          current_as_file->file_name_entries = generate_file_name(
            current_as_file->file_name, ENT_FILE_ENDING);
        }
    }
  else
    {
      if (current_as_file->file_name_externals == NULL)
        {
          current_as_file->file_name_externals = generate_file_name(
            current_as_file->file_name, EXT_FILE_ENDING);
        }
    }
  insert_item_with_symbol(current_as_file->symbol_table, symbol,
                          type ? ENTRY_DEC : EXTERN_DEC,
                          0,
                          false, type, !type, false);
  return true;
}

void add_end_of_line(as_file *current_as_file, char *symbol_name)
{
  add_node(symbol_name, "END OF LINE", 0, &current_as_file->IC, false,
           current_as_file->lines);
}
int handle_data_inside(as_file *current_as_file, char *symbol_name, char
                        **token)
{
  int current_case, start = false, end = false, i =0;
  current_case = strcmp(*token, STRING_DEC) == 0;
  *token = strtok(NULL, " \t\r\n");
  /* String case */
  if (current_case)
    {
      while (strlen(*token) != i)
        {
          if (start && end)
            {
              /* there was still text after quotation marks  ' " ' */
              current_as_file->is_valid = false;
              printf(ERROR_INVALID_STRING_DECLARATION, *token);
            }
          else if (start && (*token)[i] == '"')
            {
              end = true;
            }
          else if ((*token)[i] == '"')
            {
              if (i != 0)
                {
                  /* there was text before quotation marks */
                  current_as_file->is_valid = false;
                  printf(ERROR_INVALID_STRING_DECLARATION, *token);
                  return false;
                }
              start = true;
            }
          else
            {
              add_node(symbol_name, *token, (*token)[i], &current_as_file->IC,
                       true,
                       current_as_file->lines);
              current_as_file->DC += 1;
            }
          i++;
        }
      if (!start)return false;
      add_end_of_line(current_as_file, symbol_name);
      current_as_file->DC += 1;
    }
  else
    {
      /*.data case*/
      *token = strtok(*token, ", \r\n");
      do
        {
          int number;
          if (!validate_number(*token, &number))
            {
              current_as_file->is_valid = false;
              printf(ERROR_INVALID_STRING_DECLARATION, *token);
              return false;
            }
          add_node(symbol_name, *token, number, &current_as_file->IC,true,
                   current_as_file->lines);
          current_as_file->DC += 1;
        }
      while ((*token = strtok(NULL, ", \r\n")));
    }
  return true;
}
int handle_symbol_line(as_file *current_as_file, char *current_line, int with_label)
{
  char *symbol_type, *symbol_name, *token;
  table_item *item;
  /* current_case is true if .string, false if .data*/
  symbol_name = NO_LABEL;
  token = strtok(current_line, " :\n\r");
  if (token == NULL)
    {
      current_as_file->is_valid = false;
      printf(ERROR_INVALID_LABEL, current_line);
      return false;
    }

  if (with_label == LABEL_ALONE)
    {
      symbol_name = token;
    }
  else if (with_label == LABEL_WITH_VARIABLE)
    {
      symbol_name = token;
      token = strtok(NULL, " \n\r");
      if (token == NULL)
        {
          return false;
        }
    }
  if (with_label != LABEL_WITHOUT_VARIABLE)
    {
      symbol_type = token;
      item = search_table(
        current_as_file->symbol_table, symbol_name);
      if (item)
        {
          if (item->symbol->is_entry)
            {
              /*TODO understand this*/
              item->symbol->is_data = true;
              item->symbol->location = current_as_file->IC;
            }
          else
            {
              current_as_file->is_valid = false;
              printf(ERROR_DUPLICATE_SYMBOL_LABEL, symbol_name);
              return false;
            }
        }
      else
        {
          insert_item_with_symbol(current_as_file->symbol_table, symbol_name,
                                  symbol_type,
                                  current_as_file->IC,
                                  true, false, false, false);
        }
      if (with_label == LABEL_ALONE)
        {
          return true;
        }
    }
  handle_data_inside(current_as_file, symbol_name, &token);

  return true;
}

int immediate_addressing(as_file *as_file, char *line,
                          table_item *command,int type)
{
  int number, address;
  if (type && command->inst_rule->source_addressing[0] !=
      IMMEDIATE_ADDRESSING_SIGN)
    {
      printf(ERROR_OPERAND_INVALID_IMMEDIATE_ADDRESSING, line);
      return false;
    }

  if (!type && command->inst_rule->dest_addressing[0] !=
      IMMEDIATE_ADDRESSING_SIGN)
    {
      printf(ERROR_OPERAND_INVALID_IMMEDIATE_ADDRESSING, line);
      return false;
    }
  /* Skips over '#' sign' , DONT DELETEEEEE*/
  line++;
  if (!validate_number(line, &number))
    {
      printf(ERROR_INVALID_IMMEDIATE_ADDRESSING, line);
      return false;
    }
  address = translate_address(number, 1, 0, 0);
  add_node("Immediate", line, address, &as_file->IC,true,
           as_file->lines);
  return true;
}

int direct_addressing(as_file *current_as_file, char *line,
                       table_item *command,
                       int type)
{
  if (type && strstr(command->inst_rule->source_addressing,
                     DIRECT_ADDRESSING_SIGN) == NULL)
    {
      printf(ERROR_OPERAND_INVALID_DIRECT_ADDRESSING, line);
      return -1;
    }
  if (!type && strstr(command->inst_rule->dest_addressing,
                      DIRECT_ADDRESSING_SIGN) == NULL)
    {
      printf(ERROR_OPERAND_INVALID_DIRECT_ADDRESSING, line);
      return -1;
    }
  add_node(line, "Direct",
           0,
           &current_as_file->IC,false, current_as_file->lines);
  return true;
}


int relative_addressing(as_file *current_as_file, char *line,
                         table_item *command,int type)
{
  if (type && strstr(command->inst_rule->source_addressing,
                     RELATIVE_ADDRESSING_SIGN) == NULL)
    {
      printf(ERROR_OPERAND_INVALID_RELATIVE_ADDRESSING, line);
      return false;
    }
  if (!type && strstr(command->inst_rule->dest_addressing,
                      RELATIVE_ADDRESSING_SIGN) == NULL)
    {
      printf(ERROR_OPERAND_INVALID_RELATIVE_ADDRESSING, line);
      return false;
    }
  /**important don't remove, gets rid of '&' sign*/
  line++;
  add_node(line, "Relative", 0,
           &current_as_file->IC,false, current_as_file->lines);

  return true;
}

int direct_register_addressing(char *line,
                               table_item *command,int type,
                               constants *globals)
{
  table_item *item;
  if (type && strstr(command->inst_rule->source_addressing,
                     REGISTER_ADDRESSING_SIGN) == NULL)
    {
      return -1;
    }
  if (!type && strstr(command->inst_rule->dest_addressing,
                      REGISTER_ADDRESSING_SIGN) == NULL)
    {
      return -1;
    }

  item = search_table(
    globals->registers_table, line);
  if (item)
    {
      return (int) strtol(item->value,NULL, 10);
    }
  return -1;
}

/**
* Checks if the addressing type is valid for the given command.
returns 0 if immediate, 1 if direct, 2 if relative, 3 if direct register
-1 if failed
*/
int check_addressing_type(as_file *current_as_file, char *line,
                          table_item
                          *operand, int type,
                          instruction *current_instruction,
                          constants *globals)
{
  /* type = true if source, false if dest */
  int reg;
  if (*line == IMMEDIATE_ADDRESSING_SYMBOL)
    {
      if (immediate_addressing(current_as_file, line, operand, type))
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
          return IMMEDIATE_ADDRESSING_SIGN_INT;
        }
      current_as_file->is_valid = false;
      return -1;
    }
  if (strstr(line, RELATIVE_ADDRESSING_SYMBOL))
    {
      if (relative_addressing(current_as_file, line, operand, type))
        return RELATIVE_ADDRESSING_SIGN_INT;
      current_as_file->is_valid = false;
      return -1;
    }
  reg = direct_register_addressing(line, operand, type,
                                   globals);
  if (reg != -1)
    {
      if (type)
        {
          current_instruction->source_reg = reg;
          current_instruction->source_addressing =
              REGISTER_ADDRESSING_SIGN_INT;
        }
      else
        {
          current_instruction->destination_reg = reg;
          current_instruction->destination_addressing =
              REGISTER_ADDRESSING_SIGN_INT;
        }
      return REGISTER_ADDRESSING_SIGN_INT;
    }
  if (direct_addressing(current_as_file, line, operand, type) == DIRECT_ADDRESSING_SIGN_INT)
    {
      return DIRECT_ADDRESSING_SIGN_INT;
    }
  printf(ERROR_INVALID_DIRECT_ADDRESSING, line);
  current_as_file->is_valid = false;
  return -1;
}

void initialize_instruction(instruction *current_instruction,
                            int op_code,
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

void handle_instruction(char *full_line, char *line, as_file *current_as_file,
                        constants *globals)
{
  instruction current_instruction;
   node *current_node;
  int source, dest;
  char *token;
  table_item *command = search_table(
    globals->op_code_table, line);
  if (command == NULL)
    {
      current_as_file->is_valid = false;
      printf(ERROR_INVALID_OP_CODE, line);
      return;
    }
  current_node = add_node(line, line, 0, &current_as_file->IC,true,
                          current_as_file->lines);
  initialize_instruction(&current_instruction,
                         command->inst_rule->op_code,
                         command->inst_rule->funct);
  source = strlen(command->inst_rule->source_addressing);
  dest = strlen(command->inst_rule->dest_addressing);
  token = strtok(NULL, ", \r\n");
  /*this is also equal to !dest && !source */
  if (strcmp(line, STOP_OP_CODE) == 0 || strcmp(line, RTS_OP_CODE) == 0)
    {
      if (token != NULL)
        {
          current_as_file->is_valid = false;
          printf(ERROR_OPERAND_WRONG_NUM_ARGS, full_line);
          return;
        }
      current_node->instruction = translate_instruction_address(
        &current_instruction);
      return;
    }
  if ((token == NULL && source) || (token == NULL && dest))
    {
      current_as_file->is_valid = false;
      printf(ERROR_OPERAND_WRONG_NUM_ARGS, full_line);
      return;
    }
  if (dest && !source)
    {
      current_instruction.destination_addressing =
          check_addressing_type(current_as_file, token, command, false,
                                &current_instruction, globals);
      token = strtok(NULL, ", \r\n");
      if (token != NULL)
        {
          current_as_file->is_valid = false;
          printf(ERROR_OPERAND_WRONG_NUM_ARGS, full_line);
          return;
        }
    }
  else
    {
      current_instruction.source_addressing = check_addressing_type(
        current_as_file, token, command, true, &current_instruction,
        globals);
      if (current_instruction.source_addressing == -1) return;
      token = strtok(NULL, ", \r\n");
      if (dest && token == NULL)
        {
          current_as_file->is_valid = false;
          printf(ERROR_OPERAND_WRONG_NUM_ARGS, full_line);
          return;
        }
      current_instruction.destination_addressing =
          check_addressing_type(
            current_as_file, token, command, false,
            &current_instruction, globals);
      if (current_instruction.destination_addressing == -1) return;
    }
  current_node->instruction = translate_instruction_address(
    &current_instruction);
}

void first_run(as_file *current_as_file, constants *globals)
{
  FILE *input_file;
  table_item *item;
  char line[LINE_LENGTH];
  char *temp = NULL, *token = NULL;
  input_file = fopen(current_as_file->file_name_macros, "r");
  if (input_file == NULL)
    {
      current_as_file->is_valid = false;
      printf(ERROR_INVALID_FILE, current_as_file->file_name_macros);
      return;
    }
  while (fgets(line, LINE_LENGTH, input_file) != NULL)
    {
      if (temp) free(temp);
      /* Need a copy of the line to use the line fully */
      temp = strdup(line);
      if (!temp)
        {
          printf(ERROR_MEM_FAILED);
          exit(EXIT_FAILURE);
        }
      token = strtok(line, " \r\n");
      if (token == NULL)
        {
          /* Empty line, we should never arrive here? */
          continue;
        }
      /* first command was an instruction! */
      item = search_table(globals->op_code_table, token);
      if (item != NULL)
        {
          handle_instruction(temp, token, current_as_file, globals);
          continue;
        }

      if (strstr(line, LABEL_DEC))
        {
          /* .data, String or Array case */
          if (strstr(temp, STRING_DEC) || strstr(temp, DATA_DEC))
            {
              if (!handle_symbol_line(current_as_file, temp, LABEL_WITH_VARIABLE))
                continue;
            }
          else
            {
              if (!handle_symbol_line(current_as_file, temp, LABEL_ALONE))
                continue;
            }
          /* Checks if the rest is an instruction with opcode*/
          token = strtok(NULL, " \r\n");
          item = search_table(globals->op_code_table, token);
          if (item != NULL)
            {
              handle_instruction(temp, token, current_as_file,
                                 globals);
            }
          continue;
        }
      /* .data or .string without label! */
      if (strstr(temp, STRING_DEC) || strstr(temp, DATA_DEC))
        {
          handle_symbol_line(current_as_file, temp, LABEL_WITHOUT_VARIABLE);
          continue;
        }
      /* Extern or entry case */
      if (strstr(temp,EXTERN_DEC) || strstr(temp,ENTRY_DEC))
        {
          handle_entry_extern(current_as_file, temp);
          continue;
        }

      printf(ERROR_INVALID_INSTRUCTION, temp);
      current_as_file->is_valid = false;
    }
  if (temp) free(temp);
  fclose(input_file);
  return;
}
