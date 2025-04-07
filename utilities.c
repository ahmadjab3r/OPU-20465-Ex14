#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void *handle_malloc(long object_size) {
    void *object_ptr = malloc(object_size);
    if (object_ptr == NULL) {
    }
    return object_ptr;
}


char *add_new_file(char *file_name, char *ending) {
    char *c, *new_file_name;
    new_file_name = handle_malloc(LINE_LENGTH * sizeof(char));
    strcpy(new_file_name, file_name);
    /* deleting the file name if a '.' exists and forth */
    if ((c = strchr(new_file_name, '.')) != NULL) {
        *c = '\0';
    }
    /* adds the ending of the new file name */
    strcat(new_file_name, ending);
    return new_file_name;
}

int copy_file(char *file_name_dest, char *file_name_orig) {
    char str[LINE_LENGTH];
    FILE *fp, *fp_dest;
    fp = fopen(file_name_orig, "r");
    if (fp == NULL) {
        return 0;
    }
    fp_dest = fopen(file_name_dest, "w");
    if (fp_dest == NULL) {
        fclose(fp);
        return 0;
    }
    while (fgets(str, LINE_LENGTH, fp) != NULL) {
        fprintf(fp_dest, "%s", str);
    }
    fclose(fp);
    fclose(fp_dest);
    return 1;
}

void abrupt_close(int num_args, ...) {
    int i;
    char *str;
    FILE *fp;
    va_list args;
    va_start(args, num_args);
    for (i = 0; i < num_args; i++) {
        /* next argument is a string whose allocated memory needs to be freed*/
        if (strcmp(va_arg(args, char*), "%s") == 0) {
            i++;
            str = va_arg(args, char*);
            remove(str);
            free(str);
        }
        /* next argument is a file pointer that needs to be closed */
        else {
            fp = va_arg(args, FILE*);
            fclose(fp);
        }
    }
    va_end(args);
}


