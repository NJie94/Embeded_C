#include "derivative.h" /* include peripheral declarations */
/*
 * Initialise bar graph system
 */
#define DATA_MASK (1<<1);
#define SCLOCK_MASK (1<<2);
#define SLATCH_MASK (1<<3);
void initialiseBargraph(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;//Clock on for port D
	
	//set up the 3 pin to drive the shift register 
	PORTD_PCR1 = PORT_PCR_MUX(1);//RED wire//  Serial DATA 
	PORTD_PCR2 = PORT_PCR_MUX(1);//GREEN wire// Serial Clock	
	PORTD_PCR3 = PORT_PCR_MUX(1);//BLUE wire//Register Clock		
	GPIOD_PDDR |=DATA_MASK ;// make  pin an output
    GPIOD_PDDR |=SLATCH_MASK ;// make pin an output
    GPIOD_PDDR |=SCLOCK_MASK;// make  pin an output
}
void bar_Graph(int sData)
{
	int i;
	for (i=0; i<8;i++){
		GPIOD_PCOR =SCLOCK_MASK ; //FALING EDGE
		if(i<sData) {
			GPIOD_PSOR = DATA_MASK;}//SET TO HIGH		   
		else {
			GPIOD_PCOR = DATA_MASK;}//CLEAR TO ZERO				 	 
	 GPIOD_PSOR = SCLOCK_MASK;// RISING EDGE
	}
GPIOD_PSOR = SLATCH_MASK;// LATCH VALUES TO LED
GPIOD_PCOR = SLATCH_MASK;// CLEAR THE LATCH
}
int main(void)
{
	initialiseBargraph();// Initialise all ports and pin required for function bar_Graph 
	bar_Graph(8); //calling function bar_Graph and assign a value to level			
	return 0; 
} 
