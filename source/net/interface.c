#include "interface.h"

#include <string.h>

#include "lwip/etharp.h"

#include "emac/emac.h"
#include "netif/ethernet.h"

emac_data_t emac;

enet_rx_bd_struct_t rx_desc[PORT_COUNT][RX_DESC_COUNT] __attribute__((aligned(8))) __attribute__((section (".enet_bss")));
enet_tx_bd_struct_t tx_desc[PORT_COUNT][TX_DESC_COUNT] __attribute__((aligned(8))) __attribute__((section (".enet_bss"))) ;
uint8_t tx_tail_buffer[PORT_COUNT][TX_PKT_COUNT * TX_TAIL_BUFFER_SIZE] __attribute__((aligned(64))) __attribute__((section (".enet_bss")));
uint8_t tx_head_buffer[PORT_COUNT][TX_PKT_COUNT * TX_HEAD_BUFFER_SIZE] __attribute__((aligned(64))) __attribute__((section (".enet_bss")));


char lwip_heap[MEM_SIZE] __attribute__((section (".enet_bss"))) __attribute__((aligned(64)));

int cntr_adv = 0;

void enet_handler(port_data_t* port) {
  signed portBASE_TYPE xRecTaskWoken = pdFALSE, XTXTaskWoken = pdFALSE;
  ENET_Type* enet = port->enet;
  uint32_t ints = enet->EIR;

  cntr_adv++;

  if (ints & (ENET_EIR_RXF_MASK | ENET_EIR_RXB_MASK))
    xSemaphoreGiveFromISR(port->rx_sem, &xRecTaskWoken);

  if (ints & (ENET_EIR_TXF_MASK | ENET_EIR_TXB_MASK)) {
    xSemaphoreGiveFromISR(port->tx_clean_sem, &XTXTaskWoken);
	}

  enet->EIR |= (ints & (ENET_EIR_RXF_MASK | ENET_EIR_RXB_MASK |
                               ENET_EIR_TXF_MASK | ENET_EIR_TXB_MASK));
  portEND_SWITCHING_ISR(xRecTaskWoken || XTXTaskWoken); 
}

void ENET_IRQHandler() {
  enet_handler(emac.port + 0);
}

void ENET2_IRQHandler() {
  enet_handler(emac.port + 1);
}

void link_input(void* arg, const uint8_t* src, const uint8_t* dst, uint16_t type, struct pbuf* p) {
  struct netif* netif = (struct netif*)arg;

  switch (htons(type)) {
  case ETHTYPE_IP:
  case ETHTYPE_ARP:
  case ETHTYPE_IPV6: {
#if PPPOE_SUPPORT
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    err_t err = ethernet_input(netif, p, src, dst, type);
    if (err != ERR_OK) {
      LWIP_DEBUGF(NETIF_DEBUG, ("link_input: ethernet_input input error\n"));
      pbuf_free(p);
    }
    break;
  }
  default:
    pbuf_free(p);
    break;
  }
}

static err_t output(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr) {
  if (netif->flags & NETIF_FLAG_LINK_UP) {
    return etharp_output(netif, q, ipaddr);
  }
  return ERR_CONN;
}

err_t interface_init(struct netif *netif)
{
  err_t err;

  LWIP_ASSERT("netif != NULL", (netif != NULL));

  memcpy(netif->hwaddr, EMAC_ADDR, ETH_HWADDR_LEN);
  netif->hwaddr_len = ETH_HWADDR_LEN;
  netif->mtu = 1500;
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_UP |
    NETIF_FLAG_ETHERNET | NETIF_FLAG_LINK_UP;

  /* Initialize the hardware */
  memset(&emac, 0, sizeof(emac));
  netif->state = &emac;
  emac.port[0].enet = ENET;
  emac.port[1].enet = ENET2;
  for (unsigned int i = 0; i < PORT_COUNT; i++) {
    emac.port[i].rx_desc = rx_desc[i];
    emac.port[i].tx_desc = tx_desc[i];
    emac.port[i].tx_tail_buffer = tx_tail_buffer[i];
    emac.port[i].tx_head_buffer = tx_head_buffer[i];
  }
  err = emac_init(&emac, netif->hwaddr, link_input, netif);
  if (err != ERR_OK) {
    return err;
  }

#if LWIP_NETIF_HOSTNAM
  netif->hostname = "com";
#endif

  netif->name[0] = 'e';
  netif->name[1] = 'n';

  netif->output = output;
  netif->linkoutput = NULL;

  return ERR_OK;
}