void allocate_new_file_name(char *file_name, char **edited_file_name,
                            int additional_size,
                            int removal_size, char *ending) {
    *edited_file_name = malloc(strlen(file_name) + additional_size);
    if (*edited_file_name == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    *edited_file_name = strcpy(*edited_file_name, file_name);
    (*edited_file_name)[strlen(*edited_file_name) - removal_size] =
            '\0'; //TODO
    strcat(*edited_file_name, ending);
}

char *generate_file_name(char *file_name, int stage) {
    char *edited_file_name;
    /* First stage, the current file name ends in .as */
    if (stage == PRE_MACRO_STAGE) {
        allocate_new_file_name(file_name, &edited_file_name, 2, 3,
                               PREC_FILE_ENDING);
    }
    /* Second stage, the current file name ends in .prec */
    if (stage == MACRO_STAGE) {
        allocate_new_file_name(file_name, &edited_file_name, 1, 5,
                               MCRO_FILE_ENDING);
    }

    return edited_file_name;
}

int is_space_or_tab(char c) {
    /* Check if the char is a space or a tab */
    return (isspace(c) && c != '\n');
}

void remove_spaces_next_to_comma(char *str) {
    char *ptr = str;
    /* If the line starts with ',' avoiding accessing outside the str */
    if (*ptr == ',') {
        return;
    }
    while ((ptr = strchr(ptr, ',')) != NULL) {
        /* Space before the comma */
        if (*(ptr - 1) == ' ') {
            memmove(ptr - 1, ptr, strlen(ptr) + 1);
            if (*(ptr) == ' ') {
                /* Also space after the comma */
                memmove(ptr, ptr + 1, strlen(ptr + 1) + 1);
            }
        } else if (*(ptr + 1) == ' ') {
            /* Only space after the comma */
            memmove(ptr + 1, ptr + 2, strlen(ptr + 2) + 1);
            ptr++;
        } else {
            ptr++;
        }
    }
}

/**
 * This function removes all extra unnecessary white spaces from the file
 * @param file_name string of the input file name
 * @return string the name of the new file after white spaces removed
 */
char *remove_spaces(char *file_name, char *output_file_name) {
    FILE *input_file = fopen(file_name, "r");
    if (input_file == NULL) {
        //TODO HANDLE ERROR
        return NULL;
    }

    FILE *output_file = fopen(output_file_name, "w");
    if (output_file == NULL) {
        perror("Error creating file");
        fclose(input_file);
        free(output_file_name);
        return NULL;
    }

    int c;
    int last_char = ' ';
    while ((c = fgetc(input_file)) != EOF) {
        if (!isspace(c) || (isspace(c) && !isspace(last_char))) {
            fputc(c, output_file);
        }
        last_char = c;
    }

    fclose(input_file);
    fclose(output_file);

    return output_file_name;
}

void remove_extra_spaces_str(char str[]) {
    /* i for original string, j for modified string */
    int i, j;
    char str_temp[LINE_LENGTH];
    i = j = 0;
    /* eliminating white-spaces in the beginning of the line */
    while (is_space_or_tab(*(str + i))) {
        i++;
    }
    while (*(str + i) != '\0') {
        /* copying character */
        while (!is_space_or_tab(*(str + i)) && *(str + i) != '\0') {
            *(str_temp + j) = *(str + i);
            i++;
            j++;
        }
        /* if loop stopped because end of line char */
        if (*(str + i) == '\0') {
            break;
        }
        /* if loop stopped because of a white-space skipping them until another character is encountered*/
        while (is_space_or_tab(*(str + i))) {
            i++;
        }
        /* if stopped not because of end of line char then copy one space for all the others that were skipped */
        if (!(*(str + i) == '\n' || *(str + i) == '\0')) {
            *(str_temp + j) = ' ';
            j++;
        }
    }
    *(str_temp + j) = *(str + i);
    *(str_temp + j + 1) = '\0';
    remove_spaces_next_to_comma(str_temp);
    strcpy(str, str_temp);
}

typedef struct location {
    char *file_name;
    int line_num;
} location;


char *remove_extra_spaces_file(char file_name[]) {
    char *new_file_name;
    char str[LINE_LENGTH];
    int line_num;
    FILE *fp, *fp_temp;
    /* opening input file for reading */
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        // print_internal_error(ERROR_CODE_2);
        return NULL;
    }
    /* saving new name for a temp file */
    new_file_name = add_new_file(file_name, ".t01");
    if (new_file_name == NULL) {
        abrupt_close(2, "file", fp);
        return NULL;
    }
    /* opening new file for writing */
    fp_temp = fopen(new_file_name, "w");
    if (fp_temp == NULL) {
        abrupt_close(4, "file", fp, "%s", new_file_name);
        return NULL;
    }
    /* reading each line of the input file and removing extra unnecessary white-spaces */
    line_num = 0;
    while (fgets(str, 999, fp) != NULL) {
        line_num++;
        if (strlen(str) > LINE_LENGTH) {
            location as_file;
            as_file.file_name = file_name;
            as_file.line_num = line_num;
            fclose(fp);
            fclose(fp_temp);
            return NULL;
        }
        /* replacing a comment line with newline character */
        else if (*str == ';') {
            *str = '\n';
            *(str + 1) = '\0';
        } else {
            /* removing extra unnecessary white-spaces from the line */
            remove_extra_spaces_str(str);
        }
        /* saving the changed line to the new file */
        fprintf(fp_temp, "%s", str);
    }
    fclose(fp);
    fclose(fp_temp);
    return new_file_name;
}

char *copy_text(FILE *fp, fpos_t *pos, int length) {
    /* the function assumes that pos + length < end. this was checked by save_mcro_content function */
    int i;
    char *str;
    if (fsetpos(fp, pos) != 0) {
        printf("fsetpos in copy_text failed\n");
        return NULL;
    }
    str = handle_malloc((length + 1) * sizeof(char));
    for (i = 0; i < length; i++) {
        *(str + i) = getc(fp);
    }
    *(str + i) = '\0';
    fgetpos(fp, pos);
    return str;
}

char *copy_line(char *macro_content, char *current_line) {
    int length;
    if (macro_content == NULL) {
        macro_content = malloc(strlen(current_line) + 1);
        if (macro_content == NULL) {
            //TODO HANDLE ERROR
            return NULL;
        }
        strcpy(macro_content, current_line);
        return macro_content;
    }
    length = strlen(macro_content) + strlen(current_line) + 1;
    macro_content = realloc(macro_content, length);
    if (macro_content == NULL) {
        //TODO HANDLE ERROR
        return NULL;
    }
    strcat(macro_content, current_line);
    return macro_content;
}

