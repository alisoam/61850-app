#ifndef LIBIEC61850_HAL_DEBUG_H
# define LIBIEC61850_HAL_DEBUG_H

# include <stdio.h>

# include "stack_config.h"

# ifndef IEC61850_DEFAULT_DEBUG_DOMAIN
#   define IEC61850_DEFAULT_DEBUG_DOMAIN "IEC61850 "
# endif

# ifndef DEBUG_SOCKET
#   define DEBUG_SOCKET 0
# endif

# if DEBUG
#   ifdef IEC61850_DEBUG_DOMAIN
#     define IEC61850_DEBUGF(fmt, ...) printf(IEC61850_DEBUG_DOMAIN"DEBUG: "fmt, ##__VA_ARGS__)
#   else
#     define IEC61850_DEBUGF(fmt, ...) printf(IEC61850_DEFAULT_DEBUG_DOMAIN"DEBUG: "fmt, ##__VA_ARGS__)
#   endif
# else
#   define IEC61850_DEBUGF(...)
# endif

#endif
