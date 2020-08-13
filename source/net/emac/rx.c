#include "emac/rx.h"

#include <string.h>

static void rx_desc_init(enet_rx_bd_struct_t* desc, size_t count) {
  memset(desc, 0, count * sizeof(enet_tx_bd_struct_t));
  for (unsigned int i = 0; i < count; i++)
    desc[count].controlExtend1 |= ENET_BUFFDESCRIPTOR_RX_INTERRUPT_MASK;
  desc[count - 1].control |= ENET_BUFFDESCRIPTOR_RX_WRAP_MASK;
}

static void port_rx_queue_pbuf(port_data_t* port, struct pbuf *p) {
  uint32_t idx = port->rx_next_desc;
  port->rx_pbuf[idx] = p;
  enet_rx_bd_struct_t* desc = &(port->rx_desc[idx]);
  desc->buffer = p->payload;
  desc->control |= ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK;

  // TODO intrupt flag/?
  //
  idx++;
  if (idx >= RX_DESC_COUNT) {
    idx = 0;
  }

  port->rx_free_descs -= 1;
  port->rx_next_desc = idx;

  port->enet->RDAR |= ENET_RDAR_RDAR_MASK;

  // LWIP_DEBUGF(EMAC_DEBUG | LWIP_DBG_TRACE,
  //   (
  //     "port_rx_queue_pbuf: pbuf packet queued: %p (free desc=%d)\n", p,
  //     port->rx_free_descs
  //   )
  // );
}

static int port_rx_queue_one(port_data_t *port) {
  struct pbuf *p;
  p = pbuf_alloc(PBUF_RAW, (uint16_t)RX_PKT_SIZE, PBUF_RAM);
  if (p == NULL) {
    LWIP_DEBUGF(EMAC_DEBUG | LWIP_DBG_TRACE,
      ("port_rx_queue: could not allocate RX pbuf\n")
    );
    return -1;
  }

  LWIP_ASSERT(
    "port_rx_queue: pbuf is not contiguous (chained)", pbuf_clen(p) <= 1
  );

  port_rx_queue_pbuf(port, p);
  return 0;
}


static void port_rx_queue_all(port_data_t *port) {
  for (int i = 0; i < RX_DESC_COUNT; i++) {
    int ret = port_rx_queue_one(port);
    LWIP_ASSERT("port_rx_queue_all: port_rx_queue_one == 0", ret == 0);
  }
}

/* Sets up the RX descriptor ring buffers. */
err_t port_rx_init(port_data_t* port) {
  rx_desc_init(port->rx_desc, RX_DESC_COUNT);

  port->rx_free_descs = RX_DESC_COUNT;
  port->rx_next_desc = 0;
  memset(port->rx_pbuf, 0, sizeof(port->rx_pbuf));

  port_rx_queue_all(port);

  return ERR_OK;
}

err_t emac_rx_init(emac_data_t* emac) {
  for (unsigned int i = 0; i < PORT_COUNT; i++) {
    err_t err = port_rx_init(emac->port + i);
    if (err != ERR_OK)
      return err;
  }

  return ERR_OK;
}

void rx_port_overrun(port_data_t* port) {
    for (uint32_t idx = 0; idx < RX_DESC_COUNT; idx++) {
      if (port->rx_pbuf[idx] != NULL) {
        pbuf_free(port->rx_pbuf[idx]);
        port->rx_desc[idx].buffer = NULL;
      }
    }

    port_rx_init(port);
}

static void port_check_input(emac_data_t* emac, unsigned int port_index) {
  port_data_t* port = &emac->port[port_index];
  if (xSemaphoreTake(port->rx_sem, 0) != pdTRUE)
    return;
  printf("recv on port %d\n", port_index);

  ENET_Type* enet = port->enet;

  while (true) {
    uint32_t idx = port->rx_next_desc;
    enet_rx_bd_struct_t* desc = &(port->rx_desc[idx]);
    uint32_t control = desc->control;

    if (control | ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK) {
      break;
    }

    // overrun
    if (control | ENET_BUFFDESCRIPTOR_RX_OVERRUN_MASK) {
      return;
    }

    port->rx_free_descs++;
    struct pbuf* p = port->rx_pbuf[idx];

    /* Handle errors */
    bool error = control | ENET_BUFFDESCRIPTOR_RX_LENVLIOLATE_MASK | ENET_BUFFDESCRIPTOR_RX_NOOCTET_MASK |
      ENET_BUFFDESCRIPTOR_RX_CRC_MASK | ENET_BUFFDESCRIPTOR_RX_TRUNC_MASK;
    if (error) {
      port_rx_queue_pbuf(port, p);
      LWIP_DEBUGF(EMAC_DEBUG | LWIP_DBG_TRACE, ("port_low_level_input: Packet dropped\n"));
    }
    else {
      /* A packet is waiting, get length */
      uint32_t length = desc->length - (control | ENET_BUFFDESCRIPTOR_RX_LAST_MASK? 4: 0);  /* Remove FCS */

      /* Zero-copy */
      p->len = (uint16_t) length;

      /* Queue new buffer(s) */
      if (port_rx_queue_one(port) != 0) {
        /* Re-queue the pbuf for receive */
        port_rx_queue_pbuf(port, p);

        LWIP_DEBUGF(EMAC_DEBUG | LWIP_DBG_TRACE, ("port_low_level_input: Packet dropped since "
                                                  "it could not allocate Rx Buffer\n"));
      }
      else {
        LWIP_DEBUGF(EMAC_DEBUG | LWIP_DBG_TRACE, ("port_low_level_input: Packet received: "
                                                  "%p, size %ld (index=%ld)\n",p, length, idx));

        /* Save size */
        p->tot_len = (uint16_t) length;

        if (p != NULL)
          emac_input(emac, port_index, p);
      }
    }

    idx++;
    if (idx >= RX_DESC_COUNT)
      idx = 0;
    port->rx_next_desc = idx;
  }
}

static void check_rx_clean(emac_data_t* emac) {
  for (unsigned int i = 0; i < PORT_COUNT; i++)
    port_check_input(emac, i);
}

void rxTask(void *pvParameters) {
  emac_data_t *emac = pvParameters;

  while (1) {
    xQueueSelectFromSet(emac->rx_queue_set, portMAX_DELAY);
    check_rx_clean(emac);
  }
}
