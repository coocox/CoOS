/**
 * @file mii.h
 * @date  1 Aug 2013
 * @brief MII interface for PHY
 *
 * @author Jozef Maslik (maslo@binarylemon.com)
 *
 */ 

#ifndef _MII_H_
#define _MII_H_
     
#include <stdint.h>

#define MII_TIMEOUT     0xffff

/** standard MII registers
 */
#define MII_BMCR                0x00    // basic mode configuration
#define MII_BMSR                0x01    // basic mode status
#define MII_PHYID1              0x02    // PHY ID 1
#define MII_PHYID2              0x03    // PHY ID 2
#define MII_ADVERTISE           0x04    // autonegotation advertisement
#define MII_LPA                 0x05    // link partner ability
#define MII_EXPANSION           0x06    // autonegotation expansion

/** basic mode configuration register
 */
#define MII_BMCR_CTST           0x0080  // collision test
#define MII_BMCR_FULLDPLX       0x0100  // full duplex
#define MII_BMCR_ANRESTART      0x0200  // autonegotiation restart
#define MII_BMCR_ISOLATE        0x0400  // isolate data paths from MII
#define MII_BMCR_PDOWN          0x0800  // enable low power state
#define MII_BMCR_ANENABLE       0x1000  // enable auto negotiation
#define MII_BMCR_SPEED100       0x2000  // select 100Mbps
#define MII_BMCR_LOOPBACK       0x4000  // TXD loopback bits
#define MII_BMCR_RESET          0x8000  // reset to default state

/** basic mode status register
 */
#define MII_BMSR_ERCAP          0x0001  // ext-reg capability
#define MII_BMSR_JCD            0x0002  // jabber detected
#define MII_BMSR_LSTATUS        0x0004  // link status
#define MII_BMSR_ANEGCAPABLE    0x0008  // able to do autonegotiation
#define MII_BMSR_RFAULT         0x0010  // remote fault detected
#define MII_BMSR_ANEGCOMPLETE   0x0020  // autonegotiation complete
#define MII_BMSR_ESTATEN        0x0100  // extended status in R15
#define MII_BMSR_100HALF2       0x0200  // can do 100BASE-T2 HDX
#define MII_BMSR_100FULL2       0x0400  // can do 100BASE-T2 FDX
#define MII_BMSR_10HALF         0x0800  // can do 10Mps, half-duplex
#define MII_BMSR_10FULL         0x1000  // can do 10Mbps, full-duplex
#define MII_BMSR_100HALF        0x2000  // can do 100Mbps, half-duplex
#define MII_BMSR_100FULL        0x4000  // can do 100Mbps, full-duplex
#define MII_BMSR_100BASE4       0x8000  // can do 100Mbps, 4k packets


/** Read data from PHY.
 */
int mii_read(uint8_t phy, uint8_t reg, uint16_t *data);

/** Write data to PHY.
 */
int mii_write(uint8_t phy, uint8_t reg, uint16_t data);

/** Initialize MII interface.
 */
void mii_init(uint32_t clk);

#endif /* _MII_H_ */