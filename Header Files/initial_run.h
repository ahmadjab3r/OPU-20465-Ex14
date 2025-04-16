#ifndef FINAL_PROJECT_INITIAL_RUN
#define FINAL_PROJECT_INITIAL_RUN

#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "skeleton.h"
#include "utilities.h"

/**
 * Reads an assembly file and creates a new file with the mcros unpacked
 * @param current_as_file a struct with the current assembly file details
 */
void initial_run(as_file *current_as_file);

/**
 * Gets a start of a macro declaration and saves the content of the macro
 * in a hashtable
 * @param line current line in the file
 * @param fp the current file
 * @param current_as_file the table to store macros in
 * @return failure of any sort
 */
int store_mcro(as_file *current_as_file, char* current_line, FILE *fp);

/**
 * This function writes the content of the file without spaces
 * to a new file after finding all mcros and unpacking them in their
 * correct positions
 * @param current_as_file
 */
int write_mcro_file(as_file *current_as_file);

#endif
