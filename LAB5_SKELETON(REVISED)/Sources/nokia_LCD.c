/*!
 *  @file
 *    
 *  @brief  Driver for Nokia 6610 LCD Display (PCF8833 Controller)
 *    
 *     Controller used for LCD Display is a Philips PCF8833 STN RGB-132x132x3 driver
 *     Note: Leadis LDS176 controller (GE-12) is 100% compatible with Philips PCF8833
 *    
 *     Nokia 6610 has a 132 x 132 pixel matrix - each pixel has 12 bits of colour information.
 *    
 * @verbatim     
 *                                 +--+
 *                                 |  |
 *              ___________________|__|_____
 *              |  ======================  |
 *     ^ (131,0)|--------------------------|(131,131)
 *     |        |                          |
 *     |        |                          |
 *     |        |                          |
 *     |        |                          |
 *     |        |                          |
 *     |    Rows|   Nokia 6610 Display     | Note: In general, you can't view Row 0 or Row 131
 *     |        |                          |        and Column 0 and Column 131.
 *     Y        |                          |
 *     |        |
 *     |        |                          |
 *     |        |                          |
 *     |        |                          |
 *     |        |                          |
 *     +        |--------------------------|
 *             (0,0)     Columns          (0,131)
 *              +---------- X ------------->
 *    
 * @endverbatim
 * 
 *     132 x 132 pixel matrix has three methods to specify the colour info
 *    
 *     1. 12 bits per pixel
 *        requires command and 1.5 data bytes to specify a single pixel
 *        (3 data bytes can specify 2 pixels)
 *    
 *     2. 8 bits per pixel
 *        requires one command and one data byte to specify the single pixel
 *        note: pixel data byte converted by RGB table to 12-bit format above
 *    
 *     3. 16 bits per pixel
 *        requires one command and two data bytes to specify the single pixel
 *        note: pixel data bytes converted by dithering circuitry to 12-bit format above
 *        
 *     THIS IMPLEMENTATION USES THE 12 BITS PER PIXEL METHOD!
 *     -------------------------
 *    
 *     Notes: To orient the LCD display so that the origin (0, 0) is at the opposite
 *            corner from the connector (to match the orientation of the Olimex SAM7-EX256
 *            boards silk screen printing), I selected "mirror x" and "mirror y" setting
 *            in the MADCTL command (MY=1, MX=1).
 *            
 *            Also, while I can't yet explain it, I had to reverse the RGB orientation in the
 *            MADCTL command and invert the entire display using the INVON command to make
 *            0x0=black, 0xF= full red.
 *            
 * @author James P Lynch July 7, 2007,
 *         modified for Kinetis PO'Donoghue May, 2013
 *            
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include "nokia_LCD.h"
#define   LCD_SPI_PRIVATE
#include "nokia_LCD_private.h"
#include "lcd_spi.h"
#include "lcd_fonts.h"
#include "derivative.h"

#define DEFAULT_LCD_CONTRAST (65) // Philips controller

void lcd_spiTxCommand(uint8_t data);
void lcd_spiTxData(uint8_t data);

#ifdef ELEC_FREAKS
#define   PHILIPS
#undef    EPSON
#endif

#ifdef ELEC_FREAKS
#include "utilities.h"
#define LCD_PWM_REG            C  // = D10 on arduino
#define LCD_PWM_NUM            2

#define LCD_PWM_MASK         (1<<LCD_PWM_NUM)
#define LCD_PWM_PCR          PCR(LCD_PWM_REG,LCD_PWM_NUM)
#define LCD_PWM_PDOR         PDOR(LCD_PWM_REG)
#define LCD_PWM_PSOR         PSOR(LCD_PWM_REG)  // Data set
#define LCD_PWM_PCOR         PCOR(LCD_PWM_REG)  // Data clear
#define LCD_PWM_PTOR         PTOR(LCD_PWM_REG)  // Data toggle
#define LCD_PWM_PDIR         PDIR(LCD_PWM_REG)  // Data input
#define LCD_PWM_PDDR         PDDR(LCD_PWM_REG)  // Data direction

void lcdPwmOn(void) {
   LCD_PWM_PSOR = LCD_PWM_MASK;
}
void lcdPwmOff(void) {
   LCD_PWM_PCOR = LCD_PWM_MASK;
}

void lcdPwmInitialise(void) {
   SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
   lcdPwmOn();

   LCD_PWM_PDDR  |= LCD_PWM_MASK;
   LCD_PWM_PCR    = PORT_PCR_MUX(1)|PORT_PCR_DSE_MASK|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
}
#endif

//! Initialise the LCD before first use
//!
void lcd_initialise(void) {
   lcd_spiInitialise();
   lcd_spiSetSpeed(12000 /* kHz */);

