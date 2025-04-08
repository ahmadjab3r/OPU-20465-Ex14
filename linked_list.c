#include "linked_list.h"

LinkedList * initialize_linked_list(void) {
    LinkedList *linked_list = (LinkedList *)malloc(sizeof(LinkedList));
    if (linked_list == NULL) {
        //TODO handle Error
        return NULL;
    }
    linked_list->head = NULL;
    linked_list->tail = NULL;
    linked_list->size = 0;
    return linked_list;
}
Node *search_list(LinkedList *linked_list, char *name, int *found) {
    Node *current = linked_list->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            *found = 1;
            return current;
        }
        current = current->next;
    }
    *found = 0;
    return NULL;
}
Node *add_node(char *name, char* content, int instruction, int *line, bool declared,LinkedList* linked_list) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        //TODO handle ERROR
        return NULL;
    }
    new_node->name = strdup(name);
    new_node->content = strdup(content);
    new_node->instruction = instruction;
    new_node->line = *line;
    new_node->declared = declared;
    new_node->next = NULL;

    if (linked_list->head == NULL) {
        linked_list->head = new_node;
        linked_list->tail = new_node;
    } else {
        linked_list->tail->next = new_node;
        linked_list->tail = new_node;
    }
    linked_list->size++;
    (*line)++;
    return new_node;
}
void free_list(LinkedList **linked_list) {
    Node *current = (*linked_list)->head;
    Node *next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current->name);
        free(current->content);
        free(current);
        current = next_node;
    }
    free(*linked_list);
    *linked_list = NULL;
}