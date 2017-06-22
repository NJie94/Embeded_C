/*
 * SPI.c
 *
 *  Created on: 07/08/2012
 *      Author: podonoghue
 */
#include "derivative.h"
#define LCD_SPI_PRIVATE
#include "lcd_spi.h"
#include "utilities.h"

// SPI used                            
#define SPI_NUM 0 // Modify to change which SPI is used

#define DEFAULT_SPI_FREQUENCY  (1000)    //!< Initial SPI frequency 1000 kHz

#if (SPI_NUM==0)
   // SPI data out pin
   #define SPI_SOUT_REG          D
   #define SPI_SOUT_NUM          2
   // SPI clock pin
   #define SPI_SCK_REG           D
   #define SPI_SCK_NUM           1
   // Display CS
   #define SPI_CSn_REG           A
   #define SPI_CSn_NUM           2
   // Display reset
   #define LCD_RSTn_REG          A
   #define LCD_RSTn_NUM          12
   // Pin ALT function for SPI pins (SOUT,SCK)
   #define SPI_ALT_FN 2
   // Pin ALT function for GPIO pins (SPI_CS, LCD_RST)
   #define GPIO_ALT_FN 1
   // SPI clock enable
   #define SPI_CLK_ENABLE()      (SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK)
#else
   #error "Unknown SPI number"
#endif

// SPI data out pin
#define SPI_SOUT_MASK         (1<<SPI_SOUT_NUM)
#define SPI_SOUT_PCR          PCR(SPI_SOUT_REG,SPI_SOUT_NUM)
#define SPI_SOUT_PDOR         PDOR(SPI_SOUT_REG)
#define SPI_SOUT_PSOR         PSOR(SPI_SOUT_REG)  // Data set 
#define SPI_SOUT_PCOR         PCOR(SPI_SOUT_REG)  // Data clear
#define SPI_SOUT_PTOR         PTOR(SPI_SOUT_REG)  // Data toggle
#define SPI_SOUT_PDIR         PDIR(SPI_SOUT_REG)  // Data input
#define SPI_SOUT_PDDR         PDDR(SPI_SOUT_REG)  // Data direction

// SPI clock pin
#define SPI_SCK_MASK           (1<<SPI_SCK_NUM)
#define SPI_SCK_PCR            PCR(SPI_SCK_REG,SPI_SCK_NUM)
#define SPI_SCK_PDOR           PDOR(SPI_SCK_REG)
#define SPI_SCK_PSOR           PSOR(SPI_SCK_REG)  // Data set 
#define SPI_SCK_PCOR           PCOR(SPI_SCK_REG)  // Data clear
#define SPI_SCK_PTOR           PTOR(SPI_SCK_REG)  // Data toggle
#define SPI_SCK_PDIR           PDIR(SPI_SCK_REG)  // Data input
#define SPI_SCK_PDDR           PDDR(SPI_SCK_REG)  // Data direction

// SPI CS pin (GPIO)
#define SPI_CSn_MASK           (1<<SPI_CSn_NUM)
#define SPI_CSn_PCR            PCR(SPI_CSn_REG,SPI_CSn_NUM)
#define SPI_CSn_PDOR           PDOR(SPI_CSn_REG)
#define SPI_CSn_PSOR           PSOR(SPI_CSn_REG)  // Data set 
#define SPI_CSn_PCOR           PCOR(SPI_CSn_REG)  // Data clear
#define SPI_CSn_PTOR           PTOR(SPI_CSn_REG)  // Data toggle
#define SPI_CSn_PDIR           PDIR(SPI_CSn_REG)  // Data input
#define SPI_CSn_PDDR           PDDR(SPI_CSn_REG)  // Data direction

// LCD reset pin
#define LCD_RSTn_MASK           (1<<LCD_RSTn_NUM)
#define LCD_RSTn_PCR            PCR(LCD_RSTn_REG,LCD_RSTn_NUM)
#define LCD_RSTn_PDOR           PDOR(LCD_RSTn_REG)
#define LCD_RSTn_PSOR           PSOR(LCD_RSTn_REG)  // Data set 
#define LCD_RSTn_PCOR           PCOR(LCD_RSTn_REG)  // Data clear
#define LCD_RSTn_PTOR           PTOR(LCD_RSTn_REG)  // Data toggle
#define LCD_RSTn_PDIR           PDIR(LCD_RSTn_REG)  // Data input
#define LCD_RSTn_PDDR           PDDR(LCD_RSTn_REG)  // Data direction

//! SPI CTAR value
#define SPI_CTAR_MASK  (SPI_CTAR_PASC(1)|SPI_CTAR_ASC(0))

// SPI_PUSHR_PCS for Tx operations 
#define SWD_PUSHR_TX  SPI_PUSHR_PCS((1<<0)|(1<<1)) // PCS0=SWDIO_O_En, PCS1=SWDCLK_En 

// SPI_PUSHR_PCS for Rx operations 
#define SWD_PUSHR_RX  SPI_PUSHR_PCS((1<<1))        // PCS1=SWDCLK_En

