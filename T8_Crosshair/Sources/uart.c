/*
 * uart.cpp
 *
 *  Created on: 14/04/2013
 *      Author: pgo
 *      
 *  Very basic serial I/O routines for Codewarrior
 */

#include <stdio.h>

#include <derivative.h>
#include "system.h"
#include "uart.h"
#include "Freedom.h"

#define UART_NUM               0        

#define UART_RX_PIN_REG        B
#define UART_RX_PIN_NUM       16
#define UART_RX_PIN_FN         3

#define UART_TX_PIN_REG        B
#define UART_TX_PIN_NUM       17
#define UART_TX_PIN_FN         3


#define UART_TX_PIN_MASK      (1<<UART_TX_PIN_NUM)
#define UART_TX_PIN_PCR       PCR(UART_TX_PIN_REG,UART_TX_PIN_NUM)
#define UART_TX_PIN_PDOR      PDOR(UART_TX_PIN_REG)
#define UART_TX_PIN_PSOR      PSOR(UART_TX_PIN_REG)  // Data set
#define UART_TX_PIN_PCOR      PCOR(UART_TX_PIN_REG)  // Data clear
#define UART_TX_PIN_PTOR      PTOR(UART_TX_PIN_REG)  // Data toggle
#define UART_TX_PIN_PDIR      PDIR(UART_TX_PIN_REG)  // Data input
#define UART_TX_PIN_PDDR      PDDR(UART_TX_PIN_REG)  // Data direction

#define UART_RX_PIN_MASK      (1<<UART_RX_PIN_NUM)
#define UART_RX_PIN_PCR       PCR(UART_RX_PIN_REG,UART_RX_PIN_NUM)
#define UART_RX_PIN_PDOR      PDOR(UART_RX_PIN_REG)
#define UART_RX_PIN_PSOR      PSOR(UART_RX_PIN_REG)  // Data set
#define UART_RX_PIN_PCOR      PCOR(UART_RX_PIN_REG)  // Data clear
#define UART_RX_PIN_PTOR      PTOR(UART_RX_PIN_REG)  // Data toggle
#define UART_RX_PIN_PDIR      PDIR(UART_RX_PIN_REG)  // Data input
#define UART_RX_PIN_PDDR      PDDR(UART_RX_PIN_REG)  // Data direction

#define SIM_SCGC5_PORTx_MASK(reg)      CONCAT3_(SIM_SCGC5_PORT,reg,_MASK)
#define SIM_SCGC4_UARTx_MASK(uartNum)  CONCAT3_(SIM_SCGC4_UART,uartNum,_MASK) 
#define UARTx_BASE_PTR(uartNum)        CONCAT3_(UART,uartNum,_BASE_PTR) 

#define UART_BASE_PTR            UARTx_BASE_PTR(UART_NUM)

#define SIM_SCGC4_UART_MASK      SIM_SCGC4_UARTx_MASK(UART_NUM)
#define SIM_SCGC5_PORT_UART_MASK SIM_SCGC5_PORTx_MASK(UART_RX_PIN_REG)

#define DEFAULT_BAUDRATE (19200)

static int uartInitialised = 0;

void InitializeUART(int baudRate) {
   uint16_t ubd;

   uartInitialised = 1;
   
   // Enable clock to UART
   SIM_SCGC4 |= SIM_SCGC4_UART_MASK;
   SIM_SCGC5 |= SIM_SCGC5_PORT_UART_MASK;

   // Set Tx & Rx Pin function
   UART_TX_PIN_PCR = PORT_PCR_MUX(UART_TX_PIN_FN) | PORT_PCR_DSE_MASK;
   UART_RX_PIN_PCR = PORT_PCR_MUX(UART_RX_PIN_FN);

   // Set Tx & Rx pins in use
   SIM_SOPT5 &= ~(SIM_SOPT5_UART0RXSRC_MASK|SIM_SOPT5_UART0TXSRC_MASK);

   // Disable UART before changing registers
   UART_BASE_PTR->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

   // Calculate baud settings
   ubd = (uint16_t)(SystemCoreClock/(baudRate * 16));

   // Set Baud rate register
   UART_BASE_PTR->BDH = (UART_BASE_PTR->BDH&~UART_BDH_SBR_MASK) | UART_BDH_SBR((ubd>>8));
   UART_BASE_PTR->BDL = UART_BDL_SBR(ubd);

#ifdef UART_C4_BRFA_MASK
   // Determine fractional divider to get closer to the baud rate
   uint16_t brfa;
   brfa     = (uint8_t)(((SystemCoreClock*32000)/(baudRate * 16)) - (ubd * 32));
   UART_BASE_PTR->C4 = (UART_BASE_PTR->C4&~UART_C4_BRFA_MASK) | UART_C4_BRFA(brfa);

#endif
   UART_BASE_PTR->C1 = 0;

   // Enable UART Tx & Rx
   UART_BASE_PTR->C2 = UART_C2_TE_MASK|UART_C2_RE_MASK;
}

void WriteUARTN(int ch) {
   while ((UART_BASE_PTR->S1 & UART_S1_TDRE_MASK) == 0) {
      // Wait for Tx buffer empty
   }
   UART_BASE_PTR->D = ch;
}

int ReadUARTN(void) {
   while ((UART_BASE_PTR->S1 & UART_S1_RDRF_MASK) == 0) {
      // Wait for Rx buffer full
   }
   return UART_BASE_PTR->D;
}

int __write_console(__file_handle handle, unsigned char* buffer, size_t* count) {
   size_t charCount = 0;
   
   (void) handle;
   if (!uartInitialised) {
	   clock_initialise();
      InitializeUART(DEFAULT_BAUDRATE);
   }
   while (*count>0) {
      char ch = *buffer++;
      if (ch == '\n') {
         WriteUARTN('\r');
      }
      WriteUARTN(ch);
      (*count)--;
      charCount++;
   }
   // Return number of characters written
   *count = charCount;
   return (__no_io_error);
}

int __read_console(__file_handle handle, unsigned char* buffer, size_t * count)  {
   char ch;
   (void) handle;
   if (!uartInitialised) {
      InitializeUART(DEFAULT_BAUDRATE);
   }
   // gets a single char
   ch = ReadUARTN();
   WriteUARTN(ch);
   if (ch == '\r') {
      ch = '\n';
      WriteUARTN(ch);
   }
   *buffer = ch;
   *count  = 1;
   return (__no_io_error);
}

int __close_console(__file_handle handle) {
   (void) handle;
   return (__no_io_error);   
}

