/*
 * SPI.h
 *
 *  Created on: 07/08/2012
 *      Author: podonoghue
 */

#ifndef LCD_SPI_H_
#define LCD_SPI_H_

#ifndef LCD_SPI_PRIVATE
#error "This is a private header file - don't include"
#endif

#include <stdint.h>

//! Initialise SPI
//!
//! @note a default frequency is used.
//!
void lcd_spiInitialise(void);

//! Sets Communication speed for SPI
//!
//! @param freq => Frequency in kHz (0 => use default value)
//!
//! @return  \ref 0  => Success                 \n
//!          \ref 1  => Speed is not supported
//!
uint8_t lcd_spiSetSpeed(uint16_t freq);

//! Transmit a 8-bit command to display 
//!
//! @param data - command to send
//!
void lcd_spiTxCommand(uint8_t data);

//! Transmit 8-bit data to display 
//!
//! @param data - data to send
//!
void lcd_spiTxData(uint8_t data);

#endif /* LCD_SPI_H_ */
