#ifndef EMAC_RX_H
#define EMAC_RX_H

#include "emac/emac.h"

#ifdef __cplusplus
extern "C" {
#endif

err_t emac_rx_init(emac_data_t* emac);
void rxTask(void *pvParameters);
void emac_input(emac_data_t* emac, uint32_t port, struct pbuf* p);
void rx_port_overrun(port_data_t* port);

#ifdef __cplusplus
}
#endif

#endif
