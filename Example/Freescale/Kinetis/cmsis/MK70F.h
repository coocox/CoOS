/**************************************************************************//**
 * @file     MK70F.h
 * @brief    CMSIS Cortex-M4 Core Peripheral Access Layer Header File for
 *           Device MK70F
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


#ifndef MK70F_H
#define MK70F_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup MK70F_Definitions MK70F Definitions
  This file defines all structures and symbols for MK70F:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup MK70F_CMSIS Device CMSIS Definitions
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
  DMA0_DMA16_IRQn               = 0,               /**< DMA channel 0/16 transfer complete interrupt */
  DMA1_DMA17_IRQn               = 1,               /**< DMA channel 1/17 transfer complete interrupt */
  DMA2_DMA18_IRQn               = 2,               /**< DMA channel 2/18 transfer complete interrupt */
  DMA3_DMA19_IRQn               = 3,               /**< DMA channel 3/19 transfer complete interrupt */
  DMA4_DMA20_IRQn               = 4,               /**< DMA channel 4/20 transfer complete interrupt */
  DMA5_DMA21_IRQn               = 5,               /**< DMA channel 5/21 transfer complete interrupt */
  DMA6_DMA22_IRQn               = 6,               /**< DMA channel 6/22 transfer complete interrupt */
  DMA7_DMA23_IRQn               = 7,               /**< DMA channel 7/23 transfer complete interrupt */
  DMA8_DMA24_IRQn               = 8,               /**< DMA channel 8/24 transfer complete interrupt */
  DMA9_DMA25_IRQn               = 9,               /**< DMA channel 9/25 transfer complete interrupt */
  DMA10_DMA26_IRQn              = 10,               /**< DMA channel 10/26 transfer complete interrupt */
  DMA11_DMA27_IRQn              = 11,               /**< DMA channel 11/27 transfer complete interrupt */
  DMA12_DMA28_IRQn              = 12,               /**< DMA channel 12/28 transfer complete interrupt */
  DMA13_DMA29_IRQn              = 13,               /**< DMA channel 13/29 transfer complete interrupt */
  DMA14_DMA30_IRQn              = 14,               /**< DMA channel 14/30 transfer complete interrupt */
  DMA15_DMA31_IRQn              = 15,               /**< DMA channel 15/31 transfer complete interrupt */
  DMA_Error_IRQn                = 16,               /**< DMA error interrupt */
  MCM_IRQn                      = 17,               /**< Normal interrupt */
  FTFE_IRQn                     = 18,               /**< FTFE interrupt */
  Read_Collision_IRQn           = 19,               /**< Read collision interrupt */
  LVD_LVW_IRQn                  = 20,               /**< Low Voltage Detect, Low Voltage Warning */
  LLW_IRQn                      = 21,               /**< Low Leakage Wakeup */
  Watchdog_IRQn                 = 22,               /**< WDOG interrupt */
  RNG_IRQn                      = 23,               /**< RNGA interrupt */
  I2C0_IRQn                     = 24,               /**< I2C0 interrupt */
  I2C1_IRQn                     = 25,               /**< I2C1 interrupt */
  SPI0_IRQn                     = 26,               /**< SPI0 interrupt */
  SPI1_IRQn                     = 27,               /**< SPI1 interrupt */
  SPI2_IRQn                     = 28,               /**< SPI2 interrupt */
  CAN0_ORed_Message_buffer_IRQn = 29,               /**< CAN0 OR'd message buffers interrupt */
  CAN0_Bus_Off_IRQn             = 30,               /**< CAN0 bus off interrupt */
  CAN0_Error_IRQn               = 31,               /**< CAN0 error interrupt */
  CAN0_Tx_Warning_IRQn          = 32,               /**< CAN0 Tx warning interrupt */
  CAN0_Rx_Warning_IRQn          = 33,               /**< CAN0 Rx warning interrupt */
  CAN0_Wake_Up_IRQn             = 34,               /**< CAN0 wake up interrupt */
  I2S0_Tx_IRQn                  = 35,               /**< I2S0 transmit interrupt */
  I2S0_Rx_IRQn                  = 36,               /**< I2S0 receive interrupt */
  CAN1_ORed_Message_buffer_IRQn = 37,               /**< CAN1 OR'd message buffers interrupt */
  CAN1_Bus_Off_IRQn             = 38,               /**< CAN1 bus off interrupt */
  CAN1_Error_IRQn               = 39,               /**< CAN1 error interrupt */
  CAN1_Tx_Warning_IRQn          = 40,               /**< CAN1 Tx warning interrupt */
  CAN1_Rx_Warning_IRQn          = 41,               /**< CAN1 Rx warning interrupt */
  CAN1_Wake_Up_IRQn             = 42,               /**< CAN1 wake up interrupt */
  Reserved59_IRQn               = 43,               /**< Reserved interrupt 59 */
  UART0_LON_IRQn                = 44,               /**< UART0 LON interrupt */
  UART0_RX_TX_IRQn              = 45,               /**< UART0 receive/transmit interrupt */
  UART0_ERR_IRQn                = 46,               /**< UART0 error interrupt */
  UART1_RX_TX_IRQn              = 47,               /**< UART1 receive/transmit interrupt */
  UART1_ERR_IRQn                = 48,               /**< UART1 error interrupt */
  UART2_RX_TX_IRQn              = 49,               /**< UART2 receive/transmit interrupt */
  UART2_ERR_IRQn                = 50,               /**< UART2 error interrupt */
  UART3_RX_TX_IRQn              = 51,               /**< UART3 receive/transmit interrupt */
  UART3_ERR_IRQn                = 52,               /**< UART3 error interrupt */
  UART4_RX_TX_IRQn              = 53,               /**< UART4 receive/transmit interrupt */
  UART4_ERR_IRQn                = 54,               /**< UART4 error interrupt */
  UART5_RX_TX_IRQn              = 55,               /**< UART5 receive/transmit interrupt */
  UART5_ERR_IRQn                = 56,               /**< UART5 error interrupt */
  ADC0_IRQn                     = 57,               /**< ADC0 interrupt */
  ADC1_IRQn                     = 58,               /**< ADC1 interrupt */
  CMP0_IRQn                     = 59,               /**< CMP0 interrupt */
  CMP1_IRQn                     = 60,               /**< CMP1 interrupt */
  CMP2_IRQn                     = 61,               /**< CMP2 interrupt */
  FTM0_IRQn                     = 62,               /**< FTM0 fault, overflow and channels interrupt */
  FTM1_IRQn                     = 63,               /**< FTM1 fault, overflow and channels interrupt */
  FTM2_IRQn                     = 64,               /**< FTM2 fault, overflow and channels interrupt */
  CMT_IRQn                      = 65,               /**< CMT interrupt */
  RTC_IRQn                      = 66,               /**< RTC interrupt */
  RTC_Seconds_IRQn              = 67,               /**< RTC seconds interrupt */
  PIT0_IRQn                     = 68,               /**< PIT timer channel 0 interrupt */
  PIT1_IRQn                     = 69,               /**< PIT timer channel 1 interrupt */
  PIT2_IRQn                     = 70,               /**< PIT timer channel 2 interrupt */
  PIT3_IRQn                     = 71,               /**< PIT timer channel 3 interrupt */
  PDB0_IRQn                     = 72,               /**< PDB0 interrupt */
  USB0_IRQn                     = 73,               /**< USB0 interrupt */
  USBDCD_IRQn                   = 74,               /**< USBDCD interrupt */
  ENET_1588_Timer_IRQn          = 75,               /**< Ethernet MAC IEEE 1588 timer interrupt */
  ENET_Transmit_IRQn            = 76,               /**< Ethernet MAC transmit interrupt */
  ENET_Receive_IRQn             = 77,               /**< Ethernet MAC receive interrupt */
  ENET_Error_IRQn               = 78,               /**< Ethernet MAC error and miscelaneous interrupt */
  Reserved95_IRQn               = 79,               /**< Reserved interrupt 95 */
  SDHC_IRQn                     = 80,               /**< SDHC interrupt */
  DAC0_IRQn                     = 81,               /**< DAC0 interrupt */
  DAC1_IRQn                     = 82,               /**< DAC1 interrupt */
  TSI0_IRQn                     = 83,               /**< TSI0 interrupt */
  MCG_IRQn                      = 84,              /**< MCG interrupt */
  LPTimer_IRQn                  = 85,              /**< LPTimer interrupt */
  Reserved102_IRQn              = 86,              /**< Reserved interrupt 102 */
  PORTA_IRQn                    = 87,              /**< Port A interrupt */
  PORTB_IRQn                    = 88,              /**< Port B interrupt */
  PORTC_IRQn                    = 89,              /**< Port C interrupt */
  PORTD_IRQn                    = 90,              /**< Port D interrupt */
  PORTE_IRQn                    = 91,              /**< Port E interrupt */
  PORTF_IRQn                    = 92,              /**< Port F interrupt */
  DDR_IRQn                      = 93,              /**< DDR interrupt */
  SWI_IRQn                      = 94,              /**< Software interrupt */
  NFC_IRQn                      = 95,              /**< NAND flash controller interrupt */
  USBHS_IRQn                    = 96,              /**< USB high speed OTG interrupt */
  LCD_IRQn                      = 97,              /**< Graphical LCD interrupt */
  CMP3_IRQn                     = 98,              /**< CMP3 interrupt */
  Tamper_IRQn                   = 99,              /**< Tamper detect interrupt */
  Reserved116_IRQn              = 100,              /**< Reserved interrupt 116 */
  FTM3_IRQn                     = 101,              /**< FTM3 fault, overflow and channels interrupt */
  ADC2_IRQn                     = 102,              /**< ADC2 interrupt */
  ADC3_IRQn                     = 103,              /**< ADC3 interrupt */
  I2S1_Tx_IRQn                  = 104,              /**< I2S1 transmit interrupt */
  I2S1_Rx_IRQn                  = 105               /**< I2S1 receive interrupt */
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
#define __FPU_PRESENT             1         /*!< FPU present or not                                */