#ifdef ELEC_FREAKS
   lcdPwmInitialise();
#endif
   
#ifdef PHILIPS
   lcd_spiTxCommand(P_INVOFF);                  // Inversion off
   lcd_spiTxCommand(P_SLEEPOUT);                // Sleep out

   lcd_spiTxCommand(P_COLMOD);                  // Set Colour Mode
   lcd_spiTxData(0x03);                         // 3 = 12-bits/pixel

   lcd_spiTxCommand(P_MADCTL);                  // Memory Access Control
   lcd_spiTxData(0x80);                         // Mirror x,y

   lcd_setContrast(DEFAULT_LCD_CONTRAST);       // Set default contrast
//   spi_txCommand(P_BSTRON);                   // Boost On

   lcd_spiTxCommand(P_DISPON);                  // Display on
#endif
#ifdef EPSON   
   lcd_spiTxCommand(DISCTL);        // Display control(EPSON)
   lcd_spiTxData(0x00);             // P1: 0x00 = 2 divisions, switching period=8 (default)
   lcd_spiTxData(0x20);             // P2: 0x20 = nlines/4 - 1 = 132/4 - 1 = 32)
   lcd_spiTxData(0x00);             // P3: 0x00 = no inversely highlighted lines
   lcd_spiTxData(0x01);             // P4 ??

   lcd_spiTxCommand(COMSCN);        // COM scan
   lcd_spiTxData(0x01);             // P1: 0x01 = Scan 1->80, 160<-81

   lcd_spiTxCommand(OSCON);         // Internal oscillator ON

   lcd_spiTxCommand(SLPOUT);        // Sleep out

   lcd_spiTxCommand(PWRCTR);        // Power control
   lcd_spiTxData(0x0F);             // Reference voltage regulator on, circuit voltage follower on, BOOST ON

   lcd_spiTxCommand(DISINV);        // Inverse display

   lcd_spiTxCommand(DATCTL);        // Data control
   lcd_spiTxData(0x03);             // P1: //0x01 = page address inverted, col address normal, address scan in col direction
   lcd_spiTxData(0x00);             // P2: 0x00 = RGB sequence (default value)
   lcd_spiTxData(0x02);             // P3: 0x02 = Grayscale -> 16 (selects 12-bit color, type A)

   lcd_spiTxCommand(VOLCTR);        // Voltage control (contrast setting)
   lcd_spiTxData(35);               // P1: 32 volume value (adjust this setting for your display 0 .. 63)
   lcd_spiTxData(0x03);             // P2: 3 resistance ratio (determined by experiment)

   WAIT_MS(200);                    // Allow power supply to stabilise

   lcd_spiTxCommand(DISON);         // Turn on the display

//   int i;
//   for (i=0; i<64; i++) {
//      lcd_spiTxCommand(VOLCTR);        // Voltage control (contrast setting)
//      lcd_spiTxData(i);                // P1: 32 volume value (adjust this setting for your display 0 .. 63)
//      lcd_spiTxData(0x03);             // P2: 3 resistance ratio (determined by experiment)
//      WAIT_MS(200);
//   }

//   lcd_spiTxCommand(DATCTL);        // data control(EPSON)
//   lcd_spiTxData(0x03);             // correct for normal sin7
//   lcd_spiTxData(0x00);             // normal RGB arrangement
//   lcd_spiTxData(0x01);             // 8-bit Grayscale
//   lcd_spiTxData(0x02);             // 16-bit Grayscale Type A
   
