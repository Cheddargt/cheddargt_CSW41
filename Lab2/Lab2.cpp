/*__________________________________________________________________________________
|       Disciplina de Sistemas Embarcados - 2022-1
|       Prof. Douglas Renaux
| __________________________________________________________________________________
|
|		Lab 1
|       Alunos: Gustavo Zeni 
|               Lucas Perin
| __________________________________________________________________________________
*/

/**
 * @file     main.cpp
 * @author   Douglas P. B. Renaux
 * @brief    Solution to Lab1 of ELF74/CSW41 - UTFPR. \n 
 *           Tools instalation and validation procedure.\n 
 *           Show messages on terminal using std::cout. \n 
 *           Show current value of some predefined macros (preprocessor symbols).\n 
 *           Read float value from terminal using std::cin.
 * @version  V2 -for 2022-1 semester
 * @date     Feb, 2022
 ******************************************************************************/

/*------------------------------------------------------------------------------
 *
 *      Use Doxygen to report lab results
 *
 *------------------------------------------------------------------------------*/
/** @mainpage Results from Lab1
 *
 * @section Ouput Values
 *
 * The values of ...
 *
 * @section Terminal
 *
 * @subsection Output
 *
 * etc...
 */

/*------------------------------------------------------------------------------
 *
 *      File includes
 *
 *------------------------------------------------------------------------------*/
#include <stdint.h>

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "template.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"


/*------------------------------------------------------------------------------
 *
 *      Typedefs and constants
 *
 *------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 *      Global vars
 *
 *------------------------------------------------------------------------------*/
	volatile uint32_t tempo = 0;
	volatile uint32_t timer = 0;
        volatile uint32_t chave = 0;
	uint32_t ui32SysClock; //Clock em Hz

/*------------------------------------------------------------------------------
 *
 *      File scope vars
 *
 *------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 *      Functions and Methods
 *
 *------------------------------------------------------------------------------*/

extern void SysTickIntHandler(void)
{
    timer++;
    //SysTickEnable();
}

extern void GPIOJIntHandler(void)
{	
	// Set GPIO high to indicate entry to this interrupt handler.
    GPIOPinWrite(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_PIN_0);

	SysTickDisable();
	chave = 1;
	

    // Set GPIO low to indicate exit from this interrupt handler.
    MAP_GPIOPinWrite(GPIO_PORTJ_BASE, GPIO_PIN_0, 0);
}
/**
 * Main function.
 *
 * @param[in] argc - not used, declared for compatibility
 * @param[in] argv - not used, declared for compatibility
 * @returns int    - not used, declared for compatibility
 */
int main(int argc, char ** argv)
{
          uint32_t max_tick = 120000000/4;

      ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_PLL |
                                       SYSCTL_CFG_VCO_240), 120000000);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION); //LED
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ); //Switch
	while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)&&SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ))){}

	GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
	GPIODirModeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_DIR_MODE_IN);
        GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
        
	IntMasterEnable();
        SysTickIntEnable();
        GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_LOW_LEVEL);
        IntEnable(INT_GPIOJ_TM4C129);

	
  while (1)
  {
	tempo = 0;
        chave = 0;
        timer = 0;
	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1); //LED Ligado
	SysTickPeriodSet(max_tick);
        SysTickEnable(); //Contador
	while((timer < 12) && (!chave))
	{
            
	}
	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0); //LED Desligado
	if (chave)
        {
          uint32_t valor = SysTickValueGet();
          tempo = timer * max_tick;
          tempo += valor;
          cout <<"Tempo de resposta: ";
          cout << tempo;
          cout << "\n";
        }
        else
        {
          cout <<"Tempo de resposta: 3s\n";
          SysTickDisable();
        }
  }
}	
