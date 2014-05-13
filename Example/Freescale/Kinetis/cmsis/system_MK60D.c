/**************************************************************************//**
 * @file     system_MK60D.c
 * @brief    CMSIS Cortex-M4 Device Peripheral Access Layer Source File for
 *           Device MK60Dxxx
 * @version  V3.01
 * @date     06. March 2012
 *
 * @note
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
#include "MK60D.h"


/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
//#define __HSI             ( 6000000UL)
#define __XTAL            (12000000UL)    /* Oscillator frequency             */
#define __SYS_OSC_CLK     (50000000UL)    /* Main oscillator frequency        */

#define __SYSTEM_CLOCK    (CORE_CLOCK)

/* clock configuration
 */
#define CLOCK_SRC               (50000000ul)    // oscillator freq
#define REF_CLOCK_SRC           (2000000ul)     // must be 2-4MHz

#define CORE_DIV                1
#define BUS_DIV                 2
#define FLEXBUS_DIV             1
#define FLASH_DIV               4

// PLL_CLOCK_MUL from interval 24 - 55
#define PLL_CLOCK_MUL           50      // CORE_CLOCK = 100MHz

#define REF_CLOCK_DIV           (CLOCK_SRC / REF_CLOCK_SRC)

#define PLL_CLOCK               (REF_CLOCK_SRC * PLL_CLOCK_MUL)
#define CORE_CLOCK              (PLL_CLOCK / CORE_DIV)          // CORE CLK, max 100MHz
#define SYSTEM_CLOCK            (CORE_CLOCK)                    // SYSTEM CLK, max 100MHz
#define BUS_CLOCK               (PLL_CLOCK / BUS_DIV)           // max 50MHz
#define FLEXBUS_CLOCK           (PLL_CLOCK / FLEXBUS_DIV)
#define FLASH_CLOCK             (PLL_CLOCK / FLASH_DIV)         // max 25MHz


// clock range check
#if 100000000 < CORE_CLOCK
#error Core clock is too high, must be max 100MHz
#endif

#if 100000000 < SYSTEM_CLOCK
#error Sys clock is too high, must be max 100MHz
#endif

#if 50000000 < BUS_CLOCK
#error Bus clock is too high, must be max 50MHz
#endif

#if 25000000 < FLASH_CLOCK
#error Flash clock is too high, must be max 25MHz
#endif


/** Disable watchdog.
 */
static void wdt_disable(void) {
    // unlock watchdog
    WDOG_UNLOCK = 0xc520;
    WDOG_UNLOCK = 0xd928;

    // disable watchdog
    WDOG_STCTRLH &= ~(WDOG_STCTRLH_WDOGEN_MASK);
}

/** Clock to peripherals enable.
 */
static void clock_enable(void) {
    // GPIO clock enable
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK);

    // ENET clock enable
    SIM_SCGC2 |= SIM_SCGC2_ENET_MASK;

    SIM_SCGC1 = 0xFFFFFFFF;
    SIM_SCGC2 = 0xFFFFFFFF;
    SIM_SCGC3 = 0xFFFFFFFF;
    SIM_SCGC4 = 0xFFFFFFFF;
    SIM_SCGC5 = 0xFFFFFFFF;
    SIM_SCGC6 = 0xFFFFFFFF;
    SIM_SCGC7 = 0xFFFFFFFF;
}

static void trace_clk_init(void) {
    // trace clock to the core clock frequency
    SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;

    // enable the TRACE_CLKOUT pin function on PTA6 (alt7 function)
    PORTA_PCR6 = PORT_PCR_MUX(0x7);
}


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __SYSTEM_CLOCK;  /*!< System Clock Frequency (Core Clock)*/


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
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
	wdt_disable();
	clock_enable();
	trace_clk_init();
    // First move to FBE mode
    // Enable external oscillator, RANGE=0, HGO=, EREFS=, LP=, IRCS=
    MCG_C2 = 0;
    
    // Select external oscilator and Reference Divider and clear IREFS to start ext osc
    // CLKS=2, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);
    
    while (MCG_S & MCG_S_IREFST_MASK); // wait for Reference clock Status bit to clear
    
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2); // Wait for clock status bits to show clock source is ext ref clk
    
    // ... FBE mode
    // Configure PLL Ref Divider, PLLCLKEN=0, PLLSTEN=0, PRDIV=0x18
    // The crystal frequency is used to select the PRDIV value. Only even frequency crystals are supported
    // that will produce a 2MHz reference clock to the PLL.
    MCG_C5 = MCG_C5_PRDIV(REF_CLOCK_DIV - 1);

    // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear
    MCG_C6 = 0;

    // Set system options dividers
    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(CORE_DIV - 1) |           // core/system clock
                  SIM_CLKDIV1_OUTDIV2(BUS_DIV - 1) |            // peripheral clock
                  SIM_CLKDIV1_OUTDIV3(FLEXBUS_DIV - 1) |        // FlexBus clock driven to the external pin (FB_CLK).
                  SIM_CLKDIV1_OUTDIV4(FLASH_DIV - 1);           // flash clock
    
    // Set the VCO divider and enable the PLL, LOLIE = 0, PLLS = 1, CME = 0, VDIV = PLL_CLOCK_MUL
    MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(PLL_CLOCK_MUL - 24);    // 2MHz * PLL_CLOCK_MUL

    while (!(MCG_S & MCG_S_PLLST_MASK));        // wait for PLL status bit to set
    while (!(MCG_S & MCG_S_LOCK_MASK));         // Wait for LOCK bit to set

    // ...running PBE mode

    // Transition into PEE by setting CLKS to 0
    // CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
    MCG_C1 &= ~MCG_C1_CLKS_MASK;

    // Wait for clock status bits to update
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3);

    // ...running PEE mode  
    
    SystemCoreClock = __SYSTEM_CLOCK;
}
