/*!
 * ============================================================================
 *  @file
 *
 *  @brief  TicTacToe for Nokia LCD with FRDM-MK20 board
 *
 * TicTacToe.c
 *
 *  Created on: 04/12/2012
 *      Author: podonoghue
 * ============================================================================
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "derivative.h"
#include "timer.h"
#include "nokia_LCD.h"
#include "Joystick.h"
#include "TicTacToe.h"
#include "Music.h"

/*
 * ================================================================================
 *   TicTacToe LCD routines
 */

/**
 * Screen placement
 */
#define NOUGHT_RADIUS            10
#define NOUGHT_THICKNESS          3
#define CROSS_THICKNESS           3
#define ORIGIN_X                 ((LCD_X_MIN+LCD_X_MAX)/2)
#define ORIGIN_Y                 ((LCD_Y_MIN+LCD_Y_MAX)/2)

#define SPACING                   40
#define BOARD_SIZE               ((LCD_X_MAX-LCD_X_MIN)-15)

/**
 * Colour scheme
 */
#define BOARD_COLOUR             WHITE
#define LINE_COLOUR              BLACK
#define PLAYER1_COLOUR           EMERALD
#define PLAYER2_COLOUR			 PURPLE
#define ACTIVE_CELL_BOX_COLOUR   BLUE
#define WIN_COLOUR				 SKYBLUE
#define LOSE_COLOUR				 RED
#define	DRAW_COLOUR				 SALMON

/*
 * size of nought and crosses
 */
#define PLAYER_SIZE				 18	

/**
 * Draws the board background
 */
void drawBoardBackground(void) {
	lcd_clear(BOARD_COLOUR);
}

/**
 * Draws the board background with grid
 */
void drawBoardGrid(void) {
	lcd_drawLine(LCD_X_MIN, ORIGIN_Y+SPACING/2, LCD_X_MAX,ORIGIN_Y+SPACING/2, LINE_COLOUR);
	lcd_drawLine(LCD_X_MIN, ORIGIN_Y-SPACING/2, LCD_X_MAX,ORIGIN_Y-SPACING/2, LINE_COLOUR);
	lcd_drawLine(ORIGIN_X-SPACING/2, LCD_Y_MIN, ORIGIN_X-SPACING/2, LCD_Y_MAX, LINE_COLOUR);
	lcd_drawLine(ORIGIN_X+SPACING/2, LCD_Y_MIN, ORIGIN_X+SPACING/2, LCD_Y_MAX, LINE_COLOUR);
}

/**
 * Draws a nought cell
 *
 * @param x - Cell in X direction
 * @param y - Cell in Y direction
 *
 * @note Contents of the cell are cleared
 */
void drawNought(int x, int y, int colour) {
	lcd_drawCircle(x, y, PLAYER_SIZE, colour, FULLCIRCLE);
}

/**
 * Draws a cross cell
 *
 * @param x - Cell in X direction
 * @param y - Cell in Y direction
 *
 * @note Contents of the cell are cleared
 */
void drawCross(int x, int y, int colour) {
	lcd_drawLine(x - PLAYER_SIZE,y + PLAYER_SIZE, x + PLAYER_SIZE, y - PLAYER_SIZE, colour); 
	lcd_drawLine(x - PLAYER_SIZE,y - PLAYER_SIZE, x + PLAYER_SIZE, y + PLAYER_SIZE, colour);
}

typedef enum {EMPTY, CROSS_PLAYER, NOUGHT_PLAYER, GAME_DRAWN} PlayerType;

/*
* Structure representing the game
*/
typedef struct {
	PlayerType gameStatus;
	int xCoordinates;
	int yCoordinates;
	/* Put your stuff here */
} GamesType;

