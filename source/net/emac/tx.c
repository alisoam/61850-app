#include "emac/tx.h"

#include <string.h>


static void tx_desc_init(enet_tx_bd_struct_t* desc, size_t count) {
  memset(desc, 0, count * sizeof(enet_tx_bd_struct_t));
  desc[count - 1].control |= ENET_BUFFDESCRIPTOR_TX_WRAP_MASK;
}

/* Sets up the TX descriptor ring buffers */
static err_t port_tx_init(port_data_t* port) {
  /* Build TX descriptors for local buffers */
  tx_desc_init(port->tx_desc, TX_DESC_COUNT);

  for (uint32_t i = 0; i < TX_PKT_COUNT; i++) {
    memset(port->tx_tail_buffer[i], 0, sizeof(port->tx_tail_buffer[i]));
    *(port->tx_tail_buffer[i] + TX_TAIL_PRP_BUFFER_START + 4) = 0x88;
    *(port->tx_tail_buffer[i] + TX_TAIL_PRP_BUFFER_START + 5) = 0xfb;
  }

//  port->last_tx_idx = 0;
  port->first_tx_packet = 0;
  port->last_tx_packet = 0;

  return ERR_OK;
}

err_t emac_tx_init(emac_data_t* emac) {
  emac->tx_sequence = 0;

  for (unsigned int i = 0; i < PORT_COUNT; i++) {
    err_t err = port_tx_init(emac->port + i);
    if (err != ERR_OK)
      return err;
  }

  return ERR_OK;
}

/* Free TX buffers that are complete */
//static void port_tx_reclaim_st(port_data_t* port, u32_t cidx) {
//  u32_t idx;
//  while (cidx != (idx = port->last_tx_idx)) {
//    if (port->txb[idx] != NULL) {
//      LWIP_DEBUGF(EMAC_DEBUG | LWIP_DBG_TRACE,
//        ("port_tx_reclaim_st: Freeing packet %p (index %d)\n", port->txb[idx], idx)
//      );
//      uint32_t packet_index = port->first_tx_packet;
//      if (port->tx_idx[packet_index] == idx) {
//        if (NULL != port->txb[packet_index])
//          pbuf_free(port->txb[packet_index]);
//        //port->txb[packet_index] = NULL;
//        xSemaphoreGive(port->tx_packet_sem);
//
//        port->first_tx_packet++;
//        if (port->first_tx_packet >= NUM_PCKT_TX)
//          port->first_tx_packet = 0;
//      }
//    }
//
//    port->last_tx_idx++;
//    if (port->last_tx_idx >= NUM_BUFF_TXDESCS)
//      port->last_tx_idx = 0;
//  }
//}

void port_tx_reclaim(port_data_t* port) {
//  port_tx_reclaim_st(port, Chip_ENET_GetTXConsumeIndex(port->enet));
//
  uint32_t packet_index = port->first_tx_packet;
  struct pbuf* p = port->tx_pbuf[packet_index];
  if (p != NULL)
    pbuf_free(p);
  //port->txb[packet_index] = NULL;
  xSemaphoreGive(port->tx_packet_sem);

  port->first_tx_packet++;
  if (port->first_tx_packet >= TX_PKT_COUNT)
    port->first_tx_packet = 0;
}

