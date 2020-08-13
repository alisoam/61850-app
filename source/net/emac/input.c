#include "emac/emac.h"

static uint32_t get_duplicate_drop_time() {
  return xTaskGetTickCount() / portTICK_RATE_MS;
}

static void hsr_input(emac_data_t* emac, uint32_t port, struct pbuf* p) {
  char* pckt = (char*)p->payload;
  printf(
    "rcv from: %02x:%02x:%02x:%02x:%02x:%02x to: %02x:%02x:%02x:%02x:%02x:%02x type: 0x%02x%02x\n",
    pckt[0], pckt[1], pckt[2],
    pckt[3], pckt[4], pckt[5],
    pckt[6], pckt[7], pckt[8],
    pckt[9], pckt[10], pckt[11],
    pckt[12], pckt[13]
  );

  uint8_t* src = (uint8_t*)p->payload;
  uint8_t* dst = src + ETHERNET_ADDRESS_SIZE;
  uint16_t type = *(uint16_t*)(dst + ETHERNET_ADDRESS_SIZE);
  printf("type:%u\n", type);
  if (pbuf_remove_header(p, ETHERNET_HEADER_SIZE)) {
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
                ("ethernet_input: IPv4 packet dropped, too short (%"U16_F"/%"U16_F")\n",
                  p->tot_len, 0));
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("Can't move over header in packet"));
    pbuf_free(p);
  }

  emac->recv_callback(emac->arg, src, dst, type, p);
}

static void prp_input(emac_data_t* emac, uint32_t port, struct pbuf* p)  {
  static int i = 0;
  i++;
  printf("prp_input %d\n", i);
  uint16_t tot_len = p->tot_len;
  if (tot_len > SIZEOF_ETH_HDR + PRP_PACKET_TRAILER_SIZE)
  {
    prp_trailer_t* trailer = p->payload + tot_len - PRP_PACKET_TRAILER_SIZE;
    for (unsigned int i = 0; i < 6; i++)
      printf("0x%02hx ", *((char*)(trailer)+i));
    printf("\n");
    uint16_t prp_size = ntohs(trailer->size) & 0x0fff;
    printf("prp_size: %x total_len: %u %x\n", prp_size, tot_len, (trailer->suffix & 0xFF00) >> 8);
    if (
      (prp_size == (tot_len - 14)) && //- 14 + 6)) &&
      (memcmp(&(trailer->suffix), "\x88\xfb", 2) == 0)
    ) {
      uint8_t* dst_mac_address = p->payload + ETH_HWADDR_LEN;
      uint16_t prp_sequence = ntohs(trailer->sequence_number);
      printf("prp %u\r\n", prp_sequence);
      rb_tree_node_data_t d;
      memcpy(d.value.mac_addr, dst_mac_address, 6);
      d.value.sequence_number = prp_sequence;
      bool is_new;
      rb_tree_node_t* node = get_or_create_rb_tree_node(&(emac->mac_table), &d, &is_new);
      node->data.status |= DUPLICATE_DROP_NODE_STATUS_PORT_RECEIVED(port);
      uint32_t now = get_duplicate_drop_time();
      printf("duplicate status: %u current port %u\n", node->data.status, DUPLICATE_DROP_NODE_STATUS_PORT_RECEIVED(1-port));
      if (is_new) {
        uint32_t now = get_duplicate_drop_time();
        node->data.time = now;
      } else{

        uint32_t t = node->data.time;
        uint32_t time_delta = (now >= t)? now - t: UINT32_MAX - (t - now) + 1;
        uint32_t node_status = node->data.status;
        uint8_t other_port = 1 - port;

        if (
          (node_status & DUPLICATE_DROP_NODE_STATUS_PORT_RECEIVED(other_port)) &&
          (time_delta < DUPLICATE_DROP_ENTRY_FORGET_TIME)
        ) {
          printf("drop\n");
          pbuf_free(p);
          return;
        } else
          node->data.time = now;
      }
    }
  }
  hsr_input(emac, port, p);
}

void emac_input(emac_data_t* emac, uint32_t port, struct pbuf* p) {
  prp_input(emac, port, p);
}
