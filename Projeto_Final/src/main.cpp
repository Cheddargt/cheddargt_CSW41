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
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#define DEMO_STACK_SIZE         1024
#define DEMO_BYTE_POOL_SIZE     9120
#define DEMO_BLOCK_POOL_SIZE    100
#define DEMO_QUEUE_SIZE         100

TX_THREAD               thread_game;
TX_THREAD               thread_joy;
TX_THREAD               thread_lcd;
TX_EVENT_FLAGS_GROUP    flag_joy;
TX_BYTE_POOL            byte_pool_0;
TX_BLOCK_POOL           block_pool_0;
UCHAR                   byte_pool_memory[DEMO_BYTE_POOL_SIZE];

uint32_t ui32SysClock; //Clock em Hz
extern volatile bool pause;
extern uint32_t joy_X;
extern uint32_t joy_Y;


extern void initJOY(void); 
extern void updateJOY(void); 
extern void initPAUSE(void);
extern void initSysTick(void);
extern void initLCD(void); 
extern void initBackground(void);
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
    
    /* Allocate the stack for thread 0.  */
    tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
    
    /* Create the main thread.  */
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
            
    /* Create the event flags group used by threads 0 and 1.  */
    tx_event_flags_create(&flag_joy, "flag joy");
    
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
    initPAUSE();
    IntMasterEnable(); 
    while(1)
    {
        while (!pause)
        {
            tx_thread_sleep(100);
        }
    }
}

void thread_joy_entry(ULONG thread_input)
{
    initJOY();
    while(1)
    {
        while (!pause)
        {
            
            updateJOY();
            
            tx_thread_sleep(100);
        }
    }
}
int i = 0;
void thread_lcd_entry(ULONG thread_input)
{
    initLCD();
    while(1)
    {
        while (!pause)
        {
          i++;  
          initBackground();
          tx_thread_sleep(100);
        }
    }
}

