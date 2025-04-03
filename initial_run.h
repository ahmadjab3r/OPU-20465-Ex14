#ifndef FINAL_PROJECT_INITIAL_RUN
#define FINAL_PROJECT_INITIAL_RUN


#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "skeleton.h"
#include "utilities.h"
#include "hashtable.h"



int initial_run(char *file_name, char *output_file_name,MacroTable *table);
int store_mcro(char *line, FILE *fp, struct MacroTable *table );
#endif
