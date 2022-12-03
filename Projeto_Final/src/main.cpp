/*__________________________________________________________________________________
|       Disciplina de Sistemas Embarcados - 2022-1
|       Prof. Douglas Renaux
| __________________________________________________________________________________
|
|       Projeto Final
|       Alunos: Gustavo Zeni 
|               Lucas Perin
| __________________________________________________________________________________
*/

#include <stdint.h>
#include "tx_api.h"
#include "snake_list.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#define DEMO_STACK_SIZE         1024
#define DEMO_BYTE_POOL_SIZE     9120
#define DEMO_BLOCK_POOL_SIZE    100
#define DEMO_QUEUE_SIZE         100

#define CENTER_JOY 2000


TX_THREAD               thread_game;
TX_THREAD               thread_joy;
TX_THREAD               thread_lcd;
TX_QUEUE                joy_updated;
TX_QUEUE                update_lcd;
TX_BYTE_POOL            byte_pool_0;
TX_BLOCK_POOL           block_pool_0;
UCHAR                   byte_pool_memory[DEMO_BYTE_POOL_SIZE];

UINT ui32SysClock; //Clock em Hz

extern volatile bool pause;
extern UINT joy_X;
extern UINT joy_Y;

struct Node *head, *tail, *food;
int snake_speed = 1;       //Inicia em uma posição/segundo

extern void initJOY(void); 
extern void updateJOY(void); 
extern void initPAUSE(void);
extern void initSysTick(void);
extern void initLCD(void); 
extern void initBackground(void);
extern void new_print(UINT x, UINT y, UINT flag);
void thread_game_entry(ULONG thread_input);
void thread_joy_entry(ULONG thread_input);
void thread_lcd_entry(ULONG thread_input);

// Main function.

int main(int argc, char ** argv)
{
    ui32SysClock = SysCtlClockFreqSet(( SYSCTL_XTAL_25MHZ |
                                        SYSCTL_OSC_MAIN |
                                        SYSCTL_USE_PLL |
                                        SYSCTL_CFG_VCO_240), 120000000);
   tx_kernel_enter();
   
}   

void tx_application_define(void *first_unused_memory)
{
    char    *pointer = (char*) TX_NULL;
    
    /* Create a byte memory pool from which to allocate the thread stacks.  */
    tx_byte_pool_create(&byte_pool_0, "byte pool 0", byte_pool_memory, DEMO_BYTE_POOL_SIZE);
    
    tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
        tx_thread_create(&thread_game, "thread game", thread_game_entry, 0,  
            pointer, DEMO_STACK_SIZE, 
            3, 3, TX_NO_TIME_SLICE, TX_AUTO_START);
    
    tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
    tx_thread_create(&thread_joy, "thread joy", thread_joy_entry, 1,  
            pointer, DEMO_STACK_SIZE, 
            4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);
            
    tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
    tx_thread_create(&thread_lcd, "thread lcd", thread_lcd_entry, 1,  
            pointer, DEMO_STACK_SIZE, 
            2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);
            
    tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_QUEUE_SIZE*sizeof(ULONG), TX_NO_WAIT);
    tx_queue_create(&joy_updated, "joy update", TX_1_ULONG, pointer, DEMO_QUEUE_SIZE*sizeof(ULONG));
    
    tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_QUEUE_SIZE*sizeof(ULONG), TX_NO_WAIT);
    tx_queue_create(&update_lcd, "updated lcd", TX_1_ULONG, pointer, DEMO_QUEUE_SIZE*sizeof(ULONG));
    
    /* Allocate the memory for a small block pool.  */
    tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_BLOCK_POOL_SIZE, TX_NO_WAIT);
    
    /* Create a block memory pool to allocate a message buffer from.  */
    tx_block_pool_create(&block_pool_0, "block pool 0", sizeof(ULONG), pointer, DEMO_BLOCK_POOL_SIZE);

    /* Allocate a block and release the block memory.  */
    tx_block_allocate(&block_pool_0, (VOID **) &pointer, TX_NO_WAIT);

    /* Release the block back to the pool.  */
    tx_block_release(pointer);
}

