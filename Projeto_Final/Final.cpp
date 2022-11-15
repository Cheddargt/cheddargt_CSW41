/*__________________________________________________________________________________
|       Disciplina de Sistemas Embarcados - 2022-1
|       Prof. Douglas Renaux
| __________________________________________________________________________________
|
|		Projeto Final
|       Alunos: Gustavo Zeni 
|               Lucas Perin
| __________________________________________________________________________________
*/

/*------------------------------------------------------------------------------
 *
 *      File includes
 *
 *------------------------------------------------------------------------------*/
#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "hw_memmap.h"
#include "gpio.h"
#include "interrupt.h"
#include "systick.h"
#include "sysctl.h"

// #include "inc/hw_memmap.h"
// #include "driverlib/gpio.h"
// #include "driverlib/interrupt.h"
// #include "driverlib/systick.h"
// #include "driverlib/sysctl.h"
// #include "../../TivaWare_C_Series-2.2.0.295/inc/tm4c1294ncpdt.h"


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
	volatile int32_t game_on = 0;
	volatile int32_t joy_x = 0;
	volatile int32_t joy_y = 0;
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

extern void SysTick_IntHandler(void)
{
    
}

extern void GPIOL_IntHandler(void)
{	
    GPIOIntDisable(GPIO_PORTL_BASE, GPIO_INT_PIN_1);
    if (game_on)
      game_on = 0;
    else
      game_on = 1;
	
   GPIOIntEnable(GPIO_PORTL_BASE, GPIO_INT_PIN_1);
}
extern void GPIOE_IntHandler(void)
{	
    GPIOIntDisable(GPIO_PORTE_BASE, GPIO_PIN_1 & GPIO_PIN_3);
	joy_x = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1);
	joy_y = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3);
	GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_1 & GPIO_PIN_3);
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

  }
}	

void initGPIO(void) {
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); 	//D1 - LCD, SPI MOSI 
												//D3 - LCD, SPI CLK
												
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); 	//E4 - JoyX
												//E3 - JoyY
												
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG); 	//G0 - LCD, ~RST
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL); 	//L1 - Botao1 
												//L3 - LCD, RS PIN
												
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION); 	//N2 - LCD, SPI CD
  
  //Espera a inicialização dos GPIO's
  while(!(	SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD) 	&&
			SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE) 	&&
			SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOG) 	&&
			SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL) 	&&
			SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)		)){}

  
  //Entradas
  GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 & GPIO_PIN_4);
  GPIODirModeSet(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_DIR_MODE_IN);
  
  //Saidas
  GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_1 & GPIO_PIN_3, GPIO_DIR_MODE_OUT);
  GPIODirModeSet(GPIO_PORTG_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);
  GPIODirModeSet(GPIO_PORTL_BASE, GPIO_PIN_3, GPIO_DIR_MODE_OUT);
  GPIODirModeSet(GPIO_PORTN_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);
  
  
  //Resistores Pull-up
  GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_3 & GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_ANALOG);
  GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

  //SysTickIntEnable();
}

void initInterrupt()
{
 
    //Interrupcao do PAUSE
	GPIOIntEnable(GPIO_PORTL_BASE, GPIO_INT_PIN_1);
	GPIOIntTypeSet(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_LOW_LEVEL);
	IntEnable(INT_GPIOL);
	IntPrioritySet(INT_GPIOL, 0x01);	//Mais importante
	
	//Interrupcao do Joystick
	GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_1 & GPIO_PIN_3);
	GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_1 & GPIO_PIN_3, GPIO_LOW_LEVEL);
	IntEnable(INT_GPIOE);
	IntPrioritySet(INT_GPIOE, 0x00);
     
	IntMasterEnable(); // Enable the Master key of interrupt.
    
}


