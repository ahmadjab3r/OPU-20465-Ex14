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
/**
 * This function removes all extra unnecessary white spaces from the file
 * @param file_name string of the input file name
 * @return string the name of the new file after white spaces removed
 */
int is_trim_space(int c) {
    return c == ' ' || c == '\t' || c == '\r';
}

char *remove_spaces(char *file_name, char *output_file_name) {
    int c, next;
    FILE *input_file = fopen(file_name, "r");
    if (input_file == NULL) {
            // TODO HANDLE ERROR
            return NULL;
    }

    FILE *output_file = fopen(output_file_name, "w");
    if (output_file == NULL) {
            perror("Error creating file");
            fclose(input_file);
            free(output_file_name);
            return NULL;
    }


    while ((c = fgetc(input_file)) != EOF) {
            // If current char is space-like, peek ahead to see if it's around a comma
            if (is_trim_space(c)) {
                    long curr_pos = ftell(input_file);

                    // Look ahead for next non-space character
                    while ((next = fgetc(input_file)) != EOF && is_trim_space(next));

                    if (next == ',') {
                            // Skip this space — it's before a comma
                            // Write the comma instead
                            fputc(',', output_file);

                            // Skip spaces after the comma
                            while ((next = fgetc(input_file)) != EOF && is_trim_space(next));

                            if (next != EOF) {
                                    fputc(next, output_file);
                            }
                    } else {
                            // Not around a comma — preserve the space
                            fputc(c, output_file);
                            fseek(input_file, curr_pos, SEEK_SET); // reset to where we left off
                    }
            } else if (c == ',') {
                    // Check for spaces after comma
                    fputc(',', output_file);
                    while ((next = fgetc(input_file)) != EOF && is_trim_space(next));
                    if (next != EOF) {
                            fputc(next, output_file);
                    }
            } else {
                    // Normal character
                    fputc(c, output_file);
            }
    }

    fclose(input_file);
    fclose(output_file);
    return output_file_name;
}
char *generate_file_name(char *file_name, char *ending) {
    /* gets file name, finds the '.' and adds the new ending */
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
    insert_item_with_instructions(op_code_table, "mov", "0", 0, "013",
                                  "13");
    insert_item_with_instructions(op_code_table, "cmp", "1", 0, "013",
                                  "013");
    insert_item_with_instructions(op_code_table, "add", "2", 1, "013",
                                  "13");
    insert_item_with_instructions(op_code_table, "sub", "2", 2, "013",
                                  "13");
    insert_item_with_instructions(op_code_table, "lea", "4", 0, "1",
                                  "13");
    insert_item_with_instructions(op_code_table, "clr", "5", 1, "",
                                  "13");
    insert_item_with_instructions(op_code_table, "not", "5", 2, "",
                                  "13");
    insert_item_with_instructions(op_code_table, "inc", "5", 3, "",
                                  "13");
    insert_item_with_instructions(op_code_table, "dec", "5", 4, "",
                                  "13");
    insert_item_with_instructions(op_code_table, "jmp", "9", 1, "",
                                  "12");
    insert_item_with_instructions(op_code_table, "bne", "9", 2, "",
                                  "12");
    insert_item_with_instructions(op_code_table, "jsr", "9", 3, "",
                                  "12");
    insert_item_with_instructions(op_code_table, "red", "12", 0, "",
                                  "13");
    insert_item_with_instructions(op_code_table, "prn", "13", 0, "",
                                  "013");
    insert_item_with_instructions(op_code_table, "rts", "14", 0, "",
                                  "");
    insert_item_with_instructions(op_code_table, "stop", "15", 0, "",
                                  "");
    return op_code_table;
}

table *initialize_registers_table(void) {
    table *registers_table = create_table();
    if (registers_table == NULL) {
        //TODO HANDLE ERROR
        return NULL;
    }
    insert_table_item(registers_table, "r0", "0");
    insert_table_item(registers_table, "r1", "1");
    insert_table_item(registers_table, "r2", "2");
    insert_table_item(registers_table, "r3", "3");
    insert_table_item(registers_table, "r4", "4");
    insert_table_item(registers_table, "r5", "5");
    insert_table_item(registers_table, "r6", "6");
    insert_table_item(registers_table, "r7", "7");
    return registers_table;
}

void print_hash_table(table *table) {
    for (int i = 0; i < table->size; i++) {
        if (table->bucket[i] != NULL) {
            printf("Index %d, ", i);
            printf("Key: %s Value: %d\n", table->bucket[i]->key,
                   table->bucket[i]->symbol->location);

            if (table->bucket[i]->next != NULL) {
                struct table_item *current_macro =
                        table->bucket[i]->next;
                while (current_macro != NULL) {
                        if (current_macro->symbol){
                    printf("          Key: %s Value: %d\n",
                           current_macro->key,
                           current_macro->symbol->location);
                        }
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

bool validate_number(char *str, int *number) {
    //with negative sign
    int i;
    char *letter = str;
    i = 0;

    while (*letter != '\0') {
        if (*letter != '-' && !isdigit(*letter)) {
            //TODO handle ERROR
            printf("ERRROR");
                return false;
        }
        if (*letter == '-' && i != 0) {
            //TODO handle error!
                return false;
        }
        printf("token: %c\n", *letter);
        letter++;
        i++;
    }

    *number = (int) strtol(str,NULL, 10);
    return true;
}
/**
 *
 * @param value 21 bits value
 * @param A
 * @param R
 * @param E
 * @return
 */
int translate_address(int value, int A, int R, int E)

{

    int result=value<<1;
    result += A;
    result = result<<1;
    result +=R;
    result = result<<1;
    result +=E;
    return result;
}
int translate_instruction_address(instruction *instruction)
{
    int result = instruction->op_code;
    result = result << 2;
    result += instruction->source_addressing;
    result = result << 3;
    result += instruction->source_reg;
    result = result << 2;
    result += instruction->destination_addressing;
    result = result << 3;
    result += instruction->destination_reg;
    result = result << 5;
    result += instruction->funct;
    result = result << 1;
    result += instruction->A;
    result = result << 1;
    result += instruction->R;
    result = result << 1;
    result += instruction->E;
    return result;
}

