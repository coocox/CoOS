/**
 * @file low_init.c
 * @date 18. Aug 2013
 * @brief This file provides low level initialization for K60N512
 *
 */ 

#include <MK70F.h>

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
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTF_MASK);
    
    // ENET clock enable
    SIM_SCGC2 |= SIM_SCGC2_ENET1_MASK;
    
    SIM_SCGC1 = 0xFFFFFFFF;
    SIM_SCGC2 = 0xFFFFFFFF;
    SIM_SCGC3 = 0xFFFFFFFF;
    SIM_SCGC4 = 0xFFFFFFFF;
    SIM_SCGC5 = 0xFFFFFFFF;
    SIM_SCGC6 = 0xFFFFFFFF;
    SIM_SCGC7 = 0xFFFFFFFF;
}

void trace_clk_init(void) {
    // trace clock to the core clock frequency
    SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;

    // enable the TRACE_CLKOUT pin function on PTA6 (alt7 function)
    PORTA_PCR6 = PORT_PCR_MUX(0x7);
}

/** Low level init function.
 */
int __low_level_init(void) {
    wdt_disable();
    clock_enable();
    SystemInit();
//    trace_clk_init();

    return 1;
}
