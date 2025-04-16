#ifndef FINAL_PROJECT_UTILITIES
#define FINAL_PROJECT_UTILITIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "skeleton.h"

/**
 * @param file_name current file name
 * @param ending new file ending
 * @return new file name with the new ending
 */
char *generate_file_name(char *file_name, char* ending);
/**
 * This function removes all extra unnecessary white spaces from the file
 * @param file_name string of the input file name
 * @return string the name of the new file after white spaces removed
 */
char *remove_spaces(char *file_name, char *output_file_name);

/**
 * gets a continuation of a macro and returns the old and the new one compacted
 * @param macro_content current saved macro content
 * @param current_line
 * @return
 */
char *copy_line(char *macro_content, char *current_line);

/**
 * initializes the opcode table
 * @return
 */
hash_table* initialize_op_code_table(void);


char *remove_extra_spaces_file(char file_name[]);

int validate_number( char *str, int *number);

constants *initialize_constants(void);
int translate_address(int value, int A, int R, int E);
int translate_instruction_address(instruction *instruction);

void print_hash_table(hash_table *table);
void print_binary(int value);
void print_lines(linked_list *lines);
/* int testing_hash_table(void) */

#endif
