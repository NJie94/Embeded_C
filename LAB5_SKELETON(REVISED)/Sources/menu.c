/*
 * menu.c
 *
 *  Created on: Oct 22, 2014
 *      Author: 100012429
 */

#include "nokia_LCD.h"
#include "uart.h"
#include "clock_configure.h"
#include "Joystick.h"
/*
 * colour used
 */
#define FONT_COLOUR			SALMON
#define BACKGROUND_COLOUR	BLACK
/*
 * information used to navigate the cursor
 */
#define CURSOR_POSITION_X	10
#define CURSOR_RADIUS		2	
#define CURSOR__COLOUR		WHITE
#define FONT_HEIGHT			8
#define SPACING				20
#define MAX_HEIGHT			108
#define MIN_HEIGHT_1		48
#define MIN_HEIGHT_2		28
#define MIN_HEIGHT_3		68
/*
 * position of different menu
 */
#define MAIN_MENU_OPTION_1	108
#define MAIN_MENU_OPTION_2	88
#define MAIN_MENU_OPTION_3	68
#define MAIN_MENU_OPTION_4	48
#define MAIN_MENU_OPTION_5	28
/*
 * default value of sampling rate, trigger threshold and direction
 */
#define DEFAULT_RATE		3
#define DEFAULT_THRESHOLD	0
#define DEFAULT_DIRECTION	0
/*
 * various type of trigger direction
 */
#define RISING_EDGE	 1
#define FALLING_EDGE 2
#define DEFAULT		 0

/*
 * variable used in the DSO menu
 */
static int cursorPositionY = 100 + FONT_HEIGHT;
static int ENTER = 0;
static int COMPLETE = 0;
static int RATE = DEFAULT_RATE;
static int Threshold = 0;
static int thresholdDirection = 0;
/*
 * poll the value from getJoysticks
 * navigate the position of cursor
 * 
 * @param firstOption - the maximun height of the cursor
 * @param lastOption - the minimun height of the cursor
 *  
 */
void readJoystick(int firstOption, int lastOption){
	switch(getJoystick()){
		case NORTH_PRESS: 
			// check if the cursor is at the top of cells
			if(cursorPositionY == firstOption){
				break;
			}
			cursorPositionY += SPACING;
			break;
			// check if the cursor is at the bottom of cell
		case SOUTH_PRESS:
			if(cursorPositionY == lastOption){
				break;
			}
			cursorPositionY -= SPACING;
			break;
		case WEST_PRESS:
			break;
		case EAST_PRESS:
			break;
		case CENTRE_PRESS:
			ENTER = 1;
			break;
		default:
			break;
	}
}
/*
 * draw the cursor on the DSO screen
 */
void drawCursor(int positionY, int colour){
	lcd_drawCircle(CURSOR_POSITION_X, positionY, CURSOR_RADIUS, colour, FULLCIRCLE);
}
/*
 * menu used to poll the sampling rate from user
 * display the menu on the DSO screen
 */
