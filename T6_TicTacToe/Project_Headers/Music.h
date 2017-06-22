/*
 * Music.h
 *
 *  Created on: Sep 22, 2014
 *      Author: podonoghue
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include <stdint.h>

/**
 * Does initialisation of the Music system
 * 
 */

void initMusic(void);
void restartMusic (void);
/**
 * Plays the given note continuously
 * 
 * @param period - Period of note in timer 'ticks'
 * 
 * @note This function returns immediately.  The note is played by interrupt driven code.
 * @note The note is output on FTM1.CH0 = PTB0(Fn3)
 */
void playNote(uint16_t period);

#endif /* MUSIC_H_ */
