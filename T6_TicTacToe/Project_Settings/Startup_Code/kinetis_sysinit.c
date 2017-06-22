/*
 *    kinetis_sysinit.c - Default init routines for P0
 *                     		Kinetis ARM systems
 *    Copyright © 2012 Freescale semiConductor Inc. All Rights Reserved.
 */
 
#include "kinetis_sysinit.h"
#include "derivative.h"
#include "system.h"

/**
 **===========================================================================
 **  External declarations
 **===========================================================================
 */
#if __cplusplus
extern "C" {
#endif
extern uint32_t __vector_table[];
extern unsigned long _estack;
extern void __thumb_startup(void);
#if __cplusplus
}
#endif

typedef struct {
   unsigned int r0;
   unsigned int r1;
   unsigned int r2;
   unsigned int r3;
   unsigned int r12;
   void       (*lr)();
   void       (*pc)();
   unsigned int psr;
} ExceptionFrame;

typedef struct {
   unsigned int scb_hfsr;
   unsigned int scb_cfsr;
   unsigned int scb_bfar;
} ExceptionInfo;

/*  Low-level exception handler
 *
 *  Interface from asm to C.
 *  Passes address of exception handler to C-level handler
 *
 *  See http://www.freertos.org/Debugging-Hard-Faults-On-Cortex-M-Microcontrollers.html
 */
__attribute__((__naked__, __weak__, __interrupt__))
void HardFault_Handler(void) {
        __asm volatile ( "  tst   lr, #4              \n");  // Check mode
        __asm volatile ( "  ite   eq                  \n");  // Get active SP
        __asm volatile ( "  mrseq r0, msp             \n");
        __asm volatile ( "  mrsne r0, psp             \n");
//        __asm volatile ( "  ldr   r1,[r0,#24]         \n");  // PC
//        __asm volatile ( "  push  {r1}                \n");  // Dummy ?
        __asm volatile ( "  bl    _HardFault_Handler  \n");  // Go to C handler
}

/******************************************************************************/
/* Exception frame without floating-point storage
 * hard fault handler in C,
 * with stack frame location as input parameter
 *
 * @param exceptionFrame address of exception frame
 *
 */
void _HardFault_Handler(volatile ExceptionFrame *exceptionFrame) {
   (void)exceptionFrame;
   for(;;) {
     __asm__("bkpt #0");
   }
}


/**
 **===========================================================================
 **  Default interrupt handler
 **===========================================================================
 */
void Default_Handler()
{
	__asm__("bkpt");
}

/**
 **===========================================================================
 **  Reset handler
 **===========================================================================
 */
void __init_hardware()
{
	SCB_VTOR = (uint32_t)__vector_table; /* Set the interrupt vector table position */
	/*
		Disable the Watchdog because it may reset the core before entering main().
		There are 2 unlock words which shall be provided in sequence before
		accessing the control register.
	*/
	WDOG_UNLOCK = KINETIS_WDOG_UNLOCK_SEQ_1;
	WDOG_UNLOCK = KINETIS_WDOG_UNLOCK_SEQ_2;
	WDOG_STCTRLH = KINETIS_WDOG_DISABLED_CTRL;
	
	clock_initialise();
}

/*
 * Each vector is assigned an unique name.  This is then 'weakly' assigned to the
 * default handler.
 * To install a handler, create a function with the name shown and it will override
 * the weak default.
 */
