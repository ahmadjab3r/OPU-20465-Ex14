#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct Node {
    char *name;         /* Name associated with the node */
    char *content;      /* Content stored in the node */
    int instruction;
    int line;           /* Line number associated with the content */
    struct Node *next;  /* Pointer to the next node in the linked list */
    bool declared;   /* Flag indicating if the node has been declared */
} Node;
typedef struct LinkedList{
    Node *head;        /* Pointer to the head of the linked list */
    Node *tail;        /* Pointer to the tail of the linked list */
    int size;
} LinkedList;

LinkedList* initialize_linked_list(void);
Node *add_node(char *name, char* content, int instruction, int *line, bool declared,LinkedList *linked_list);
Node *search_list(LinkedList *linked_list, char *name, int *found);
void free_list(LinkedList **linked_list);



