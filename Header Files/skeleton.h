#ifndef FINAL_PROJECT_SKELETON
#define FINAL_PROJECT_SKELETON

#include "hash_table.h"
#include "linked_list.h"


/* #############     MESSAGES        #################*/
#define PROCESSING_FILE "Processing file: %s\n"
#define ENTRY_EXTERNAL_PRINT_FORMAT "%s %d\n"
#define PRINT_FORMAT "%07d %06x\n"

/* #############     MESSAGES END        #################*/

 /* #############     ERRORS        #################*/
#define ERROR_INVALID_FILE "ERROR: Failed to open file %s\n"
#define ERROR_MEM_FAILED "ERROR: Failed to allocate memory\n"
#define ERROR_INVALID_MACRO "ERROR: Invalid Mcro definition\n"
#define ERROR_INVALID_INSTRUCTION "ERROR: Invalid instruction/command: %s\n"
#define ERROR_INVALID_LABEL "ERROR: Invalid Label declaration: %s\n"
#define ERROR_DUPLICATE_SYMBOL_LABEL "ERROR: Duplicate Symbol/Label: %s\n"
#define ERROR_INVALID_OP_CODE "ERROR: Invalid Op Code: %s\n"
#define ERROR_INVALID_STRING_DECLARATION "ERROR: Invalid String declaration: %s\n"
#define ERROR_INVALID_INT_DECLARATION "ERROR: Invalid Integer declaration: %s\n"
#define ERROR_OPERAND_WRONG_NUM_ARGS "ERROR: Wrong amount of args for Operand: %s"
#define ERROR_INVALID_IMMEDIATE_ADDRESSING "ERROR: Invalid Immediate addressing: %s\n"
#define ERROR_OPERAND_INVALID_IMMEDIATE_ADDRESSING "ERROR: Operand doesn't support Immediate Addressing %s\n"
#define ERROR_INVALID_RELATIVE_ADDRESSING "ERROR: Invalid Relative addressing: %s\n"
#define ERROR_OPERAND_INVALID_RELATIVE_ADDRESSING "ERROR: Operand doesn't support Relative Addressing %s\n"
#define ERROR_INVALID_REGISTER "ERROR: Invalid Register: %s\n"
#define ERROR_OPERAND_INVALID_REGISTER_ADDRESSING "ERROR: Operand doesn't support Register Addressing %s\n"
#define ERROR_INVALID_DIRECT_ADDRESSING "ERROR: Invalid Direct addressing: %s\n"
#define ERROR_OPERAND_INVALID_DIRECT_ADDRESSING "ERROR: Operand doesn't support Direct Addressing %s\n"

#define SYMBOL_NOT_FOUND "SECOND PHASE ERROR: Symbol %s not found\n"
/* #############     ERRORS END     #################*/


/* #############  ASFILE   #################*/
#define LINE_LENGTH 80 /* Maybe add one more for /n doouble check that*/
#define IC_INITIAL 100
#define DC_INITIAL 0
#define AS_FILE_ENDING ".as"
#define REMOVED_SPACES_FILE_ENDING ".prec"
#define MACRO_FILE_ENDING ".am"
#define OB_FILE_ENDING ".ob"
#define ENT_FILE_ENDING ".ent"
#define EXT_FILE_ENDING ".ext"
/* ############# ASFILE INITIALS END #################*/


/* ############# TYPES OF LABELS/DATA #################*/
#define STRING_DEC ".string"
#define DATA_DEC ".data"
#define EXTERN_DEC ".extern"
#define ENTRY_DEC ".entry"

#define LABEL_DEC ":"

#define LABEL_WITHOUT_VARIABLE 0
#define LABEL_WITH_VARIABLE 1
#define LABEL_ALONE 2
#define NO_LABEL "NO LABEL"
/* ############# TYPES OF LABELS/DATA END #################*/

/* ############# INSTRUCTION TYPES #################*/
#define IMMEDIATE_ADDRESSING_SIGN '0'
#define DIRECT_ADDRESSING_SIGN "1"
#define RELATIVE_ADDRESSING_SIGN "2"
#define REGISTER_ADDRESSING_SIGN "3"
#define IMMEDIATE_ADDRESSING_SIGN_INT 0
#define DIRECT_ADDRESSING_SIGN_INT 1
#define RELATIVE_ADDRESSING_SIGN_INT 2
#define REGISTER_ADDRESSING_SIGN_INT 3

#define DIRECT_ADDRESSING_WORD "Direct"
#define RELATIVE_ADDRESSING_WORD "Relative"

#define IMMEDIATE_ADDRESSING_SYMBOL '#'
#define RELATIVE_ADDRESSING_SYMBOL "&"
/* ############# INSTRUCTION TYPES END #################*/



/* #############     MCRO   #################*/
#define MCRO "mcro"
#define ENDMCRO "mcroend"
/* #############   MCRO END  #################*/

#define STOP_OP_CODE "stop"
#define RTS_OP_CODE "rts"
/* #############   opcode END  #################*/

#define true 1
#define false 0

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

typedef struct as_file {
    char *file_name; /* The name of the assembly file */
    char *file_name_spaces;
    char *file_name_macros;
    char *file_name_ob;
    char *file_name_entries;
    char *file_name_externals;
    linked_list *lines;
    hash_table *symbol_table;
    hash_table *macro_table;
    int IC;
    int DC;
    int is_valid;
}as_file;
typedef struct constants {
        hash_table *op_code_table;
        hash_table *registers_table;
} constants;

#endif
