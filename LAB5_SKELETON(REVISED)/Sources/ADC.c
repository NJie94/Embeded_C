/*
 * ADC.c
 *
 *  Created on: Oct 20, 2014
 *      Author: 100012429
 */
#include "derivative.h"
#include "clock_configure.h"
#include <stdio.h>

#define GROUND_MASK (1<<2)

void initialiseADC (void){
	//enable clock to ADC and port B
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;	
	//set PORT B.2 as ground
	PORTB_PCR2 = PORT_PCR_MUX(1);
	GPIOB_PDDR |= GROUND_MASK;
	GPIOB_PCOR = GROUND_MASK;
	//set PORT B.3 as ADC
	PORTB_PCR3 = PORT_PCR_MUX(0);
	ADC0_SC2 = ADC_SC2_REFSEL(0);
	ADC0_SC3 = 0;
	ADC0_CFG1 = ADC_CFG1_ADIV(0)|ADC_CFG1_MODE(2)|ADC_CFG1_ADICLK(0);
	ADC0_CFG2 = ADC_CFG2_MUXSEL_MASK;
}
/*
 * do ADC conversion
 * return the value from ADC
 */
int doADCConversion(void){
	ADC0_SC1A = ADC_SC1_ADCH(13);
	while((ADC0_SC1A&ADC_SC1_COCO_MASK)==0){
		
	}
	return ADC0_RA;
}


