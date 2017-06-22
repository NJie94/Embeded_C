/*
 * Joystick.c
 *
 *  Created on: 18 Sep 2014
 *      Author: podonoghue
 */
#include <stdlib.h>
#include "derivative.h"
#include "timer.h"
#include "Freedom.h"
#include "Joystick.h"

#define MASK(x)   (1<<(x))

/**
 * Initialises the ports associated with the switches
 */
void initJoystick() {
	
}

/*
 *  Polls switches
 *
 *  @return see SwitchValueType
 */
static SwitchValueType pollJoystick(void) {
	
	return NO_PRESS;
}

static void delay50ms(void) {
   millisecondTimerWait(50);
}

/*
 *  Polls switches with de-bounce
 *  Doesn't return until a button has been (released &) pressed
 *
 *  @return see \ref SwitchValueType
 */
SwitchValueType getJoystick(void) {
	
	return NO_PRESS;
}
