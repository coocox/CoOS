/**
 * @file ethernetif.c
 * @brief Ethernet interface
 *
 */

#include "ethernetif.h"

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "lwip/sys.h"
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

#include <AppConfig.h>
#include <coos.h>


/* Standard library includes. */
#include <string.h>

/* MAC address */
#define MAC_ADDR0               0x00
#define MAC_ADDR1               0xcf
#define MAC_ADDR2               0x52
#define MAC_ADDR3               0x35
#define MAC_ADDR4               0x00
#define MAC_ADDR5               0x01

/* ethernet rx buffers setings */
#define ENET_RX_SINGLE_BUFFER   0     /* multiple buffers per frame - much more effective for buffer usage*/

#if ENET_RX_SINGLE_BUFFER
    #define ENET_RX_BUF_SIZE        1520
    #if (LWIP_PERF_SETTINGS == 1)
        #define NUM_ENET_RX_BUFS        12
    #else
        #define NUM_ENET_RX_BUFS        8
    #endif
#else
    #define ENET_RX_BUF_SIZE        256     /* must be PBUF_POOL_BUFSIZE multiple */
    #if (LWIP_PERF_SETTINGS == 1)
        #define NUM_ENET_RX_BUFS        72
    #else
        #define NUM_ENET_RX_BUFS        48
    #endif

/* PBUF_POOL_BUFSIZE macro must be greater than ENET min pkt */
#endif  /* ENET_RX_SINGLE_BUFFER */

/* ethernet tx buffers setings */
#define NUM_ENET_TX_BUFS        3       /* more is useless for this specific driver: might try with lower */
#define ENET_TX_BUF_SIZE        1520    /* no need to reduce this number */

#define ENET_MAX_PKT_SIZE       1520

#define USE_PROMISCUOUS_MODE    0
#define USE_MII_MODE            0       /* RMII mode */

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

/* enet */
#include <enet/mii.h>
#include <enet/enet.h>

/* PHY hardware specifics. */
#define PHY_ADDR                0

#define PHY_STATUS              0x1f
#define PHY_DUPLEX_STATUS       (4 << 2)
#define PHY_SPEED_STATUS        (1 << 2)

/* Delay to wait for a DMA buffer to become available if one is not already available. */
#define BUF_WAIT_ATTEMPTS       10
#define BUF_WAIT_DELAY          10

/* Delay between polling the PHY to see if a link has been established. */
#define PHY_LINK_DELAY          500

/* Delay between looking for incoming packets.  In ideal world this would be
infinite. */
#define BLOCK_TIME_WAITING_FOR_INPUT    PHY_LINK_DELAY

static unsigned char tx_bd_unaligned[(NUM_ENET_TX_BUFS * sizeof(enet_bd_t)) + 16];
static unsigned char rx_bd_unaligned[(NUM_ENET_RX_BUFS * sizeof(enet_bd_t)) + 16];
static unsigned char tx_bufs[(NUM_ENET_TX_BUFS * ENET_TX_BUF_SIZE) + 16];
static unsigned char rx_bufs[(NUM_ENET_RX_BUFS * ENET_RX_BUF_SIZE) + 16];

static enet_bd_t *tx_bd;
static enet_bd_t *rx_bd;

static uint32_t rx_next_buf = 0, tx_next_buf = 0;


#define TASK_ETHIF_STACK_SIZE   400
#define TASK_ETHIF_PRIO         (TCPIP_THREAD_PRIO + 1)
static OS_STK stack[TASK_ETHIF_STACK_SIZE];


/* Semaphores used by the ENET interrupt handler to wake the handler task. */
static OS_EventID sem_rx;
static OS_EventID sem_tx;


/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
};

/* Standard lwIP netif handlers. */
static void low_level_init(struct netif *netif);
static err_t low_level_output(struct netif *netif, struct pbuf *p);
static struct pbuf *low_level_input(struct netif *netif);

/* Forward declarations. */
static void  ethernetif_input(void *pParams);

/** Internal helper function. Initialize buffers and buffers descriptors for ENET module.
 */
