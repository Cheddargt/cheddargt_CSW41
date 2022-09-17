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
#include "../../TivaWare_C_Series-2.2.0.295/inc/tm4c1294ncpdt.h"


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
        uint32_t max_tick = 30000000;
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
        
//Declarations
void initGPIO(void);
void initInterrupt(void);
//void GPIOJIntHandler(void);

extern void SysTickIntHandler(void)
{
    timer++;
    //SysTickEnable();
}

extern void GPIOJIntHandler(void)
{	
    // Set GPIO high to indicate entry to this interrupt handler.
    //GPIOPinWrite(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_PIN_0);

    SysTickDisable();
    chave = 1;
	
    // Set GPIO low to indicate exit from this interrupt handler.
    //MAP_GPIOPinWrite(GPIO_PORTJ_BASE, GPIO_PIN_0, 0);
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


  ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                   SYSCTL_OSC_MAIN |
                                   SYSCTL_USE_PLL |
                                   SYSCTL_CFG_VCO_240), 120000000);

  initGPIO();
  initInterrupt();
  
	
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
          // Current iteration 
          uint32_t valor = SysTickValueGet();
          tempo = timer * max_tick + valor;
          //tempo += valor;
          //double aux = 0;
          //cout << "timer: " << timer << " max_tick: " << max_tick << "\n";
          //cout << "valor: " << valor << " tempo: " << tempo << "\n";
          //aux = tempo/120000000;
          //cout << aux << "\n";
          cout <<"Tempo de resposta: " << (tempo/120000000) << "ms" << "\n";
        }
        else
        {
          cout <<"Tempo de resposta: 3s\n";
          SysTickDisable();
        }
  }
}	

void initGPIO(void) {
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION); //LED
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ); //Switch
  while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)&&SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ))){}

  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
  GPIODirModeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_DIR_MODE_IN);
  GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  

  SysTickIntEnable();
  
  /*
  // Enable the GPIO port that is used for the on-board LED.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

  // Check if the peripheral access is enabled.
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
  {
  }

  // Enable the GPIOJ peripheral
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
  
  // Wait for the GPIOJ module to be ready.
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ))
  {
  }
  
  // Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
  // enable the GPIO pin for digital function.
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
  
  // Enable the BTN pin (PJ0).  Set the direction as input, and
  // enable the GPIO pin for digital function.
  GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);    
    
  */
}

void initInterrupt()
{
 
   /*
   IntMasterEnable();
   GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_LOW_LEVEL);
   IntEnable(INT_GPIOJ_TM4C129);
   */

   // Enables the specified GPIO interruputs
   GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
  
   // Set the specified type in the specified pin (high_level/low_level),
   // (falling_edge/rising_edge)
   GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_LOW_LEVEL);
   
   // Enable the interrupt for PORTJ.
   IntEnable(INT_GPIOJ);
   
   // Set the priority
   IntPrioritySet(INT_GPIOJ, 0x00);
   
   // Register the handler for GPIOJ interrupts
   //IntRegister(INT_GPIOJ, GPIOJIntHandler);
   
   IntMasterEnable(); // Enable the Master key of interrupt.
    
}


