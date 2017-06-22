/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "derivative.h" /* include peripheral declarations */

#define LED_MASK (1<<1)
#define BUTTON_MASK (1<<0)


void doDot(void){
	
	GPIOC_PSOR = LED_MASK;
	delayDot();
	GPIOC_PCOR = LED_MASK;
	delayDot();
	
}

void doDash(void){
	
	GPIOC_PSOR = LED_MASK;
	delayDot();
	delayDot();
	delayDot();
	GPIOC_PCOR = LED_MASK;
	delayDot();
	
}

void sendS(void){
	
	doDot();
	doDot();
	doDot();
	delayDot();
	delayDot();
}

void sendO(void){
	
	doDash();
	doDash();
	doDash();
	delayDot();
	delayDot();
	
}

void sendSOS (void){
	sendS();
	sendO();
	sendS();
	delayDot();
	delayDot();
}

int main(void){
		//enable clock to PORTC
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
		
		//set PORT C.0 as simple pins with pull ups
		PORTC_PCR0 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
		
		//set PORT C.1 as simple pins with high drive strength
		PORTC_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
		
		//set PORT C.1 as outputs
		GPIOC_PDDR |= LED_MASK;
		
		//set PORTC.0 as inputs
		GPIOC_PDDR &= ~BUTTON_MASK;
		
		GPIOC_PCOR = LED_MASK;
	for(;;) {
		// wait for button to be pressed
		while((GPIOC_PDIR & BUTTON_MASK) != 0){
		}
		//wait for button to be released
		while((GPIOC_PDIR & BUTTON_MASK) == 0){
		}
		sendSOS();
	}
	return 0;
}
