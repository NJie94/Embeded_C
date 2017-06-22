/*
 * PIT.h
 *
 *  Created on: Oct 20, 2014
 *      Author: 100012429
 */

#ifndef PIT_H_
#define PIT_H_

/** 
 * Initialises a PIT channel
 * 
 * @param channel  - channel (0-3) to configure
 * @param interval - PIT interval (re-load value+1)
 * 
 * Configures:
 *   - Enables PIT clock
 *   - Sets PIT re-load value
 *   - Enables PIT
 *   - Enables interrupts 
 */ 
void initialisePIT(int channel, uint32_t interval);

int status(void);
int DATA_1(int index);
int DATA_2(int index);
#endif /* PIT_H_ */
