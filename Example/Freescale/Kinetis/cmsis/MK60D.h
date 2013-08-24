/**************************************************************************//**
 * @file     MK60D.h
 * @brief    CMSIS Cortex-M4 Core Peripheral Access Layer Header File for
 *           Device MK60D
 * @version  V3.01
 * @date     06. March 2012
 *
 * @note
 * Copyright (C) 2010-2012 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#ifndef MK60D_H
#define MK60D_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup MK60D_Definitions MK60D Definitions
  This file defines all structures and symbols for MK60D:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup MK60D_CMSIS Device CMSIS Definitions
  Configuration of the Cortex-M4 Processor and Core Peripherals
  @{
*/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  MemoryManagement_IRQn         = -12,      /*!<  4 Memory Management Interrupt                   */
  BusFault_IRQn                 = -11,      /*!<  5 Bus Fault Interrupt                           */
  UsageFault_IRQn               = -10,      /*!<  6 Usage Fault Interrupt                         */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
  DebugMonitor_IRQn             = -4,       /*!< 12 Debug Monitor Interrupt                       */
  PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
  SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */

/******  Device Specific Interrupt Numbers ********************************************************/
/* ToDo: add here your device specific external interrupt numbers
         according the interrupt handlers defined in startup_Device.s
         eg.: Interrupt for Timer#1       TIM1_IRQHandler   ->   TIM1_IRQn                        */
  DMA0_IRQn                     = 0,               /*!< DMA Channel 0 Transfer Complete */
  DMA1_IRQn                     = 1,               /*!< DMA Channel 1 Transfer Complete */
  DMA2_IRQn                     = 2,               /*!< DMA Channel 2 Transfer Complete */
  DMA3_IRQn                     = 3,               /*!< DMA Channel 3 Transfer Complete */
  DMA4_IRQn                     = 4,               /*!< DMA Channel 4 Transfer Complete */
  DMA5_IRQn                     = 5,               /*!< DMA Channel 5 Transfer Complete */
  DMA6_IRQn                     = 6,               /*!< DMA Channel 6 Transfer Complete */
  DMA7_IRQn                     = 7,               /*!< DMA Channel 7 Transfer Complete */
  DMA8_IRQn                     = 8,               /*!< DMA Channel 8 Transfer Complete */
  DMA9_IRQn                     = 9,               /*!< DMA Channel 9 Transfer Complete */
  DMA10_IRQn                    = 10,               /*!< DMA Channel 10 Transfer Complete */
  DMA11_IRQn                    = 11,               /*!< DMA Channel 11 Transfer Complete */
  DMA12_IRQn                    = 12,               /*!< DMA Channel 12 Transfer Complete */
  DMA13_IRQn                    = 13,               /*!< DMA Channel 13 Transfer Complete */
  DMA14_IRQn                    = 14,               /*!< DMA Channel 14 Transfer Complete */
  DMA15_IRQn                    = 15,               /*!< DMA Channel 15 Transfer Complete */
  DMA_Error_IRQn                = 16,               /*!< DMA Error Interrupt */
  MCM_IRQn                      = 17,               /*!< Normal Interrupt */
  FTFL_IRQn                     = 18,               /*!< FTFL Interrupt */
  Read_Collision_IRQn           = 19,               /*!< Read Collision Interrupt */
  LVD_LVW_IRQn                  = 20,               /*!< Low Voltage Detect, Low Voltage Warning */
  LLW_IRQn                      = 21,               /*!< Low Leakage Wakeup */
  Watchdog_IRQn                 = 22,               /*!< WDOG Interrupt */
  RNG_IRQn                      = 23,               /*!< RNGB Interrupt */
  I2C0_IRQn                     = 24,               /*!< I2C0 interrupt */
  I2C1_IRQn                     = 25,               /*!< I2C1 interrupt */
  SPI0_IRQn                     = 26,               /*!< SPI0 Interrupt */
  SPI1_IRQn                     = 27,               /*!< SPI1 Interrupt */
  SPI2_IRQn                     = 28,               /*!< SPI2 Interrupt */
  CAN0_ORed_Message_buffer_IRQn = 29,               /*!< CAN0 OR'd Message Buffers Interrupt */
  CAN0_Bus_Off_IRQn             = 30,               /*!< CAN0 Bus Off Interrupt */
  CAN0_Error_IRQn               = 31,               /*!< CAN0 Error Interrupt */
  CAN0_Tx_Warning_IRQn          = 32,               /*!< CAN0 Tx Warning Interrupt */
  CAN0_Rx_Warning_IRQn          = 33,               /*!< CAN0 Rx Warning Interrupt */
  CAN0_Wake_Up_IRQn             = 34,               /*!< CAN0 Wake Up Interrupt */
  Reserved51_IRQn               = 35,               /*!< Reserved interrupt 51 */
  Reserved52_IRQn               = 36,               /*!< Reserved interrupt 52 */
  CAN1_ORed_Message_buffer_IRQn = 37,               /*!< CAN1 OR'd Message Buffers Interrupt */
  CAN1_Bus_Off_IRQn             = 38,               /*!< CAN1 Bus Off Interrupt */
  CAN1_Error_IRQn               = 39,               /*!< CAN1 Error Interrupt */
  CAN1_Tx_Warning_IRQn          = 40,               /*!< CAN1 Tx Warning Interrupt */
  CAN1_Rx_Warning_IRQn          = 41,               /*!< CAN1 Rx Warning Interrupt */
  CAN1_Wake_Up_IRQn             = 42,               /*!< CAN1 Wake Up Interrupt */
  Reserved59_IRQn               = 43,               /*!< Reserved interrupt 59 */
  Reserved60_IRQn               = 44,               /*!< Reserved interrupt 60 */
  UART0_RX_TX_IRQn              = 45,               /*!< UART0 Receive/Transmit interrupt */
  UART0_ERR_IRQn                = 46,               /*!< UART0 Error interrupt */
  UART1_RX_TX_IRQn              = 47,               /*!< UART1 Receive/Transmit interrupt */
  UART1_ERR_IRQn                = 48,               /*!< UART1 Error interrupt */
  UART2_RX_TX_IRQn              = 49,               /*!< UART2 Receive/Transmit interrupt */
  UART2_ERR_IRQn                = 50,               /*!< UART2 Error interrupt */
  UART3_RX_TX_IRQn              = 51,               /*!< UART3 Receive/Transmit interrupt */
  UART3_ERR_IRQn                = 52,               /*!< UART3 Error interrupt */
  UART4_RX_TX_IRQn              = 53,               /*!< UART4 Receive/Transmit interrupt */
  UART4_ERR_IRQn                = 54,               /*!< UART4 Error interrupt */
  UART5_RX_TX_IRQn              = 55,               /*!< UART5 Receive/Transmit interrupt */
  UART5_ERR_IRQn                = 56,               /*!< UART5 Error interrupt */
  ADC0_IRQn                     = 57,               /*!< ADC0 interrupt */
  ADC1_IRQn                     = 58,               /*!< ADC1 interrupt */
  CMP0_IRQn                     = 59,               /*!< CMP0 interrupt */
  CMP1_IRQn                     = 60,               /*!< CMP1 interrupt */
  CMP2_IRQn                     = 61,               /*!< CMP2 interrupt */
  FTM0_IRQn                     = 62,               /*!< FTM0 fault, overflow and channels interrupt */
  FTM1_IRQn                     = 63,               /*!< FTM1 fault, overflow and channels interrupt */
  FTM2_IRQn                     = 64,               /*!< FTM2 fault, overflow and channels interrupt */
  CMT_IRQn                      = 65,               /*!< CMT interrupt */
  RTC_IRQn                      = 66,               /*!< RTC interrupt */
  Reserved83_IRQn               = 67,               /*!< Reserved interrupt 83 */
  PIT0_IRQn                     = 68,               /*!< PIT timer channel 0 interrupt */
  PIT1_IRQn                     = 69,               /*!< PIT timer channel 1 interrupt */
  PIT2_IRQn                     = 70,               /*!< PIT timer channel 2 interrupt */
  PIT3_IRQn                     = 71,               /*!< PIT timer channel 3 interrupt */
  PDB0_IRQn                     = 72,               /*!< PDB0 Interrupt */
  USB0_IRQn                     = 73,               /*!< USB0 interrupt */
  USBDCD_IRQn                   = 74,               /*!< USBDCD Interrupt */
  ENET_1588_Timer_IRQn          = 75,               /*!< Ethernet MAC IEEE 1588 Timer Interrupt */
  ENET_Transmit_IRQn            = 76,               /*!< Ethernet MAC Transmit Interrupt */
  ENET_Receive_IRQn             = 77,               /*!< Ethernet MAC Receive Interrupt */
  ENET_Error_IRQn               = 78,               /*!< Ethernet MAC Error and miscelaneous Interrupt */
  I2S0_IRQn                     = 79,               /*!< I2S0 Interrupt */
  SDHC_IRQn                     = 80,               /*!< SDHC Interrupt */
  DAC0_IRQn                     = 81,               /*!< DAC0 interrupt */
  DAC1_IRQn                     = 82,               /*!< DAC1 interrupt */
  TSI0_IRQn                     = 83,               /*!< TSI0 Interrupt */
  MCG_IRQn                      = 84,              /*!< MCG Interrupt */
  LPTimer_IRQn                  = 85,              /*!< LPTimer interrupt */
  Reserved102_IRQn              = 86,              /*!< Reserved interrupt 102 */
  PORTA_IRQn                    = 87,              /*!< Port A interrupt */
  PORTB_IRQn                    = 88,              /*!< Port B interrupt */
  PORTC_IRQn                    = 89,              /*!< Port C interrupt */
  PORTD_IRQn                    = 90,              /*!< Port D interrupt */
  PORTE_IRQn                    = 91,              /*!< Port E interrupt */
  Reserved108              = 92,              /*!< Reserved interrupt 108 */
  Reserved109              = 93,              /*!< Reserved interrupt 109 */
  Reserved110              = 94,              /*!< Reserved interrupt 110 */
  Reserved111              = 95,              /*!< Reserved interrupt 111 */
  Reserved112              = 96,              /*!< Reserved interrupt 112 */
  Reserved113              = 97,              /*!< Reserved interrupt 113 */
  Reserved114              = 98,              /*!< Reserved interrupt 114 */
  Reserved115              = 99,              /*!< Reserved interrupt 115 */
  Reserved116              = 100,              /*!< Reserved interrupt 116 */
  Reserved117              = 101,              /*!< Reserved interrupt 117 */
  Reserved118              = 102,              /*!< Reserved interrupt 118 */
  Reserved119              = 103               /*!< Reserved interrupt 119 */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M4 Processor and Core Peripherals */
