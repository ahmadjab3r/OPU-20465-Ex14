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

#endif
