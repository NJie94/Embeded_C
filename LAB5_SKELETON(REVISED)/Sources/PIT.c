/*
 * PIT.c
 *
 *  Created on: Oct 20, 2014
 *      Author: 100012429
 */

#include "derivative.h"
#include "clock_configure.h"
#include "utilities.h"
#include "ADC.h"
#include "menu.h"

#define SYSTEM_CLOCK_FREQUENCY      (48000000UL)
#define PIT_CLOCK_FREQUENCY         SYSTEM_CLOCK_FREQUENCY
#define PIT_TICKS_PER_MICROSECOND   (PIT_CLOCK_FREQUENCY/1000000)
#define PIT_TICKS_PER_MILLISECOND   (PIT_CLOCK_FREQUENCY/1000)

#define NOISE		 2
#define RISING_EDGE	 1
#define FALLING_EDGE 2
#define DEFAULT		 0

//=========================================================================
// Timer routines
//
//=========================================================================

/*! Initialises a PIT channel
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
volatile int firstBuffer[100];
volatile int secondBuffer[100];
volatile int start = 0; 
volatile int bufferStatus = 0;
volatile int sequenceCounter = 0;
volatile int risingEdge = 0;
volatile int fallingEdge = 0;


void initialisePIT(int channel, uint32_t interval) {

   // Enable clock to PIT
   SIM_SCGC6  |= SIM_SCGC6_PIT_MASK;
   // Enable PIT module
   PIT_MCR     = PIT_MCR_FRZ_MASK;
   // Set re-load value
   PIT_LDVAL(channel)  = (interval * PIT_TICKS_PER_MILLISECOND) - 1;
   // Enable this channel with interrupts
   PIT_TCTRL(channel)  = PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK;
   // Enable PIT0 interrupts in NVIC
   NVIC_EnableIrq(INT_PIT0+channel);
}

/**
 *  PIT Channel 0 Handler
 *  Toggles a pin
 */
void PIT_Ch0_IRQHandler(void) {
	// clear the interrupt request from PIT
	PIT_TFLG0 = PIT_TFLG_TIF_MASK;
	//check if trigger direction is a rising edge
	if(triggerDirection() == RISING_EDGE){
		if(((doADCConversion()*100)/1023) < (triggerThreshold() * 10) - NOISE){
			risingEdge = 1;
		}
		if(((doADCConversion()*10)/1023) >= triggerThreshold() && risingEdge == 1){
			start = 1;
		}
	}
	else{
		//check if trigger direction is a falling edge
		if(triggerDirection() == FALLING_EDGE){
			if(((doADCConversion()*100)/1023) > (triggerThreshold() * 10) + NOISE){
				fallingEdge = 1;
			}
			if(((doADCConversion()*10)/1023) <= (triggerThreshold() - 1) && fallingEdge == 1){
				start = 1;
			}
		}
	}
	//check if trigger direction is default
	if(triggerDirection() == DEFAULT){
		start = 1;
	}
	if(start == 1){
		//store data into array 1 for double buffering
		if(bufferStatus == 0){
			firstBuffer[sequenceCounter] = ((doADCConversion()*100)/1023);
			sequenceCounter++;
			if(sequenceCounter > 99){
				sequenceCounter = 0;
				bufferStatus = 1;
				start = 0;
				fallingEdge = 0;
				risingEdge =0;
			}
		}
		else{
			//store data into array 2 for double buffering
			if(bufferStatus == 1){
				secondBuffer[sequenceCounter] = ((doADCConversion()*100)/1023);
				sequenceCounter++;
				if(sequenceCounter > 99){
					sequenceCounter = 0;
					bufferStatus = 0;
					start = 0;
					fallingEdge = 0;
					risingEdge =0;
				}
			}
		}
	}
}
/*
 * return the bufferStatus 
 */
int status(void){
	return bufferStatus;
}
/*
 * return the data in the first array
 */
int DATA_1(int index){
	return firstBuffer[index];
}
/*
 * return the data in the second array
 */
int DATA_2(int index){
	return secondBuffer[index];
}