//#define __CM4_REV                 0x0201    /*!< Core Revision r2p1                               */
#define __NVIC_PRIO_BITS          4         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __FPU_PRESENT             0        /*!< FPU present or not                                */

/*@}*/ /* end of group MK60D_CMSIS */


#include <core_cm4.h>                       /* Cortex-M4 processor and core peripherals           */
#include "system_MK60D.h"                /* MK60D System  include file                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/
/** @addtogroup MK60D_Peripherals MK60D Peripherals
  MK60D Device Specific Peripheral registers structures
  @{
*/

#if 0

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/* ToDo: add here your device specific peripheral access structure typedefs
         following is an example for a timer                                  */

/*------------- 16-bit Timer/Event Counter (TMR) -----------------------------*/
/** @addtogroup MK60D_TMR MK60D 16-bit Timer/Event Counter (TMR)
  @{
*/
typedef struct
{
  __IO uint32_t EN;                         /*!< Offset: 0x0000   Timer Enable Register           */               
  __IO uint32_t RUN;                        /*!< Offset: 0x0004   Timer RUN Register              */
  __IO uint32_t CR;                         /*!< Offset: 0x0008   Timer Control Register          */
  __IO uint32_t MOD;                        /*!< Offset: 0x000C   Timer Mode Register             */
       uint32_t RESERVED0[1];
  __IO uint32_t ST;                         /*!< Offset: 0x0014   Timer Status Register           */
  __IO uint32_t IM;                         /*!< Offset: 0x0018   Interrupt Mask Register         */
  __IO uint32_t UC;                         /*!< Offset: 0x001C   Timer Up Counter Register       */
  __IO uint32_t RG0                         /*!< Offset: 0x0020   Timer Register                  */
       uint32_t RESERVED1[2];
  __IO uint32_t CP;                         /*!< Offset: 0x002C   Capture register                */
} <DeviceAbbreviation>_TMR_TypeDef;
/*@}*/ /* end of group MK60D_TMR */


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

