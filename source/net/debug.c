#include "lwip/opt.h"

#ifdef LWIP_DEBUG

void assert_printf(char *msg, int line, char *file)
{
	if (msg) {
		LWIP_DEBUGF(LWIP_DBG_ON, ("ASSERT %s:%d in file %s\n", msg, line, file));
	}
	while (1) {}
}

#else

void assert_loop(void)
{
	while (1) {}
}

 #endif
