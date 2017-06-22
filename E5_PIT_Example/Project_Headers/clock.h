/*
 * clock.h
 *
 *  Created on: Nov 6, 2012
 *      Author: podonoghue
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t const SystemCoreClock;
extern uint32_t const SystemBusClock;

void initialiseClock(void);

#ifdef __cplusplus
}
#endif
#endif /* CLOCK_H_ */
