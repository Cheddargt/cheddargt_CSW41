#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "grlib/grlib.h"
#include "cfaf128x128x16.h"
#include "tx_api.h"

bool pause = false;
extern TX_EVENT_FLAGS_GROUP pause_flag;
extern void print_pause(bool pause);

void initPAUSE(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);    //L1 - Botao1
    
    while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL))){}
    
    GPIODirModeSet(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_DIR_MODE_IN); //Entrada
    
    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);    //Pull-up
    
    // Interrupcao          
    GPIOIntEnable(GPIO_PORTL_BASE, GPIO_INT_PIN_1);
    GPIOIntTypeSet(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_LOW_LEVEL);
    IntEnable(INT_GPIOL_TM4C129);
    IntPrioritySet(INT_GPIOL_TM4C129, 0x00);    //Mais importante por parar o jogo
}

void pause_IntHandler(void)
{      
    int status;
    
    if (pause)
        pause = false;
    else
        pause = true;
    
    status = tx_event_flags_set(&pause_flag, 0x1, TX_OR);
}