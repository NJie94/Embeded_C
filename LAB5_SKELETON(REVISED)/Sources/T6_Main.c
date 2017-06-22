/*
 ============================================================================
 * T4_Ex3.c
 ============================================================================
 */
#include <stdio.h>
#include "derivative.h"
#include "utilities.h"
#include "system.h"
#include "timer.h"
#include "display.h"
#include "nokia_LCD.h"
#include "PIT.h"
#include "ADC.h"
#include "menu.h"
#include "Joystick.h"

;int main(void) {
	//initialise clock, joystick and lcd
	clock_initialise();
	initJoystick();
	lcd_initialise();
	//prompt for user input and draw the grid
	doMenu();
	drawBackground();
	drawGrid();
	//initialise ADC and PIT
	initialiseADC();
	initialisePIT(0, samplingRate());

	for(;;) {
		__asm("wfi");//wait for interrupt
		drawData(YELLOW);//draw the curve
		printf("ADC = %d\n", ((doADCConversion()*100)/1023));
	}
	return 0;
}	

