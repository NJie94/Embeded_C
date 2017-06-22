/*
* uart.h
 *
 *  Created on: 14/04/2013
 *      Author: pgo
 */

#include <derivative.h>
#include "clock.h"

#ifndef UART_H_
#define UART_H_

#ifdef __cplusplus
extern "C" {
#endif

void InitializeUART(int baudRate);
void WriteUARTN(int ch);
int  ReadUARTN(void);

#ifdef __cplusplus
}
#endif
#endif /* UART_H_ */
