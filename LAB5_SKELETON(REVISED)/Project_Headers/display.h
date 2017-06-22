/*
 * display.h
 *
 *  Created on: Oct 20, 2014
 *      Author: 100012429
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#define DATA_POINTS	(100) //Number of points in X direction
#define DATA_RANGE	(100) //Number of points in Y direction

/**
 * Draw empty background for DSO screen
 */
void drawBackground();
/**
 * Draws data on DSO screen (as fast as possible!)
 * 
 * @param data - Array of data to plot
 * @param colour - Colour to use
 */
void drawData(int colour);
/**
 * Draw grid and axis labels for DSO
 */
void drawGrid();
#endif /* DISPLAY_H_ */