table *initialize_op_code_table(void) {
    table *op_code_table = create_table();
    if (op_code_table == NULL) {
        //TODO HANDLE ERROR
        return NULL;
    }
    insert_macro_with_instruction(op_code_table, "mov", "0", 0, "013",
                                  "13");
    insert_macro_with_instruction(op_code_table, "cmp", "1", 0, "013",
                                  "013");
    insert_macro_with_instruction(op_code_table, "add", "2", 1, "013",
                                  "13");
    insert_macro_with_instruction(op_code_table, "sub", "2", 2, "013",
                                  "13");
    insert_macro_with_instruction(op_code_table, "lea", "4", 0, "1",
                                  "13");
    insert_macro_with_instruction(op_code_table, "clr", "5", 1, "",
                                  "13");
    insert_macro_with_instruction(op_code_table, "not", "5", 2, "",
                                  "13");
    insert_macro_with_instruction(op_code_table, "inc", "5", 3, "",
                                  "13");
    insert_macro_with_instruction(op_code_table, "dec", "5", 4, "",
                                  "13");
    insert_macro_with_instruction(op_code_table, "jmp", "9", 1, "",
                                  "12");
    insert_macro_with_instruction(op_code_table, "bne", "9", 2, "",
                                  "12");
    insert_macro_with_instruction(op_code_table, "jsr", "9", 3, "",
                                  "12");
    insert_macro_with_instruction(op_code_table, "red", "12", 0, "",
                                  "13");
    insert_macro_with_instruction(op_code_table, "prn", "13", 0, "",
                                  "013");
    insert_macro_with_instruction(op_code_table, "rts", "14", 0, "",
                                  "");
    insert_macro_with_instruction(op_code_table, "stop", "15", 0, "",
                                  "");
    return op_code_table;
}

table *initialize_registers_table(void) {
    table *registers_table = create_table();
    if (registers_table == NULL) {
        //TODO HANDLE ERROR
        return NULL;
    }
    insert_macro(registers_table, "r0", "0");
    insert_macro(registers_table, "r1", "1");
    insert_macro(registers_table, "r2", "2");
    insert_macro(registers_table, "r3", "3");
    insert_macro(registers_table, "r4", "4");
    insert_macro(registers_table, "r5", "5");
    insert_macro(registers_table, "r6", "6");
    insert_macro(registers_table, "r7", "7");
    return registers_table;
}

void print_hash_table(table *table) {
    for (int i = 0; i < table->size; i++) {
        if (table->macros[i] != NULL) {
            printf("Index %d, ", i);
            printf("Key: %s Value: %s\n", table->macros[i]->key,
                   table->macros[i]->value);
            if (table->macros[i]->next != NULL) {
                struct table_item *current_macro =
                        table->macros[i]->next;
                while (current_macro != NULL) {
                    printf("          Key: %s Value: %s\n",
                           current_macro->key,
                           current_macro->value);
                    current_macro = current_macro->next;
                }
            }
        }
    }
}

Constants *initialize_constants(void) {
    Constants *constants = malloc(sizeof(Constants));
    if (constants == NULL) {
        //TODO HANDLE ERROR
        return NULL;
    }
    constants->op_code_table = initialize_op_code_table();
    if (constants->op_code_table == NULL) {
        //TODO HANDLE ERROR
        free(constants);
        return NULL;
    }
    constants->registers_table = initialize_registers_table();
    if (constants->registers_table == NULL) {
        //TODO HANDLE ERROR
        free(constants->op_code_table);
        free(constants);
        return NULL;
    }
    return constants;
}

int validate_number(char *str) {
    //with negative sign
    int i;
    char *letter = str;
    i = 0;
    while (*letter != '\0') {
        if (*letter != '-' && !isdigit(*letter)) {
            //TODO handle ERROR
            printf("ERRROR");
        }
        if (*letter == '-' && i != 0) {
            //TODO handle error!
        }
        printf("token: %c\n", *letter);
        letter++;
        i++;
    }

    int number = strtol(str,NULL, 10);
    if (strstr('-', str) != NULL) {
        number = -number;
    }
    return number;
}
