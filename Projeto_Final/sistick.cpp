#include <stdint.h>

#include "../../TivaWare_C_Series-2.2.0.295/driverlib/interrupt.h"
#include "../../TivaWare_C_Series-2.2.0.295/driverlib/systick.h"

volatile uint32_t systickCount = 0;
static const uint32_t systickPeriod = 120e6 / 1000; // 1ms
extern uint32_t g_ui32SysClock;
extern void SysTickIntHandler(void);

void SysTickIntHandler(void) 
{
    systickCount++;
}

void initSysTick(void) 
{
    SysTickPeriodSet(systickPeriod);
    SysTickIntEnable();
    SysTickEnable();
    IntMasterEnable();
}
