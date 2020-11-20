#ifndef LWIP_OPTS_H
#define LWIP_OPTS_H

#define NO_SYS                          0

#define LWIP_DNS                        1
#define LWIP_SO_RCVTIMEO                1
#define LWIP_SO_SNDTIMEO                1
#define LWIP_SO_SNDRCVTIMEO_NONSTANDARD 1

#define LWIP_RAND                       rand
#define NO_SYS_NO_TIMERS                0
#define SYS_LIGHTWEIGHT_PROT            1
#define MEM_ALIGNMENT                   8
#define PBUF_LINK_HLEN                  0

#define PBUF_POOL_SIZE                  8
#define ETH_PAD_SIZE                    0
#define IP_SOF_BROADCAST                1
#define IP_SOF_BROADCAST_RECV           1

#define CHECKSUM_GEN_IP                 1
#define CHECKSUM_GEN_UDP                1
#define CHECKSUM_GEN_TCP                1
#define CHECKSUM_CHECK_IP               1
#define CHECKSUM_CHECK_UDP              1
#define CHECKSUM_CHECK_TCP              1
#define LWIP_CHECKSUM_ON_COPY           1
#define LWIP_PLATFORM_BYTESWAP          0

//#define LWIP_RAW                        1
#define LWIP_DHCP                       0
#define LWIP_UDP                        1
#define LWIP_ICMP                       1

#define LWIP_BROADCAST_PING             1
#define TCP_MSS                         1000
#define TCP_SND_BUF                     (2 * TCP_MSS)
    
#define LWIP_SOCKET                     1
#define LWIP_NETCONN                    1
#define MEMP_NUM_SYS_TIMEOUT            300

//#define LWIP_STATS                      0
//#define LINK_STATS                      0
//#define LWIP_STATS_DISPLAY              0

#define LWIP_DEBUG                      LWIP_DBG_OFF
#define SNTP_DEBUG                      LWIP_DBG_OFF
#define SLIP_DEBUG                      LWIP_DBG_OFF
#define PPP_DEBUG                       LWIP_DBG_OFF
#define MEM_DEBUG                       LWIP_DBG_OFF
#define MEMP_DEBUG                      LWIP_DBG_OFF
#define PBUF_DEBUG                      LWIP_DBG_OFF
#define API_LIB_DEBUG                   LWIP_DBG_OFF
#define API_MSG_DEBUG                   LWIP_DBG_OFF
#define TCPIP_DEBUG                     LWIP_DBG_OFF
#define NETIF_DEBUG                     LWIP_DBG_OFF
#define SOCKETS_DEBUG                   LWIP_DBG_OFF
#define DEMO_DEBUG                      LWIP_DBG_OFF
#define IP_DEBUG                        LWIP_DBG_OFF
#define IP_REASS_DEBUG                  LWIP_DBG_OFF
#define RAW_DEBUG                       LWIP_DBG_OFF
#define ICMP_DEBUG                      LWIP_DBG_OFF
#define UDP_DEBUG                       LWIP_DBG_OFF
#define TCP_DEBUG                       LWIP_DBG_OFF
#define TCP_INPUT_DEBUG                 LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG                LWIP_DBG_OFF
#define TCP_RTO_DEBUG                   LWIP_DBG_OFF
#define TCP_CWND_DEBUG                  LWIP_DBG_OFF
#define TCP_WND_DEBUG                   LWIP_DBG_OFF
#define TCP_FR_DEBUG                    LWIP_DBG_OFF
#define TCP_QLEN_DEBUG                  LWIP_DBG_OFF
#define TCP_RST_DEBUG                   LWIP_DBG_OFF
#define DHCP_DEBUG                      LWIP_DBG_OFF
#define EMAC_DEBUG                      LWIP_DBG_OFF

#define DEFAULT_THREAD_PRIO             (tskIDLE_PRIORITY + 1)
#define DEFAULT_THREAD_STACKSIZE        (512)
#define DEFAULT_ACCEPTMBOX_SIZE         6
#define DEFAULT_ACCEPTMBOX_SIZE         6
#define DEFAULT_TCP_RECVMBOX_SIZE       6
#define DEFAULT_UDP_RECVMBOX_SIZE       6

#define TCPIP_THREAD_PRIO               (configMAX_PRIORITIES - 1)

#define TCPIP_THREAD_STACKSIZE          (512)

#define TCPIP_MBOX_SIZE                 6

#define MEMP_MEM_MALLOC                 1
#define MEM_LIBC_MALLOC                 0
// #define mem_clib_free vPortFree
// #define mem_clib_malloc pvPortMalloc
// #define mem_clib_calloc pvPortCalloc
//#define MEMP_OVERFLOW_CHECK         0
//#define MEMP_SANITY_CHECK           0
//#define ARP_QUEUEING                0
//#define ETHARP_TRUST_IP_MAC         0
//#define LWIP_NETIF_HOSTNAME         0
//#define LWIP_NETIF_API              0
//#define LWIP_NETIF_STATUS_CALLBACK  0
//#define LWIP_NETIF_LINK_CALLBACK    0
//#define LWIP_NETIF_HWADDRHINT       0
//#define LWIP_AUTOIP                 0
//#define LWIP_RAW                    0
//#define IP_REASSEMBLY               0
//#define LWIP_NOASSERT               0
#define MEM_SIZE                      0x00040000
extern char lwip_heap[];
#define LWIP_RAM_HEAP_POINTER         lwip_heap

#define LWIP_TIMEVAL_PRIVATE            0
#define LWIP_IPV6                       1

#define LWIP_ARP                        1
#define LWIP_ETHERNET                   0
#include <stdlib.h>
// #include <FreeRTOS.h>

#endif