void samplingRateMenu(){
	static int currentPositionY;
	static int lastPositionY;
	
	ENTER = 0;
	
	lcd_clear(BACKGROUND_COLOUR);
	lcd_putStr("1000 Hz", 20, 100, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
	lcd_putStr("500  Hz", 20, 80, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
	lcd_putStr("100  Hz", 20, 60, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
	lcd_putStr("50   Hz", 20, 40, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
	lcd_putStr("DEFAULT", 20, 20, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
	while(ENTER == 0){
		//update the cursor position
		currentPositionY = cursorPositionY;
		//draw the cursor
		drawCursor(currentPositionY, CURSOR__COLOUR);
		//wait until joystick is pressed
		while(getJoystick() == NO_PRESS){	
		}
		//setup the last position of cursor
		lastPositionY = currentPositionY;
		if(getJoystick() != NO_PRESS){
			readJoystick(MAX_HEIGHT, MIN_HEIGHT_2); // read the input from the joystick
		}
		if(ENTER == 1){
			switch(currentPositionY){
				case MAIN_MENU_OPTION_1:
					RATE = 1;
					break;
				case MAIN_MENU_OPTION_2:
					RATE = 5;
					cursorPositionY += SPACING;
					break;
				case MAIN_MENU_OPTION_3:
					RATE = 10;
					cursorPositionY += (2 * SPACING);
					break;
				case MAIN_MENU_OPTION_4:
					RATE = 20;
					cursorPositionY += (3 * SPACING);
					break;
				case MAIN_MENU_OPTION_5:
					RATE = DEFAULT_RATE;
					cursorPositionY += (4 * SPACING);
					break;
				default:
					break;
			}
		}
		//clear cursor
		drawCursor(lastPositionY, BACKGROUND_COLOUR);
	}
	lcd_clear(BACKGROUND_COLOUR);
}

/*
 * menu used to poll the trigger threshold from user
 * display the menu on the DSO screen
 */
void triggerThresholdMenu(){
	static int currentPositionY;
	static int lastPositionY;
	
	ENTER = 0;
	
	lcd_clear(BACKGROUND_COLOUR);
	lcd_putStr("8 units", 20, 100, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
	lcd_putStr("6 units", 20, 80, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
	lcd_putStr("4 units", 20, 60, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
	lcd_putStr("2 units", 20, 40, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
	lcd_putStr("DEFAULT", 20, 20, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
	while(ENTER == 0){
		//update the cursor position
		currentPositionY = cursorPositionY;
		//draw the cursor
		drawCursor(currentPositionY, CURSOR__COLOUR);
		//wait until joystick is pressed
		while(getJoystick() == NO_PRESS){	
		}
		//setup the last position of cursor
		lastPositionY = currentPositionY;
		if(getJoystick() != NO_PRESS){
			readJoystick(MAX_HEIGHT, MIN_HEIGHT_2); // read the input from the joystick
		}
		if(ENTER == 1){
			switch(currentPositionY){
				case MAIN_MENU_OPTION_1:
					Threshold = 8;
					cursorPositionY -= SPACING;
					break;
				case MAIN_MENU_OPTION_2:
					Threshold = 6;
					break;
				case MAIN_MENU_OPTION_3:
					Threshold = 4;
					cursorPositionY += SPACING;
					break;
				case MAIN_MENU_OPTION_4:
					Threshold = 2;
					cursorPositionY += (2 * SPACING);
					break;
				case MAIN_MENU_OPTION_5:
					Threshold = DEFAULT_THRESHOLD;
					cursorPositionY += (3 * SPACING);
					break;
				default:
					break;
			}
		}
		//clear cursor
		drawCursor(lastPositionY, BACKGROUND_COLOUR);
	}
	lcd_clear(BACKGROUND_COLOUR);
}

/*
 * menu used to poll the trigger direction from user
 * display the menu on the DSO screen
 */

void triggerDirectionMenu(){
	static int currentPositionY;
		static int lastPositionY;
		
		ENTER = 0;
		
		lcd_clear(BACKGROUND_COLOUR);
		lcd_putStr("RISING EDGE", 20, 100, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
		lcd_putStr("FALLING EDGE", 20, 80, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
		lcd_putStr("DEFAULT", 20, 60, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
		while(ENTER == 0){
			//update the cursor position
			currentPositionY = cursorPositionY;
			//draw the cursor
			drawCursor(currentPositionY, CURSOR__COLOUR);
			//wait until joystick is pressed
			while(getJoystick() == NO_PRESS){	
			}
			//setup the last position of cursor
			lastPositionY = currentPositionY;
			if(getJoystick() != NO_PRESS){
				readJoystick(MAX_HEIGHT, MIN_HEIGHT_3); // read the input from the joystick
			}
			if(ENTER == 1){
				switch(currentPositionY){
					case MAIN_MENU_OPTION_1:
						thresholdDirection = RISING_EDGE;
						cursorPositionY -= (2*SPACING);
						break;
					case MAIN_MENU_OPTION_2:
						thresholdDirection = FALLING_EDGE;
						cursorPositionY -= SPACING;
						break;
					case MAIN_MENU_OPTION_3:
						thresholdDirection = DEFAULT;
						break;
					default:
						break;
				}
			}
			//clear cursor
			drawCursor(lastPositionY, BACKGROUND_COLOUR);
		}
		lcd_clear(BACKGROUND_COLOUR);
}
/*
 * display the main menu on DSO screen
 * prompt user to set the sampling rate, trigger threshold and trigger direction of the DSO
 * settings will be set as DEFAULT if user chose not to change the settings
 */
void doMenu(){
	static int currentPositionY;
	static int lastPositionY;
	//clear the lcd screen
	lcd_clear(BACKGROUND_COLOUR);
	while(COMPLETE == 0){	
		ENTER = 0;
		//draw the menu
		lcd_putStr("Sampling Rate", 20, 100, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
		lcd_putStr("Threshold", 20, 80, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
		lcd_putStr("Direction", 20, 60, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
		lcd_putStr("Start", 20, 40, FontLarge, FONT_COLOUR, BACKGROUND_COLOUR);
		//update the cursor position
		currentPositionY = cursorPositionY;
		//draw the cursor
		drawCursor(currentPositionY, CURSOR__COLOUR);
		//wait until joystick is pressed
		while(getJoystick() == NO_PRESS){	
		}
		//setup the last position of cursor
		lastPositionY = currentPositionY;
		if(getJoystick() != NO_PRESS){
			readJoystick(MAX_HEIGHT, MIN_HEIGHT_1); // read the input from the joystick
		}
		//check if user presses enter
		if(ENTER == 1){
			switch(currentPositionY){
				case MAIN_MENU_OPTION_1:
					samplingRateMenu();
					break;
				case MAIN_MENU_OPTION_2:
					cursorPositionY += SPACING;
					triggerThresholdMenu();
					break;
				case MAIN_MENU_OPTION_3:
					cursorPositionY += (2 * SPACING);
					triggerDirectionMenu();
					break;
				case MAIN_MENU_OPTION_4:
					COMPLETE = 1; // start the DSO
					break;
				default:
					break;
			}
		}
		//clear cursor
		drawCursor(lastPositionY, BACKGROUND_COLOUR);
	}
}

/*
 * return the sampling rate of the DSO
 */
int samplingRate(){
	return RATE;
}
/*
 * return the trigger threshold of the DSO
 */
int triggerThreshold(){
	return Threshold;
}
/*
 * return the trigger direction of the DSO
 */
int triggerDirection(){
	return thresholdDirection;
}
