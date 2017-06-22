/*
 * timer.h
 *
 *  Created on: 12/05/2013
 *      Author: podonoghue
 */

#ifndef TIMER_H_
#define TIMER_H_

void millisecondTimerWait(uint16_t delay);
//! Wait for given time in fast timer ticks
//!
//!  @param delay Delay time in fast timer ticks
//!
//!  @note Limited to 2 ms
//!

//! Initialises the PIT for general purpose delays
//!
uint8_t timer_initialise(void);

#endif /* TIMER_H_ */