#endif
}

//! Writes the entire LCD screen from a bmp file
//! Uses Olimex BmpToArray.exe utility
//!
//! @param bmp - bitmap to display
//!
//! @author Olimex, James P Lynch July 7, 2007
//!
void lcd_write130x130bmp(uint8_t bmp[131*131]) {
   unsigned j; // loop counter

#ifdef PHILIPS
   // Memory access controller (command 0x36)
   lcd_spiTxCommand(P_MADCTL);
   lcd_spiTxData(0x48); // no mirror Y (temporary to satisfy Olimex bmptoarray utility)
   // Display OFF
   lcd_spiTxCommand(P_DISPOFF);
   // Column address set (command 0x2A)
   lcd_spiTxCommand(P_CASET);
   lcd_spiTxData(0);
   lcd_spiTxData(131);
   // Page address set (command 0x2B)
   lcd_spiTxCommand(P_PASET);
   lcd_spiTxData(0);
   lcd_spiTxData(131);
   // WRITE MEMORY
   lcd_spiTxCommand(P_RAMWR);
   for (j = 0; j < sizeof(bmp); j++) {
      lcd_spiTxData(bmp[j]);
   }
   // Memory access controller (command 0x36)
   lcd_spiTxCommand(P_MADCTL);
   lcd_spiTxData(0xC8); // restore to (mirror x and y, reverse rgb)
   // Display On
   lcd_spiTxCommand(P_DISPON);
#endif
#ifdef EPSON
   // Memory access controller (command 0x36)
   lcd_spiTxCommand(DATCTL);
   lcd_spiTxData(0x48); // no mirror Y (temporary to satisfy Olimex bmptoarray utility)
   // Display OFF
   lcd_spiTxCommand(DISOFF);
   // Column address set (command 0x2A)
   lcd_spiTxCommand(CASET);
   lcd_spiTxData(0);
   lcd_spiTxData(131);
   // Page address set (command 0x2B)
   lcd_spiTxCommand(PASET);
   lcd_spiTxData(0);
   lcd_spiTxData(131);
   // WRITE MEMORY
   lcd_spiTxCommand(RAMWR);
   for (j = 0; j < sizeof(bmp); j++) {
      lcd_spiTxData(bmp[j]);
   }
   // Memory access controller (command 0x36)
   lcd_spiTxCommand(DATCTL);
   lcd_spiTxData(0xC8); // restore to (mirror x and y, reverse rgb)
   // Display On
   lcd_spiTxCommand(DISON);
#endif
}

//! This function will clear the screen to the given color.
//!
//! @param color - 12-bit color value rrrrggggbbbb
//!
//! @author James P Lynch July 7, 2007
//!
void lcd_clear(int color) {
   int i;

#ifdef PHILIPS
   lcd_spiTxCommand(P_PASET);
   lcd_spiTxData(0);
   lcd_spiTxData(131);

   lcd_spiTxCommand(P_CASET);
   lcd_spiTxData(0);
   lcd_spiTxData(131);

   // Construct RRRRGGGG|GGGGRRRR|GGGGBBBB in 3 bytes = 2 pixels
   uint8_t data1 = (color >> 4);
   uint8_t data2 = ((color & 0xF) << 4) | ((color >> 8) & 0xF);
   uint8_t data3 = color & 0xFF;;
   lcd_spiTxCommand(P_RAMWR);
   for (i = 0; i < ((131 * 131) / 2); i++) {
      lcd_spiTxData(data1);
      lcd_spiTxData(data2);
      lcd_spiTxData(data3); 
   }
#endif
#ifdef EPSON
   lcd_spiTxCommand(PASET);
   lcd_spiTxData(0);
   lcd_spiTxData(131);

   lcd_spiTxCommand(CASET);
   lcd_spiTxData(0);
   lcd_spiTxData(131);

   // Construct RRRRGGGGBBBB=>RRRRGGGG|BBBBRRRR|GGGGBBBB in 3 bytes = 2 pixels
   uint8_t data1 = (color >> 4);
   uint8_t data2 = ((color & 0xF) << 4) | ((color >> 8) & 0xF);
   uint8_t data3 = color & 0xFF;;

   lcd_spiTxCommand(RAMWR);

   for (i=0; i < (131*131)/2; i++) {
      lcd_spiTxData(data1);
      lcd_spiTxData(data2);
      lcd_spiTxData(data3); 
   }
#endif
}

