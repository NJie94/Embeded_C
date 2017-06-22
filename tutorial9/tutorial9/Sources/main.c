/*
 ============================================================================
 * main.c
 *
 *  Created on: 04/12/2012
 *      Author: podonoghue
 ============================================================================
 */
#include <stdio.h>
#include "derivative.h"
#include "servo.h"
#include "clock.h"

// Simple delay - not for real programs!
void delay(void) {
   volatile unsigned long i;
   for (i=100000; i>0; i--) {
      __asm__("nop");
   }
}

int main(void) {

   clock_initialise();
   initServo();
   
   for(;;){
      int position;
      for(position=0; position<=100;position++) {
         setServoPosition(position);
         delay();
      }
      for(; position>=0;position--) {
         setServoPosition(position);
         delay();
      }
   }

   return 0;
}
