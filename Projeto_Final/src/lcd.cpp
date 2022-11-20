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

void initTela(void)
{
	GrFlush(&sContext);
        
        GrContextFontSet(&sContext, g_psFontFixed6x8);

	GrContextForegroundSet(&sContext, ClrGreen);
	GrContextBackgroundSet(&sContext, ClrBlack);


	GrStringDraw(&sContext,"EMBEDDED SNAKE", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
	GrStringDraw(&sContext,"---------------------", -1, 0, (sContext.psFont->ui8Height+2)*1, true);
	GrStringDraw(&sContext,"PRESS PAUSE BUTTON", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"TO START", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
}

void initBackground(void)
{
	tRectangle paredeE, paredeD, paredeC, paredeB, backgound;
	
        GrFlush(&sContext);
	GrContextForegroundSet(&sContext, ClrBlack);
	
	paredeE.i16XMin = 0;
	paredeE.i16YMin = 0;
	paredeE.i16XMax = 7;
	paredeE.i16YMax = 127;
	
	paredeD.i16XMin = 120;
	paredeD.i16YMin = 0;
	paredeD.i16XMax = 127;
	paredeD.i16YMax = 127;
	
	paredeB.i16XMin = 7;
	paredeB.i16YMin = 120;
	paredeB.i16XMax = 120;
	paredeB.i16YMax = 127;
	
	paredeC.i16XMin = 7;
	paredeC.i16YMin = 0;
	paredeC.i16XMax = 120;
	paredeC.i16YMax = 7;
        
    backgound.i16XMin = 8;
	backgound.i16YMin = 8;
	backgound.i16XMax = 120;
	backgound.i16YMax = 120;
	
	GrRectFill(&sContext, &paredeE);
	GrRectFill(&sContext, &paredeD);
	GrRectFill(&sContext, &paredeC);
	GrRectFill(&sContext, &paredeB);
    
    GrFlush(&sContext);    
    GrContextForegroundSet(&sContext, grayColor);
    GrRectFill(&sContext, &backgound);
}

void initLCD(void)
{
	cfaf128x128x16Init();
	
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	initTela();
	initBackground();
}