static void init_enet_bufs(void) {
    int i;
    uint8_t *buf;

    tx_bd = (enet_bd_t*)(((uint32_t)tx_bd_unaligned + 0x0f) & 0xfffffff0);
    rx_bd = (enet_bd_t*)(((uint32_t)rx_bd_unaligned + 0x0f) & 0xfffffff0);
        
    /* setup the buffers and initialize buffers descriptors */
    buf = (uint8_t*)(((uint32_t)tx_bufs + 0x0f) & 0xfffffff0);

    for (i = 0; i < NUM_ENET_TX_BUFS; i++) {
        tx_bd[i].status = ENET_TX_BD_TC;
#ifdef ENET_LITTLE_ENDIAN
        tx_bd[i].data = (uint8_t*)__REV((uint32_t)buf);
#else
        tx_bd[i].data = buf;
#endif /* ENET_LITTLE_ENDIAN */
        tx_bd[i].length = 0;
#ifdef ENHANCED_BD
        tx_bd[i].ebd_status = TX_BD_INT
#if ENET_HARDWARE_CHECKSUM
            | TX_BD_IINS
            | TX_BD_PINS
#endif /* ENET_HARDWARE_CHECKSUM */
                    ;
#endif /* ENHANCED_BD */
        buf += ENET_TX_BUF_SIZE;
    }
    
    buf = (uint8_t*)(((uint32_t)rx_bufs + 0x0f) & 0xfffffff0);

    for (i = 0; i < NUM_ENET_RX_BUFS; i++) {
        rx_bd[i].status = ENET_RX_BD_E;
        rx_bd[i].length = 0;
#ifdef ENET_LITTLE_ENDIAN
        rx_bd[i].data = (uint8_t*)__REV((uint32_t)buf);
#else
        rx_bd[i].data = buf;
#endif /* ENET_LITTLE_ENDIAN */

#ifdef ENHANCED_BD
        rx_bd[i].bdu = 0x00000000;
        rx_bd[i].ebd_status = RX_BD_INT;
#endif /* ENHANCED_BD */
        buf += ENET_RX_BUF_SIZE;
    }
    
    /* set the wrap bit in the last descriptors to form a ring */
    tx_bd[NUM_ENET_TX_BUFS - 1].status |= ENET_TX_BD_W;
    rx_bd[NUM_ENET_RX_BUFS - 1].status |= ENET_RX_BD_W;
    
    rx_next_buf = 0;
    tx_next_buf = 0;
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(struct netif *netif) {
    StatusType result;
    int data;

    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;
    
    /* set MAC hardware address */
    netif->hwaddr[0] = MAC_ADDR0;
    netif->hwaddr[1] = MAC_ADDR1;
    netif->hwaddr[2] = MAC_ADDR2;
    netif->hwaddr[3] = MAC_ADDR3;
    netif->hwaddr[4] = MAC_ADDR4;
    netif->hwaddr[5] = MAC_ADDR5;
    
    /* maximum transfer unit */
    netif->mtu = ENET_MAX_PKT_SIZE - 20;
    
    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
    
    /* enable the ENET clock */
//    SIM_SCGC2 |= SIM_SCGC2_ENET_MASK;
    
    /* allow concurrent access to MPU controller. Example: ENET uDMA to SRAM, otherwise bus error */
    MPU_CESR = 0;         
    
    init_enet_bufs();
    
    /* create semaphores */
    sem_rx = CoCreateSem(NUM_ENET_TX_BUFS, NUM_ENET_TX_BUFS, 1);
    sem_tx = CoCreateSem(0, NUM_ENET_RX_BUFS, 1);
    
    /* Set the Reset bit and clear the Enable bit */
    ENET_ECR = ENET_ECR_RESET_MASK;
    
    /* Wait at least 8 clock cycles */
    for (data = 0; data < 10; data++) {
        asm("NOP");
    }
    
    /* start MII interface */
    mii_init(50/*BUS_CLOCK*/);       
    
    // enet tx interrupt
    NVIC_SetPriority(ENET_Transmit_IRQn, 6);
    NVIC_EnableIRQ(ENET_Transmit_IRQn);
    
    // enet rx interrupt
    NVIC_SetPriority(ENET_Receive_IRQn, 6);
    NVIC_EnableIRQ(ENET_Receive_IRQn);
    
    //enet error interrupt
    NVIC_SetPriority(ENET_Error_IRQn, 6);
    NVIC_EnableIRQ(ENET_Error_IRQn);
    
    /* Make sure the external interface signals are enabled */
    PORTB_PCR0  = PORT_PCR_MUX(4);      //RMII0_MDIO/MII0_MDIO
    PORTB_PCR1  = PORT_PCR_MUX(4);      //RMII0_MDC/MII0_MDC    
    
#if USE_MII_MODE
    PORTA_PCR14 = PORT_PCR_MUX(4);      //RMII0_CRS_DV/MII0_RXDV
    //PORTA_PCR5  = PORT_PCR_MUX(4);      //RMII0_RXER/MII0_RXER
    PORTA_PCR12 = PORT_PCR_MUX(4);      //RMII0_RXD1/MII0_RXD1
    PORTA_PCR13 = PORT_PCR_MUX(4);      //RMII0_RXD0/MII0_RXD0
    PORTA_PCR15 = PORT_PCR_MUX(4);      //RMII0_TXEN/MII0_TXEN
    PORTA_PCR16 = PORT_PCR_MUX(4);      //RMII0_TXD0/MII0_TXD0
    PORTA_PCR17 = PORT_PCR_MUX(4);      //RMII0_TXD1/MII0_TXD1
    PORTA_PCR11 = PORT_PCR_MUX(4);      //MII0_RXCLK
    PORTA_PCR25 = PORT_PCR_MUX(4);      //MII0_TXCLK
    PORTA_PCR9  = PORT_PCR_MUX(4);      //MII0_RXD3
    PORTA_PCR10 = PORT_PCR_MUX(4);      //MII0_RXD2  
    PORTA_PCR28 = PORT_PCR_MUX(4);      //MII0_TXER
    PORTA_PCR24 = PORT_PCR_MUX(4);      //MII0_TXD2
    PORTA_PCR26 = PORT_PCR_MUX(4);      //MII0_TXD3
    PORTA_PCR27 = PORT_PCR_MUX(4);      //MII0_CRS
    PORTA_PCR29 = PORT_PCR_MUX(4);      //MII0_COL
#else
    PORTA_PCR14 = PORT_PCR_MUX(4);      //RMII0_CRS_DV/MII0_RXDV
//    PORTA_PCR5  = PORT_PCR_MUX(4);      //RMII0_RXER/MII0_RXER
    PORTA_PCR12 = PORT_PCR_MUX(4);      //RMII0_RXD1/MII0_RXD1
    PORTA_PCR13 = PORT_PCR_MUX(4);      //RMII0_RXD0/MII0_RXD0
    PORTA_PCR15 = PORT_PCR_MUX(4);      //RMII0_TXEN/MII0_TXEN
    PORTA_PCR16 = PORT_PCR_MUX(4);      //RMII0_TXD0/MII0_TXD0
    PORTA_PCR17 = PORT_PCR_MUX(4);      //RMII0_TXD1/MII0_TXD1
#endif /* USE_MII_MODE */
    
    /* Can we talk to the PHY? */
    do {
        CoTickDelay(PHY_LINK_DELAY);
        data = 0xffff;
        mii_read(PHY_ADDR, MII_PHYID1, (uint16_t*)&data); 
    } while (data == 0xffff);
    
    /* Start auto negotiate. */
    mii_write(PHY_ADDR, MII_BMCR, (MII_BMCR_ANRESTART | MII_BMCR_ANENABLE));
    
    /* Wait for auto negotiate to complete. */
    do {
        CoTickDelay(PHY_LINK_DELAY);
        mii_read(PHY_ADDR, MII_BMSR, (uint16_t*)&data);
    } while (!(data & MII_BMSR_ANEGCOMPLETE));
    
    /* When we get here we have a link - find out what has been negotiated. */
    data = 0;
    mii_read(PHY_ADDR, PHY_STATUS, (uint16_t*)&data);  

    /* Set the Physical Address for the selected ENET */
    ENET_PALR = (uint32_t)((netif->hwaddr[0] << 24) | (netif->hwaddr[1] << 16) | (netif->hwaddr[2] << 8) | netif->hwaddr[3]);
    ENET_PAUR = (uint32_t)((netif->hwaddr[4] << 24) | (netif->hwaddr[5] << 16));
    
    /* Clear the Individual and Group Address Hash registers */
    ENET_IALR = 0;
    ENET_IAUR = 0;
    ENET_GALR = 0;
    ENET_GAUR = 0;
    
#if USE_MII_MODE        
    /* various mode/status setup */
    ENET_RCR = ENET_RCR_MAX_FL(ENET_MAX_PKT_SIZE) | ENET_RCR_MII_MODE_MASK | ENET_RCR_CRCFWD_MASK;
#else
    ENET_RCR = ENET_RCR_MAX_FL(ENET_MAX_PKT_SIZE) | ENET_RCR_MII_MODE_MASK | ENET_RCR_CRCFWD_MASK | ENET_RCR_RMII_MODE_MASK;
#endif
    
    /* clear rx/tx control registers */
    ENET_TCR = 0;
    
    /* setup half or full duplex */
    if (data & PHY_DUPLEX_STATUS) {
        /* full duplex */
        ENET_RCR &= ~ENET_RCR_DRT_MASK;
        ENET_TCR |= ENET_TCR_FDEN_MASK;
    }
    else {
        /* half duplex */
        ENET_RCR |= ENET_RCR_DRT_MASK;
        ENET_TCR &= ~ENET_TCR_FDEN_MASK;
    }
    
    /* Setup speed */
    if (data & PHY_SPEED_STATUS) {
        /* 10Mbps */
        ENET_RCR |= ENET_RCR_RMII_10T_MASK;
    }
    
#if USE_PROMISCUOUS_MODE
    ENET_RCR |= ENET_RCR_PROM_MASK;
#endif
    
#ifdef ENHANCED_BD
    ENET_ECR = ENET_ECR_EN1588_MASK;
#else
    ENET_ECR = 0;
#endif
    
#if ENET_HARDWARE_CHECKSUM
    /* enable discard on wrong protocol checksum and other nice features */
    ENET_RACC = ENET_RACC_IPDIS_MASK | ENET_RACC_PRODIS_MASK
        | ENET_RACC_LINEDIS_MASK | ENET_RACC_IPDIS_MASK | ENET_RACC_PADREM_MASK;
    
    /* enable protocol checksum insertion */
    ENET_TACC = ENET_TACC_IPCHK_MASK | ENET_TACC_PROCHK_MASK;
#endif
    
    ENET_TFWR = ENET_TFWR_STRFWD_MASK;
    
#if ENET_HARDWARE_SHIFT
    /* enable ethernet header alignment for rx */
    ENET_RACC |= ENET_RACC_SHIFT16_MASK;
    
    /* enable ethernet header alignment for tx */
    ENET_TACC |= ENET_TACC_SHIFT16_MASK;
#endif
    
    /* set rx buffer size */
    ENET_MRBR = ENET_RX_BUF_SIZE;
    
    /* point to the start of the circular rx buffer descriptor queue */
    ENET_RDSR = (uint32_t)rx_bd;
    
    /* point to the start of the circular tx buffer descriptor queue */
    ENET_TDSR = (uint32_t)tx_bd;
    
    /* clear all ENET interrupt events */
    ENET_EIR = -1u;
    
    /* enable interrupts */
    ENET_EIMR = 0
        /* rx irqs */
        | ENET_EIMR_RXF_MASK    /* only for complete frame, not partial buffer descriptor */
        /* tx irqs */
        | ENET_EIMR_TXF_MASK    /* only for complete frame, not partial buffer descriptor */
        /* others enet irqs */
        | ENET_EIMR_UN_MASK | ENET_EIMR_RL_MASK | ENET_EIMR_LC_MASK | ENET_EIMR_BABT_MASK | ENET_EIMR_BABR_MASK | ENET_EIMR_EBERR_MASK;
    
    /* create the task that handles the MAC ENET */
    result = CoCreateTask((FUNCPtr)ethernetif_input, (void *)netif, TASK_ETHIF_PRIO, &stack[TASK_ETHIF_STACK_SIZE - 1], TASK_ETHIF_STACK_SIZE);
    
    if (result == E_CREATE_FAIL) {
        LWIP_DEBUGF(NETIF_DEBUG,("Task for ETH recive created failed !\n\r "));
    };
    
    /* Enable the MAC itself */
    ENET_ECR |= ENET_ECR_ETHEREN_MASK;
    
    /* Indicate that there have been empty receive buffers produced */
    ENET_RDAR = ENET_RDAR_RDAR_MASK;
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p) {
    struct pbuf *q;
    u16_t l = 0;
    uint8_t *buf = NULL;
    int i;

    //initiate transfer();
#if 0 == ENET_HARDWARE_SHIFT
#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
#endif
    
    /* Get a DMA buffer into which we can write the data to send. */
    for (i = 0; i < BUF_WAIT_ATTEMPTS; i++) {
        if (tx_bd[tx_next_buf].status & ENET_TX_BD_R) {
            /* wait for the buffer to become available */
            CoTickDelay(BUF_WAIT_DELAY);
        }
        else {
#ifdef ENET_LITTLE_ENDIAN
            buf = (uint8_t*)__REV((uint32_t)tx_bd[tx_next_buf].data);
#else
            buf = tx_bd[tx_next_buf].data;
#endif
            break;
        }
    }
    
    if (NULL == buf) {
        return ERR_BUF;
    }
    else { 
        for (q = p; q != NULL; q = q->next) {
            /* Send the data from the pbuf to the interface, one pbuf at a
            time. The size of the data in each pbuf is kept in the ->len
            variable. */
            memcpy(&buf[l], (u8_t*)q->payload, q->len);
            l += q->len;
        }    
    }
    
    //signal that packet should be sent();
    
    /* Setup the buffer descriptor for transmission */
#ifdef ENET_LITTLE_ENDIAN
    tx_bd[tx_next_buf].length = __REVSH(l);//nbuf->length + ETH_HDR_LEN;
#else
    tx_bd[tx_next_buf].length = l;//nbuf->length + ETH_HDR_LEN;
#endif
    tx_bd[tx_next_buf].status |= (ENET_TX_BD_R | ENET_TX_BD_L);
    
    tx_next_buf++;
    if (tx_next_buf >= NUM_ENET_TX_BUFS) {
        tx_next_buf = 0;
    }
    
#if 0 == ENET_HARDWARE_SHIFT
#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE);       /* reclaim the padding word */
#endif
#endif
    
    LINK_STATS_INC(link.xmit);
    
    /* only one task can be here. wait until pkt is sent, then go ahead */
    /* semaphore released inside isr */
    /* start expiring semaphore: no more than 3 ticks */
    /* no blocking code */
    CoPendSem(sem_tx, 3); 
    
    /* Request xmit process to MAC-NET */
    ENET_TDAR = ENET_TDAR_TDAR_MASK;
    
    return ERR_OK;
}

#if ENET_RX_SINGLE_BUFFER
/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf* low_level_input(struct netif *netif) {
    u32_t l = 0;
    struct pbuf *p = NULL, *q = NULL;
    u16_t len;
#ifdef ENET_LITTLE_ENDIAN
    u8_t *data_temp;
#endif
    
    (void)netif;
  
    /* Obtain the size of the packet and put it into the "len" variable */
#ifdef ENET_LITTLE_ENDIAN
    len = __REVSH(rx_bd[rx_next_buf].length);
#else
    len = rx_bd[rx_next_buf].length;
#endif
    
    if ((len != 0) && ((rx_bd[rx_next_buf].status & RX_BD_E) == 0)) {  
#if (ENET_HARDWARE_SHIFT==0)
#if ETH_PAD_SIZE
        len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif
#endif
        
        /* We allocate a pbuf chain of pbufs from the pool. */
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
        
        if (p != NULL) {
#if (ENET_HARDWARE_SHIFT==0)
#if ETH_PAD_SIZE
            pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif
#endif
            
            /* We iterate over the pbuf chain until we have read the entire
            * packet into the pbuf. */
            for (q = p; q != NULL; q = q->next) {
                /* Read enough bytes to fill this pbuf in the chain. The
                * available data in the pbuf is given by the q->len
                * variable.
                * This does not necessarily have to be a memcpy, you can also preallocate
                * pbufs for a DMA-enabled MAC and after receiving truncate it to the
                * actually received size. In this case, ensure the tot_len member of the
                * pbuf is the sum of the chained pbuf len members.
                */
#ifdef ENET_LITTLE_ENDIAN
                data_temp = (u8_t*)__REV((u32_t)rx_bd[rx_next_buf].data);
                memcpy((u8_t*)q->payload, &(data_temp[l]), q->len);
#else
                memcpy((u8_t*)q->payload, &(rx_bd[rx_next_buf].data[l]), q->len);
#endif
                l = l + q->len;
            }
            
#if (ENET_HARDWARE_SHIFT==0)
#if ETH_PAD_SIZE
            pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
#endif
            LINK_STATS_INC(link.recv);        
        }
        else {
            /* drop packet() */
            LINK_STATS_INC(link.memerr);
            LINK_STATS_INC(link.drop);     
        }
        
        /* acknowledge that packet has been read() */
        /* free the descriptor */
        rx_bd[rx_next_buf].status |= RX_BD_E;
        ENET_RDAR = ENET_RDAR_RDAR_MASK;
        
        rx_next_buf++;
        if (rx_next_buf >= NUM_ENET_RX_BUFS) {
            rx_next_buf = 0;
        }
    } 
    
    return p;  
}
#else
static struct pbuf* low_level_input(struct netif *netif) {
    u16_t l, temp_l;
    struct pbuf *first_pbuf, *next_pbuf, *q;
    u16_t len;
#ifdef ENET_LITTLE_ENDIAN
    u8_t *data_temp;
#endif
    u8_t more_pkts = 1, processing_error = 0;
    
    (void)netif;
    
    /* initial pkt handling */
    if (!(rx_bd[rx_next_buf].status & ENET_RX_BD_E)) {       /* if pkt is filled */
        if (rx_bd[rx_next_buf].status & ENET_RX_BD_L) {
            more_pkts = 0;
            if (rx_bd[rx_next_buf].status & (ENET_RX_BD_LG | ENET_RX_BD_NO | ENET_RX_BD_CR | ENET_RX_BD_OV)) {
                /* bad packet */
                LINK_STATS_INC(link.memerr);
                LINK_STATS_INC(link.drop);
                goto EXIT_RX_PKT;
            }
            else {
#ifdef ENET_LITTLE_ENDIAN
                len = __REVSH(rx_bd[rx_next_buf].length);
#else
                len = rx_bd[rx_next_buf].length;
#endif 
                LINK_STATS_INC(link.recv);
            }
        }
        else /* if not L bit, then buffer's length */
            len = ENET_RX_BUF_SIZE;
        
        if ((first_pbuf = pbuf_alloc(PBUF_RAW, len, PBUF_POOL)) != NULL) {
            /* get data */
            l = 0;
            temp_l = 0;        
            /* We iterate over the pbuf chain until we have read the entire
            * packet into the pbuf. */
            for (q = first_pbuf; q != NULL; q = q->next) {
                /* Read enough bytes to fill this pbuf in the chain. The
                * available data in the pbuf is given by the q->len
                * variable.
                * This does not necessarily have to be a memcpy, you can also preallocate
                * pbufs for a DMA-enabled MAC and after receiving truncate it to the
                * actually received size. In this case, ensure the tot_len member of the
                * pbuf is the sum of the chained pbuf len members.
                */
                temp_l = LWIP_MIN(len, LWIP_MEM_ALIGN_SIZE(PBUF_POOL_BUFSIZE));
#ifdef ENET_LITTLE_ENDIAN
                data_temp = (u8_t *)__REV((u32_t)rx_bd[ rx_next_buf ].data);
                memcpy((u8_t*)q->payload, &( data_temp[l] ), temp_l);
#else
                memcpy((u8_t*)q->payload, &( rx_bd[ rx_next_buf ].data[l] ), temp_l);
#endif
                l += temp_l;
                len -= temp_l;
            }
        }
        else {
            /* bad buffers */
            LINK_STATS_INC(link.memerr);
            LINK_STATS_INC(link.drop);
            processing_error = 1;
        }
        
    EXIT_RX_PKT:
        rx_bd[rx_next_buf++].status |= ENET_RX_BD_E; /* consumed pkt */
        ENET_RDAR = ENET_RDAR_RDAR_MASK;
        if (rx_next_buf >= NUM_ENET_RX_BUFS)
            rx_next_buf = 0;
    }
    else
        return (struct pbuf*)NULL;      /* special NULL case */
    
    /* more pkts handling */
    while (more_pkts) {
       //if(!(rx_bd[ rx_next_buf ].status & RX_BD_E) )
       ///*if pkt is filled*/
       //{
        if (rx_bd[rx_next_buf].status & ENET_RX_BD_L) {
            more_pkts = 0; 
            if (rx_bd[rx_next_buf].status & (ENET_RX_BD_LG | ENET_RX_BD_NO | ENET_RX_BD_CR | ENET_RX_BD_OV)) {
                /* bad packet */
                LINK_STATS_INC(link.memerr);
                LINK_STATS_INC(link.drop);
                goto EXIT_RX_PKT2;
            }
            else {
#ifdef ENET_LITTLE_ENDIAN
                len = __REVSH(rx_bd[rx_next_buf].length);
#else
                len = rx_bd[rx_next_buf].length;
#endif
                /* buffer with L bit has total frame's length instead of remaining bytes from frame's lenght */
                len %= ENET_RX_BUF_SIZE;
                LINK_STATS_INC(link.recv);
            }
        }
        else /* if not L bit, then buffer's length */
            len = ENET_RX_BUF_SIZE;
        
        if (((next_pbuf = pbuf_alloc(PBUF_RAW, len, PBUF_POOL)) != NULL) && (!processing_error)) {
            /* get data */
            l = 0;
            temp_l = 0;        
            /* We iterate over the pbuf chain until we have read the entire
            * packet into the pbuf. */
            for (q = next_pbuf; q != NULL; q = q->next) {
                /* Read enough bytes to fill this pbuf in the chain. The
                * available data in the pbuf is given by the q->len
                * variable.
                * This does not necessarily have to be a memcpy, you can also preallocate
                * pbufs for a DMA-enabled MAC and after receiving truncate it to the
                * actually received size. In this case, ensure the tot_len member of the
                * pbuf is the sum of the chained pbuf len members.
                */
                temp_l = LWIP_MIN(len, LWIP_MEM_ALIGN_SIZE(PBUF_POOL_BUFSIZE));
#ifdef ENET_LITTLE_ENDIAN
                data_temp = (u8_t *)__REV((u32_t)rx_bd[rx_next_buf].data);
                memcpy((u8_t*)q->payload, &(data_temp[l]), temp_l);
#else
                memcpy((u8_t*)q->payload, &(rx_bd[rx_next_buf].data[l] ), temp_l);
#endif
                l += temp_l;
                len -= temp_l;
            }
            
            /* link pbufs */
            pbuf_cat(first_pbuf, next_pbuf);
        }
        else {
            /* bad buffer - out of lwip buffers */
            LINK_STATS_INC(link.memerr);
            LINK_STATS_INC(link.drop);
            processing_error = 1;
        }
    EXIT_RX_PKT2:
        rx_bd[rx_next_buf++].status |= ENET_RX_BD_E; /* consumed pkt */
        ENET_RDAR = ENET_RDAR_RDAR_MASK;
        if (rx_next_buf >= NUM_ENET_RX_BUFS)
            rx_next_buf = 0;  
    }
    
    return first_pbuf;
}
#endif


/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void ethernetif_input(void *pdata) {
    struct netif *netif = (struct netif*)pdata;
    struct eth_hdr *ethhdr;
    struct pbuf *p;

    while (1) {
        do {
            /* move received packet into a new pbuf */
            p = low_level_input(netif);
            /* no packet could be read, silently ignore this */
            if (p == NULL) {
                /* No packet could be read.  Wait a for an interrupt to tell us
                there is more data available. */
                CoPendSem( sem_rx, BLOCK_TIME_WAITING_FOR_INPUT);        
            }
        } while (NULL == p);
        
        /* points to packet payload, which starts with an Ethernet header */
        ethhdr = p->payload;
        
        switch (htons(ethhdr->type)) {
        /* IP or ARP packet? */
        case ETHTYPE_IP:
        case ETHTYPE_ARP:
#if PPPOE_SUPPORT
        /* PPPoE packet? */
        case ETHTYPE_PPPOEDISC:
        case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
            /* full packet send to tcpip_thread to process */
            if (ERR_OK != netif->input(p, netif)) {
                LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
                pbuf_free(p);
                p = NULL;
            }
            break;
        default:
            pbuf_free(p);
            p = NULL;
            break;
        }
    }
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif) {
    struct ethernetif *ethernetif;
    
    LWIP_ASSERT("netif != NULL", (netif != NULL));
    
    ethernetif = mem_malloc(sizeof(struct ethernetif));
    if (ethernetif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
        return ERR_MEM;
    }
    
#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */
    
    /*
    * Initialize the snmp variables and counters inside the struct netif.
    * The last argument should be replaced with your link speed, in units
    * of bits per second.
    */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);
    
    netif->state = ethernetif;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
    * You can instead declare your own function an call etharp_output()
    * from it if you have to do some checks before sending (e.g. if link
    * is available...) */
    netif->output = etharp_output;
    netif->linkoutput = low_level_output;
    
    ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);
    
    /* initialize the hardware */
    low_level_init(netif);
    
    return ERR_OK;
}