/*@}*/ /* end of group MK60D_Peripherals */


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* ToDo: add here your device peripherals base addresses                
         following is an example for timer                                    */
/** @addtogroup MK60D_MemoryMap MK60D Memory Mapping
  @{
*/

/* Peripheral and SRAM base address */
#define <DeviceAbbreviation>_FLASH_BASE       (0x00000000UL)                              /*!< (FLASH     ) Base Address */
#define <DeviceAbbreviation>_SRAM_BASE        (0x20000000UL)                              /*!< (SRAM      ) Base Address */
#define <DeviceAbbreviation>_PERIPH_BASE      (0x40000000UL)                              /*!< (Peripheral) Base Address */

/* Peripheral memory map */
#define <DeviceAbbreviation>TIM0_BASE         (<DeviceAbbreviation>_PERIPH_BASE)          /*!< (Timer0    ) Base Address */
#define <DeviceAbbreviation>TIM1_BASE         (<DeviceAbbreviation>_PERIPH_BASE + 0x0800) /*!< (Timer1    ) Base Address */
#define <DeviceAbbreviation>TIM2_BASE         (<DeviceAbbreviation>_PERIPH_BASE + 0x1000) /*!< (Timer2    ) Base Address */
/*@}*/ /* end of group MK60D_MemoryMap */


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
/* ToDo: add here your device peripherals pointer definitions                
         following is an example for timer                                    */

/** @addtogroup MK60D_PeripheralDecl MK60D Peripheral Declaration
  @{
*/

#define <DeviceAbbreviation>_TIM0        ((<DeviceAbbreviation>_TMR_TypeDef *) <DeviceAbbreviation>TIM0_BASE)
#define <DeviceAbbreviation>_TIM1        ((<DeviceAbbreviation>_TMR_TypeDef *) <DeviceAbbreviation>TIM0_BASE)
#define <DeviceAbbreviation>_TIM2        ((<DeviceAbbreviation>_TMR_TypeDef *) <DeviceAbbreviation>TIM0_BASE)
/*@}*/ /* end of group MK60D_PeripheralDecl */

/*@}*/ /* end of group MK60D_Definitions */

#endif // 0

//#include <MK60N512VMD100.h>
#include <MK60DZ10.h>

#ifdef __cplusplus
}
#endif

#endif  /* MK60D_H */
