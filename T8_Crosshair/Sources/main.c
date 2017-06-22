#include "derivative.h"
#include "clock.h"
#include <stdio.h>
#include "nokia_LCD.h"

#define CROSS_HAIR_RADIUS	10
#define CROSS_HAIR_COLOUR	SALMON
#define CROSS_HAIR_SIZE		12
#define BACKGROUND_COLOUR	BLUE

void initialiseADC (void){
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
		
		PORTC_PCR0 = PORT_PCR_MUX(0);
		SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
		ADC0_SC2 = ADC_SC2_REFSEL(0);
		ADC0_SC3 = 0;
		ADC0_CFG1 = ADC_CFG1_ADIV(1)|ADC_CFG1_MODE(0)|ADC_CFG1_ADICLK(0);
		ADC0_CFG2 = ADC_CFG2_MUXSEL_MASK;
}

int doADCConversion14(void){
	ADC0_SC1A = ADC_SC1_ADCH(14);
	while((ADC0_SC1A&ADC_SC1_COCO_MASK)==0){
	
	}
	return ADC0_RA;
}

int doADCConversion15(void){
	ADC0_SC1A = ADC_SC1_ADCH(15);
	while((ADC0_SC1A&ADC_SC1_COCO_MASK)==0){
			
	}
	return ADC0_RA;
}

void drawCrossHair(int x, int y){
	//DRAW CIRCLE
	lcd_drawCircle(x, y, CROSS_HAIR_RADIUS, CROSS_HAIR_COLOUR, FULLCIRCLE);
	//lcd_drawCircle(x, y, CROSS_HAIR_RADIUS + 1, CROSS_HAIR_COLOUR, FULLCIRCLE);
	//lcd_drawCircle(x, y, CROSS_HAIR_RADIUS - 1, CROSS_HAIR_COLOUR, FULLCIRCLE);
	//DRAW HORIZONTAL LINE
	lcd_drawLine(x - CROSS_HAIR_SIZE, y, x + CROSS_HAIR_SIZE, y, CROSS_HAIR_COLOUR);
	//lcd_drawLine(x - CROSS_HAIR_SIZE, y + 1, x + CROSS_HAIR_SIZE, y + 1, CROSS_HAIR_COLOUR);
	//lcd_drawLine(x - CROSS_HAIR_SIZE, y - 1, x + CROSS_HAIR_SIZE, y - 1, CROSS_HAIR_COLOUR);
	//DRAW VERTICAL LINE
	lcd_drawLine(x, y - CROSS_HAIR_SIZE, x, y + CROSS_HAIR_SIZE, CROSS_HAIR_COLOUR);
	//lcd_drawLine(x + 1, y - CROSS_HAIR_SIZE, x + 1, y + CROSS_HAIR_SIZE, CROSS_HAIR_COLOUR);
	//lcd_drawLine(x - 1, y - CROSS_HAIR_SIZE, x - 1, y + CROSS_HAIR_SIZE, CROSS_HAIR_COLOUR);
}

void clearCrossHair(int x, int y){
	//DRAW CIRCLE
	lcd_drawCircle(x, y, CROSS_HAIR_RADIUS, BACKGROUND_COLOUR, FULLCIRCLE);
	//lcd_drawCircle(x, y, CROSS_HAIR_RADIUS + 1, BACKGROUND_COLOUR, FULLCIRCLE);
	//lcd_drawCircle(x, y, CROSS_HAIR_RADIUS - 1, BACKGROUND_COLOUR, FULLCIRCLE);
	//DRAW HORIZONTAL LINE
	lcd_drawLine(x - CROSS_HAIR_SIZE, y, x + CROSS_HAIR_SIZE, y, BACKGROUND_COLOUR);
	//lcd_drawLine(x - CROSS_HAIR_SIZE, y + 1, x + CROSS_HAIR_SIZE, y + 1, BACKGROUND_COLOUR);
	//lcd_drawLine(x - CROSS_HAIR_SIZE, y - 1, x + CROSS_HAIR_SIZE, y - 1, BACKGROUND_COLOUR);
	//DRAW VERTICAL LINE
	lcd_drawLine(x, y - CROSS_HAIR_SIZE, x, y + CROSS_HAIR_SIZE, BACKGROUND_COLOUR);
	//lcd_drawLine(x + 1, y - CROSS_HAIR_SIZE, x + 1, y + CROSS_HAIR_SIZE, BACKGROUND_COLOUR);
	//lcd_drawLine(x - 1, y - CROSS_HAIR_SIZE, x - 1, y + CROSS_HAIR_SIZE, BACKGROUND_COLOUR);
}
int main(void){
	clock_initialise();
	initialiseADC();
	lcd_initialise();
	lcd_clear(BACKGROUND_COLOUR);
	int tempX;
	int tempY;
	for(;;){
		tempX = (doADCConversion15()/2);
		tempY = (doADCConversion14()/2);
		drawCrossHair(tempX, tempY);
		while(tempX == (doADCConversion15()/2)|| tempY == (doADCConversion14()/2) ){
			
		}
		clearCrossHair(tempX, tempY);
		tempX = (doADCConversion15()/2);
		tempY = (doADCConversion14()/2);
	}
	return 0;
}
