/*
 * timer.h
 *
 *  Created on: 12/05/2013
 *      Author: podonoghue
 */

#ifndef TIMER_H_
#define TIMER_H_

/** 
 * Wait for given time in milliseconds
 *
 *  @param delay Delay time in milliseconds
 *  
 *  @note uses PIT CH0
 */
void millisecondTimerWait(uint16_t delay);

#endif /* TIMER_H_ */
