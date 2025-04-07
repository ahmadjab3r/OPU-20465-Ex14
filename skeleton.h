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
#define LABEL_DEC ":"
#define MCRO "mcro"
#define ENDMCRO "endmcro"
#define INSTRUCTION_NUMBER_SYMBOL '#'
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
  int address_source;
  int source_reg;
  int address_dest;
  int destination_reg;
  int funct;
  int A;
  int R;
  int E;
};
typedef struct instructions {

};
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
