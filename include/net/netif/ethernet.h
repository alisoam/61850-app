#ifndef LWIP_HDR_NETIF_ETHERNET_H
#define LWIP_HDR_NETIF_ETHERNET_H

#include "lwip/opt.h"

#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/prot/ethernet.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LWIP_ARP_FILTER_NETIF
#define LWIP_ARP_FILTER_NETIF 0
#endif

err_t ethernet_input(struct netif *netif, struct pbuf *p, const uint8_t* src, const uint8_t* dst, uint16_t type);
err_t ethernet_output(struct netif* netif, struct pbuf* p, const struct eth_addr* src, const struct eth_addr* dst, u16_t eth_type);

extern const struct eth_addr ethbroadcast, ethzero;

#ifdef __cplusplus
}
#endif

#endif /* LWIP_HDR_NETIF_ETHERNET_H */