/*@}*/ /* end of group MK70F_CMSIS */


#include <core_cm4.h>                       /* Cortex-M4 processor and core peripherals           */
#include "system_MK70F.h"                /* MK70F System  include file                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/
/** @addtogroup MK70F_Peripherals MK70F Peripherals
  MK70F Device Specific Peripheral registers structures
  @{
*/

#if 0

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/* ToDo: add here your device specific peripheral access structure typedefs
         following is an example for a timer                                  */

/*------------- 16-bit Timer/Event Counter (TMR) -----------------------------*/
/** @addtogroup MK70F_TMR MK70F 16-bit Timer/Event Counter (TMR)
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
/*@}*/ /* end of group MK70F_TMR */


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

/*@}*/ /* end of group MK70F_Peripherals */


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* ToDo: add here your device peripherals base addresses
         following is an example for timer                                    */
/** @addtogroup MK70F_MemoryMap MK70F Memory Mapping
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
/*@}*/ /* end of group MK70F_MemoryMap */


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
/* ToDo: add here your device peripherals pointer definitions
         following is an example for timer                                    */

/** @addtogroup MK70F_PeripheralDecl MK70F Peripheral Declaration
  @{
*/

#define <DeviceAbbreviation>_TIM0        ((<DeviceAbbreviation>_TMR_TypeDef *) <DeviceAbbreviation>TIM0_BASE)
#define <DeviceAbbreviation>_TIM1        ((<DeviceAbbreviation>_TMR_TypeDef *) <DeviceAbbreviation>TIM0_BASE)
#define <DeviceAbbreviation>_TIM2        ((<DeviceAbbreviation>_TMR_TypeDef *) <DeviceAbbreviation>TIM0_BASE)
/*@}*/ /* end of group MK70F_PeripheralDecl */

/*@}*/ /* end of group MK70F_Definitions */

#endif

#include <MK70F12.h>

#ifdef __cplusplus
}
#endif

#endif  /* MK70F_H */
