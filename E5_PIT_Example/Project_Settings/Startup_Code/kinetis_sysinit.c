/*
 *    kinetis_sysinit.c - Default init routines for P0
 *                     		Kinetis ARM systems
 *    Copyright © 2012 Freescale semiConductor Inc. All Rights Reserved.
 */
#include <string.h>
#include "kinetis_sysinit.h"
#include "derivative.h"

/*
 * Vector table related
 */
typedef void( *const intfunc )( void );

typedef struct {
   uint32_t *initialSP;
   intfunc  handlers[];
} VectorTable;

/**
 **===========================================================================
 **  External declarations
 **===========================================================================
 */
#if __cplusplus
extern "C" {
#endif

extern unsigned long _estack;
void __thumb_startup(void) __attribute__((__interrupt__));
extern unsigned long _estack;
extern VectorTable const __vector_table;
#if __cplusplus
}
#endif

/**
 **===========================================================================
 **  Reset handler
 **===========================================================================
 */
void __init_hardware()
{
   SCB_VTOR = (uint32_t)&__vector_table; /* Set the interrupt vector table position */
   /*
      Disable the Watchdog because it may reset the core before entering main().
      There are 2 unlock words which shall be provided in sequence before
      accessing the control register.
   */
   WDOG_UNLOCK = KINETIS_WDOG_UNLOCK_SEQ_1;
   WDOG_UNLOCK = KINETIS_WDOG_UNLOCK_SEQ_2;
   WDOG_STCTRLH = KINETIS_WDOG_DISABLED_CTRL;
}

/*
 * Vector table related
 */
typedef void( *const intfunc )( void );

#define WEAK_DEFAULT_HANDLER __attribute__ ((__weak__, alias("Default_Handler")))

#ifndef SCB_ICSR
#define SCB_ICSR (*(volatile uint32_t*)(0xE000ED04))
#endif

/**
 **===========================================================================
 **  Default interrupt handler
 **===========================================================================
 */
__attribute__((__interrupt__))
void Default_Handler(void) {

   uint32_t vectorNum = SCB_ICSR;

   (void)vectorNum;

   while (1) {
      asm("bkpt #0");
   }
}

