#ifndef LWIP_ENET_H
#define LWIP_ENET_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lwip/opt.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"

#define NETIF_FLAG_PRP                              0x80U

#define DUPLICATE_DROP_NODE_STATUS_PORT1_RECEIVED   (1 <<  0)
#define DUPLICATE_DROP_NODE_STATUS_PORT2_RECEIVED   (1 <<  1)
#define DUPLICATE_DROP_NODE_STATUS_PORT             DUPLICATE_DROP_NODE_STATUS_PORT1_RECEIVED | DUPLICATE_DROP_NODE_STATUS_PORT2_RECEIVED
#define DUPLICATE_DROP_NODE_STATUS_ACTIVE           (1 << 31)
#define DUPLICATE_DROP_NODE_STATUS_PORT_RECEIVED(X) (1 << X)

#define ENTRY_FORGET_TIME 480

err_t enet_input(struct pbuf *p, struct netif *netif);

#ifdef __cplusplus
}
#endif

#endif
