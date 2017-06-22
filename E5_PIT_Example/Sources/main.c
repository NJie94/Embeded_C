/*
 =====================================================================================
 * main.c
 *
 *  Created on: 12/05/2013
 *      Author: podonoghue
 *      
 *  Simple demonstration of PIT interrupts
 *  
 ====================================================================================
 */

#include "derivative.h"
#include "clock.h"
#include "utilities.h"

#define SYSTEM_CLOCK_FREQUENCY      (48000000UL)
#define PIT_CLOCK_FREQUENCY         SYSTEM_CLOCK_FREQUENCY
#define PIT_TICKS_PER_MICROSECOND   (PIT_CLOCK_FREQUENCY/1000000)
#define PIT_TICKS_PER_MILLISECOND   (PIT_CLOCK_FREQUENCY/1000)

//=========================================================================
// Timer routines
//
//=========================================================================

/*! Initialises a PIT channel
 * 
 * @param channel  - channel (0-3) to configure
 * @param interval - PIT interval (re-load value+1)
 * 
 * Configures:
 *   - Enables PIT clock
 *   - Sets PIT re-load value
 *   - Enables PIT
 *   - Enables interrupts 
 */ 
void initialisePIT(int channel, uint32_t interval) {

   // Enable clock to PIT
   SIM_SCGC6  |= SIM_SCGC6_PIT_MASK;
   // Enable PIT module
   PIT_MCR     = PIT_MCR_FRZ_MASK;
   // Set re-load value
   PIT_LDVAL(channel)  = interval-1;
   // Enable this channel with interrupts
   PIT_TCTRL(channel)  = PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK;
   // Enable PIT0 interrupts in NVIC
   NVIC_EnableIrq(INT_PIT0+channel);
   // Set arbitrary priority level
   NVIC_SetIrqPriority(INT_PIT0, 8);
}

// Mask for pin to toggle
#define LED_MASK (1<<12)

/*!
 *  PIT Channel 0 Handler
 *  Toggles a pin
 */
void PIT_Ch0_IRQHandler(void) {
   // Toggle the pin
   GPIOA_PTOR = LED_MASK;
   // clear the interrupt request from PIT
   PIT_TFLG0 = PIT_TFLG_TIF_MASK;
}

int main(void) {

   initialiseClock();

   // Initialise PTA12
   SIM_SCGC5   |= SIM_SCGC5_PORTA_MASK;
   PORTA_GPCLR  = PORT_GPCLR_GPWE(LED_MASK)|PORT_PCR_MUX(1);  
   GPIOA_PDDR  |=  LED_MASK;

   // Configure PIT channel
   initialisePIT(0, 200*PIT_TICKS_PER_MICROSECOND);
   
   for(;;) {
      // Reduce power consumption between interrupts
      __asm("wfi");
   }
}
