#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skeleton.h"
#include "utilities.h"

typedef struct Node {
    char *name;         /* Name associated with the node */
    char *content;      /* Content stored in the node */
    int line;           /* Line number associated with the content */
    struct Node *next;  /* Pointer to the next node in the linked list */
    bool declared;   /* Flag indicating if the node has been declared */
} Node;
typedef struct LinkedList{
    Node *head;        /* Pointer to the head of the linked list */
    Node *tail;        /* Pointer to the tail of the linked list */
    int size;
} LinkedList;

