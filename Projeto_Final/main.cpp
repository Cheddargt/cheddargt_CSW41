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

/*------------------------------------------------------------------------------
 *
 *      File includes
 *
 *------------------------------------------------------------------------------*/
#include <stdint.h>

#include "../../TivaWare_C_Series-2.2.0.295/inc/hw_memmap.h"
#include "../../TivaWare_C_Series-2.2.0.295/driverlib/gpio.h"
#include "../../TivaWare_C_Series-2.2.0.295/driverlib/interrupt.h"
#include "../../TivaWare_C_Series-2.2.0.295/driverlib/sysctl.h"

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
uint32_t ui32SysClock; //Clock em Hz
extern volatile bool pause;
extern uint32_t joy_X;
extern uint32_t joy_Y;
extern volatile uint32_t systickCount;

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

extern void initJOY(void); 
extern void updateJOY(void); 
extern void initPAUSE(void);
extern void initSysTick(void);
extern void initLCD(void); 

/**
 * Main function.
 *
 * @param[in] argc - not used, declared for compatibility
 * @param[in] argv - not used, declared for compatibility
 * @returns int    - not used, declared for compatibility
 */
int main(int argc, char ** argv)
{
    ui32SysClock = SysCtlClockFreqSet(( SYSCTL_XTAL_25MHZ |
                                        SYSCTL_OSC_MAIN |
                                        SYSCTL_USE_PLL |
                                        SYSCTL_CFG_VCO_240), 120000000);

    
    initJOY();
    initPAUSE();
    initSysTick();
    initLCD();
    IntMasterEnable(); 
    while (1)
    {
              if (systickCount == 5000)
              {
                systickCount = 0;
                updateJOY();
              }
    }
}   

void initGPIO(void) {
  
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);    //D1 - LCD, SPI MOSI 
                                                    //D3 - LCD, SPI CLK
                                                
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);    //G0 - LCD, ~RST
  
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);    //L3 - LCD, RS PIN
                                                
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);    //N2 - LCD, SPI CD
  
    //Espera a inicialização dos GPIO's
    while(!(    SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)  &&
                SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOG)  &&
                SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL)  &&
                SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)
                )){}

  
      
    //Saidas
    GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_1 | GPIO_PIN_3, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIO_PORTG_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIO_PORTL_BASE, GPIO_PIN_3, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIO_PORTN_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);

}


