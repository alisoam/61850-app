#ifndef EMAC_H
#define EMAC_H

#ifdef __cplusplus
extern "C"
{
#endif

//#define ENET_ENHANCEDBUFFERDESCRIPTOR_MODE


#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "netif/etharp.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

#include "fsl_enet.h"

#include "config.h"
#include "rb_tree.h"


#if TX_PKT_COUNT < 2
#error TX_PKT_COUT must be at least 2
#endif

#if RX_PKT_COUNT < 3
#error RX_PKT_COUNT must be at least 3
#endif

#define DUPLICATE_DROP_ENTRY_FORGET_TIME            440
#define DUPLICATE_DROP_NODE_STATUS_PORT1_RECEIVED   (1 <<  0)
#define DUPLICATE_DROP_NODE_STATUS_PORT2_RECEIVED   (1 <<  1)
#define DUPLICATE_DROP_NODE_STATUS_PORT             DUPLICATE_DROP_NODE_STATUS_PORT1_RECEIVED | DUPLICATE_DROP_NODE_STATUS_PORT2_RECEIVED
#define DUPLICATE_DROP_NODE_STATUS_ACTIVE           (1 << 31)
#define DUPLICATE_DROP_NODE_STATUS_PORT_RECEIVED(X) (1 << (X))

#define ETHTYPE_PRP      0x88FB
#define ETHTYPE_HSR      0x892F
#define PRP_PACKET_TRAILER_SIZE                     6
#define HSR_PACKET_HEADER_SIZE                      4
#define ETHERNET_ADDRESS_SIZE                       6
#define ETHERNET_TYPE_SIZE                          2
#define ETHERNET_HEADER_SIZE                        (2 * ETHERNET_ADDRESS_SIZE + ETHERNET_TYPE_SIZE)
#define ETHERNET_HSR_HEADER_SIZE                    (ETHERNET_HEADER_SIZE + HSR_PACKET_HEADER_SIZE)

#define PORT_COUNT                                  2
#define MIN_PACKET_SIZE                             68

#define TX_QUEUE_SIZE                               20
#define TX_QUEUE_COUNT                              2
#define TX_HEAD_BUFFER_SIZE                         32
#define TX_TAIL_BUFFER_SIZE                         64
#define TX_TAIL_PRP_BUFFER_START                    (MIN_PACKET_SIZE)
#define TX_MIN_SIZE                                 MIN_PACKET_SIZE

#define tskRECPKT_PRIORITY   (configMAX_PRIORITIES - 2)
#define tskTX_PRIORITY  (configMAX_PRIORITIES)


#pragma pack(push)
typedef struct
{
  uint16_t sequence_number;
  uint16_t size;
  uint16_t suffix;
} prp_trailer_t;
#pragma pack(pop)

typedef void (*recv_callback_t)(void* arg, const uint8_t* src_addr, const uint8_t* dst_addr, uint16_t eth_type, struct pbuf*);

typedef struct {
  ENET_Type* enet;

  enet_rx_bd_struct_t rx_desc[RX_DESC_COUNT] __attribute__((aligned(8)));
  enet_tx_bd_struct_t tx_desc[TX_DESC_COUNT] __attribute__((aligned(8)));

  struct pbuf* rx_pbuf[RX_DESC_COUNT];
  struct pbuf* tx_pbuf[TX_DESC_COUNT];
  
  uint8_t tx_tail_buffer[TX_PKT_COUNT][TX_TAIL_BUFFER_SIZE] __attribute__((aligned(8)));
  uint8_t tx_head_buffer[TX_PKT_COUNT][TX_HEAD_BUFFER_SIZE] __attribute__((aligned(8)));

  uint32_t rx_next_desc;
  volatile u32_t rx_free_descs;

  uint32_t first_tx_desc;
  uint32_t next_tx_desc;
  uint32_t first_tx_packet;
  uint32_t next_tx_packet;
  uint32_t last_tx_desc[TX_PKT_COUNT];
  SemaphoreHandle_t tx_packet_sem;

  SemaphoreHandle_t rx_sem;
  SemaphoreHandle_t tx_clean_sem;
} port_data_t;

typedef struct {
  port_data_t port[PORT_COUNT];
  
  u16_t tx_sequence;
  QueueHandle_t tx_priority_queue[TX_QUEUE_COUNT];

  QueueSetHandle_t rx_queue_set;
  QueueSetHandle_t tx_queue_set;

  rb_tree_t mac_table;

  recv_callback_t recv_callback;
  void* arg;
} emac_data_t;

typedef struct {
  uint8_t dst_addr[ETHERNET_ADDRESS_SIZE];
  uint16_t eth_type;
  struct pbuf* p;
} send_data_t;

extern void msDelay(uint32_t ms);

err_t emac_init(emac_data_t* emac, const uint8_t* hwaddr, recv_callback_t recv_callback, void* arg);

extern void msDelay(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif
