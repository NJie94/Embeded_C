/*
 * Joystick.c
 *
 *  Created on: 18 Sep 2014
 *      Author: 100012429
 */
#include <stdlib.h>
#include "derivative.h"
#include "timer.h"
#include "Freedom.h"
#include "Joystick.h"

#define MASK(x)   (1<<(x))
#define MUX_GPIO (1)
/**
 * Initialises the ports associated with the switches
 */
void initJoystick() {
	/*
	 * Turn on clocks to ports B, C & D
	 */
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTC_MASK|SIM_SCGC5_PORTD_MASK;
	
	/*
	 * Initialise the Pins for the Joystick buttons
	 */
	PORTB_PCR1 = 	PORT_PCR_MUX(MUX_GPIO)| //north
					PORT_PCR_PE_MASK | 
					PORT_PCR_PS_MASK;
	PORTC_PCR0 = 	PORT_PCR_MUX(MUX_GPIO)| //East
					PORT_PCR_PE_MASK | 
					PORT_PCR_PS_MASK;
	PORTD_PCR6 = 	PORT_PCR_MUX(MUX_GPIO)| //west
		        	PORT_PCR_PE_MASK | 
		        	PORT_PCR_PS_MASK;
	PORTC_PCR1 = 	PORT_PCR_MUX(MUX_GPIO)|  //south
		        	PORT_PCR_PE_MASK | 
		        	PORT_PCR_PS_MASK;
	PORTD_PCR5 = 	PORT_PCR_MUX(MUX_GPIO)|  //centre
		        	PORT_PCR_PE_MASK | 
		        	PORT_PCR_PS_MASK;
	/*
	 * Set the above pins on Ports B, C & D to be inputs
	 */
	GPIOD_PDDR &= ~((1<<CENTRE_SWITCH_NUM)|(1<<WEST_SWITCH_NUM));
	GPIOC_PDDR &= ~((1<<EAST_SWITCH_NUM)|(1<<SOUTH_SWITCH_NUM));
	GPIOB_PDDR &= ~(1<<NORTH_SWITCH_NUM);
}

/*
 *  Polls switches
 *
 *  @return see SwitchValueType
 */
static SwitchValueType pollJoystick(void) {
	if ((GPIOB_PDIR&(1<<NORTH_SWITCH_NUM))==0){
		return NORTH_PRESS;
		}
	else if ((GPIOC_PDIR&(1<<EAST_SWITCH_NUM))==0){
		return EAST_PRESS;
		}
	else if ((GPIOD_PDIR&(1<<WEST_SWITCH_NUM))==0){
		return WEST_PRESS;
		}
	else if ((GPIOC_PDIR&(1<<SOUTH_SWITCH_NUM))==0){
		return SOUTH_PRESS;
		}
	else if ((GPIOD_PDIR&(1<<CENTRE_SWITCH_NUM))==0){
		return CENTRE_PRESS;  // Checked last to prevent accidental presses while selecting others
		}
	else{
	return NO_PRESS;  }
}

static void delay20ms(void) {
    timer_initialise();   
	millisecondTimerWait(20);
}

/*
 *  Polls switches with de-bounce
 *  Doesn't return until a button has been (released &) pressed
 *
 *  @return see \ref SwitchValueType
 */
SwitchValueType getJoystick(void) {
	SwitchValueType swValue= NO_PRESS;
	int matchCount = 0;
	do{
		SwitchValueType temp = pollJoystick();
		delay20ms();
		
		if (temp == swValue){
			matchCount ++;
		}
		else{
			matchCount = 0;
			swValue = temp;			
		}
	}while(matchCount<3);
	
	return swValue;
}
