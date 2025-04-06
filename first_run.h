#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skeleton.h"
#include "utilities.h"

int first_run( char *file_name, char *output_file_name,  table *symbol_table,table *data_table,Constants *constants);
bool handle_line(char * str, int *line_number, table * data_table) ;

