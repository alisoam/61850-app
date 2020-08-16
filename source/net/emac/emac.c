#include "emac/emac.h"

#include <string.h>

#include "emac/rx.h"
#include "emac/tx.h"

static void port_init(port_data_t* port, const uint8_t* mac_addr) {
  ENET_Type* enet = port->enet;

  enet->ECR |= ENET_ECR_RESET_MASK;

  enet->EIMR |= ENET_EIMR_RXF_MASK | //ENET_EIMR_RXB_MASK |
                  ENET_EIMR_TXF_MASK;// | ENET_EIMR_TXB_MASK;
//                ENET_EIMR_BABR_MASK | ENET_EIMR_BABT_MASK | 
//                ENET_EIMR_EBERR_MASK | ENET_EIMR_LC_MASK |
//                ENET_EIMR_RL_MASK | ENET_EIMR_UN_MASK |
//                ENET_EIMR_PLR_MASK;

  enet->RCR |= ENET_RCR_RMII_MODE_MASK | ENET_RCR_RMII_MODE_MASK | ENET_RCR_MII_MODE_MASK | ENET_RCR_MAX_FL(RX_PKT_SIZE);
  enet->RCR &= ~(ENET_RCR_LOOP_MASK);
  enet->TCR |= ENET_TCR_ADDINS_MASK;

  uint32_t l_addr = ((uint32_t)mac_addr[0] << 24) | ((uint32_t)mac_addr[1] << 16) | ((uint32_t)mac_addr[2] << 8) | ((uint32_t)mac_addr[3]);
  uint32_t u_addr = ((uint32_t)mac_addr[4] << 24) | ((uint32_t)mac_addr[5] << 16);
  enet->PALR = l_addr;
  enet->PAUR = u_addr;

//  enet->TFWR |= ENET_TFWR_STRFWD_MASK | 0x1U;

  rx_desc_init(port->rx_desc, RX_DESC_COUNT);
  enet->RDSR = (uint32_t)port->rx_desc;
  tx_desc_init(port->tx_desc, TX_DESC_COUNT);
  enet->TDSR = (uint32_t)port->tx_desc;;

  enet->MRBR |= ENET_MRBR_R_BUF_SIZE(RX_DESC_SIZE);

  enet->RSFL = ENET_RSFL_RX_SECTION_FULL(20);
  enet->RSEM |= ENET_RSEM_RX_SECTION_EMPTY(45);
  enet->RAEM |= ENET_RAEM_RX_ALMOST_EMPTY(10);
  enet->RAFL |= ENET_RAFL_RX_ALMOST_FULL(60);

  enet->TSEM |= ENET_TSEM_TX_SECTION_EMPTY(45);
  enet->TAEM |= ENET_TAEM_TX_ALMOST_EMPTY(10);
  enet->TAFL |= ENET_TAFL_TX_ALMOST_FULL(60);

  enet->TACC |= 0;
  enet->RACC |= ENET_RACC_LINEDIS_MASK;

  enet->ECR |= ENET_ECR_DBSWP_MASK /*| ENET_ECR_EN1588_MASK */| ENET_ECR_ETHEREN_MASK;
}

static err_t emac_low_level_init(emac_data_t* emac, const uint8_t* hwaddr) {
  err_t err;
  for (u32_t i = 0; i < PORT_COUNT; i++) {
    port_data_t* port = (emac->port + i);
    port_init(port, hwaddr);
    err = port_rx_init(port);
    if (err != ERR_OK)
      return err;
    err = port_tx_init(port);
    if (err != ERR_OK)
      return err;
  }
  return ERR_OK;
}

err_t emac_init(emac_data_t* emac, const uint8_t* hwaddr, recv_callback_t recv_callback, void* arg) {
  err_t err;

  emac->recv_callback = recv_callback;
  emac->arg = arg;

  rb_tree_init(&(emac->mac_table));
  err = emac_low_level_init(emac, hwaddr);
  LWIP_ASSERT("emac low level init error", (err == ERR_OK));

  emac->rx_queue_set = xQueueCreateSet(PORT_COUNT);
  LWIP_ASSERT("rx_queue_set creation error", (emac->rx_queue_set != NULL));

  for (u32_t i = 0; i < PORT_COUNT; i++) {
    port_data_t* port = emac->port + i;

    port->rx_sem = xSemaphoreCreateBinary();
    LWIP_ASSERT("rx_sem creation error", (port->rx_sem!= NULL));
    xQueueAddToSet(port->rx_sem, emac->rx_queue_set);
  }


  sys_thread_new("rx_thread", rxTask, emac, DEFAULT_THREAD_STACKSIZE, tskRECPKT_PRIORITY);

  emac_tx_init(emac);

  return ERR_OK;
}
