#ifndef FINAL_PROJECT_UTILITIES
#define FINAL_PROJECT_UTILITIES

#include "stdlib.h"
#include "stdio.h"
#include "skeleton.h"
#include <ctype.h>
#include <string.h>

char *generate_file_name(char *file_name, int stage);
/**
 * This function removes all extra unnecessary white spaces from the file
 * @param file_name string of the input file name
 * @return string the name of the new file after white spaces removed
 */
char *remove_spaces(char *file_name, char *output_file_name);
void allocate_new_file_name(char *file_name, char **edited_file_name,
                       int additional_size,
                       int removal_size,char *ending);
char *copy_text(FILE *fp, fpos_t *pos, int length);
char *copy_line(char *macro_content, char *current_line);

table* initialize_op_code_table(void);

void print_hash_table(table *table);

char *remove_extra_spaces_file(char file_name[]);

bool validate_number( char *str, int *number);

Constants *initialize_constants(void);

#endif
