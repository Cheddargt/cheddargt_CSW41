#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "grlib/grlib.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "cfaf128x128x16.h"

#define WALL_COLOR 0x00000000
#define TEXT_COLOR 0x00008000
#define SNAKE_COLOR 0x00006400
#define FOOD_COLOR 0x00FF0000
#define BACKGROUND_COLOR 0x00FFFAF0

tContext sContext;

void initTela(void)
{
    GrFlush(&sContext);
        
    GrContextFontSet(&sContext, g_psFontFixed6x8);

    GrContextForegroundSet(&sContext, TEXT_COLOR);
    GrContextBackgroundSet(&sContext, WALL_COLOR);


    GrStringDraw(&sContext,"EMBEDDED SNAKE", -1, 0, (sContext.psFont->ui8Height+2)*0, false);
    GrStringDraw(&sContext,"---------------------", -1, 0, (sContext.psFont->ui8Height+2)*1, false);
    GrStringDraw(&sContext,"PRESS PAUSE BUTTON", -1, 0, (sContext.psFont->ui8Height+2)*2, false);
    GrStringDraw(&sContext,"TO START", -1, 0, (sContext.psFont->ui8Height+2)*3, false);
}

void initBackground(void)
{
    tRectangle paredeE, paredeD, paredeC, paredeB, backgound;
    
    GrFlush(&sContext);
    GrContextForegroundSet(&sContext, WALL_COLOR);
    
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
    GrContextForegroundSet(&sContext, BACKGROUND_COLOR);
    GrRectFill(&sContext, &backgound);
}

void initLCD(void)
{
    cfaf128x128x16Init();
    
    GrContextInit(&sContext, &g_sCfaf128x128x16);
    
    initTela();
}

// Marca uma nova posição da cobra ou a comida no LCD
// Se flag == 0 -> Comida
// Se flag == 1 -> Posição da Cobra
// Se flag == 2 -> Posição do Background
void new_print(uint32_t x, uint32_t y, uint32_t flag)
{
    tRectangle new_snake;
    
    GrFlush(&sContext);
    if (flag == 0)
        GrContextForegroundSet(&sContext, FOOD_COLOR);
    else if (flag == 1)
        GrContextForegroundSet(&sContext, SNAKE_COLOR);
    else
        GrContextForegroundSet(&sContext, BACKGROUND_COLOR);
    
    new_snake.i16XMin = 8*x;
    new_snake.i16YMin = 8*y;
    new_snake.i16XMax = (8*(x+1)) - 1;
    new_snake.i16YMax = (8*(y+1)) - 1;
    
    GrRectFill(&sContext, &new_snake);    
}