//! Sets the Row and Column addresses
//!
//! @param x = row address (0 .. 131)
//! @param y = column address (0 .. 131)
//!
//! @author James P Lynch July 7, 2007
//!
static void lcd_setXY(int x, int y) {
   // Row address set (command 0x2B)
#ifdef PHILIPS
   lcd_spiTxCommand(P_PASET);
   lcd_spiTxData(y);
   lcd_spiTxData(y);
   // Column address set (command 0x2A)
   lcd_spiTxCommand(P_CASET);
   lcd_spiTxData(x);
   lcd_spiTxData(x);
#endif

#ifdef EPSON
   lcd_spiTxCommand(PASET);
   lcd_spiTxData(y);
   lcd_spiTxData(y);
   // Column address set (command 0x2A)
   lcd_spiTxCommand(CASET);
   lcd_spiTxData(x);
   lcd_spiTxData(x);
#endif
}

//! Lights a single pixel in the specified color at the specified x and y addresses
//!
//! @param  x     - row address (0 .. 131)
//! @param  y     - column address (0 .. 131)
//! @param  color - 12-bit color value rrrrggggbbbb
//!
//! @note See lcd.h for some sample color settings
//!
//! @author James P Lynch July 7, 2007
//!
void lcd_drawPixel(int x, int y, int color) {
#ifdef PHILIPS
   lcd_setXY(x, y);
   lcd_spiTxCommand(P_RAMWR);
   // Construct RRRRGGGGBBBB=>RRRRGGGG|BBBBxxxx in 3 bytes = 2 pixels
   lcd_spiTxData((unsigned char)(color >> 4));
   lcd_spiTxData((unsigned char)(((color & 0x0F) << 4) | 0x00));
   lcd_spiTxCommand(P_NOP);
#endif

#ifdef EPSON
   lcd_setXY(x, y);
   lcd_spiTxCommand(RAMWR);
   // Construct RRRRGGGGBBBB=>RRRRGGGG|BBBBxxxx in 3 bytes = 2 pixels
   lcd_spiTxData((unsigned char)(color >> 4));
   lcd_spiTxData((unsigned char)(((color & 0x0F) << 4) | 0x0F));
   lcd_spiTxCommand(0xFF);
#endif

}

//! Draws a line in the specified color from (x0,y0) to (x1,y1)
//!
//! @param  x0     - row address (0 .. 131)
//! @param  y0     - column address (0 .. 131)
//! @param  x1     - row address (0 .. 131)
//! @param  y1     - column address (0 .. 131)
//! @param  color  - 12-bit color value rrrrggggbbbb
//!
//! @note See lcd.h for some sample color settings
//!
//! @author James P Lynch July 7, 2007
//!
//! @note Good write-up on this algorithm in Wikipedia (search for Bresenham's line algorithm)
//! @note See lcd.h for some sample color settings
//!       
//! @author Authors: Dr. Leonard McMillan, Associate Professor UNC \n
//!                  Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962)
//!       
//! @note Taken verbatim from Professor McMillan's presentation: \n
//!       http://www.cs.unc.edu/~mcmillan/comp136/Lecture6/Lines.html
//!
void lcd_drawLine(int x0, int y0, int x1, int y1, int color) {
   int dy = y1 - y0;
   int dx = x1 - x0;
   int stepx, stepy;
   if (dy < 0) {
      dy = -dy;
      stepy = -1;
   } else {
      stepy = 1;
   }
   if (dx < 0) {
      dx = -dx;
      stepx = -1;
   } else {
      stepx = 1;
   }
   dy <<= 1; // dy is now 2*dy
   dx <<= 1; // dx is now 2*dx
   lcd_drawPixel(x0, y0, color);
   if (dx > dy) {
      int fraction = dy - (dx >> 1); // same as 2*dy - dx
      while (x0 != x1) {
         if (fraction >= 0) {
            y0 += stepy;
            fraction -= dx; // same as fraction -= 2*dx
         }
         x0 += stepx;
         fraction += dy; // same as fraction -= 2*dy
         lcd_drawPixel(x0, y0, color);
      }
   } else {
      int fraction = dx - (dy >> 1);
      while (y0 != y1) {
         if (fraction >= 0) {
            x0 += stepx;
            fraction -= dy;
         }
         y0 += stepy;
         fraction += dx;
         lcd_drawPixel(x0, y0, color);
      }
   }
}

