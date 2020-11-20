/*
 *  lib_memory.c
 *
 *  Copyright 2014-2018 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

#include <stdlib.h>
#include "lib_memory.h"

#define IEC61850_DEBUG_DOMAIN IEC61850_DEFAULT_DEBUG_DOMAIN"MEMORY "

#include "hal_debug.h"

#if DEBUG_HAL_MEMORY
# define DEBUGF IEC61850_DEBUGF
#else
# define DEBUGF(...)
#endif

// void *pvPortMalloc( size_t xWantedSize );
// void *pvPortReAlloc(void* ptr, size_t xNewSize);
// extern void *pvPortCalloc(size_t xNItemxs, size_t xWantedSize);
// void vPortFree( void *pv );

static MemoryExceptionHandler exceptionHandler = NULL;
static void* exceptionHandlerParameter = NULL;

static void
noMemoryAvailableHandler(void)
{
    if (exceptionHandler != NULL)
        exceptionHandler(exceptionHandlerParameter);
}

void
Memory_installExceptionHandler(MemoryExceptionHandler handler, void* parameter)
{
    exceptionHandler = handler;
    exceptionHandlerParameter = parameter;
}

void*
Memory_malloc(size_t size)
{
    DEBUGF("Memory_malloc(%u)\n", size);
    void* memory = malloc(size);

    if (memory == NULL)
        noMemoryAvailableHandler();

    DEBUGF("Memory_malloc(%u) -> 0x%x\n", size, memory);
    return memory;
}


void*
Memory_calloc(size_t nmemb, size_t size)
{
    DEBUGF("Memory_calloc(%u, %u)\n", nmemb, size);
    void* memory = calloc(nmemb, size);

    if (memory == NULL)
        noMemoryAvailableHandler();

    DEBUGF("Memory_calloc(%u, %u) -> 0x%x\n", nmemb, size, memory);
    return memory;
}


void *
Memory_realloc(void *ptr, size_t size)
{
    DEBUGF("Memory_realloc(0x%x, %u)\n", ptr, size);
    void* memory = realloc(ptr, size);

    if (memory == NULL)
        noMemoryAvailableHandler();

    DEBUGF("Memory_realloc(0x%x, %u) -> 0x%x\n", ptr, size, memory);
    return memory;
}

void
Memory_free(void* memb)
{
    DEBUGF("Memory_free(0x%x)\n", memb);
    free(memb);
}

