#ifndef _node_h_
#define _node_h_

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int x;
  int y;
  struct Node* next;
} Node;

Node* snake_create (void);

Node* snake_add (Node* node, int x, int y);

void snake_print (Node* node);

void snake_free (Node* node);

int snake_size (Node* node);

Node* snake_search (Node* node , int x, int y);

Node* snake_update (Node* node , int dir);

#endif