//! Draws a rectangle in the specified color from (x1,y1) to (x2,y2)
//! Rectangle can be filled with a color if desired
//!
//! @param  x0     - row address (0 .. 131)
//! @param  y0     - column address (0 .. 131)
//! @param  x1     - row address (0 .. 131)
//! @param  y1     - column address (0 .. 131)
//! @param  color  - 12-bit color value rrrrggggbbbb
//!
//! @note See lcd.h for some sample color settings
//!
//! @author James P Lynch July 7, 2007
//!
//! @note
//!     The best way to fill a rectangle is to take advantage of the "wrap-around" feature
//!     built into the Philips PCF8833 controller. By defining a drawing box, the memory can
//!     be simply filled by successive memory writes until all pixels have been illuminated.
//!     
//!     1. Given the coordinates of two opposing corners (x0, y0) (x1, y1)
//!        calculate the minimums and maximums of the coordinates
//!        
//!        xmin = (x0 <= x1) ? x0 : x1;
//!        xmax = (x0 > x1) ? x0 : x1;
//!        ymin = (y0 <= y1) ? y0 : y1;
//!        ymax = (y0 > y1) ? y0 : y1;
//!     
//!     2. Now set up the drawing box to be the desired rectangle
//!     
//!        LCDCommand(PASET); // set the row boundaries
//!        LCDData(xmin);
//!        LCDData(xmax);
//!        LCDCommand(CASET); // set the column boundaries
//!        LCDData(ymin);
//!        LCDData(ymax);
//!     
//!     3. Calculate the number of pixels to be written divided by 2
//!     
//!        NumPixels = ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 1)
//!        
//!        You may notice that I added one pixel to the formula.
//!        This covers the case where the number of pixels is odd and we
//!        would lose one pixel due to rounding error. In the case of
//!        odd pixels, the number of pixels is exact.
//!        in the case of even pixels, we have one more pixel than
//!        needed, but it cannot be displayed because it is outside
//!        the drawing box.
//!        
//!        We divide by 2 because two pixels are represented by three bytes.
//!        So we work through the rectangle two pixels at a time.
//!     
//!     4. Now a simple memory write loop will fill the rectangle
//!     
//!        for (i = 0; i < ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 1); i++) {
//!           LCDData((color >> 4) & 0xFF);
//!           LCDData(((color & 0xF) << 4) | ((color >> 8) & 0xF));
//!           LCDData(color & 0xFF);
//!        }
//!        
//!        In the case of an unfilled rectangle, drawing four lines with the Bresenham line
//!        drawing algorithm is reasonably efficient.
//!     
void lcd_drawRect(int x0, int y0, int x1, int y1, int fill, int color) {
   int xmin, xmax, ymin, ymax;
   int i;
   // check if the rectangle is to be filled
   if (fill) {
      // best way to create a filled rectangle is to define a drawing box
      // and loop two pixels at a time
      // calculate the min and max for x and y directions
      xmin = (x0 <= x1) ? x0 : x1;
      xmax = (x0 > x1) ? x0 : x1;
      ymin = (y0 <= y1) ? y0 : y1;
      ymax = (y0 > y1) ? y0 : y1;
      // specify the controller drawing box according to those limits
      // Row address set (command 0x2B)
#ifdef PHILIPS
      lcd_spiTxCommand(P_CASET);
#endif
#ifdef EPSON
      lcd_spiTxCommand(CASET);
#endif
      lcd_spiTxData(xmin);
      lcd_spiTxData(xmax);
      // Column address set (command 0x2A)
#ifdef PHILIPS
      lcd_spiTxCommand(P_PASET);
#endif
#ifdef EPSON
      lcd_spiTxCommand(PASET);
#endif
      lcd_spiTxData(ymin);
      lcd_spiTxData(ymax);
      // WRITE MEMORY
#ifdef PHILIPS
      lcd_spiTxCommand(P_RAMWR);
#endif
#ifdef EPSON
      lcd_spiTxCommand(RAMWR);
#endif
      // loop on total number of pixels / 2
      for (i = 0; i < ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 1);
            i++) {
         // use the color value to output three data bytes covering two pixels
         lcd_spiTxData((color >> 4) & 0xFF);
         lcd_spiTxData(((color & 0xF) << 4) | ((color >> 8) & 0xF));
         lcd_spiTxData(color & 0xFF);
      }
   } else {
      // best way to draw unfilled rectangle is to draw four lines
      lcd_drawLine(x0, y0, x1, y0, color);
      lcd_drawLine(x0, y1, x1, y1, color);
      lcd_drawLine(x0, y0, x0, y1, color);
      lcd_drawLine(x1, y0, x1, y1, color);
   }
}

