#include "interface.h"

#include <string.h>

#include "lwip/etharp.h"

#include "emac/emac.h"
#include "netif/ethernet.h"

emac_data_t emac __attribute__((section (".bss_itcm"))) __attribute__((aligned(64)));

char lwip_heap[MEM_SIZE] __attribute__((section (".bss_ncache"))) __attribute__((aligned(64)));

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
