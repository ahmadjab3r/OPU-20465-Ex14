#ifndef FINAL_PROJECT_INITIAL_RUN
#define FINAL_PROJECT_INITIAL_RUN


#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "skeleton.h"
#include "utilities.h"


int initial_run(ASFile *current_as_file);

int store_mcro(char *line, FILE *fp,  table *macro_table);

bool handle_data(char *str, int line_number, table *table);
#endif
