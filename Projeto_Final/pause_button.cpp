#include <stdint.h>

#include "../../TivaWare_C_Series-2.2.0.295/inc/hw_memmap.h"
#include "../../TivaWare_C_Series-2.2.0.295/inc/hw_ints.h"
#include "../../TivaWare_C_Series-2.2.0.295/driverlib/gpio.h"
#include "../../TivaWare_C_Series-2.2.0.295/driverlib/sysctl.h"
#include "../../TivaWare_C_Series-2.2.0.295/driverlib/pin_map.h"
#include "../../TivaWare_C_Series-2.2.0.295/driverlib/interrupt.h"

bool pause = false;
extern void pause_IntHandler(void);

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
    IntPrioritySet(INT_GPIOL_TM4C129, 0x01);    //Mais importante por parar o jogo
}

extern void pause_IntHandler(void)
{   
    // Apenas troca o estado da flag "pause"
    //GPIOIntDisable(GPIO_PORTL_BASE, GPIO_INT_PIN_1);
    if (pause)
      pause = false;
    else
      pause = true;
    
    //GPIOIntEnable(GPIO_PORTL_BASE, GPIO_INT_PIN_1);
}