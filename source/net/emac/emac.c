#include "emac/emac.h"

#include <string.h>

#include "emac/rx.h"
#include "emac/tx.h"

const uint32_t tx_priority_num[] = {8, 4};

static void port_init(ENET_Type* enet, const uint8_t* mac_addr, uint32_t rx_desc, uint32_t tx_desc) {
  enet->ECR |= ENET_ECR_RESET_MASK;

  enet->EIMR |= ENET_EIMR_RXB_MASK; // FIXME | ENET_EIMR_TXB_MASK;
//                ENET_EIMR_BABR_MASK | ENET_EIMR_BABT_MASK |
//                ENET_EIMR_EBERR_MASK | ENET_EIMR_LC_MASK |
//                ENET_EIMR_RL_MASK | ENET_EIMR_UN_MASK |
//                ENET_EIMR_PLR_MASK;

  enet->RCR |= ENET_RCR_RMII_MODE_MASK | ENET_RCR_RMII_MODE_MASK | ENET_RCR_MII_MODE_MASK | ENET_RCR_MAX_FL(RX_PKT_SIZE);
  enet->RCR &= ~(ENET_RCR_LOOP_MASK);
  enet->TCR |= ENET_TCR_ADDINS_MASK;

	uint32_t l_addr = ((uint32_t)mac_addr[3] << 24) | ((uint32_t)mac_addr[2] << 16) | ((uint32_t)mac_addr[1] << 8) | ((uint32_t)mac_addr[0]);
  uint32_t u_addr = ((uint32_t)mac_addr[5] << 8) | ((uint32_t)mac_addr[4]);
	enet->PALR = l_addr;
  enet->PAUR = u_addr;

//  enet->TFWR |= ENET_TFWR_STRFWD_MASK | 0x1U;

  enet->RDSR = rx_desc;
  enet->TDSR = tx_desc;

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

  enet->ECR |= ENET_ECR_DBSWP_MASK | ENET_ECR_EN1588_MASK | ENET_ECR_ETHEREN_MASK;
}

static err_t emac_low_level_init(emac_data_t* emac, const uint8_t* hwaddr) {
  err_t err = emac_rx_init(emac);
  if (err != ERR_OK)
    return err;
//  err = emac_tx_init(emac); FIXME
//  if (err != ERR_OK)
//    return err;
  for (u32_t i = 0; i < PORT_COUNT; i++) {
    port_data_t* port = (emac->port + i);
    ENET_Type* enet = port->enet;
    port_init(enet, hwaddr, (uint32_t)port->rx_desc, (uint32_t)port->tx_desc);
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

  emac->tx_queue_set = xQueueCreateSet(TX_QUEUE_COUNT * TX_QUEUE_SIZE + PORT_COUNT);
  LWIP_ASSERT("tx_queue_set creation error", (emac->tx_queue_set != NULL));

  for (u32_t i = 0; i < PORT_COUNT; i++) {
    port_data_t* port = emac->port + i;

    port->rx_sem = xSemaphoreCreateBinary();
    LWIP_ASSERT("rx_sem creation error", (port->rx_sem!= NULL));
    xQueueAddToSet(port->rx_sem, emac->rx_queue_set);

    port->tx_clean_sem = xSemaphoreCreateBinary();
    LWIP_ASSERT("tx_clean_sem creation error", (port->tx_clean_sem!= NULL));
    xQueueAddToSet(port->tx_clean_sem, emac->tx_queue_set);

    port->tx_packet_sem = xSemaphoreCreateCounting(TX_PKT_COUNT, TX_PKT_COUNT);
    LWIP_ASSERT("tx_packet_sem creation error", (port->tx_packet_sem != NULL));
  }


  for (u32_t i = 0; i < TX_QUEUE_COUNT; i++) {
    emac->tx_priority_queue[i] = xQueueCreate(tx_priority_num[i], sizeof(send_data_t));
    LWIP_ASSERT("tx_priority_queue creation error", (emac->tx_priority_queue[i] != NULL));

    xQueueAddToSet(emac->tx_priority_queue[i], emac->tx_queue_set);
  }

  sys_thread_new("rx_thread", rxTask, emac, DEFAULT_THREAD_STACKSIZE, tskRECPKT_PRIORITY);
  sys_thread_new("tx_thread", txTask, emac, DEFAULT_THREAD_STACKSIZE, tskTX_PRIORITY);

  return ERR_OK;
}