typedef struct {
   unsigned int r0;
   unsigned int r1;
   unsigned int r2;
   unsigned int r3;
   unsigned int r12;
   unsigned int lr;
   unsigned int pc;
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
   __asm volatile (
          "       tst lr, #4                                    \n"
          "       ite eq                                        \n"
          "       mrseq r0, msp                                 \n"
          "       mrsne r0, psp                                 \n"
          "       ldr r2, handler_addr_const                    \n"
          "       bx r2                                         \n"
          "       handler_addr_const: .word _HardFault_Handler  \n"
      );
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
#ifdef SCB_HFSR
   char reason[200] = "";
   volatile ExceptionInfo exceptionInfo = {0};
   exceptionInfo.scb_hfsr = SCB_HFSR;
   (void)exceptionInfo.scb_hfsr;
   if ((exceptionInfo.scb_hfsr&SCB_HFSR_FORCED_MASK) != 0) {
      // Forced
      exceptionInfo.scb_cfsr = SCB_CFSR;

      if (SCB_CFSR&SCB_CFSR_BFARVALID_MASK) {
         exceptionInfo.scb_bfar = SCB_BFAR;
      }
      /* CFSR Bit Fields */
      if (SCB_CFSR&SCB_CFSR_DIVBYZERO_MASK  ) { strcat(reason, "Divide by zero,"); }
      if (SCB_CFSR&SCB_CFSR_UNALIGNED_MASK  ) { strcat(reason, "Unaligned access,"); }
      if (SCB_CFSR&SCB_CFSR_NOCP_MASK       ) { strcat(reason, "No co-processor"); }
      if (SCB_CFSR&SCB_CFSR_INVPC_MASK      ) { strcat(reason, "Invalid PC (on return),"); }
      if (SCB_CFSR&SCB_CFSR_INVSTATE_MASK   ) { strcat(reason, "Invalid state (EPSR.T/IT,"); }
      if (SCB_CFSR&SCB_CFSR_UNDEFINSTR_MASK ) { strcat(reason, "Undefined Instruction,"); }
      if (SCB_CFSR&SCB_CFSR_BFARVALID_MASK  ) { strcat(reason, "BFAR contents valid,"); }
      if (SCB_CFSR&SCB_CFSR_LSPERR_MASK     ) { strcat(reason, "Bus fault on FP state save,"); }
      if (SCB_CFSR&SCB_CFSR_STKERR_MASK     ) { strcat(reason, "Bus fault on exception entry,"); }
      if (SCB_CFSR&SCB_CFSR_UNSTKERR_MASK   ) { strcat(reason, "Bus fault on exception return,"); }
      if (SCB_CFSR&SCB_CFSR_IMPRECISERR_MASK) { strcat(reason, "Imprecise data access error,"); }
      if (SCB_CFSR&SCB_CFSR_PRECISERR_MASK  ) { strcat(reason, "Precise data access error,"); }
      if (SCB_CFSR&SCB_CFSR_IBUSERR_MASK    ) { strcat(reason, "Bus fault on instruction pre-fetch,"); }
      if (SCB_CFSR&SCB_CFSR_MMARVALID_MASK  ) { strcat(reason, "MMAR contents valid,"); }
      if (SCB_CFSR&SCB_CFSR_MLSPERR_MASK    ) { strcat(reason, "MemManage fault on FP state save,"); }
      if (SCB_CFSR&SCB_CFSR_MSTKERR_MASK    ) { strcat(reason, "MemManage fault on exception entry,"); }
      if (SCB_CFSR&SCB_CFSR_MUNSTKERR_MASK  ) { strcat(reason, "MemManage fault on exception return,"); }
      if (SCB_CFSR&SCB_CFSR_DACCVIOL_MASK   ) { strcat(reason, "MemManage access violation on data access,"); }
      if (SCB_CFSR&SCB_CFSR_IACCVIOL_MASK   ) { strcat(reason, "MemManage access violation on instruction fetch,"); }
   }
#endif
   while (1) {
      asm("bkpt #0");
   }
}

/*
 * Each vector is assigned an unique name.  This is then 'weakly' assigned to the
 * default handler.
 * To install a handler, create a function with the name shown and it will override
 * the weak default.
 */
void NMI_Handler(void)                 WEAK_DEFAULT_HANDLER;
void MemManage_Handler(void)           WEAK_DEFAULT_HANDLER;
void BusFault_Handler(void)            WEAK_DEFAULT_HANDLER;
void UsageFault_Handler(void)          WEAK_DEFAULT_HANDLER;
void SVC_Handler(void)                 WEAK_DEFAULT_HANDLER;
void DebugMon_Handler(void)            WEAK_DEFAULT_HANDLER;
void PendSV_Handler(void)              WEAK_DEFAULT_HANDLER;
void SysTick_Handler(void)             WEAK_DEFAULT_HANDLER;

