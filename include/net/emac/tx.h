#ifndef EMAC_TX_H
#define EMAC_TX_H

#include "emac/emac.h"

#ifdef __cplusplus
extern "C" {
#endif

void tx_desc_init(enet_tx_bd_struct_t* desc, size_t count);
err_t emac_tx_init(emac_data_t* emac);
err_t port_tx_init(port_data_t* port);

void send_with_priority(emac_data_t* emac, const uint8_t* hwaddr, uint16_t eth_type, struct pbuf* p, uint32_t priority);

#ifdef __cplusplus
}
#endif

#endif