GamesType game[3][3] = {	
		{{EMPTY, ORIGIN_X - SPACING, ORIGIN_Y - SPACING},{EMPTY, ORIGIN_X, ORIGIN_Y - SPACING},{EMPTY, ORIGIN_X + SPACING, ORIGIN_Y - SPACING}},
		{{EMPTY, ORIGIN_X - SPACING, ORIGIN_Y},{EMPTY,ORIGIN_X,ORIGIN_Y},{EMPTY,ORIGIN_X + SPACING, ORIGIN_Y}},
		{{EMPTY, ORIGIN_X - SPACING, ORIGIN_Y + SPACING},{EMPTY, ORIGIN_X, ORIGIN_Y + SPACING},{EMPTY, ORIGIN_X + SPACING, ORIGIN_Y + SPACING}}
};

/*
 * a structure named PositionType 
 * contains variable of currentRow and currentColumn indicating the current position of players
 */
typedef struct{
	int currentRow;
	int currentColumn;
}PositionType;

/**
 * Draws the board players
 *
 * @param game The game with board information
 */
PositionType currentPosition = {1,1};
void drawPlayers(void) {
	int row;
	int column;
	lcd_drawRect(game[currentPosition.currentColumn][currentPosition.currentRow].xCoordinates + PLAYER_SIZE, 
				 game[currentPosition.currentColumn][currentPosition.currentRow].yCoordinates + PLAYER_SIZE, 
				 game[currentPosition.currentColumn][currentPosition.currentRow].xCoordinates - PLAYER_SIZE,
				 game[currentPosition.currentColumn][currentPosition.currentRow].yCoordinates - PLAYER_SIZE, 
				 0,
				 ACTIVE_CELL_BOX_COLOUR); // draw the cursor indicating the position of player
	//redraw the game status for every cell
	for(column = 0; column <3; column ++)
		for(row = 0; row <3; row ++)
		{
			if(game[column][row].gameStatus == CROSS_PLAYER)
			{
				drawCross(game[column][row].xCoordinates, game[column][row].yCoordinates, PLAYER2_COLOUR);
			}
			if(game[column][row].gameStatus == NOUGHT_PLAYER)
			{
				drawNought(game[column][row].xCoordinates, game[column][row].yCoordinates, PLAYER1_COLOUR);
			}
		}
}

/*
 * Logic for Machine player
 *
 * Finds an unoccupied cell and places the piece.
 *
 * @param board The 3x3 board being played on
 * @param piece The value to place in an unoccupied cell
 */
void machineAI(int piece) {
	
	int xMap[] = {1, 0, 2};
	int yMap[] = {1, 2, 0};
	int x;
	int y;
   // Find empty cell and occupy
   for (x = 0; x < 3; x ++) {
      for (y = 0; y < 3; y ++) {
         if (game[xMap[x]][yMap[y]].gameStatus == EMPTY) {
        	 game[xMap[x]][yMap[y]].gameStatus = piece;
            return;
         }
      }
   }
}

/*
 * a function named compareStatus to compare the game status
 * @param statOne first game status to compare
 * @param statTwo second game status to compare
 * @param statThree third game status to compare
 * return True when all three status are the same
 * return False when not all three status are the same
 */
BOOL compareStatus(PlayerType statOne, PlayerType statTwo, PlayerType statThree){
	if((statOne == statTwo) && (statOne == statThree)){
		return TRUE;
	}
	return FALSE;
}

/*
 * a function named checkWinner to check for the winner
 * check every rows for winner
 * check every columns for winner
 * check both diagonals for winner
 * draw line connecting the winner's nought or crosses
 * return False when there is no winner
 * return True when there is winner
 */