void DMA0_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void DMA1_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void DMA2_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void DMA3_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void DMA_Error_Handler(void)           WEAK_DEFAULT_HANDLER;
void DMA_Other_IRQHandler(void)        WEAK_DEFAULT_HANDLER;
void FTFL_Command_IRQHandler(void)     WEAK_DEFAULT_HANDLER;
void FTFL_Collision_IRQHandler(void)   WEAK_DEFAULT_HANDLER;
void PMC_IRQHandler(void)              WEAK_DEFAULT_HANDLER;
void LLWU_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void WDOG_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void I2C0_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void SPI0_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void I2S0_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void I2S1_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void UART0_LON_IRQHandler(void)        WEAK_DEFAULT_HANDLER;
void UART0_Status_IRQHandler(void)     WEAK_DEFAULT_HANDLER;
void UART0_Error_IRQHandler(void)      WEAK_DEFAULT_HANDLER;
void UART1_Status_IRQHandler(void)     WEAK_DEFAULT_HANDLER;
void UART1_Error_IRQHandler(void)      WEAK_DEFAULT_HANDLER;
void UART2_Status_IRQHandler(void)     WEAK_DEFAULT_HANDLER;
void UART2_Error_IRQHandler(void)      WEAK_DEFAULT_HANDLER;
void ADC0_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void CMP0_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void CMP1_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void FTM0_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void FTM1_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void CMT_IRQHandler(void)              WEAK_DEFAULT_HANDLER;
void RTC_Alarm_IRQHandler(void)        WEAK_DEFAULT_HANDLER;
void RTC_Seconds_IRQHandler(void)      WEAK_DEFAULT_HANDLER;
void PIT_Ch0_IRQHandler(void)          WEAK_DEFAULT_HANDLER;
void PIT_Ch1_IRQHandler(void)          WEAK_DEFAULT_HANDLER;
void PIT_Ch2_IRQHandler(void)          WEAK_DEFAULT_HANDLER;
void PIT_Ch3_IRQHandler(void)          WEAK_DEFAULT_HANDLER;
void PDB_Handler(void)                 WEAK_DEFAULT_HANDLER;
void USBOTG_IRQHandler(void)           WEAK_DEFAULT_HANDLER;
void USB0_Charge_IRQHandler(void)      WEAK_DEFAULT_HANDLER;
void TSI0_IRQHandler(void)             WEAK_DEFAULT_HANDLER;
void MCG_IRQHandler(void)              WEAK_DEFAULT_HANDLER;
void LPTMR0_IRQHandler(void)           WEAK_DEFAULT_HANDLER;
void PORTA_IRQHandler(void)            WEAK_DEFAULT_HANDLER;
void PORTB_IRQHandler(void)            WEAK_DEFAULT_HANDLER;
void PORTC_IRQHandler(void)            WEAK_DEFAULT_HANDLER;
void PORTD_IRQHandler(void)            WEAK_DEFAULT_HANDLER;
void PORTE_IRQHandler(void)            WEAK_DEFAULT_HANDLER;
void SWI_IRQHandler(void)              WEAK_DEFAULT_HANDLER;