#define WEAK_DEFAULT_HANDLER __attribute__ ((__weak__, alias("Default_Handler")))
void NMI_Handler(void)                        WEAK_DEFAULT_HANDLER;
void MemManage_Handler(void)                  WEAK_DEFAULT_HANDLER;
void BusFault_Handler(void)                   WEAK_DEFAULT_HANDLER;
void UsageFault_Handler(void)                 WEAK_DEFAULT_HANDLER;
void SVC_Handler(void)                        WEAK_DEFAULT_HANDLER;
void DebugMon_Handler(void)                   WEAK_DEFAULT_HANDLER;
void PendSV_Handler(void)                     WEAK_DEFAULT_HANDLER;
void SysTick_Handler(void)                    WEAK_DEFAULT_HANDLER;
void DMA0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void DMA1_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void DMA2_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void DMA3_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void DMA_Error_IRQHandler(void)               WEAK_DEFAULT_HANDLER;
void FTFL_Command_IRQHandler(void)            WEAK_DEFAULT_HANDLER;
void FTFL_Collision_IRQHandler(void)          WEAK_DEFAULT_HANDLER;
void PMC_IRQHandler(void)                     WEAK_DEFAULT_HANDLER;
void LLWU_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void WDOG_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void I2C0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void SPI0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void I2S0_Tx_IRQHandler(void)                 WEAK_DEFAULT_HANDLER;
void I2S0_Rx_IRQHandler(void)                 WEAK_DEFAULT_HANDLER;
void UART0_LON_IRQHandler(void)               WEAK_DEFAULT_HANDLER;
void UART0_RxTx_IRQHandler(void)              WEAK_DEFAULT_HANDLER;
void UART0_Error_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void UART1_RxTx_IRQHandler(void)              WEAK_DEFAULT_HANDLER;
void UART1_Error_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void UART2_RxTx_IRQHandler(void)              WEAK_DEFAULT_HANDLER;
void UART2_Error_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void ADC0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void CMP0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void CMP1_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void FTM0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void FTM1_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void CMT_IRQHandler(void)                     WEAK_DEFAULT_HANDLER;
void RTC_Alarm_IRQHandler(void)               WEAK_DEFAULT_HANDLER;
void RTC_Seconds_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void PIT_Ch0_IRQHandler(void)                 WEAK_DEFAULT_HANDLER;
void PIT_Ch1_IRQHandler(void)                 WEAK_DEFAULT_HANDLER;
void PIT_Ch2_IRQHandler(void)                 WEAK_DEFAULT_HANDLER;
void PIT_Ch3_IRQHandler(void)                 WEAK_DEFAULT_HANDLER;
void PDB0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void USBOTG_IRQHandler(void)                  WEAK_DEFAULT_HANDLER;
void USBDCD_IRQHandler(void)                  WEAK_DEFAULT_HANDLER;
void TSI0_IRQHandler(void)                    WEAK_DEFAULT_HANDLER;
void MCG_IRQHandler(void)                     WEAK_DEFAULT_HANDLER;
void LPTMR0_IRQHandler(void)                  WEAK_DEFAULT_HANDLER;
void PORTA_IRQHandler(void)                   WEAK_DEFAULT_HANDLER;
void PORTB_IRQHandler(void)                   WEAK_DEFAULT_HANDLER;
void PORTC_IRQHandler(void)                   WEAK_DEFAULT_HANDLER;
void PORTD_IRQHandler(void)                   WEAK_DEFAULT_HANDLER;
void PORTE_IRQHandler(void)                   WEAK_DEFAULT_HANDLER;
void SWI_IRQHandler(void)                     WEAK_DEFAULT_HANDLER;


