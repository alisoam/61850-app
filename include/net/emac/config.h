#ifndef __EMAC_CONFIG_H_
#define __EMAC_CONFIG_H_

#include "lwip/opt.h"
#include "FreeRTOSConfig.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define EMAC_ADDR "\x12\x45\x67\x89\xab\xcd"

#define config_ENET_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)

#define RX_PKT_COUNT      3
#define RX_DESC_SIZE      1600
#define RX_DESC_PER_PKT   1
#define RX_DESC_COUNT     (RX_PKT_COUNT * RX_DESC_PER_PKT)
#define RX_PKT_SIZE       (RX_DESC_SIZE * RX_DESC_PER_PKT)


#define TX_PKT_COUNT      4
#define TX_DESC_SIZE      1600
#define TX_DESC_PER_PKT   1
#define TX_DESC_COUNT     (TX_PKT_COUNT * TX_DESC_PER_PKT + 2 * TX_PKT_COUNT)
#define TX_PKT_SIZE       (TX_DESC_SIZE * TX_DESC_PER_PKT)


#ifdef __cplusplus
}
#endif

#endif