#if 0
/** ENET interrupt handler
 */
void ethernetif_isr(void) {
    uint32_t evnt;
    
    evnt = ENET_EIR & ENET_EIMR;
    ENET_EIR = evnt;
    
    /* tx */
    if (evnt & ENET_EIR_TXF_MASK) {    
        isr_PostSem(sem_tx);
    }
    
    /* rx */
    if (evnt & ENET_EIR_RXF_MASK) {    
        isr_PostSem(sem_rx);
    }
    
    /* error */
    if (evnt & (ENET_EIR_UN_MASK | ENET_EIR_RL_MASK | ENET_EIR_LC_MASK | ENET_EIR_EBERR_MASK | ENET_EIR_BABT_MASK | ENET_EIR_BABR_MASK | ENET_EIR_EBERR_MASK)) {
        init_enet_bufs();
        ENET_RDAR = ENET_RDAR_RDAR_MASK;
    }
}
#endif

/** ENET interrupt handler
 */
void ENET_Transmit_IRQHandler(void) {
    ENET_EIR = ENET_EIR_TXF_MASK;

    isr_PostSem(sem_tx);
}

/** ENET interrupt handler
 */
void ENET_Receive_IRQHandler(void) {
    ENET_EIR = ENET_EIR_RXF_MASK;

    isr_PostSem(sem_rx);
}

/** ENET interrupt handler
 */
void ENET_Error_IRQHandler(void) {
    ENET_EIR = (ENET_EIR_UN_MASK | ENET_EIR_RL_MASK | ENET_EIR_LC_MASK | ENET_EIR_EBERR_MASK | ENET_EIR_BABT_MASK | ENET_EIR_BABR_MASK | ENET_EIR_EBERR_MASK);

	init_enet_bufs();
	ENET_RDAR = ENET_RDAR_RDAR_MASK;
}
