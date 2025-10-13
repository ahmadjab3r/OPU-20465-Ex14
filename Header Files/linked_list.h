#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#

typedef struct node {
    char *name;         /* Name associated with the node */
    char *content;      /* Content stored in the node */
    int instruction;
    int line;           /* Line number associated with the content */
    struct node *next;  /* Pointer to the next node in the linked list */
    int declared;   /* Flag indicating if the node has been declared */
} node;
typedef struct linked_list{
    node *head;        /* Pointer to the head of the linked list */
    node *tail;        /* Pointer to the tail of the linked list */
    int size;
} linked_list;

linked_list* initialize_linked_list(void);
node *add_node(char *name, char* content, int instruction, int *line, int declared,linked_list *linked_list);
node *search_list(linked_list *linked_list, char *name, int *found);
void free_list(linked_list **linked_list);