#include "snake_list.h"

/* */
Node* snake_create (void) 
{
   return NULL;
}

/* Adiciona uma nova posição na cobra, sempre adiciona e retorna a cabeça*/
Node* snake_add (Node* node, int x, int y);
{
   Node* novo =(Node*)malloc(sizeof(Node));
   novo->x = x;
   novo->y = y;
   novo->next = node;
   return novo;
}


void snake_print (Node*node) 
{
    Node* v; 
    for (v = node; v != NULL; v = v->next) 
        printf("VX: %d, Y: %d\n", v->x, v->y);
} 


void snake_free (Node* node) 
{
    while (node != NULL) {
        Node* aux = node->next; /*guarda ref. p/ prox.*/
        free (node); /*libera a memória.*/
        node = aux; /*faz node apontar p/ o prox. elem.*/
    }
}

int snake_size (Node* node)
{
    int n = 0;
    Node* p;
    for (p = node; p!=NULL; p = p->next)
        n++;

    return n;
}

Node* snake_search (Node* node , int x, int y)
{
    Node* i;
    for (i = node; i != NULL; i = i->next)
    {
        if (i->x == x)
        {
            if (i->y == y)
                return i;
        }
    }
    return NULL;
}
