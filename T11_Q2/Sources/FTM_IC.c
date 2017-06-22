/*
 * IC.c
 *
 *  Created on: 28 Oct 2014
 *      Author: podonoghue
 */

#include <stdint.h>

#include "derivative.h"
#include "utilities.h"
#include "FTM_IC.h"

/*
 * FTM0.Ch6 = PTD6 = Arduino pin A2
 *
 */
#define SENSOR_MASK (1<<6)
#define GPIO_FN (1)
#define FTM_FN  (4)

// System clock frequency
#define SYSTEM_CLOCK_FREQUENCY (48000000UL)

// Choice of prescale value (FTM0_SC.PS)
#define FTM0_PRESCALE_VALUE   (7)
#define FTM0_PRESCALE         (1<<FTM0_PRESCALE_VALUE)
#define FTM0_CLK_FREQUENCY    (SYSTEM_CLOCK_FREQUENCY/FTM0_PRESCALE)
#define ONE_MILLISECOND       (FTM0_CLK_FREQUENCY/1000)

#if (100*ONE_MILLISECOND>65535)
#error "WAVEFORM_HALFPERIOD is too large"
#endif

uint16_t period;

volatile uint16_t currentEdge = 0;
volatile uint16_t lastEdge = 0;
volatile uint16_t periodTicks = 0;

void startCapture() {
	//Initialise FTM0.CH3 to drive pin
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTD_PCR6 = PORT_PCR_MUX(4)|PORT_PCR_DSE_MASK;
	
	//enable clock to FTM0
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	
	//common registers
	FTM0_SC = FTM_SC_CLKS(0);
	FTM0_CNTIN = 0;
	FTM0_CNT = 0;
	FTM0_MOD = 0XFFFF;
	
	//channel registers - OC-Toggle
	FTM0_C6SC = FTM_CnSC_CHIE_MASK|FTM_CnSC_ELSA_MASK;
	
	FTM0_SC = FTM_SC_CLKS(1)|FTM_SC_PS(FTM0_PRESCALE_VALUE);
	
	NVIC_EnableIrq(INT_FTM0);
}

int getPeriod() {
	int waveform = 0;
	waveform = ((periodTicks * 1000) / ONE_MILLISECOND);
	return waveform;
}

void FTM0_IRQHandler(void) {
	if((FTM0_STATUS & FTM_STATUS_CH6F_MASK) != 0){
		//clear the interrupt request from FTM0.CH0
		FTM0_STATUS =~FTM_STATUS_CH6F_MASK;
		//Re-trigger at given period
		lastEdge = currentEdge;
		currentEdge = FTM0_C6V;
		periodTicks = (currentEdge - lastEdge);
	}	
}
