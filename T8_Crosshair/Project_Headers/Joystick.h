/*
 * Joystick.h
 *
 *  Created on: 18 Sep 2014
 *      Author: podonoghue
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

/*
 * Values returned from getJoystick()
 */
typedef enum {
   NO_PRESS,
   NORTH_PRESS,
   EAST_PRESS,
   WEST_PRESS,
   SOUTH_PRESS,
   CENTRE_PRESS,
} SwitchValueType;

void initJoystick();
SwitchValueType getJoystick(void);

#endif /* JOYSTICK_H_ */
