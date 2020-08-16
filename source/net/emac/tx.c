#include "emac/tx.h"

#include <string.h>

const uint32_t tx_priority_num[] = {8, 4};


void tx_desc_init(enet_tx_bd_struct_t* desc, size_t count) {
  memset(desc, 0, count * sizeof(enet_tx_bd_struct_t));
  desc[count - 1].control |= ENET_BUFFDESCRIPTOR_TX_WRAP_MASK;
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

static uint32_t next_tx_desc(uint32_t idx) {
  idx++;
  if (idx >= TX_DESC_COUNT)
    idx = 0;
  return idx;
}

static uint32_t next_tx_packet(uint32_t idx) {
  idx++;
  if (idx >= TX_PKT_COUNT)
    idx = 0;
  return idx;
}

void port_tx_reclaim(port_data_t* port) {
  uint32_t idx = port->first_tx_desc;
  uint32_t next_idx = port->next_tx_desc;
  
  while (true) {
    enet_tx_bd_struct_t* desc = &port->tx_desc[idx];
    if ((idx == next_idx) |
        (desc->control & ENET_BUFFDESCRIPTOR_TX_READY_MASK)) {
      break;
    }
  
    uint32_t packet_index = port->first_tx_packet;
    if (port->last_tx_desc[packet_index] == idx) {
      struct pbuf* p = port->tx_pbuf[packet_index];
      if (p != NULL)
        pbuf_free(p);

      xSemaphoreGive(port->tx_packet_sem);

      port->first_tx_packet = next_tx_packet(packet_index);
    }

    idx = next_tx_desc(idx);
  }

  port->first_tx_desc = idx;
}

static void transmit_with_priority(emac_data_t* emac, u32_t priority)
{
  send_data_t send_data;
  if (xQueueReceive(emac->tx_priority_queue[priority], &send_data, 0) == pdTRUE) {
    struct pbuf* p = send_data.p;
//    printf("Transmit\n");
    uint8_t* dst_addr = send_data.dst_addr;
    uint16_t eth_type = send_data.eth_type;
//    printf("Transmiting packet to %02x:%02x:%02x:%02x:%02x:%02x %p\n",
//      dst_addr[0], dst_addr[1], dst_addr[2],
//      dst_addr[3], dst_addr[4], dst_addr[5], p
//    );

    LWIP_ASSERT("pbuf should not be null", p != NULL);
    uint32_t chain_len = (u32_t) pbuf_clen(p);
    LWIP_ASSERT("chain_len should be <= NUM_BUFF_TXDESCS - 2", chain_len <= TX_DESC_PER_PKT);

    uint16_t payload_size = p->tot_len;
    uint16_t total_size = payload_size + ETHERNET_HEADER_SIZE + PRP_PACKET_TRAILER_SIZE;//FIXME
    uint16_t prp_size = payload_size + PRP_PACKET_TRAILER_SIZE;

    uint16_t bytes_to_pad = 0;
    if (total_size < MIN_PACKET_SIZE)
      bytes_to_pad = MIN_PACKET_SIZE - total_size;
    uint16_t tx_sequence = htons(emac->tx_sequence);
    emac->tx_sequence++;

    for (unsigned int port_index = 0; port_index < PORT_COUNT; port_index++) {
      uint16_t mid_trailer = (0 == port_index? 0xa000: 0xb000) | prp_size;
      mid_trailer = htons(mid_trailer);

      port_data_t* port = &(emac->port[port_index]);
      ENET_Type* enet = port->enet;
      uint32_t packet_index = port->next_tx_packet;

      uint32_t idx = port->next_tx_desc;
      uint8_t* head_buffer = (uint8_t*)(port->tx_head_buffer[packet_index]);
      memcpy(head_buffer, dst_addr, 6);
      memcpy(head_buffer + 12, &eth_type, 2);
      port->tx_desc[idx].buffer = head_buffer;
      port->tx_desc[idx].length = 14;
      port->tx_desc[idx].control &= ~(ENET_BUFFDESCRIPTOR_TX_LAST_MASK |
                                      ENET_BUFFDESCRIPTOR_TX_TRANMITCRC_MASK);
      port->tx_desc[idx].control |= ENET_BUFFDESCRIPTOR_TX_READY_MASK;
//      port->enet->TDAR |= ENET_TDAR_TDAR_MASK;

      struct pbuf* q = p;
      uint32_t dn = chain_len;
      while (dn > 0) {
        dn--;
        idx = next_tx_desc(idx);
        port->tx_desc[idx].buffer = q->payload;
        port->tx_desc[idx].length = q->len;
        port->tx_desc[idx].control &= ~(ENET_BUFFDESCRIPTOR_TX_LAST_MASK |
                                        ENET_BUFFDESCRIPTOR_TX_TRANMITCRC_MASK);
        port->tx_desc[idx].control |= ENET_BUFFDESCRIPTOR_TX_READY_MASK;
//        enet->TDAR |= ENET_TDAR_TDAR_MASK;
        LWIP_DEBUGF(EMAC_DEBUG | LWIP_DBG_TRACE, ("transmit_with_priority: pbuf packet(%p) sent, chain#=%lu, "
                                                  "size = %d (index=%lu)\n", q->payload, dn, q->len, idx));
        q = q->next;
      } 

      uint8_t* tail_buffer = port->tx_tail_buffer[packet_index];
      memset(tail_buffer, 0, bytes_to_pad);
      *(uint16_t*)(tail_buffer + bytes_to_pad) = tx_sequence;
      *(uint16_t*)(tail_buffer + bytes_to_pad + 2) = mid_trailer;
      *(tail_buffer + bytes_to_pad + 4) = 0x88;
      *(tail_buffer + bytes_to_pad + 5) = 0xfb;
      idx = next_tx_desc(idx);
      port->tx_desc[idx].buffer = tail_buffer; ///"\xaa\xbb\xcc\xdd\xee\xff";
      port->tx_desc[idx].length = bytes_to_pad + PRP_PACKET_TRAILER_SIZE;
      port->tx_desc[idx].control |= ENET_BUFFDESCRIPTOR_TX_LAST_MASK |
                                    ENET_BUFFDESCRIPTOR_TX_TRANMITCRC_MASK |
                                    ENET_BUFFDESCRIPTOR_TX_READY_MASK;
      enet->TDAR |= ENET_TDAR_TDAR_MASK;

      port->tx_pbuf[packet_index] = p;
      port->last_tx_desc[packet_index] = idx;
      port->next_tx_desc = next_tx_desc(idx);
      port->next_tx_packet = next_tx_packet(packet_index);
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
 
//  printf("Sending packet to %02x:%02x:%02x:%02x:%02x:%02x %p\n",
//    dst_addr[0], dst_addr[1], dst_addr[2],
//    dst_addr[3], dst_addr[4], dst_addr[5],
//  p);
  send_data_t send_data = {
    .eth_type=eth_type,
    .p=p,
  };
  memcpy(&(send_data.dst_addr), dst_addr, ETHERNET_ADDRESS_SIZE);
  for (volatile unsigned int i; xQueueSend(emac->tx_priority_queue[priority], &send_data, portMAX_DELAY) != pdTRUE; );
}

static void txTask(void *pvParameters) {
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

err_t port_tx_init(port_data_t* port) {
  port->next_tx_desc = 0;
  port->first_tx_packet = 0;
  port->next_tx_packet = 0;

  return ERR_OK;
}

err_t emac_tx_init(emac_data_t* emac) {
  emac->tx_sequence = 0;

  emac->tx_queue_set = xQueueCreateSet(TX_QUEUE_COUNT * TX_QUEUE_SIZE + PORT_COUNT);
  LWIP_ASSERT("tx_queue_set creation error", (emac->tx_queue_set != NULL));

  for (u32_t i = 0; i < PORT_COUNT; i++) {
    port_data_t* port = emac->port + i;

    xQueueAddToSet(port->tx_clean_sem, emac->tx_queue_set);

    port->tx_packet_sem = xSemaphoreCreateCounting(TX_PKT_COUNT, TX_PKT_COUNT);
    LWIP_ASSERT("tx_packet_sem creation error", (port->tx_packet_sem != NULL));

    port->tx_clean_sem = xSemaphoreCreateBinary();
    LWIP_ASSERT("tx_clean_sem creation error", (port->tx_clean_sem!= NULL));
  }

  for (u32_t i = 0; i < TX_QUEUE_COUNT; i++) {
    emac->tx_priority_queue[i] = xQueueCreate(tx_priority_num[i], sizeof(send_data_t));
    LWIP_ASSERT("tx_priority_queue creation error", (emac->tx_priority_queue[i] != NULL));
    xQueueAddToSet(emac->tx_priority_queue[i], emac->tx_queue_set);
  }

  sys_thread_new("tx_thread", txTask, emac, DEFAULT_THREAD_STACKSIZE, tskTX_PRIORITY);

  return ERR_OK;
}

