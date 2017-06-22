/*
 * main implementation for Tutorial 
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "utilities.h"
#include "clock.h"

// System clock frequency
#define SYSTEM_CLOCK_FREQUENCY (48000000UL)
// Choice of prescale value (FTM0_SC.PS)
#define FTM0_PRESCALE_VALUE (4)
#define FTM0_PRESCALE (1<<FTM0_PRESCALE_VALUE)
#define FTM0_CLK_FREQUENCY (SYSTEM_CLOCK_FREQUENCY/FTM0_PRESCALE)
#define ONE_MILLISECOND (FTM0_CLK_FREQUENCY/1000)

static uint32_t firstEdge = 0;
static uint32_t lastEdge = 0;
static int completeRoutine =0;

void initialiseFTM0(void){
	//Initialise FTM0.CH0 and FTM0.CH1 to drive pins
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTC_MASK;
	
	PORTA_PCR4 = PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;
	PORTC_PCR1 = PORT_PCR_MUX(4)|PORT_PCR_DSE_MASK;
	
	//enable clock to FTM0
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	
	//common registers
	FTM0_SC = FTM_SC_CLKS(0);
	FTM0_CNTIN = 0;
	FTM0_CNT = 0;
	FTM0_MOD = 0XFFFF;
	
	//channel registers - IC - rising edge
	FTM0_C0SC = FTM_CnSC_CHIE_MASK|FTM_CnSC_ELSA_MASK;
	
	FTM0_C1SC = FTM_CnSC_CHIE_MASK|FTM_CnSC_ELSA_MASK;
		
	FTM0_SC = FTM_SC_CLKS(1)|FTM_SC_PS(FTM0_PRESCALE_VALUE);
	
	NVIC_EnableIrq(INT_FTM0);
}

void FTM0_IRQHandler(void){
	//the pin is toggled by hardware
	if((FTM0_STATUS & FTM_STATUS_CH0F_MASK) != 0){
		//clear the interrupt request from FTM0.CH0
		FTM0_STATUS =~FTM_STATUS_CH0F_MASK;
		//Re-trigger at given period
		firstEdge = FTM0_C0V;
		completeRoutine = 0;
	}
	if((FTM0_STATUS &~(FTM_STATUS_CH0F_MASK)) != 0){
		//unexpected interrupt - die here for debug
		__asm__("bkpt");
	}
	if((FTM0_STATUS & FTM_STATUS_CH1F_MASK) != 0){
		//clear the interrupt request from FTM0.CH1
		FTM0_STATUS =~FTM_STATUS_CH1F_MASK;
		//Re-trigger at given period
		lastEdge = FTM0_C1V;
		completeRoutine = 1;
	}
	if((FTM0_STATUS &~(FTM_STATUS_CH1F_MASK)) != 0){
		//unexpected interrupt - die here for debug
		__asm__("bkpt");
	}
}

int measure(void){
	return (lastEdge - firstEdge);
}
int main(void)
{
	clock_initialise();
	for(;;){
		if(complete != 0){
			printf("Period = %d\n", measure());
			lastPrintedValue = measure();
		}
	}
}
