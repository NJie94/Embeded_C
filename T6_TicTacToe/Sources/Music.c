/*
 * Music.c
 *
 *  Created on: Sep 22, 2014
 *      Author: podonoghue
 */
#include "Music.h"
#include "derivative.h"
#include "utilities.h"

// System clock frequency
#define SYSTEM_CLOCK_FREQUENCY (48000000UL)

// Choice of prescale value (FTM1_SC.PS)
#define FTM0_PRESCALE_VALUE (1)
#define FTM0_PRESCALE (1<<FTM0_PRESCALE_VALUE)
#define FTM0_CLK_FREQUENCY (SYSTEM_CLOCK_FREQUENCY/FTM0_PRESCALE)
#define GROUND_MASK (1<<7)

/**
 * Does initialisation of the Music system
 * 
 * @node The node is output on FTM0.CH1 = PTA.4(Fn3)
 */
void initMusic(void) {
	//Initialise A.4(Alt3) = FTM0.Ch1, used to play music
	//Initialise D.7(Alt4) = FTM0. Ch7, used to control music tempo
	//Initialise C.7(Alt1) = GPIO PINS, used as ground
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //enable clock to PORT C
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; //enable clock to PORT A
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; //enable clock to PORT D
	PORTC_PCR7 = PORT_PCR_MUX(1); //set pin as GPIO pin
	// set pin as FTM pin
	PORTA_PCR4 = PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;
	PORTD_PCR7 = PORT_PCR_MUX(4)|PORT_PCR_DSE_MASK;
	// Enable clock to FTM
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	// Common registers
	FTM0_SC = FTM_SC_CLKS(0); 
	FTM0_CNTIN = 0; 
	FTM0_CNT = 0; 
	FTM0_MOD = 0xFFFF;
	// Channel registers - OC-Toggle
	
	GPIOC_PDDR |= GROUND_MASK;
	GPIOC_PCOR = GROUND_MASK;
	
	FTM0_C7SC = FTM_CnSC_CHIE_MASK|FTM_CnSC_MSA_MASK|FTM_CnSC_ELSA_MASK;
	FTM0_C7V = FTM0_CNT;// trigger at now+100
	FTM0_C1SC = FTM_CnSC_CHIE_MASK|FTM_CnSC_MSA_MASK|FTM_CnSC_ELSA_MASK;
	FTM0_C1V = FTM0_CNT + 100;// trigger at now+100
	FTM0_SC = FTM_SC_CLKS(1)|FTM_SC_PS(FTM0_PRESCALE_VALUE);
	// Enable FTM0 interrupts in NVIC
	NVIC_EnableIrq(INT_FTM0);
}
/*
 * definition frequency of nodes
 */ 
#define A6 	1760
#define B6 	1976
#define C6	2093
#define D6	2349
#define E6 	2637
#define F6 	2794
#define G6	3136
/*
 * structure named as nodes
 * contains int variable of nodeFrequency and nodeDuration
 */
typedef struct{
	int nodeFrequency;
	int nodeDuration;
}nodes;

/*
 * an array of size 37
 * contains nodes and tempo of the music
 */
nodes nodeArray[]={
		{C6,1}, {E6,1}, {F6,1}, {G6,4},
		{G6,1}, {C6,1}, {E6,1}, {F6,1}, {G6,4},
		{G6,1}, {C6,1}, {E6,1}, {F6,1}, {G6,2}, {E6,2},
		{C6,2}, {E6,2}, {D6,4},
		{D6,1}, {E6,1}, {E6,1}, {D6,1}, {C6,3}, {C6,1},
		{E6,2}, {G6,2}, {G6,1}, {F6,3},
		{F6,2}, {E6,1}, {F6,1}, {G6,2}, {E6,2},
		{C6,2}, {D6,2}, {C6,4},
		{C6,1}
};
volatile int nodeSpeed = 300; // music play speed
volatile int nodeIndex = 0; //index used to map the array nodeArray
volatile int nodeCounter = 0;
/**
 * FTM0 Interrupt handler
 * 
 * It is responsible for scheduling the node period interrupts
 * 
 * @node - The not toggling is done in hardware.
 */
void FTM0_IRQHandler(void) {
	if ((FTM0_STATUS & FTM_STATUS_CH7F_MASK) != 0) { // Clear the interrupt request from FTM0.Ch0 
		FTM0_STATUS = ~FTM_STATUS_CH7F_MASK; // Re-trigger at T+halfPeriod 
		FTM0_C7V += nodeArray[nodeIndex].nodeDuration*10000; //schedule next interrupt to play at the next tempo
		nodeCounter++;
		if(nodeCounter == nodeSpeed){
			nodeIndex++;
			nodeCounter = 0;
		}
		if(nodeIndex == 37){
			nodeIndex = 0;
		}
	}
	if ((FTM0_STATUS & FTM_STATUS_CH1F_MASK) != 0) { // Clear the interrupt request from FTM0.Ch0 
		FTM0_STATUS = ~FTM_STATUS_CH1F_MASK; // Re-trigger at T+halfPeriod 
		FTM0_C1V += ((SYSTEM_CLOCK_FREQUENCY/nodeArray[nodeIndex].nodeFrequency)/2); //schedule next interrupt to play next frequency
	}
}

void restartMusic(void){
	FTM0_C7SC = 0; //disable the FTM0.CH7
	FTM0_C7V = FTM0_CNT;// trigger at now+100
	FTM0_C1SC = 0; //disable the FTM0.CH1
	FTM0_C1V = FTM0_CNT + 100;// trigger at now+100
	nodeIndex = 0; //reset the music
	nodeCounter = 0; // reset the counter
}
