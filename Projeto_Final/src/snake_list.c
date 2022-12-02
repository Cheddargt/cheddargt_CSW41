#include "snake_list.h"

/* */
Node* snake_create (void) 
{
   return NULL;
}

/* Adiciona uma nova posição na cobra, sempre adiciona e retorna a cabeça*/
Node* snake_add (Node* head, int x, int y)
{
   Node* new_node =(Node*)malloc(sizeof(Node));
   new_node->x = x;
   new_node->y = y;
   new_node->next = head;
   return new_node;
}


void snake_print (Node* head) 
{
    Node* v; 
    for (v = head; v != NULL; v = v->next) 
        printf("VX: %d, Y: %d\n", v->x, v->y);
} 


void snake_free (Node* head) 
{
    while (head != NULL) {
        Node* aux = head->next; /*guarda ref. p/ prox.*/
        free (head); /*libera a memória.*/
        head = aux; /*faz node apontar p/ o prox. elem.*/
    }
}

int snake_size (Node* head)
{
    int n = 0;
    Node* p;
    for (p = head; p!=NULL; p = p->next)
        n++;

    return n;
}

Node* snake_search (Node* head , int x, int y)
{
    Node* i;
    for (i = head; i != NULL; i = i->next)
    {
        if (i->x == x)
        {
            if (i->y == y)
                return i;
        }
    }
    return NULL;
}

void snake_update (Node* head , int direction)
{
    Node* i;
    for (i = head; i != NULL; i = i->next)
    {
        switch (direction)
        {
            case 1:
                (i->x)++;
                break;
            case 2:
                (i->x)--;
                break;
            case 3:
                (i->y)--;
                break;
            case 4:
                (i->y)++;
                break;
        }
    }
}

Node* new_food(Node* head)
{
    Node *food =(Node*)malloc(sizeof(Node));
    int good_food = 0;
    
    while (!good_food)
    {
        (food->x) = rand() % 15 + 1;
        (food->y) = rand() % 15 + 1;
        
        struct Node *colision = snake_search(head , food->x, food->y);
        if (colision == NULL)
            good_food = 1;
    }
    food->next = NULL;
    return food;
}
