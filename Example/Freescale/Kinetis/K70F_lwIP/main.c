/**
 * @file main.c
 * @date 18. Aug 2013
 * @brief lwIP demonstration - simple web server on TWR-K60N512 board
 *
 */

#include "coos.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/tcpip.h"
#include "lwip/api.h"


struct netif *macnet_netif;
struct netif *loop_netif;

int http_init(void);

static void lwip_init(void) {
    struct ip_addr ipaddr, netmask, gw;
    extern err_t ethernetif_init( struct netif *netif );
    
    tcpip_init(NULL, NULL);
    
    macnet_netif = mem_malloc(sizeof(struct netif));
    
    IP4_ADDR(&gw, 192,168,1,1);
    IP4_ADDR(&ipaddr, 192,168,1,100);
    IP4_ADDR(&netmask, 255,255,255,0);
    
    netif_set_default(netif_add(macnet_netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input));
    netif_set_up(macnet_netif);
}

int main(void) {
    CoInitOS();
    lwip_init();
    http_init();
    CoStartOS();
    
    while(1);
}

