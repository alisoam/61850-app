#ifndef EMAC_RX_H
#define EMAC_RX_H

#include "emac/emac.h"

#ifdef __cplusplus
extern "C" {
#endif

err_t emac_rx_init(emac_data_t* emac);
err_t port_rx_init(port_data_t* port);
void rx_desc_init(enet_rx_bd_struct_t* desc, size_t count);
void rxTask(void *pvParameters);
void emac_input(emac_data_t* emac, uint32_t port, struct pbuf* p);
void rx_port_overrun(port_data_t* port);

#ifdef __cplusplus
}
#endif

#endif
