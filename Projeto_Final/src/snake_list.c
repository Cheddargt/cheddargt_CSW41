#include "snake_list.h"

/* */
Node* snake_create (void) 
{
   return NULL;
}

/* Adiciona uma nova posição na cobra, sempre adiciona e retorna a cabeça*/
Node* snake_add (Node* head, int x, int y);
{
   Node* new =(Node*)malloc(sizeof(Node));
   new->x = x;
   new->y = y;
   new->next = head;
   return new;
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