BOOL checkWinner(void){
	int index;
	//check winner for every column
	for(index = 0; index < 3; index ++){
		if(game[index][0].gameStatus != EMPTY){
			if(compareStatus(game[index][0].gameStatus, game[index][1].gameStatus, game[index][2].gameStatus)){
				lcd_drawLine(game[index][0].xCoordinates - PLAYER_SIZE, 
							 game[index][0].yCoordinates,
							 game[index][2].xCoordinates + PLAYER_SIZE,
							 game[index][2].yCoordinates,
							 LINE_COLOUR); // draw the line connecting the winning naughts or crosses
				return TRUE;
			}
		}
	}
	//check winner for every row
	for(index = 0; index < 3; index ++){
		if(game[0][index].gameStatus != EMPTY){
			if(compareStatus(game[0][index].gameStatus, game[1][index].gameStatus, game[2][index].gameStatus)){
				lcd_drawLine(game[0][index].xCoordinates,
							 game[0][index].yCoordinates - PLAYER_SIZE,
							 game[2][index].xCoordinates,
							 game[2][index].yCoordinates + PLAYER_SIZE,
							 LINE_COLOUR);  // draw the line connecting the winning noughts or crosses
				return TRUE;
			}
		}
	}
	//check winner for first diagonals
	if(game[0][0].gameStatus != EMPTY){
		if(compareStatus(game[0][0].gameStatus, game[1][1].gameStatus, game[2][2].gameStatus)){
			lcd_drawLine(game[0][0].xCoordinates - PLAYER_SIZE,
						 game[0][0].yCoordinates - PLAYER_SIZE,
						 game[2][2].xCoordinates + PLAYER_SIZE,
						 game[2][2].yCoordinates + PLAYER_SIZE,
						 LINE_COLOUR);  // draw the line connecting the winning naughts or crosses
			return TRUE;
		}
	}
	//check winner for second diagonals
	if(game[0][2].gameStatus != EMPTY){
		if(compareStatus(game[0][2].gameStatus, game[1][1].gameStatus, game[2][0].gameStatus)){
			lcd_drawLine(game[0][2].xCoordinates + PLAYER_SIZE,
						 game[0][2].yCoordinates - PLAYER_SIZE,
						 game[2][0].xCoordinates - PLAYER_SIZE,
						 game[2][0].yCoordinates + PLAYER_SIZE,
						 LINE_COLOUR);
			return TRUE;
		}
	}
	return FALSE;
}
/*
 * a function named checkDraw to check if the game is drawn
 * check if all the cells are filled with crosses and noughts
 * return TRUE when cells full
 * return FALSE when cells are not full
 */
BOOL checkDraw(void)
{
	int row;
	int column;
	int drawCount = 0;
	for(column = 0; column < 3; column ++)
		for(row = 0; row < 3; row ++){
			if(game[column][row].gameStatus != EMPTY){
				drawCount++;
			}
		}
	if(drawCount == 9){
		return TRUE; 
	}
	return FALSE;
}

/*
 * a structure contain the winnerType
 * @param NONE no winner
 * @param PLAYER player won
 * @param AI AI won
 */
typedef enum {NONE, PLAYER, AI} winnerType;
winnerType playerWON = NONE;

/*
 * a function called readJoystick to read the input from joystick and move the cursor accordingly
 */
void readJoystick(){
	switch(getJoystick()){
		case NORTH_PRESS: 
			// check if the cursor is at the top of cells
			if(currentPosition.currentColumn == 2){
				break;
			}
			currentPosition.currentColumn ++;
			break;
			// check if the cursor is at the bottom of cell
		case SOUTH_PRESS:
			if(currentPosition.currentColumn == 0){
				break;
			}
			currentPosition.currentColumn --;
			break;
		case WEST_PRESS:
			// check if the cursor is at the west side of cell
			if(currentPosition.currentRow == 0){
				break;
			}
			currentPosition.currentRow --;
			break;
		case EAST_PRESS:
			// check if the cursor is at the east of cell
			if(currentPosition.currentRow == 2){
				break;get
			}
			currentPosition.currentRow ++;
			break;
		case CENTRE_PRESS:
			//check if there is a winner or the game is drawn
			if(checkWinner() == 0 || checkDraw() == 0){
				if (game[currentPosition.currentColumn][currentPosition.currentRow].gameStatus == EMPTY) // check if the current cell is empty
				{
					game[currentPosition.currentColumn][currentPosition.currentRow].gameStatus = NOUGHT_PLAYER;//place noughts for player
					if(checkWinner() != 0){
						playerWON = PLAYER; 
						break;
					}
					if(checkDraw() != 0){
						break;
					}
					machineAI(CROSS_PLAYER); // AI's turn to play
					if(checkWinner() != 0){
						playerWON = AI;
						break;
					}
				}
			}
			break;
		default:
			break;
	}
}