static const int ctas_9bit  = 0;

static uint32_t ctar0Value = 0;
static uint32_t ctar1Value = 0;

static uint32_t spiBaudValue  = 0;

//! Set SPI.CTAR0 value (non-persistent)
//! 
//! @param ctar 32-bit CTAR value (excluding baud)
//!
static inline void lcd_spiSetCTAR0(uint32_t ctar) {
   SPI0_CTAR0 = spiBaudValue|ctar;
}

//! Set SPI.CTAR1 value (non-persistent)
//! 
//! @param ctar 32-bit CTAR value (excluding baud)
//!
static inline void lcd_spiSetCTAR1(uint32_t ctar) {
   SPI0_CTAR1 = spiBaudValue|ctar;
}

//! Initialise SPI
//!
//! @note a default frequency is used.
//!
void lcd_spiInitialise(void) {
   ctar0Value = SPI_CTAR_MASK|SPI_CTAR_FMSZ(9-1);   // 9-bit transfer
   ctar1Value = SPI_CTAR_MASK|SPI_CTAR_FMSZ(4-1);   // 4-bit transfer
   
   // Configure SPI
   SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTD_MASK;
   
   (void)lcd_spiSetSpeed(0);
   SPI0_MCR   = SPI_MCR_CLR_RXF_MASK|SPI_MCR_ROOE_MASK|SPI_MCR_CLR_TXF_MASK|SPI_MCR_PCSIS((1<<0)|(1<<1))|
                SPI_MCR_MSTR_MASK|SPI_MCR_FRZ_MASK|SPI_MCR_DCONF(0)|SPI_MCR_SMPL_PT(0);
   //! SPI_OUT pin
   SPI_SOUT_PCR = PORT_PCR_MUX(SPI_ALT_FN)|PORT_PCR_DSE_MASK|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;

   //! SPI_CLK pin
   SPI_SCK_PCR = PORT_PCR_MUX(SPI_ALT_FN) |PORT_PCR_DSE_MASK|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;

   // LCD SPI CS pin
   SPI_CSn_PCR   = PORT_PCR_MUX(GPIO_ALT_FN)|PORT_PCR_DSE_MASK|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
   // Initially inactive
   SPI_CSn_PSOR  = SPI_CSn_MASK;
   SPI_CSn_PDDR  |= SPI_CSn_MASK;
   
   // LCD Reset pin
   LCD_RSTn_PCR = PORT_PCR_MUX(GPIO_ALT_FN)|PORT_PCR_DSE_MASK|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
   LCD_RSTn_PDDR |= LCD_RSTn_MASK;
   
   // Reset display
   LCD_RSTn_PCOR  = LCD_RSTn_MASK;   
   int i;
   for(i=0;i<1000;i++) {
      __asm("nop");
   }
   LCD_RSTn_PSOR  = LCD_RSTn_MASK;   
   for(i=0;i<1000;i++) {
      __asm("nop");
   }
}

typedef struct {
   uint32_t freq; 
   uint32_t ctarBaud; 
} Scidata;

