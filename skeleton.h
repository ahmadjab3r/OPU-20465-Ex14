#ifndef FINAL_PROJECT_SKELETON
#define FINAL_PROJECT_SKELETON


#include <stdio.h>
#include <stdbool.h>

#include "hashtable.h"
#include "linked_list.h"

#define LINE_LENGTH 80 /* Maybe add one more for /n doouble check that*/
#define IC_INITIAL 100
#define DC_INITIAL 0
#define STRING_DEC ".string"
#define DATA_DEC ".data"
#define EXTERN_DEC ".extern"
#define ENTRY_DEC ".entry"
#define IMMEDIATE_ADDRESSING_SIGN '0'
#define DIRECT_ADDRESSING_SIGN '1'
#define RELATIVE_ADDRESSING_SIGN '2'
#define REGISTER_ADDRESSING_SIGN '3'

#define IMMEDIATE_ADDRESSING_SYMBOL '#'
#define RELATIVE_ADDRESSING_SYMBOL "&"
#define LABEL_DEC ":"
#define MCRO "mcro"
#define ENDMCRO "mcroend"

#define PREC_FILE_ENDING ".prec"
#define MCRO_FILE_ENDING ".mcro"
#define AS_FILE_ENDING ".as"
#define OB_FILE_ENDING ".ob"
enum STAGE{
  PRE_MACRO_STAGE,
  MACRO_STAGE,
  LABEL_STAGE,
  SECOND_STAGE,

};
typedef struct instruction{
  int op_code;
  int source_addressing;
  int source_reg;
  int destination_addressing;
  int destination_reg;
  int funct;
  int A;
  int R;
  int E;
}instruction;
typedef struct ASFile {
    char *file_name; /* The name of the assembly file */
    char *file_name_spaces;
    char *file_name_macros;
    char *file_name_ob;
    LinkedList *lines;
    table *symbol_table;
    table *data_table;
    table *macro_table;
    int IC;
    int DC;
}ASFile;
typedef struct Constants {
        table *op_code_table;
        table *registers_table;
} Constants;

#endif