/* The Interrupt Vector Table */
void (* const InterruptVector[])() __attribute__ ((section(".vectortable"))) = {
	/* Processor exceptions            Vec Irq */
	(void(*)(void)) &_estack,      /*   0  -16  Initial stack pointer                                                            */
	__thumb_startup,               /*   1  -15  Reset Handler                                                                    */
    NMI_Handler,                   /*   2, -14  Non maskable Interrupt, cannot be stopped or preempted                           */
    HardFault_Handler,             /*   3, -13  Hard Fault, all classes of Fault                                                 */
    MemManage_Handler,             /*   4, -12  Memory Management, MPU mismatch, including Access Violation and No Match         */
    BusFault_Handler,              /*   5, -11  Bus Fault, Pre-Fetch-, Memory Access Fault, other address/memory related Fault   */
    UsageFault_Handler,            /*   6, -10  Usage Fault, i.e. Undef Instruction, Illegal State Transition                    */
    0,                             /*   7, -9   Reserved                                                                         */
    0,                             /*   8, -8   Reserved                                                                         */
    0,                             /*   9, -7   Reserved                                                                         */
    0,                             /*  10, -6   Reserved                                                                         */
    SVC_Handler,                   /*  11, -5   System Service Call via SVC instruction                                          */
    DebugMon_Handler,              /*  12, -4   Debug Monitor                                                                    */
    0,                             /*  13, -3   Reserved                                                                         */
    PendSV_Handler,                /*  14, -2   Pendable request for system service                                              */
    SysTick_Handler,               /*  15, -1   System Tick Timer                                                                */

	/* External Interrupts */
	DMA0_IRQHandler,               /*  16, 0    DMA channel 0 transfer complete interrupt                                        */
	DMA1_IRQHandler,               /*  17, 1    DMA channel 1 transfer complete interrupt                                        */
	DMA2_IRQHandler,               /*  18, 2    DMA channel 2 transfer complete interrupt                                        */
	DMA3_IRQHandler,               /*  19, 3    DMA channel 3 transfer complete interrupt                                        */
	DMA_Error_IRQHandler,          /*  20, 4    DMA error interrupt                                                              */
	Default_Handler,               /*  21, 5    Reserved                                                                         */
	FTFL_Command_IRQHandler,       /*  22, 6    FTFL interrupt                                                                   */
	FTFL_Collision_IRQHandler,     /*  23, 7    FTFL Read collision interrupt                                                    */
	PMC_IRQHandler,                /*  24, 8    PMC Low Voltage Detect, Low Voltage Warning                                      */
	LLWU_IRQHandler,               /*  25, 9    LLWU Low Leakage Wakeup                                                          */
	WDOG_IRQHandler,               /*  26, 10   WDOG interrupt                                                                   */
	I2C0_IRQHandler,               /*  27, 11   I2C0 interrupt                                                                   */
	SPI0_IRQHandler,               /*  28, 12   SPI0 interrupt                                                                   */
	I2S0_Tx_IRQHandler,            /*  29, 13   I2S0 transmit interrupt                                                          */
	I2S0_Rx_IRQHandler,            /*  30, 14   I2S0 receive interrupt                                                           */
	UART0_LON_IRQHandler,          /*  31, 15   UART0 LON interrupt                                                              */
	UART0_RxTx_IRQHandler,         /*  32, 16   UART0 receive/transmit interrupt                                                 */
	UART0_Error_IRQHandler,        /*  33, 17   UART0 error interrupt                                                            */
	UART1_RxTx_IRQHandler,         /*  34, 18   UART1 receive/transmit interrupt                                                 */
	UART1_Error_IRQHandler,        /*  35, 19   UART1 error interrupt                                                            */
	UART2_RxTx_IRQHandler,         /*  36, 20   UART2 receive/transmit interrupt                                                 */
	UART2_Error_IRQHandler,        /*  37, 21   UART0 error interrupt                                                            */
	ADC0_IRQHandler,               /*  38, 22   ADC0 interrupt                                                                   */
	CMP0_IRQHandler,               /*  39, 23   CMP0 interrupt                                                                   */
	CMP1_IRQHandler,               /*  40, 24   CMP1 interrupt                                                                   */
	FTM0_IRQHandler,               /*  41, 25   FTM0 fault, overflow and channels interrupt                                      */
	FTM1_IRQHandler,               /*  42, 26   FTM1 fault, overflow and channels interrupt                                      */
	CMT_IRQHandler,                /*  43, 27   CMT interrupt                                                                    */
	RTC_Alarm_IRQHandler,          /*  44, 28   RTC interrupt                                                                    */
	RTC_Seconds_IRQHandler,        /*  45, 29   RTC seconds interrupt                                                            */
	PIT_Ch0_IRQHandler,            /*  46, 30   PIT timer channel 0 interrupt                                                    */
	PIT_Ch1_IRQHandler,            /*  47, 31   PIT timer channel 1 interrupt                                                    */
	PIT_Ch2_IRQHandler,            /*  48, 32   PIT timer channel 2 interrupt                                                    */
	PIT_Ch3_IRQHandler,            /*  49, 33   PIT timer channel 3 interrupt                                                    */
	PDB0_IRQHandler,               /*  50, 34   PDB0 Programmable Delay Block interrupt                                          */
	USBOTG_IRQHandler,             /*  51, 35   USB0 OTG interrupt                                                               */
	USBDCD_IRQHandler,             /*  52, 36   USBDCD interrupt                                                                 */
	TSI0_IRQHandler,               /*  53, 37   TSI0 interrupt                                                                   */
	MCG_IRQHandler,                /*  54, 38   MCG interrupt                                                                    */
	LPTMR0_IRQHandler,             /*  55, 39   LPTMR Low Power Timer interrupt                                                  */
	PORTA_IRQHandler,              /*  56, 40   Port A interrupt                                                                 */
	PORTB_IRQHandler,              /*  57, 41   Port B interrupt                                                                 */
	PORTC_IRQHandler,              /*  58, 42   Port C interrupt                                                                 */
	PORTD_IRQHandler,              /*  59, 43   Port D interrupt                                                                 */
	PORTE_IRQHandler,              /*  60, 44   Port E interrupt                                                                 */
	SWI_IRQHandler,                /*  61, 45   Software interrupt                                                               */
};

