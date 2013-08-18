/**
 * @file mii.c
 * @date 1 Aug 2013
 * @brief MII interface for PHY
 *
 * @author Jozef Maslik (maslo@binarylemon.com)
 *
 */

#include "mii.h"

#include <AppConfig.h>

#include <stddef.h>


/** Check if phy communication interface is enabled.
 */
static int mii_enabled(void) {  
   return 0 != (ENET_MSCR & ENET_MSCR_MII_SPEED_MASK);
} 

/** Read, write function to phy.
 */
static int mii_rw(uint8_t phy, uint8_t reg, int op, uint16_t wdata, uint16_t *rdata) {
    int tm;
    
    if (mii_enabled()) {
        /* clear the MII interrupt bit */
        ENET_EIR = ENET_EIR_MII_MASK;
        
        // start mii operation 
        ENET_MMFR = ENET_MMFR_ST(1)
            | ENET_MMFR_OP(op)
            | ENET_MMFR_PA(phy)
            | ENET_MMFR_RA(reg)
            | ENET_MMFR_TA(2)
            | ENET_MMFR_DATA(wdata);
        
        // wait until operation complete
        for (tm = MII_TIMEOUT; 0 < tm && 0 == (ENET_EIR & ENET_EIR_MII_MASK); tm--) {
             // delay
        }
        
        if (0 < tm && rdata) {
            *rdata = (ENET_MMFR & ENET_MMFR_DATA_MASK);
        }
        
        /* clear the MII interrupt bit */
        ENET_EIR = ENET_EIR_MII_MASK;
    }
    return (0 < tm);
}

int mii_read(uint8_t phy, uint8_t reg, uint16_t *data) {
    return mii_rw(phy, reg, 2, 0, data);
}

int mii_write(uint8_t phy, uint8_t reg, uint16_t data) {
    return mii_rw(phy, reg, 1, data, NULL);
}

/** Initialize MII interface.
 */

/********************************************************************/
/*
 * \brief   Initialize the MII interface controller
 * \param   System Clock Frequency (in MHz)
 * \warning The system clock in this case is the clock that drives
 *          the FEC logic.  This may be different from the speed at which 
 *          the CPU is operating.
 * 
 * Initialize the MII clock (EMDC) frequency. The desired MII clock is 2.5MHz:
 *
 * MII Speed Setting = System_Clock / (2.5MHz * 2)
 * (plus 1 to round up)
 */
void mii_init(uint32_t clk) {
    ENET_MSCR = ENET_MSCR_MII_SPEED((2 * clk / 5) + 1);
}