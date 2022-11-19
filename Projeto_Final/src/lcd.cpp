#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "grlib/grlib.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "cfaf128x128x16.h"

tContext sContext;

void DispMessage(void)
{
	GrContextInit(&sContext, &g_sCfaf128x128x16);

	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);

	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);


	GrStringDraw(&sContext,"Exemplo", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
	GrStringDraw(&sContext,"Tiva + BoosterPack", -1, 0, (sContext.psFont->ui8Height+2)*1, true);
	GrStringDraw(&sContext,"---------------------", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
}

void initLCD(void)
{
	cfaf128x128x16Init();
	DispMessage();
}