//! Maps frequency to SPI CTAR_PBR values
//!
static const Scidata spiBaudTable[] = {
     //  freq(kHz)                        ctarBaud
 /*  0 */ {12000, SPI_CTAR_PBR(0)|SPI_CTAR_BR(0)|SPI_CTAR_PCSSCK(0)|SPI_CTAR_CSSCK(0)},
 /*  1 */ { 8000, SPI_CTAR_PBR(1)|SPI_CTAR_BR(0)|SPI_CTAR_PCSSCK(0)|SPI_CTAR_CSSCK(1)},
 /*  2 */ { 6000, SPI_CTAR_PBR(0)|SPI_CTAR_BR(1)|SPI_CTAR_PCSSCK(0)|SPI_CTAR_CSSCK(1)},
 /*  3 */ { 4800, SPI_CTAR_PBR(2)|SPI_CTAR_BR(0)|SPI_CTAR_PCSSCK(1)|SPI_CTAR_CSSCK(0)},
 /*  4 */ { 4000, SPI_CTAR_PBR(1)|SPI_CTAR_BR(1)|SPI_CTAR_PCSSCK(1)|SPI_CTAR_CSSCK(0)},
 /*  5 */ { 3000, SPI_CTAR_PBR(0)|SPI_CTAR_BR(3)|SPI_CTAR_PCSSCK(0)|SPI_CTAR_CSSCK(2)},
 /*  6 */ { 2667, SPI_CTAR_PBR(1)|SPI_CTAR_BR(2)|SPI_CTAR_PCSSCK(2)|SPI_CTAR_CSSCK(0)},
 /*  7 */ { 2400, SPI_CTAR_PBR(2)|SPI_CTAR_BR(1)|SPI_CTAR_PCSSCK(2)|SPI_CTAR_CSSCK(0)},
 /*  8 */ { 2000, SPI_CTAR_PBR(1)|SPI_CTAR_BR(3)|SPI_CTAR_PCSSCK(1)|SPI_CTAR_CSSCK(1)},
 /*  9 */ { 1500, SPI_CTAR_PBR(0)|SPI_CTAR_BR(4)|SPI_CTAR_PCSSCK(0)|SPI_CTAR_CSSCK(3)},
 /* 10 */ { 1200, SPI_CTAR_PBR(2)|SPI_CTAR_BR(3)|SPI_CTAR_PCSSCK(2)|SPI_CTAR_CSSCK(1)},
 /* 11 */ { 1000, SPI_CTAR_PBR(1)|SPI_CTAR_BR(4)|SPI_CTAR_PCSSCK(1)|SPI_CTAR_CSSCK(2)},
 /* 12 */ {  857, SPI_CTAR_PBR(3)|SPI_CTAR_BR(3)|SPI_CTAR_PCSSCK(1)|SPI_CTAR_CSSCK(2)},
 /* 13 */ {  750, SPI_CTAR_PBR(0)|SPI_CTAR_BR(5)|SPI_CTAR_PCSSCK(2)|SPI_CTAR_CSSCK(2)},
 /* 14 */ {  600, SPI_CTAR_PBR(2)|SPI_CTAR_BR(4)|SPI_CTAR_PCSSCK(2)|SPI_CTAR_CSSCK(2)},
 /* 15 */ {  500, SPI_CTAR_PBR(1)|SPI_CTAR_BR(5)|SPI_CTAR_PCSSCK(1)|SPI_CTAR_CSSCK(3)},
 /* 16 */ {  429, SPI_CTAR_PBR(3)|SPI_CTAR_BR(4)|SPI_CTAR_PCSSCK(3)|SPI_CTAR_CSSCK(2)},
 /* 17 */ {  250, SPI_CTAR_PBR(1)|SPI_CTAR_BR(6)|SPI_CTAR_PCSSCK(1)|SPI_CTAR_CSSCK(4)},
 /* 18 */ {  125, SPI_CTAR_PBR(1)|SPI_CTAR_BR(7)|SPI_CTAR_PCSSCK(1)|SPI_CTAR_CSSCK(5)},
 /* 19 */ {  107, SPI_CTAR_PBR(3)|SPI_CTAR_BR(6)|SPI_CTAR_PCSSCK(3)|SPI_CTAR_CSSCK(4)},
};                                                                                 

//! Sets Communication speed for SPI
//!
//! @param freq => Frequency in kHz (0 => use default value)
//!
//! @return  \ref BDM_RC_OK              => Success                 \n
//!          \ref BDM_RC_ILLEGAL_PARAMS  => Speed is not supported
//!
uint8_t lcd_spiSetSpeed(uint16_t freq) {
   if (freq == 0) {
      freq = DEFAULT_SPI_FREQUENCY;
   }
   unsigned index;
   for(index=0; index<sizeof(spiBaudTable)/sizeof(spiBaudTable[0]); index++) {
      if (spiBaudTable[index].freq <= freq) {
         break;
      }
   }
   if (index>=sizeof(spiBaudTable)/sizeof(spiBaudTable[0])) {
      return 1;
   }
   // Enable SPI module clock
   SPI_CLK_ENABLE();

   spiBaudValue = spiBaudTable[index].ctarBaud;

   lcd_spiSetCTAR0(ctar0Value);
   lcd_spiSetCTAR1(ctar1Value);

   return 0;
}

//! Transmit a 8-bit command to display 
//!
//! @param send - command to send
//!
void lcd_spiTxCommand(uint8_t data) {
   SPI_CSn_PCOR  = SPI_CSn_MASK; // CS=L
   SPI0_PUSHR = SPI_PUSHR_CTAS(ctas_9bit)|SPI_PUSHR_EOQ_MASK|SWD_PUSHR_TX|SPI_PUSHR_TXDATA(data); 
   while ((SPI0_SR & SPI_SR_EOQF_MASK) == 0) {
   }
   (void)SPI0_POPR;              // Discard read data
   SPI0_SR = SPI_SR_RFDF_MASK|SPI_SR_EOQF_MASK;
   SPI_CSn_PSOR  = SPI_CSn_MASK; // CS=H
}

//! Transmit 8-bit data to display 
//!
//! @param send - data to send
//!
void lcd_spiTxData(uint8_t data) {
   SPI_CSn_PCOR  = SPI_CSn_MASK; // CS=L
   SPI0_PUSHR = SPI_PUSHR_CTAS(ctas_9bit)|SPI_PUSHR_EOQ_MASK|SWD_PUSHR_TX|SPI_PUSHR_TXDATA(0x100|data); 
   while ((SPI0_SR & SPI_SR_EOQF_MASK) == 0) {
   }
   (void)SPI0_POPR;              // Discard read data
   SPI0_SR = SPI_SR_RFDF_MASK|SPI_SR_EOQF_MASK;
   SPI_CSn_PSOR  = SPI_CSn_MASK; // CS=H
}
