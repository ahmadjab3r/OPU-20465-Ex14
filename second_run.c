#include "second_run.h"

char *compact_line_address(int line, int address)
{
  /* line is a number that represents the line in the file,
   * if it is 10 then we create a string that makes it 0000010
   * and address we represent it in hexadecimal, */
}
void write_files(ASFile *as_file)
{
}
int second_run(ASFile *as_file)
{
  FILE *ob = fopen(as_file->file_name_ob, "w");
  Node *current = as_file->lines->head;
  while (current != NULL)
    {
      if (!current->declared)
        {
          struct table_item *new_item = search_table(
            as_file->symbol_table, current->name);
          if (new_item == NULL)
            {
              //TODO handle ERROR
              printf("SYMBOL DOESNT EXIST!");
              return -1;
            }
          if (strcmp(current->content, "Direct") == 0)
            {
              if (new_item->symbol->is_extern)
                {
                  current->instruction = 1;
                }
              else
                {
                  current->instruction = translate_address(
                    new_item->symbol->location, 0,
                    1, 0);
                }
              current->declared = true;
            }
          if (strcmp(current->content, "Relative") == 0)
            {
              current->instruction = translate_address
                  (new_item->symbol->location - (current->line - 1), 1, 0, 0);
              current->declared = true;
            }
        }
      fprintf(ob, "%07d %06x\n", current->line, (unsigned)(current->instruction & 0xFFFFFF));

      current = current->next;
    }
  fclose(ob);

}
