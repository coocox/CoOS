/**
 * @file enet.c
 * @date 1 Aug 2013
 * @brief Basic enet structures
 *
 * @author Jozef Maslik (maslo@binarylemon.com)
 *
 */

#ifndef _ENET_H_
#define _ENET_H_
     
//#define ENHANCED_BD
#define ENET_LITTLE_ENDIAN

/** tx buffer descriptor bits
 */
#define ENET_TX_BD_R            0x0080
#define ENET_TX_BD_TO1          0x0040
#define ENET_TX_BD_W            0x0020
#define ENET_TX_BD_TO2          0x0010
#define ENET_TX_BD_L            0x0008
#define ENET_TX_BD_TC           0x0004
#define ENET_TX_BD_ABC          0x0002

/** rx buffer descriptor bits
 */
#define ENET_RX_BD_E            0x0080
#define ENET_RX_BD_R01          0x0040
#define ENET_RX_BD_W            0x0020
#define ENET_RX_BD_R02          0x0010
#define ENET_RX_BD_L            0x0008
#define ENET_RX_BD_M            0x0001
#define ENET_RX_BD_BC           0x8000
#define ENET_RX_BD_MC           0x4000
#define ENET_RX_BD_LG           0x2000
#define ENET_RX_BD_NO           0x1000
#define ENET_RX_BD_CR           0x0400
#define ENET_RX_BD_OV           0x0200
#define ENET_RX_BD_TR           0x0100

/** Buffer descriptor
 */
typedef struct _enet_bd_t {
    uint16_t status;            ///< control and status
    uint16_t length;	        ///< transfer length
    uint8_t  *data;	        ///< buffer address
#ifdef ENHANCED_BD
    uint32_t ebd_status;
    uint8_t hdr_len;            ///< header length
    uint8_t prot;               ///< protocol type
    uint16_t checksum;          ///< payload checksum
    uint16_t bdu;
    uint16_t resvd0;
    uint32_t timestamp;
    uint16_t resvd1;
    uint16_t resvd2;
    uint16_t resvd3;
    uint16_t resvd4;
#endif /* ENHANCED_BD */
} enet_bd_t;

#endif /* _ENET_H_ */
