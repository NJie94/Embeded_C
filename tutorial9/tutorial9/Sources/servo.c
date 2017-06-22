/*
 * pwm.c
 *
 *  Created on: 14 Oct 2014
 *      Author: podonoghue
 */

#include "derivative.h"
#include "clock.h"

// System clock frequency
#define SYSTEM_CLOCK_FREQUENCY (48000000UL)

// Choice of prescale value (FTM0_SC.PS)
#define FTM0_PRESCALE_VALUE (4)
#define FTM0_PRESCALE (1<<FTM0_PRESCALE_VALUE)
#define FTM0_CLK_FREQUENCY (SYSTEM_CLOCK_FREQUENCY/FTM0_PRESCALE)
#define ONE_MILLISECOND (FTM0_CLK_FREQUENCY/1000)
#define PWM_PERIOD (20 * ONE_MILLISECOND)


/**
 * Initialises the servo motor
 * Initial position is middle (50%)
 */

void initServo() {
	//set pin to pwm
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTD_PCR6 = PORT_PCR_MUX(4)|PORT_PCR_DSE_MASK;
	
	// Enable clock to FTM
	 SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	 
	 // Common registers
	 FTM0_SC = FTM_SC_CLKS(0); // Disable FTM so changes are immediate
	 FTM0_CNTIN = 0;
	 FTM0_CNT = 0;
	 FTM0_MOD = PWM_PERIOD - 1;
	 
	 // High-true PWM pulses
	 FTM0_CnSC(6) = FTM_CnSC_MSB_MASK|FTM_CnSC_ELSB_MASK; 
	 
	 // Left aligned PWM since CPWMS not selected 
	 FTM0_SC = FTM_SC_CLKS(1)|FTM_SC_PS(FTM0_PRESCALE_VALUE);
}

/**
 * Set servo position
 *
 * @param position Position to set (0-100)
 *
 */
void setServoPosition(int position) {
	 // PWM pulse width
	 FTM0_CnV(6) = ((ONE_MILLISECOND * position)/100) + ONE_MILLISECOND;
}

