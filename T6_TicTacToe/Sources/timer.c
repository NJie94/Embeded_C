/*
 * timer.cpp
 *
 *  Created on: 12/05/2013
 *      Author: podonoghue
 */

#include <stdint.h>
#include "derivative.h"
#include "system.h"
#include "timer.h"

//=========================================================================
// Timer routines
//
//=========================================================================

// Timer constants, 24MHz ticks
#define TIMER_FREQ               (SystemBusClock/2)
#define TIMER_MICROSECOND(x)     (((x)*(TIMER_FREQ/1000))/1000UL)  // Timer ticks in 1 us

//! Wait for given time in milliseconds
//!
//! @param delay Delay time in milliseconds
//! @note used PIT CH0
//!
void millisecondTimerWait(uint16_t delay) {
	PIT_LDVAL0 = TIMER_MICROSECOND(1000); // Set up delay
	PIT_TCTRL0 = PIT_TCTRL_TEN_MASK;      // Enable (and reset) timer
	while (delay-->0) {
		PIT_TFLG0  = PIT_TFLG_TIF_MASK;              // Clear timer flag
		while ((PIT_TFLG0&PIT_TFLG_TIF_MASK) == 0) { // Wait for timeout
		}
	}
	PIT_TCTRL0 = 0;                       // Disable timer
}

//! Initialises the timers, input captures and interrupts
//!
uint8_t timer_initialise(void) {

	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT_MCR    = PIT_MCR_FRZ_MASK; // Enable timers, timers freeze in debug mode

	return 0;
}



