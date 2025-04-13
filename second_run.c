#include "second_run.h"

void write_entry_external_files(ASFile *as_file)
{
 FILE *ext_file , *entr_file;
  if (as_file->file_name_entries != NULL)
    {
      entr_file = fopen(as_file->file_name_entries, "w");
    }
  if (as_file->file_name_externals != NULL)
    {
      ext_file = fopen(as_file->file_name_externals, "w");
    }
  int count = as_file->symbol_table->size - 1;
  while (count >= 0)
    {
      struct table_item *current = as_file->symbol_table->bucket[count];
      while (current != NULL)
        {
          if (current->symbol->is_entry)
            {
              fprintf(entr_file, "%s %d\n", current->key,
                      current->symbol->location);
            }
          if (current->symbol->is_extern)
            {
              if (current->symbol->extern_locations != NULL)
                {
                int i = 0;
                  while ( i != current->symbol->extern_locations_size)
                    {
                      fprintf(ext_file, "%s %d\n", current->key,
                              current->symbol->extern_locations[i]);

                    i++;
                    }
                }
            }
          current = current->next;
        }
    count--;
    }
  if (ext_file) {
    fclose(ext_file);

  }
  if (entr_file) {
    fclose(entr_file);
  }
}



int second_run(ASFile *as_file)
{
  FILE *ob = fopen(as_file->file_name_ob, "w");
  Node *current = as_file->lines->head;
  fprintf(ob, "%7d %x\n", as_file->lines->size - as_file->DC, as_file->DC);

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
                  insert_external(as_file->symbol_table, new_item->key,
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
  write_entry_external_files(as_file);
  fclose(ob);

}