__attribute__ ((section(".vectortable")))
VectorTable const __vector_table = {
    &_estack,                       /* Vec #0   Initial stack pointer                        */
    {
      __thumb_startup,              /* Vec #1   Reset Handler                                */
          NMI_Handler,              /* Vec #2   NMI Handler                                  */
(intfunc) HardFault_Handler,        /* Vec #3   Hard Fault Handler                           */
          MemManage_Handler,        /* Vec #4   MPU Fault Handler                            */
          BusFault_Handler,         /* Vec #5   Bus Fault Handler                            */
          UsageFault_Handler,       /* Vec #6   Usage Fault Handler                          */
          Default_Handler,          /* Vec #7   Reserved                                     */
          Default_Handler,          /* Vec #8   Reserved                                     */
          Default_Handler,          /* Vec #9   Reserved                                     */
          Default_Handler,          /* Vec #10  Reserved                                     */
          SVC_Handler,              /* Vec #11  SVCall Handler                               */
          DebugMon_Handler,         /* Vec #12  Debug Monitor Handler                        */
          Default_Handler,          /* Vec #13  Reserved                                     */
          PendSV_Handler,           /* Vec #14  PendSV Handler                               */
          SysTick_Handler,          /* Vec #15  SysTick Handler                              */

                                    /* External Interrupts */
          DMA0_IRQHandler,          /* Vec #16 - Int #0   DMA Channel 0 Transfer Complete and Error    */
          DMA1_IRQHandler,          /* Vec #17 - Int #1   DMA Channel 1 Transfer Complete and Error    */
          DMA2_IRQHandler,          /* Vec #18 - Int #2   DMA Channel 2 Transfer Complete and Error    */
          DMA3_IRQHandler,          /* Vec #19 - Int #3   DMA Channel 3 Transfer Complete and Error    */
          DMA_Error_Handler,        /* Vec #20 - Int #4   DMA error interrupt channel                  */
          DMA_Other_IRQHandler,     /* Vec #21 - Int #5   DMA ?     */
          FTFL_Command_IRQHandler,  /* Vec #22 - Int #6   FTFL Flash command complete                  */
          FTFL_Collision_IRQHandler,/* Vec #23 - Int #7   FTFL Flash read collision                    */
          PMC_IRQHandler,           /* Vec #24 - Int #8   PMC Low-voltage detect, low-voltage warning  */
          LLWU_IRQHandler,          /* Vec #25 - Int #9   LLWU Low Leakage Wake-up                     */
          WDOG_IRQHandler,          /* Vec #26 - Int #10  EWM and WDOG interrupt                       */
          I2C0_IRQHandler,          /* Vec #27 - Int #11  I2C0 interrupt                               */
          SPI0_IRQHandler,          /* Vec #28 - Int #12  SPI0 Interrupt                               */
          I2S0_IRQHandler,          /* Vec #29 - Int #13  I2S0 interrupt                               */
          I2S1_IRQHandler,          /* Vec #30 - Int #14  I2S1 interrupt                               */
          UART0_LON_IRQHandler,     /* Vec #31 - Int #15  UART0 LON interrupt                          */
          UART0_Status_IRQHandler,  /* Vec #32 - Int #16  UART0 Status Interrupt                       */
          UART0_Error_IRQHandler,   /* Vec #33 - Int #17  UART0 Error interrupt                        */
          UART1_Status_IRQHandler,  /* Vec #34 - Int #18  UART1 Status Interrupt                       */
          UART1_Error_IRQHandler,   /* Vec #35 - Int #19  UART1 Error interrupt                        */
          UART2_Status_IRQHandler,  /* Vec #36 - Int #20  UART2 Status Interrupt                       */
          UART2_Error_IRQHandler,   /* Vec #37 - Int #21  UART2 Error interrupt                        */
          ADC0_IRQHandler,          /* Vec #38 - Int #22  ADC0 interrupt                               */
          CMP0_IRQHandler,          /* Vec #39 - Int #23  CMP0 interrupt                               */
          CMP1_IRQHandler,          /* Vec #40 - Int #24  CMP1 interrupt                               */
          FTM0_IRQHandler,          /* Vec #41 - Int #25  FTM0 interrupt                               */
          FTM1_IRQHandler,          /* Vec #42 - Int #26  FTM1 interrupt                               */
          CMT_IRQHandler,           /* Vec #43 - Int #27  CMT  interrupt                               */
          RTC_Alarm_IRQHandler,     /* Vec #44 - Int #28  RTC Alarm interrupt                          */
          RTC_Seconds_IRQHandler,   /* Vec #45 - Int #29  RTC Seconds interrupt                        */
          PIT_Ch0_IRQHandler,       /* Vec #46 - Int #30  PIT Channel 0 interrupt                      */
          PIT_Ch1_IRQHandler,       /* Vec #47 - Int #31  PIT Channel 1 interrupt                      */
          PIT_Ch2_IRQHandler,       /* Vec #48 - Int #32  PIT Channel 2 interrupt                      */
          PIT_Ch3_IRQHandler,       /* Vec #49 - Int #33  PIT Channel 3 interrupt                      */
          PDB_Handler,              /* Vec #50 - Int #34  PDB interrupt                                */
          USBOTG_IRQHandler,        /* Vec #51 - Int #35  USB OTG interrupt                            */
          USB0_Charge_IRQHandler,   /* Vec #52 - Int #36  USB Charger detect interrupt                 */
          TSI0_IRQHandler,          /* Vec #53 - Int #37  TSI0 Interrupt                               */
          MCG_IRQHandler,           /* Vec #54 - Int #38  MCG Interrupt                                */
          LPTMR0_IRQHandler,        /* Vec #55 - Int #39  LPTMR0 interrupt                             */
          PORTA_IRQHandler,         /* Vec #56 - Int #40  Port A interrupt                             */
          PORTB_IRQHandler,         /* Vec #57 - Int #41  Port B interrupt                             */
          PORTC_IRQHandler,         /* Vec #58 - Int #42  Port C interrupt                             */
          PORTD_IRQHandler,         /* Vec #59 - Int #43  Port D interrupt                             */
          PORTE_IRQHandler,         /* Vec #60 - Int #44  Port E interrupt                             */
          SWI_IRQHandler,           /* Vec #61 - Int #45  SWI interrupt                                */
    }
};

