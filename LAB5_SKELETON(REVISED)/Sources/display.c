/*
 * display.c
 *
 *  Created on: Oct 20, 2014
 *      Author: 100012429
 */
#include "nokia_LCD.h"
#include "uart.h"
#include "clock_configure.h"
#include "display.h"
#include "PIT.h"

#define ORIGIN				15
#define GAP					20
#define LINE_COLOUR			BLACK
#define BACKGROUND_COLOUR	GRAY
/**
 * Draw empty background for DSO screen
 */
void drawBackground(){
	lcd_clear(BACKGROUND_COLOUR);
}
/**
 * Draw grid and axis labels for DSO
 */
void drawGrid(){
	//Horizontal line and x-axis labels
	lcd_drawLine(ORIGIN, ORIGIN, LCD_X_MAX, ORIGIN, LINE_COLOUR);
	lcd_drawLine(ORIGIN, ORIGIN + GAP, LCD_X_MAX, ORIGIN + GAP, LINE_COLOUR);
	lcd_drawLine(ORIGIN, ORIGIN + (2 * GAP), LCD_X_MAX, ORIGIN + (2 * GAP), LINE_COLOUR);
	lcd_drawLine(ORIGIN, ORIGIN + (3 * GAP), LCD_X_MAX, ORIGIN + (3 * GAP), LINE_COLOUR);
	lcd_drawLine(ORIGIN, ORIGIN + (4 * GAP), LCD_X_MAX, ORIGIN + (4 * GAP), LINE_COLOUR);
	lcd_drawLine(ORIGIN, ORIGIN + (5 * GAP), LCD_X_MAX, ORIGIN + (5 * GAP), LINE_COLOUR);
	lcd_putChar('0', 13, 2, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('2', 33, 2, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('4', 53, 2, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('6', 73, 2, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('8', 93, 2, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('1', 111, 2, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('0', 116, 2, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	
	//vertical line and y-axis labels
	lcd_drawLine(ORIGIN, ORIGIN, ORIGIN, LCD_Y_MAX, LINE_COLOUR);
	lcd_drawLine(ORIGIN + GAP, ORIGIN, ORIGIN + GAP, LCD_Y_MAX, LINE_COLOUR);
	lcd_drawLine(ORIGIN + (2 * GAP), ORIGIN, ORIGIN + (2 * GAP), LCD_Y_MAX, LINE_COLOUR);
	lcd_drawLine(ORIGIN + (3 * GAP), ORIGIN, ORIGIN + (3 * GAP), LCD_Y_MAX, LINE_COLOUR);
	lcd_drawLine(ORIGIN + (4 * GAP), ORIGIN, ORIGIN + (4 * GAP), LCD_Y_MAX, LINE_COLOUR);
	lcd_drawLine(ORIGIN + (5 * GAP), ORIGIN, ORIGIN + (5 * GAP), LCD_Y_MAX, LINE_COLOUR);
	lcd_putChar('0', 5, 13, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('2', 5, 33, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('4', 5, 53, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('6', 5, 73, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('8', 5, 93, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('1', 2, 111, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
	lcd_putChar('0', 7, 111, FontSmall, LINE_COLOUR, BACKGROUND_COLOUR);
}
/*
 * variable used for drawing the curve
 */
volatile int currentFunction[100];
volatile int lastFunction[100];

static int lastStatus = 0;
static int currentStatus = 0;


void drawData(int colour){
	int sequence; //sequence counter for array
	lastStatus = currentStatus;
	currentStatus = status();
	//update the function if the status changes
	if(lastStatus != currentStatus){
		for(sequence = 0; sequence < 100; sequence++){
			lastFunction[sequence] = currentFunction[sequence];
		}
		//draw the first curve when status is 1
		if(status() == 1){
			for(sequence = 1; sequence < 100; sequence++){
				//clear the last function
				lcd_drawLine((ORIGIN + sequence - 1), (ORIGIN + lastFunction[sequence - 1]),
							 (ORIGIN + sequence), (ORIGIN + lastFunction[sequence]),
							  BACKGROUND_COLOUR);
			}
			//redraw the grid
			drawGrid();
			for(sequence = 1; sequence < 100; sequence++){
				//set curve as current function
				currentFunction[sequence] = DATA_1(sequence);
				//draw the new function
				lcd_drawLine((ORIGIN + sequence - 1), (ORIGIN + DATA_1(sequence - 1)),
							 (ORIGIN + sequence), (ORIGIN + DATA_1(sequence)),
							  colour);
			}			
		}
		//draw the second curve when status is 0
		if(status() == 0){
			for(sequence = 1; sequence < 100; sequence++){
				//clear the last function
				lcd_drawLine((ORIGIN + sequence - 1), (ORIGIN + lastFunction[sequence - 1]),
							 (ORIGIN + sequence), (ORIGIN + lastFunction[sequence]),
							  BACKGROUND_COLOUR);
			}
			//redraw the grid
			drawGrid();
			for(sequence = 1; sequence < 100; sequence++){
				//set curve as current function
				currentFunction[sequence] = DATA_2(sequence);
				//draw the new function
				lcd_drawLine((ORIGIN + sequence - 1), (ORIGIN + DATA_2(sequence - 1)),
							 (ORIGIN + sequence), (ORIGIN + DATA_2(sequence)),
							  colour);
			}
		}
	}
}

