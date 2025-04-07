#include "first_run.h"

bool handle_line(char *str, int *line_number, table *data_table) {
    bool start = false, end = false;
    /* current_case is true if .string, false if .data*/
    bool current_case;
    char *token = strtok(str, " :\n");
    if (token == NULL) {
        return false;
    }
    char *name = token;
    token = strtok(NULL, " \n");
    if (token == NULL) {
        return false;
    }
    char *type = token;
    current_case = strcmp(token, STRING_DEC) == 0;
    token = strtok(NULL, "\n");
    char *c = token;
    /* String case */

    if (current_case) {
        while (strlen(c) != 0) {
            if (start && end) {
                //TODO HANDLE ERROR!
                printf("ERROR");
            } else if (start && *c == '"') {
                end = true;
            } else if (*c == '"') {
                start = true;
            } else {
                printf("%c", *c);
            }
            //TODO handle character
            c++;
            line_number++;

        }
        //.data case
    } else {
        token = strtok(token, ", ");
        do {
            int number = validate_number(token);
            //todo handle number
            line_number++;

        } while ((token = strtok(NULL, ", ")));
    }

    return true;
}
// First
bool immediate_addressing(char *line, struct table_item *command ) {


    return true;
}

//Second
bool direct_addressing(char *line);
//Third

bool relative_addressing(char *line); //TODO change name
//Fourth
bool direct_register_addressing(char *line);


void handle_instruction(char * line, ASFile * as_file,
                       Constants * constants) { //TODO change return
    printf("this is line! %s\n", line);
    struct table_item *command = search_macro(constants->op_code_table, line);
    if (command == NULL) {
        //TODO HANDLE ERROR
        printf("ERROR");
        return;
    }
    bool source = strlen(command->inst_rule->source_addressing);
    bool dest = strlen(command->inst_rule->dest_addressing);
    char *token = strtok(NULL, ", \n");
    if(strcmp(line, "stop") ==0  || strcmp(line,"rts") == 0 ) {
        if(token != NULL) {
            //TODO handle error!
        }

    }
    if (token == NULL && source) {
        //TODO HANDLE ERROR
        printf("ERROR");
        return;
    }
    if(token && *token == INSTRUCTION_NUMBER_SYMBOL) {
        if(!immediate_addressing(token, command)) {
            //TODO HANDLE ERROR
            printf("ERROR");
            return;
        }
    }
    token = strtok(NULL, ", \n");
    if (token == NULL) {
        //TODO HANDLE ERROR
        printf("ERROR");
        return;
    }





    //Instruction cases

}



int first_run(ASFile* current_as_file,Constants *constants) {
    FILE *input_file, *output_file;
    struct table_item *item;
    char *token= NULL;
    input_file = fopen(current_as_file->file_name_spaces, "r");
    if (input_file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    char line[LINE_LENGTH];
    int line_number = 0;
    while (fgets(line, LINE_LENGTH, input_file) != NULL) {
        //TODO check label dec
        token = strtok(line," \n");
        if (token == NULL) {
            //TODO HANDLE ERROR
            continue;
        }
        printf("token: %s\n", token);
        item = search_macro(constants->op_code_table, token);
        //TODO instruction case
        if(item != NULL) {
            //TODO HANDLE MACRO
            printf("found macro\n");
            continue;
        }
        if (strstr(line, LABEL_DEC)) {
            //String/data case
            if (strstr(line, STRING_DEC) || strstr(line, DATA_DEC)) {
                handle_line(line, &line_number, current_as_file->data_table);
            }
            //Checks if the rest is an instruction!
            token = strtok(NULL, " \n");
            item = search_macro(constants->op_code_table, token);
            if(item != NULL) {
                //TODO HANDLE MACRO
                printf("found macro\n");
                handle_instruction(token, current_as_file, constants );
                continue;
            }
            printf("label token %s\n", token);

        }
    }
}
