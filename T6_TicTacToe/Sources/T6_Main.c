/*
 ============================================================================
 * This is a TIC TAC TOE game program
 * Author: WONG YEW WEE AND NICHOLAS HI
 * CREATED ON 25 SEPTEMBER
 ============================================================================
 */
#include <stdio.h>
#include "derivative.h"
#include "utilities.h"
#include "system.h"
#include "Joystick.h"
#include "timer.h"
#include "TicTacToe.h"
#include "Music.h"
#include "nokia_LCD.h"

/*
 * Prompt for MACHINE FIRST OR PLAYER FIRST from PLAYER
 * @return PlayerOrderType
 */
PlayOrderType randomPLAYER (void)
{	
	while(getJoystick() != NORTH_PRESS || getJoystick() != SOUTH_PRESS){
		if(getJoystick() == NORTH_PRESS){
			return machineFirst;
		}
		if(getJoystick() == SOUTH_PRESS){
			return humanFirst;
		}
	}
	return machineFirst;
}

int main(void) {
	initJoystick(); // initialise joystick
	timer_initialise(); 
	lcd_initialise(); // initialise lcd
	lcd_clear(WHITE); // clear the lcd screen to the designated colour
	doMenu(); // print menu to prompt user for input
	PlayOrderType random = randomPLAYER(); //player sequence
	// Real programs never die!
	for(;;) {
		playNoughtsAndCrosses(random); //game start
	}
	return 0;
}
