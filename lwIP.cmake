add_library(lwIP STATIC
    ./modules/lwip/src/core/def.c
    ./modules/lwip/src/core/dns.c
    ./modules/lwip/src/core/inet_chksum.c
    ./modules/lwip/src/core/init.c
    ./modules/lwip/src/core/ip.c
    ./modules/lwip/src/core/mem.c
    ./modules/lwip/src/core/memp.c
    ./modules/lwip/src/core/netif.c
    ./modules/lwip/src/core/pbuf.c
    ./modules/lwip/src/core/raw.c
    ./modules/lwip/src/core/stats.c
    ./modules/lwip/src/core/sys.c
    ./modules/lwip/src/core/tcp.c
    ./modules/lwip/src/core/tcp_in.c
    ./modules/lwip/src/core/tcp_out.c
    ./modules/lwip/src/core/timeouts.c
    ./modules/lwip/src/core/udp.c
    ./modules/lwip/src/core/ipv4/autoip.c
    ./modules/lwip/src/core/ipv4/dhcp.c
    ./modules/lwip/src/core/ipv4/etharp.c
    ./modules/lwip/src/core/ipv4/icmp.c
    ./modules/lwip/src/core/ipv4/igmp.c
    ./modules/lwip/src/core/ipv4/ip4.c
    ./modules/lwip/src/core/ipv4/ip4_addr.c
    ./modules/lwip/src/core/ipv4/ip4_frag.c
    ./modules/lwip/src/core/ipv6/dhcp6.c
    ./modules/lwip/src/core/ipv6/ethip6.c
    ./modules/lwip/src/core/ipv6/icmp6.c
    ./modules/lwip/src/core/ipv6/inet6.c
    ./modules/lwip/src/core/ipv6/ip6_addr.c
    ./modules/lwip/src/core/ipv6/ip6_frag.c
    ./modules/lwip/src/core/ipv6/ip6.c
    ./modules/lwip/src/core/ipv6/mld6.c
    ./modules/lwip/src/core/ipv6/nd6.c
    ./modules/lwip/src/netif/ethernet.c
    ./modules/lwip/src/api/api_lib.c

    ./modules/lwip/src/api/api_msg.c
    ./modules/lwip/src/api/err.c
    ./modules/lwip/src/api/netbuf.c
    ./modules/lwip/src/api/netdb.c
    ./modules/lwip/src/api/netifapi.c
    ./modules/lwip/src/api/sockets.c
    ./modules/lwip/src/api/tcpip.c

    ./modules/lwip/src/apps/lwiperf/lwiperf.c
)

target_include_directories(lwIP PUBLIC
    ./modules/lwip/src/include
    ./modules/lwip/src/include/ipv4
    ./modules/lwip/src/include/ipv6
    ./modules/lwip/src/include/compat/posix
)

target_link_libraries(lwIP
  FreeRTOS
)
