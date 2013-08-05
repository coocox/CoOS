/**
 * @file low_init.h
 * @date 25 Jul 2013
 * @brief This file provides low level initialization parameters for K60N512
 *
 * @author Jozef Maslik (maslo@binarylemon.com)
 *
 */

#ifndef _LOW_INIT_H_
#define _LOW_INIT_H_

/* clock configuration
 */
#define CLOCK_SRC               (50000000ul)    // oscillator freq
#define REF_CLOCK_SRC           (2000000ul)     // must be 2-4MHz

#define CORE_DIV                1
#define BUS_DIV                 2
#define FLEXBUS_DIV             1
#define FLASH_DIV               4

// PLL_CLOCK_MUL from interval 24 - 55
#define PLL_CLOCK_MUL               50      // CORE_CLOCK = 100MHz

#define REF_CLOCK_DIV           (CLOCK_SRC / REF_CLOCK_SRC)

#define PLL_CLOCK               (REF_CLOCK_SRC * CLOCK_MUL)
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

#endif // _LOW_INIT_H_