static void transmit_with_priority(emac_data_t* emac, u32_t priority)
{
  send_data_t send_data;
  if (xQueueReceive(emac->tx_priority_queue[priority], &send_data, 0) == pdTRUE) {
    struct pbuf* p = send_data.p;
    printf("Transmit\n");
    uint8_t* dst_addr = send_data.dst_addr;
    uint16_t eth_type = send_data.eth_type;
    printf("Transmiting packet to %02x:%02x:%02x:%02x:%02x:%02x %p\n",
      dst_addr[0], dst_addr[1], dst_addr[2],
      dst_addr[3], dst_addr[4], dst_addr[5], p
    );

    LWIP_ASSERT("pbuf should not be null", p != NULL);
    u32_t chain_len = (u32_t) pbuf_clen(p);
    LWIP_ASSERT("chain_len should be <= NUM_BUFF_TXDESCS - 2", chain_len <= TX_DESC_PER_PKT - 2);

    uint16_t payload_size = p->tot_len;
    uint16_t total_size = payload_size + ETHERNET_HEADER_SIZE + PRP_PACKET_TRAILER_SIZE;//FIXME
    uint16_t prp_size = payload_size + PRP_PACKET_TRAILER_SIZE;

    u16_t trailer_offset = MIN_PACKET_SIZE;
    if (MIN_PACKET_SIZE > total_size)
      trailer_offset = total_size;
    u16_t bytes_to_pad = MIN_PACKET_SIZE - trailer_offset;
    u16_t tx_sequence = htons(emac->tx_sequence);
    emac->tx_sequence++;

    for (unsigned int port_index = 0; port_index < PORT_COUNT; port_index++) {
      u16_t mid_trailer = (0 == port_index? 0xa000: 0xb000) | prp_size;
      mid_trailer = htons(mid_trailer);

      port_data_t* port = emac->port + port_index;
      uint32_t packet_index = port->last_tx_packet;

      u32_t idx = 0; // XXX FIXME
      uint8_t* head_buffer = (uint8_t*)(port->tx_head_buffer[packet_index]);
      memcpy(head_buffer, dst_addr, 6);
      memcpy(head_buffer + 6, EMAC_ADDR, 6);// FIXME // IN init ????
      memcpy(head_buffer + 12, &eth_type, 2);// FIXME
      port->tx_desc[idx].buffer = head_buffer;
      port->tx_desc[idx].length = 0; // XXX FIXME
      port->tx_desc[idx].control = 0; // XXX FIXME

      struct pbuf* q = p;
      u32_t dn = chain_len;
      while (dn > 0) {
        dn--;
        //increase idx
        port->tx_desc[idx].buffer = q->payload;
        port->tx_desc[idx].length = q->len;
        port->tx_desc[idx].control = 0; // XXX FIXME
        LWIP_DEBUGF(EMAC_DEBUG | LWIP_DBG_TRACE, ("transmit_with_priority: pbuf packet(%p) sent, chain#=%lu, "
                                                  "size = %d (index=%lu)\n", q->payload, dn, q->len, idx));
        q = q->next;
      }

      uint8_t* tail_buffer = port->tx_tail_buffer[packet_index];
      *(uint16_t*)(tail_buffer + TX_TAIL_PRP_BUFFER_START) = tx_sequence;
      *(uint16_t*)(tail_buffer + TX_TAIL_PRP_BUFFER_START + 2) = mid_trailer;
//      idx = Chip_ENET_IncTXProduceIndex(enet);
      port->tx_idx[packet_index] = idx;
      port->tx_desc[idx].buffer = tail_buffer + trailer_offset;
      port->tx_desc[idx].length = bytes_to_pad + PRP_PACKET_TRAILER_SIZE;
      port->tx_desc[idx].control = 0; // XXX FIXME

      port->enet->TDAR |= ENET_TDAR_TDAR_MASK;

      port->last_tx_packet++;
      if (port->last_tx_packet >= TX_PKT_COUNT)
        port->last_tx_packet = 0;
    }
  }
}

static void check_tx_clean(emac_data_t* emac) {
  for (unsigned int i = 0; i < PORT_COUNT; i++) {
    port_data_t* port = emac->port + i;
    if (xSemaphoreTake(port->tx_clean_sem, 0) == pdTRUE) {
      // check for underrun
      if (false) {
        // LINK_STATS_INC(link.err);
        // LINK_STATS_INC(link.drop);
        // XXX system fatal error
      }
      else {
        /* Free TX buffers that are done sending */
        port_tx_reclaim(port);
      }
    }
  }
}

static bool transmit(emac_data_t* emac, u32_t priority)
{
  if (uxQueueMessagesWaiting(emac->tx_priority_queue[priority]) > 0) {
    transmit_with_priority(emac, priority);
    return true;
  }

  return false;
}

static void pbuf_prepare(struct pbuf *p) {
  /* pbuf given? */
  if (p != NULL) {
    SYS_ARCH_SET(p->ref, (LWIP_PBUF_REF_T)(p->ref + PORT_COUNT));
    LWIP_ASSERT("pbuf ref overflow", p->ref > 0);
  }
}

static void prepare_pbuf_for_transmit(emac_data_t* emac, struct pbuf* p, u32_t priority) {
  pbuf_prepare(p);

  for (unsigned i = 0; i < PORT_COUNT * (u32_t) pbuf_clen(p); i++) {
    port_data_t* port = emac->port + i;
    for (volatile unsigned int i; xSemaphoreTake(port->tx_packet_sem, portMAX_DELAY) != pdTRUE;) // FIXME
      ;
  }
}

void send_with_priority(emac_data_t* emac, const uint8_t* dst_addr, uint16_t eth_type, struct pbuf* p, u32_t priority){
  prepare_pbuf_for_transmit(emac, p, priority);
 
  printf("Sending packet to %02x:%02x:%02x:%02x:%02x:%02x %p\n",
    dst_addr[0], dst_addr[1], dst_addr[2],
    dst_addr[3], dst_addr[4], dst_addr[5],
  p);
  send_data_t send_data = {
    .eth_type=eth_type,
    .p=p,
  };
  memcpy(&(send_data.dst_addr), dst_addr, ETHERNET_ADDRESS_SIZE);
  for (volatile unsigned int i; xQueueSend(emac->tx_priority_queue[priority], &send_data, portMAX_DELAY) != pdTRUE; );
}

/* Transmit task for FreeRTOS */
void txTask(void *pvParameters) {
  emac_data_t* emac = pvParameters;

  while (true) {
    xQueueSelectFromSet(emac->tx_queue_set, 200 / portTICK_PERIOD_MS);
    check_tx_clean(emac);
    while (true) {
      for (unsigned int i = 0; i < TX_QUEUE_COUNT; i ++)
        if (transmit(emac, i))
          continue;
      break;
    }
  }
}
