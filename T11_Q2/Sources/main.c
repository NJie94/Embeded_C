/*
 ============================================================================
 * main.c
 *
 *  Created on: 04/12/2012
 *      Author: podonoghue
 ============================================================================
 */
#include <stdio.h>
#include "system.h"
#include "derivative.h"
#include "utilities.h"
#include "FTM_IC.h"

/*
 * FTM0.Ch6 = PTD6 = Arduino pin A2
 *
 */
#if 1
#define SENSOR_MASK (1<<6)
#define GPIO_FN (1)
#define FTM_FN  (4)

int main(void) {

//   SIM_SCGC5  |= SIM_SCGC5_PORTD_MASK;
//   PORTD_PCR6  = PORT_PCR_MUX(1);//|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
//   GPIOD_PDDR &= ~SENSOR_MASK;

//   printf("SystemBusClock  = %ld\n", SystemBusClock);
//   printf("SystemCoreClock = %ld\n", SystemCoreClock);
   
   startCapture();
   int lastValue = -1;
   // Real programs never die!
   for(;;) {
//      if ((GPIOD_PDIR&SENSOR_MASK) != 0) {
//         printf("High\n");
//      }
//      else {
//         printf("Low\n");
//      }
	   int currentValue = getPeriod();
	   if(currentValue != lastValue){
		   printf("Period = %d us\n", lastValue);
		   lastValue = currentValue;
	   }
   }
   return 0;
}

#else
int main() {

   printf("SystemBusClock  = %ld\n", SystemBusClock);
   printf("SystemCoreClock = %ld\n", SystemCoreClock);

   startCapture();
   for(;;) {
      printf("Period = %d us\n", getPeriod());
   }
}
#endif
