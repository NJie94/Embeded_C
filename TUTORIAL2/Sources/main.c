/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#define SW_MASK (1<<5)
#define LED_MASK (1<<1)

void delay1ms(void){
		int i;
		for (i=0; i<1000; i++) {
			asm("nop");
		}
}

void randomDelay(){
		int i;
		for(i=0;i<1000;i++){
				delay1ms();
		}
}
void testInput(void){
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK|SIM_SCGC5_PORTD_MASK;
		
		
		PORTC_PCR1 = PORT_PCR_MUX(1);
	    PORTD_PCR5 = PORT_PCR_MUX(1)|
	    		     PORT_PCR_PE_MASK | 
	    		     PORT_PCR_PS_MASK;
	    GPIOC_PCOR = LED_MASK;
	    GPIOC_PDDR |= LED_MASK;
	    GPIOD_PDDR &= ~SW_MASK;    
	    for(;;){
	    	GPIOC_PCOR = LED_MASK;
	    	randomDelay();
	    	GPIOC_PSOR = LED_MASK;
	    }
}

int main(void)
{
	testInput();
	return 0;
}