//!  Draws an ASCII character at the specified (x,y) address and color
//!
//!  @param c      = character to be displayed
//!  @param x      = row address (0 .. 131)
//!  @param y      = column address (0 .. 131)
//!  @param size   = font pitch (SMALL, MEDIUM, LARGE)
//!  @param fcolor = 12-bit foreground color value
//!  @param bcolor = 12-bit background color value
//!
//!  @verbatim
//!  Notes: Here's an example to display "E" at address (20,20)
//! 
//!            LCDPutChar('E', 20, 20, MEDIUM, WHITE, BLACK);
//!            
//!                 (27,20)       (27,27)
//!                    |             |
//!                    |             |
//!                  ^ V             V
//!                  : _ # # # # # # #   0x7F
//!                  : _ _ # # _ _ _ #   0x31
//!                  : _ _ # # _ # _ _   0x34
//!                  x _ _ # # # # _ _   0x3C
//!                  : _ _ # # _ # _ _   0x34
//!                  : _ _ # # _ _ _ #   0x31
//!                  : _ # # # # # # #   0x7F
//!                  : _ _ _ _ _ _ _ _   0x00
//!                  
//!                    ------y------->
//!                    ^             ^
//!                    |             |
//!                    |             |
//!                 (20,20)       (20,27)
//! 
//! 
//!     The most efficient way to display a character is to make use of the "wrap-around" feature
//!     of the Philips PCF8833 LCD controller chip.
//!     
//!     Assume that we position the character at (20, 20) that's a (row, col) specification.
//!     With the row and column address set commands, you can specify an 8x8 box for the SMALL and MEDIUM
//!     characters or a 16x8 box for the LARGE characters.
//!     
//!             spi_txCommand(PASET);   // set the row drawing limits
//!             spi_txData(20);         //
//!             spi_txData(27);         // limit rows to (20, 27)
//!                                       
//!             spi_txCommand(CASET);   // set the column drawing limits
//!             spi_txData(20);         //
//!             spi_txData(27);         // limit columns to (20,27)
//!     
//!     When the algorithm completes col 27, the column address wraps back to 20
//!     At the same time, the row address increases by one (this is done by the controller)
//!     
//!     We walk through each row, two pixels at a time. The purpose is to create three
//!     data bytes representing these two pixels in the following format (as specified by Philips
//!     for RGB 4 : 4 : 4 format (see page 62 of PCF8833 controller manual).
//!     
//!             Data for pixel 0: RRRRGGGGBBBB
//!             Data for Pixel 1: RRRRGGGGBBBB
//!             
//!             spi_txCommand(RAMWR);                 // start a memory write (96 data bytes to follow)
//!                                                     
//!             spi_txData(RRRRGGGG);                 // first pixel, red and green data
//!             spi_txData(BBBBRRRR);                 // first pixel, blue data; second pixel, red data
//!             spi_txData(GGGGBBBB);                 // second pixel, green and blue data
//!             :
//!             and so on until all pixels displayed!
//!             :
//!             spi_txCommand(NOP);                   // this will terminate the RAMWR command
//!     
//!   @endverbatim  
//!
//!   @author James P Lynch July 7, 2007
//!
void lcd_putChar(char c, int x, int y, FontSize fontSize, int fColor, int bColor) {
   unsigned int nCols;
   unsigned int nRows;
   unsigned int nBytes;
   unsigned char pixelRow;
   unsigned char mask;
   unsigned int word0;
   unsigned int word1;
   unsigned char *pFont;
   unsigned char *pChar;
   static const unsigned char *FontTable[] = {
         (unsigned char *)FONT6x8,
         (unsigned char *)FONT8x8,
         (unsigned char *)FONT8x16
   };
   // Get pointer to the beginning of the selected font table
   pFont = (unsigned char *)FontTable[fontSize];

   // Get the nColumns, nRows and nBytes
   nCols  = *pFont;
   nRows  = *(pFont + 1);
   nBytes = *(pFont + 2);

   // Get pointer to the last byte of the desired character
   pChar = pFont + (nBytes * (c - 0x1F)) + nBytes - 1;

   // Row address set (command 0x2B)
#ifdef PHILIPS
      lcd_spiTxCommand(P_CASET);
#endif
#ifdef EPSON
      lcd_spiTxCommand(CASET);
#endif
   lcd_spiTxData(x);
   lcd_spiTxData(x + nCols - 1);

   // Column address set (command 0x2A)
#ifdef PHILIPS
      lcd_spiTxCommand(P_PASET);
#endif
#ifdef EPSON
      lcd_spiTxCommand(PASET);
#endif
   lcd_spiTxData(y);
   lcd_spiTxData(y + nRows - 1);
   
   // Write memory
#ifdef PHILIPS
      lcd_spiTxCommand(P_RAMWR);
#endif
#ifdef EPSON
      lcd_spiTxCommand(RAMWR);
#endif
   
   // Loop on each row, working backwards from the bottom to the top
   int i;
   for (i = nRows - 1; i >= 0; i--) {
      unsigned j;
      // Copy pixel row from font table and then decrement row
      pixelRow = *pChar--;
      // Loop on each pixel in the row (left to right)
      // Note: we do two pixels each loop
      mask = 0x80;
      for (j = 0; j < nCols; j += 2) {
         // If pixel bit set, use foreground colour; else use the background colour
         // Now get the pixel colour for two successive pixels
         if ((pixelRow & mask) == 0) {
            word0 = bColor;
         }
         else {
            word0 = fColor;
         }
         mask = mask >> 1;
         if ((pixelRow & mask) == 0) {
            word1 = bColor;
         }
         else {
            word1 = fColor;
         }
         mask = mask >> 1;

         // Use this information to output three data bytes
         lcd_spiTxData((word0 >> 4) & 0xFF);
         lcd_spiTxData(((word0 & 0xF) << 4) | ((word1 >> 8) & 0xF));
         lcd_spiTxData(word1 & 0xFF);
      }
   }
   // Terminate the Write Memory command
#ifdef PHILIPS
      lcd_spiTxCommand(P_NOP);
#endif
#ifdef EPSON
      lcd_spiTxCommand(NOP);
#endif
}

