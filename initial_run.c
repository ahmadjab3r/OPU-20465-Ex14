#include "initial_run.h"





int initial_run(char *file_name, char *output_file_name){
  char current_line[LINE_LENGTH];
  char *new_file_name;
  FILE *file;
  //TODO check file name ?
  file = fopen(file_name, "r");
  if (file == NULL){
    //TODO HANDLE ERROR
    return -1;
  }

  new_file_name = generate_file_name(file_name, MACRO_STAGE);
  while(fgets(current_line, LINE_LENGTH, file) != NULL){
       printf("%s", current_line);
      if (strstr(current_line, MCRO) != NULL){
          printf("mcro found");
          // store_mcro();
      }
  }
  free(new_file_name);
  return 0;





  }