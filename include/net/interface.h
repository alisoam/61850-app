#ifndef INTERFACE_H
#define INTERFACE_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/sys.h"

#include "rb_tree.h"


#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

err_t interface_init(struct netif *netif);

#ifdef __cplusplus
}
#endif

#endif
