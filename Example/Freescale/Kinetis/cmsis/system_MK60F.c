/**************************************************************************//**
 * @file     system_MK70F.c
 * @brief    CMSIS Cortex-M# Device Peripheral Access Layer Source File for
 *           Device MK70Fxx
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


#include <stdint.h>
#include "MK70F.h"


/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
/* ToDo: add here your necessary defines for device initialization     
         following is an example for different system frequencies             */
#define __HSI             ( 6000000UL)
#define __XTAL            (12000000UL)    /* Oscillator frequency             */
#define __SYS_OSC_CLK     (50000000UL)    /* Main oscillator frequency        */

#define __SYSTEM_CLOCK    (4*__XTAL)


#define RANGE0_VAL      1
#define FEXT            __SYS_OSC_CLK

#define PRDIV_VAL       5
#define VDIV_VAL        24


#if 1250000 >= __SYS_OSC_CLK
#define FRDIV_VAL       0
#elif 2500000 >= __SYS_OSC_CLK
#define FRDIV_VAL       1
#elif 5000000 >= __SYS_OSC_CLK
#define FRDIV_VAL       2
#elif 10000000 >= __SYS_OSC_CLK
#define FRDIV_VAL       3
#elif 20000000 >= __SYS_OSC_CLK
#define FRDIV_VAL       4
#else
#define FRDIV_VAL       5
#endif

#if (0 == RANGE0_VAL && (31250 > FEXT / (1 << FRDIV_VAL))) || (RANGE0_VAL && (31250 > FEXT / (1 << FRDIV_VAL + 5)))
#error
#elif (0 == RANGE0_VAL && (39062 < FEXT / (1 << FRDIV_VAL))) || (RANGE0_VAL && (39062 < FEXT / (1 << FRDIV_VAL + 5)))
//#error
#endif


#define REF_CLK         (__SYS_OSC_CLK / PRDIV_VAL)

#if 8000000 > REF_CLK
#error
#elif 16000000 < REF_CLK
#error
#endif

#if 1 > PRDIV_VAL || 8 < PRDIV_VAL
#error
#endif

#if 16 > VDIV_VAL || 47 < VDIV_VAL
#error
#endif

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
/* ToDo: initialize SystemCoreClock with the system core clock frequency value
         achieved after system intitialization.
         This means system core clock frequency after call to SystemInit()    */
uint32_t SystemCoreClock = __SYSTEM_CLOCK;  /*!< System Clock Frequency (Core Clock)*/


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
/* ToDo: add code to calculate the system frequency based upon the current
         register settings.
         This function can be used to retrieve the system core clock frequeny
         after user changed register sittings.                                */
  SystemCoreClock = __SYSTEM_CLOCK;
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void) {
    // system dividers
    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) | SIM_CLKDIV1_OUTDIV3(2) | SIM_CLKDIV1_OUTDIV4(5);
    
    
    // after reset, we are in FEI mode
    
    // enable external clock source - OSC0
#if __SYS_OSC_CLK <= 8000000
    MCG_C2 = MCG_C2_LOCRE0_MASK | MCG_C2_RANGE(RANGE0_VAL) | (/*hgo_val*/0 << MCG_C2_HGO_SHIFT) | (/*erefs_val*/0 << MCG_C2_EREFS_SHIFT);
#else
    // On rev. 1.0 of silicon there is an issue where the the input bufferd are enabled when JTAG is connected.
    // This has the affect of sometimes preventing the oscillator from running. To keep the oscillator amplitude
    // low, RANGE = 2 should not be used. This should be removed when fixed silicon is available.
    MCG_C2 = MCG_C2_LOCRE_MASK | MCG_C2_RANGE(2) | (/*hgo_val*/0 << MCG_C2_HGO_SHIFT) | (/*erefs_val*/0 << MCG_C2_EREFS_SHIFT);
//    MCG_C2 = MCG_C2_LOCRE_MASK | MCG_C2_RANGE(1) | (/*hgo_val*/0 << MCG_C2_HGO_SHIFT) | (/*erefs_val*/0 << MCG_C2_EREFS_SHIFT);
#endif
    
    // select clock mode, we want FBE mode
    // CLKS = 2, FRDIV = frdiv_val, IREFS = 0, IRCLKEN = 0, IREFSTEN = 0
    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(FRDIV_VAL);
    
    /* wait until the MCG has moved into the proper mode */
    // if the external oscillator is used need to wait for OSCINIT to set
//      for (i = 0 ; i < 10000 ; i++)
//      {
//        if (MCG_S & MCG_S_OSCINIT_MASK) break; // jump out early if OSCINIT sets before loop finishes
//      }
//      if (!(MCG_S & MCG_S_OSCINIT_MASK)) return 0x23; // check bit is really set and return with error if not set
    
    // wait for reference clock status bit is cleared and clock source is ext ref clk
    while ((MCG_S & MCG_S_IREFST_MASK) || MCG_S_CLKST(2) != (MCG_S & MCG_S_CLKST_MASK));

    // ... FBE mode
    
    // enable clock monitor for osc0
    MCG_C6 = MCG_C6_CME_MASK;

    // PLL0
    MCG_C5 = MCG_C5_PRDIV(PRDIV_VAL - 1);       // set PLL0 ref divider, osc0 is reference

    MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(VDIV_VAL - 16);     // set VDIV and enable PLL
        
    // wait to lock...
    while (!(MCG_S & MCG_S_PLLST_MASK));
    while (!(MCG_S & MCG_S_LOCK_MASK));
    
//    // Use actual PLL settings to calculate PLL frequency
//    prdiv = ((MCG_C5 & MCG_C5_PRDIV_MASK) + 1);
//    vdiv = ((MCG_C6 & MCG_C6_VDIV_MASK) + 16);

    // ... PBE mode
    MCG_C1 &= ~MCG_C1_CLKS_MASK;        // CLKS = 0, select PLL as MCG_OUT
    
    while (MCG_S_CLKST(3) != (MCG_S & MCG_S_CLKST_MASK));

    // ... PEE mode
    
/* ToDo: add code to initialize the system
         do not use global variables because this function is called before
         reaching pre-main. RW section maybe overwritten afterwards.          */
  SystemCoreClock = __SYSTEM_CLOCK;
}
