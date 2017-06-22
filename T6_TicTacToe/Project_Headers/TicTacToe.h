/*
 * TicTacToe.h
 *
 *  Created on: 18 Sep 2014
 *      Author: podonoghue
 */

#ifndef TICTACTOE_H_
#define TICTACTOE_H_

typedef enum {machineFirst, humanFirst} PlayOrderType;

void doMenu(void);
void playNoughtsAndCrosses(PlayOrderType choice);

#ifndef TRUE
#define FALSE (0)
#define TRUE  (!FALSE)
#define BOOL int
#endif

#endif /* TICTACTOE_H_ */