void thread_game_entry(ULONG thread_input)
{
    UINT status, direction = 1;
    
    initPAUSE();
    IntMasterEnable(); 
    
    head = snake_create();
    head = snake_add(head, 3, 8);
    tail = head;
    head = snake_add(head, 4, 8);
    head = snake_add(head, 5, 8);
    head = snake_add(head, 6, 8);
    head = snake_add(head, 7, 8);
    food = new_food(head);
    
    status = tx_queue_send(&update_lcd, &direction, TX_WAIT_FOREVER);
    
    
    while(1)
    {
        while (!pause)
        {
            status = tx_queue_receive(&joy_updated, &direction, TX_NO_WAIT);
            
            snake_update(head, direction);
            
            if  (((head->x) == (food->x)) && ((head->y) == (food->y))) //Comeu a comida
            {
                snake_add(head, food->x, food->y);
                snake_speed = snake_speed++;
                food = new_food(head);
            }
            else if (((head->x) == 0) || ((head->x) == 15) || ((head->y) == 0) || ((head->y) == 15)) //Colisão com uma das paredes
            {
                //game_over();
                //pause = 1;
            }
                
            status = tx_queue_send(&update_lcd, &direction, TX_WAIT_FOREVER);
            
            UINT sleep = (UINT)(1000*snake_speed);
            tx_thread_sleep(sleep);
        }
    }
}

void thread_joy_entry(ULONG thread_input)
{
    UINT status, dif_x = 0, dif_y = 0;
    ULONG direction;
    initJOY();
    while(1)
    {
        while (!pause)
        {
            updateJOY();
            /* direction: 0 -> Centro ; 1 -> Direita ; 2 -> Esquerda ; 3 -> Cima ; 4 -> Baixo */
            // Calcula a direferença entre a direção atual e ao centro do joystick
            if (joy_X > CENTER_JOY)
                dif_x = joy_X - CENTER_JOY;
            else
                dif_x = CENTER_JOY - joy_X;
            
            if (joy_Y > CENTER_JOY)
                dif_y = joy_Y - CENTER_JOY;
            else
                dif_y = CENTER_JOY - joy_Y;
            
            if ((dif_x > dif_y) && (dif_x > (CENTER_JOY/4)))
            {
                if (joy_X > CENTER_JOY) // Direita
                {
                    if (direction != 1) // Outra Direção
                    {
                        direction = 1;
                        status = tx_queue_send(&joy_updated, &direction, TX_WAIT_FOREVER);
                    }
                }
                else // Esquerda
                {
                    if (direction != 2) // Outra Direção
                    {
                        direction = 2;
                        status = tx_queue_send(&joy_updated, &direction, TX_WAIT_FOREVER);
                    }
                }
            }
            else if ((dif_y > dif_x) && (dif_y > (CENTER_JOY/4)))
            {
                if (joy_Y > CENTER_JOY) // Cima
                {
                    if (direction != 3) // Outra Direção
                    {
                        direction = 3;
                        status = tx_queue_send(&joy_updated, &direction, TX_WAIT_FOREVER);
                    }
                }
                else // Baixo
                {
                    if (direction != 4) // Outra Direção
                    {
                        direction = 4;
                        status = tx_queue_send(&joy_updated, &direction, TX_WAIT_FOREVER);
                    }
                }
            }
            tx_thread_sleep(10);
        }
    }
}

void thread_lcd_entry(ULONG thread_input)
{
    UINT COMIDA = 0, COBRA = 1, BACKGROUND = 2;
    UINT status, first = 1;
    ULONG update = 0;
    initLCD();
    while(1)
    {
        while (!pause)
        {
            status = tx_queue_receive(&update_lcd, &update, TX_WAIT_FOREVER);
            
            if (first)
            {
                new_print(3, 8, COBRA);
                new_print(4, 8, COBRA);
                new_print(5, 8, COBRA);
                new_print(6, 8, COBRA);
                new_print(7, 8, COBRA);
                new_print(food->x, food->y, COMIDA);
                first = 0;
            }
            
            if (update && (!first))
            {
                new_print(food->x, food->y, COMIDA);
                new_print(head->x, head->y, COBRA);
                new_print(tail->x, tail->y, BACKGROUND);
            }          
        }
    }
}

