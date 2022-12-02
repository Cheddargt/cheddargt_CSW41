#ifndef _node_h_
#define _node_h_

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int x;
  int y;
  struct Node* next;
} Node;

Node* snake_create(void);

Node* snake_add(Node* head, int x, int y);

void snake_print(Node* head);

void snake_free(Node* head);

int snake_size(Node* head);

Node* snake_search(Node* head, int x, int y);

void snake_update(Node* head, int dir);

Node* new_food(Node* head);

#endif