/*
 * a function called doMenu to print message on screen
 * ask user for the order of the game
 */
void doMenu(void)
{
	lcd_putStr("PRESS NORTH FOR",ORIGIN_X - 44, ORIGIN_Y + SPACING - 4, FontSmall, LINE_COLOUR, BOARD_COLOUR);
	lcd_putStr("MACHINE FIRST",ORIGIN_X - 38, ORIGIN_Y + (SPACING/2) - 4, FontSmall, LINE_COLOUR, BOARD_COLOUR);
	lcd_putStr("TIC TAC TOE",ORIGIN_X - 30, ORIGIN_Y - 4, FontSmall, LINE_COLOUR, WIN_COLOUR);
	lcd_putStr("PRESS SOUTH FOR",ORIGIN_X - 44, ORIGIN_Y - (SPACING/2) - 4, FontSmall, LINE_COLOUR, BOARD_COLOUR);
	lcd_putStr("HUMAN FIRST",ORIGIN_X - 36, ORIGIN_Y - (SPACING) - 4, FontSmall, LINE_COLOUR, BOARD_COLOUR);
}

/*
 * =================================================================================
 *    TicTacToe Game logic
 */
volatile int playerOrder = 0;
void playNoughtsAndCrosses(PlayOrderType choice){ 
	int x;
	int y;
	drawBoardBackground();
	drawBoardGrid();
	drawPlayers();
	//check if AI go first
	while(choice == machineFirst && playerOrder == 0){
		machineAI(CROSS_PLAYER);
		drawPlayers();
		playerOrder ++;
	}
	while(getJoystick() == NO_PRESS) //wait until joystick is pressed
	{
	}
	if(getJoystick() != NO_PRESS)
	{
		readJoystick(choice); // read the input from the joystick
	}
	while(getJoystick() != NO_PRESS) // wait until the joystick is released
	{
	}
	while(checkWinner() != 0 || checkDraw() != 0) //check if there is a winner or game is drawn
	{
		drawPlayers();
		initMusic(); // initialise and play the music
		//print if player wins the game and ask for game restart
		if(playerWON == PLAYER){
			lcd_putStr("YOU WON",ORIGIN_X - 28,ORIGIN_Y - 8, FontLarge, LINE_COLOUR, WIN_COLOUR);
			lcd_putStr("ENTER TO RESTART",ORIGIN_X - 48,ORIGIN_Y -SPACING - 4, FontSmall, LINE_COLOUR, WIN_COLOUR);
		}
		//print if player lose the game and ask for game restart
		if(playerWON == AI){
			lcd_putStr("YOU LOST",ORIGIN_X - 32,ORIGIN_Y - 8, FontLarge, LINE_COLOUR, LOSE_COLOUR);
			lcd_putStr("ENTER TO RESTART",ORIGIN_X - 48,ORIGIN_Y -SPACING - 4, FontSmall, LINE_COLOUR, LOSE_COLOUR);
		}
		//print if the game is drawn and ask for game restart
		if(playerWON == NONE){
			lcd_putStr("GAME DRAW",ORIGIN_X - 36,ORIGIN_Y - 8, FontLarge, LINE_COLOUR, DRAW_COLOUR);
			lcd_putStr("ENTER TO RESTART",ORIGIN_X - 48,ORIGIN_Y -SPACING - 4, FontSmall, LINE_COLOUR, DRAW_COLOUR);
		}

		// reset the game status after game finishes
		for(x = 0; x <3 ; x++){
			for(y = 0; y <3; y++){
				game[x][y].gameStatus = EMPTY;
			}
		}
		//reset the position of the cursor
		currentPosition.currentColumn = 1;
		currentPosition.currentRow = 1;
		//wait for game to restart
		while(getJoystick() != CENTRE_PRESS)
		{
		}
		restartMusic(); // restart the music
		playerOrder = 0; // restart game order
		playerWON = NONE;
	}
}

