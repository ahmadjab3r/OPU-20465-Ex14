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
            char * test = token;
            char *letter = test;
            while(*letter!='\0') {
                if (*letter != '-' && !isdigit(*letter)) {
                    //TODO handle ERROR
                    printf("ERRROR");

                }
                    printf("token: %c\n", *letter);
                    letter++;
            }
            int number = atoi(token);
            //todo handle number
            line_number++;

        } while ((token = strtok(NULL, ", ")));
    }

    return true;
}

int first_run(char *file_name, char *output_file_name,
              table *symbol_table, table *data_table,Constants *constants) {
    FILE *input_file, *output_file;
    struct table_item *item;
    char *token= NULL;
    int IC = IC_INITIAL, DC = DC_INITIAL;

    input_file = fopen(file_name, "r");
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
                handle_line(line, &line_number, data_table);
            }
            //Checks if the rest is an instruction!
            token = strtok(NULL, " \n");
            item = search_macro(constants->op_code_table, token);
            if(item != NULL) {
                //TODO HANDLE MACRO
                printf("found macro\n");
                continue;
            }
            printf("label token %s\n", token);

        }
    }
}
