/*
 * servo.h
 *
 *  Created on: Oct 15, 2014
 *      Author: 100012429
 */

#ifndef SERVO_H_
#define SERVO_H_

/**
* Initialises the servo motor
* Init position is middle (50%)
*/
void initServo();

/**
* Set servo position
* 
* @param position Position to set (0-100)
* 
*/
void setServoPosition(int position);

#endif /* SERVO_H_ */