//!  Draws a null-terminates character string at the specified (x,y) address and color
//!
//!  @param pString = pointer to character string to be displayed
//!  @param x = row address (0 .. 131)
//!  @param y = column address (0 .. 131)
//!  @param Size = font pitch (SMALL, MEDIUM, LARGE)
//!  @param fColor = 12-bit foreground colour value rrrrggggbbbb
//!  @param bColor = 12-bit background colour value rrrrggggbbbb
//!
//!  @note Here's an example to display "Hello World!" at address (20,20) \n
//!        LCDPutChar("Hello World!", 20, 20, WHITE, BLACK);
//!
//!  @author James P Lynch, August 30, 2007 \n
//!          Edited by Peter Davenport on August 23, 2010
//!
//!  @note For more information on how this code does it's thing look at this \n
//!        "http://www.sparkfun.com/tutorial/Nokia%206100%20LCD%20Display%20Driver.pdf"
//!
void lcd_putStr(const char *pString, int x, int y, FontSize fontSize, int fColor, int bColor) {
   // loop until null-terminator is seen
   while (*pString != 0x00) {
      // draw the character
      lcd_putChar(*pString++, x, y, fontSize, fColor, bColor);
      // advance the y position
      if (fontSize == FontSmall)
         x = x + 6;
      else if (fontSize == FontMedium)
         x = x + 8;
      else
         x = x + 8;
      // bail out if y exceeds 131
      if (y > 131) break;
   }
}

