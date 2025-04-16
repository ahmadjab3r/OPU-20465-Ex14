#include "Header Files/second_run.h"

void write_entry_external_files(as_file *as_file)
{
  FILE *ext_file, *entr_file;
  table_item *current;
  int count, i;
  if (as_file->file_name_entries != NULL)
    {
      entr_file = fopen(as_file->file_name_entries, "w");
    }
  if (as_file->file_name_externals != NULL)
    {
      ext_file = fopen(as_file->file_name_externals, "w");
    }
  count = as_file->symbol_table->size - 1;
  while (count >= 0)
    {
      current = as_file->symbol_table->bucket[count];
      while (current != NULL)
        {
          if (current->symbol->is_entry)
            {
              fprintf(entr_file, ENTRY_EXTERNAL_PRINT_FORMAT,
                      current->key,
                      current->symbol->location);
            }
          if (current->symbol->is_extern)
            {
              if (current->symbol->extern_locations != NULL)
                {
                  i = 0;
                  while (i != current->symbol->
                         extern_locations_size)
                    {
                      fprintf(ext_file, ENTRY_EXTERNAL_PRINT_FORMAT,
                              current->key,
                              current->symbol->extern_locations[i]);
                      i++;
                    }
                }
            }
          current = current->next;
        }
      count--;
    }
  if (as_file->file_name_externals)
    {
      fclose(ext_file);
    }
  if (as_file->file_name_entries)
    {
      fclose(entr_file);
    }
}

int second_run(as_file *as_file)
{
  FILE *ob;
  node *current;
  if (as_file->is_valid)
    {
      ob = fopen(as_file->file_name_ob, "w");
      fprintf(ob, "%7d %x\n", as_file->lines->size - as_file->DC,
              as_file->DC);
    }
  current = as_file->lines->head;
  while (current != NULL)
    {
      if (!current->declared)
        {
          table_item *new_item = search_table(
            as_file->symbol_table, current->name);
          if (new_item == NULL)
            {

              printf(SYMBOL_NOT_FOUND, current->name);
              as_file->is_valid = false;
              current = current->next;
              continue;
            }
          if (strcmp(current->content, DIRECT_ADDRESSING_WORD) == 0)
            {
              if (new_item->symbol->is_extern)
                {
                  insert_external(as_file->symbol_table,
                                  new_item->key,
                                  current->line);
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
          if (strcmp(current->content, RELATIVE_ADDRESSING_WORD) ==
              0)
            {
              current->instruction = translate_address
              (new_item->symbol->location - (
                 current->line - 1), 1, 0, 0);
              current->declared = true;
            }
        }
      if (as_file->is_valid)
        {
          fprintf(ob, PRINT_FORMAT, current->line,
                  (unsigned) (current->instruction & 0xFFFFFF));
        }

      current = current->next;
    }
  if (as_file->is_valid)
    {
      write_entry_external_files(as_file);
      fclose(ob);
    }
  return 0;
}
