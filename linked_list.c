#include "Header Files/linked_list.h"
#define ALLOCATION_FAIL "ERROR: Failed to allocate memory\n"
linked_list *initialize_linked_list(void)
{
  linked_list *list = malloc(sizeof(linked_list));
  if (list == NULL)
    {
      printf(ALLOCATION_FAIL);
      exit(EXIT_FAILURE);
    }
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
  return list;
}
node *search_list(linked_list *list, char *name, int *found)
{
  node *current = list->head;
  while (current != NULL)
    {
      if (strcmp(current->name, name) == 0)
        {
          *found = 1;
          return current;
        }
      current = current->next;
    }
  *found = 0;
  return NULL;
}
node *add_node(char *name, char *content, int instruction, int *line,
               int declared, linked_list *list)
{
  node *new_node = (node *) malloc(sizeof(node));
  if (new_node == NULL)
    {
      printf(ALLOCATION_FAIL);
      exit(EXIT_FAILURE);
    }
  new_node->name = strdup(name);
  new_node->content = strdup(content);
  new_node->instruction = instruction;
  new_node->line = *line;
  new_node->declared = declared;
  new_node->next = NULL;

  if (list->head == NULL)
    {
      list->head = new_node;
      list->tail = new_node;
    }
  else
    {
      list->tail->next = new_node;
      list->tail = new_node;
    }
  list->size++;
  (*line)++;
  return new_node;
}
void free_list(linked_list **list)
{
  node *current = (*list)->head;
  node *next_node;
  while (current != NULL)
    {
      next_node = current->next;
      free(current->name);
      free(current->content);
      free(current);
      current = next_node;
    }
  free(*list);
  *list = NULL;
}