//! Set LCD contrast - varies with device
//!
//! @param setting - contrast level (0..127) ?
//!
void lcd_setContrast(uint8_t setting) {
#ifdef EPSON
   lcd_spiTxCommand(VOLCTR);       // electronic volume, this is the contrast/brightness(EPSON)
   lcd_spiTxData(setting);         // volume (contrast) setting - course adjustment,  -- original was 24
   //     LCDData(0x03);     // internal resistor ratio - coarse adjustment
   //     LCDData(0x30);   
   lcd_spiTxCommand(NOP);          // nop(EPSON)
#endif
#ifdef PHILIPS
   lcd_spiTxCommand(P_SETCON);     // Contrast/brightness
   lcd_spiTxData(setting);
#endif
}

//! Draws a line in the specified colour at center (x0,y0) with radius
//!
//! @param x0 = row address (0 .. 131)
//! @param y0 = column address (0 .. 131)
//! @param radius = radius in pixels
//! @param color = 12-bit colour value rrrrggggbbbb
//!
//! @author Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962)
//!
//! @note Taken verbatim Wikipedia article on Bresenham's line algorithm \n
//!        http://www.wikipedia.org
//!
void lcd_drawCircle(int x0, int y0, int radius, int color, int circleType) {
   int f = 1 - radius;
   int ddF_x = 0;
   int ddF_y = -2 * radius;
   int x = 0;
   int y = radius;
   
   if (circleType&(SECTOR_315_360|SECTOR_0_45)) {
      lcd_drawPixel(x0 + radius, y0, color);  // 0,360
   }
   if (circleType&(SECTOR_45_90|SECTOR_90_135)) {
      lcd_drawPixel(x0, y0 + radius, color);  // 90
   }
   if (circleType&(SECTOR_135_180|SECTOR_180_225)) {
      lcd_drawPixel(x0 - radius, y0, color);  // 180
   }
   if (circleType&(SECTOR_225_270|SECTOR_270_315)) {
      lcd_drawPixel(x0, y0 - radius, color);  // 270
   }
   while (x < y) {
      if (f >= 0) {
         y--;
         ddF_y += 2;
         f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x + 1;
      if (circleType&SECTOR_0_45) {
         lcd_drawPixel(x0 + y, y0 + x, color);   // 0-45
      }
      if (circleType&SECTOR_45_90) {
         lcd_drawPixel(x0 + x, y0 + y, color);   // 45-90
      }
      if (circleType&SECTOR_90_135) {
         lcd_drawPixel(x0 - x, y0 + y, color);   // 90-135
      }
      if (circleType&SECTOR_135_180) {
         lcd_drawPixel(x0 - y, y0 + x, color);   // 135-180
      }
      if (circleType&SECTOR_180_225) {
         lcd_drawPixel(x0 - y, y0 - x, color);   // 180-225
      }
      if (circleType&SECTOR_225_270) {
         lcd_drawPixel(x0 - x, y0 - y, color);   // 225-270
      }
      if (circleType&SECTOR_270_315) {
         lcd_drawPixel(x0 + x, y0 - y, color);   // 270-315
      }
      if (circleType&SECTOR_315_360) {
         lcd_drawPixel(x0 + y, y0 - x, color);   // 315-360
      }
   }
}

