#ifndef FINAL_PROJECT_SKELETON
#define FINAL_PROJECT_SKELETON


#include <stdio.h>
#include <stdbool.h>
#define LINE_LENGTH 80 /* Maybe add one more for /n doouble check that*/
#define IC_INITIAL 100
#define DC_INITIAL 0
#define MCRO "mcro"
#define ENDMCRO "endmcro"
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
#endif
