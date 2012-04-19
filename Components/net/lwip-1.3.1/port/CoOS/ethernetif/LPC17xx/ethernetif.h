#ifndef ETHERNETIF_H_
#define ETHERNETIF_H_

#include "lwip/debug.h"
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/ip.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "netif/etharp.h"

err_t
ethernetif_init(struct netif *netif);

void
ethernetif_input(struct netif *netif);

#endif
