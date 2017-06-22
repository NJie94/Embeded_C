/*
 *Demonstration program for LAB E3 PART 3
 */
#include "derivative.h" /*include peripheral declarations*/
#include "clock.h"
#include <stdio.h>

#define PIN1_MASK (1<<1) //D.1
#define PIN2_MASK (1<<2) //D.2
#define PIN3_MASK (1<<3) //D.3
#define PIN_MASK PIN1_MASK|PIN2_MASK|PIN3_MASK 

void initialiseCharlieplexing(void){
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; //enable clock to PORT D
	
	//make pins as simple pins with high drive strength
	PORTD_PCR1 = PORT_PCR_MUX(1)|PORT_PCR_DSE_MASK;
	PORTD_PCR2 = PORT_PCR_MUX(1)|PORT_PCR_DSE_MASK;
	PORTD_PCR3 = PORT_PCR_MUX(1)|PORT_PCR_DSE_MASK;
}

void setLED(int ledNum){
	//array of 6 output registers
	int PDOR_Array[6] = {	(GPIOD_PDOR|PIN_MASK)&~PIN3_MASK,
							(GPIOD_PDOR|PIN_MASK)&~PIN1_MASK,
							(GPIOD_PDOR|PIN_MASK)&~PIN2_MASK,
							(GPIOD_PDOR|PIN_MASK)&~PIN1_MASK,
							(GPIOD_PDOR|PIN_MASK)&~PIN3_MASK,
							(GPIOD_PDOR|PIN_MASK)&~PIN2_MASK
	};
	//array of 6 direction registers
	int PDDR_Array[6] = {	(GPIOD_PDDR|PIN_MASK)&~PIN2_MASK,
							(GPIOD_PDDR|PIN_MASK)&~PIN2_MASK,
							(GPIOD_PDDR|PIN_MASK)&~PIN3_MASK,
							(GPIOD_PDDR|PIN_MASK)&~PIN3_MASK,
							(GPIOD_PDDR|PIN_MASK)&~PIN1_MASK,
							(GPIOD_PDDR|PIN_MASK)&~PIN1_MASK
	};
	
	GPIOD_PDOR = PDOR_Array[(ledNum)]; //assign output registers
	GPIOD_PDDR = PDDR_Array[(ledNum)]; //assign direction registers
	                        
}

void initialiseADC (void){
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //enable ADC clock to PORT C
		
		PORTC_PCR0 = PORT_PCR_MUX(0); //set pins as ADC
		SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; //enable the ADC clock
		ADC0_SC2 = ADC_SC2_REFSEL(0); //set reference select to Vrefh & Vrefl
		ADC0_SC3 = 0;
		ADC0_CFG1 = ADC_CFG1_ADIV(2)|ADC_CFG1_MODE(2)|ADC_CFG1_ADICLK(0); //clock division by 4 with mode of 10 bits using buc clock (48MHz)
		ADC0_CFG2 = ADC_CFG2_MUXSEL_MASK; //select multiplexors
}

int doADCConversion(void){
	ADC0_SC1A = ADC_SC1_ADCH(19); //channel 14
	//wait for the conversion to complete
	while((ADC0_SC1A&ADC_SC1_COCO_MASK)==0){
		
	}
	return ADC0_RA; //return the value from the ADC after conversion
}

int main(void){
	clock_initialise(); //call out the function in clock.c
	initialiseADC(); //call out the function initialiseADC()
	initialiseCharlieplexing(); //call out the function initialiseCharlieplexing
	for(;;){
		int result = ((doADCConversion()*330)/1023); //do calculation for the voltage
		printf("The result is %d.%dV result = %d\n", result/100, result%100, result); // print the data on the terminal
		setLED((doADCConversion()/170)); // convert ADC value to 1-6 and assigned into the fucntion setLED(int ledNum)
	}
	return 0;
}
