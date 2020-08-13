#ifndef EMAC_TX_H
#define EMAC_TX_H

#include "emac/emac.h"

#ifdef __cplusplus
extern "C" {
#endif

err_t emac_tx_init(emac_data_t* emac);

void send_with_priority(emac_data_t* emac, const uint8_t* hwaddr, uint16_t eth_type, struct pbuf* p, uint32_t priority);
void txTask(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif
