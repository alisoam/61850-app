#include <malloc.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "fsl_common.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

static SemaphoreHandle_t mallocMutex = NULL;
static SemaphoreHandle_t sbrkMutex = NULL;

__attribute__((constructor(101))) static void __syscalls_system_init()
{
  mallocMutex = xSemaphoreCreateRecursiveMutex();
  configASSERT(mallocMutex);
  sbrkMutex = xSemaphoreCreateMutex();
  configASSERT(sbrkMutex);
}

void __malloc_lock(struct _reent *reent)
{
  if (taskSCHEDULER_NOT_STARTED != xTaskGetSchedulerState())
    for (volatile unsigned int i; xSemaphoreTakeRecursive(mallocMutex, portMAX_DELAY) != pdTRUE;)
      ;
}

void __malloc_unlock (struct _reent *reent)
{
  if (taskSCHEDULER_NOT_STARTED != xTaskGetSchedulerState())
    xSemaphoreGiveRecursive(mallocMutex);
}

__attribute__ ((used)) void __env_lock()    { vTaskSuspendAll(); }
__attribute__ ((used)) void __env_unlock()  { (void)xTaskResumeAll(); }

int _close(int file) { return -1; }

int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file) { return 1; }

int _lseek(int file, int ptr, int dir) { return 0; }

int _open(const char *name, int flags, int mode) { return -1; }

int _read(int file, char *ptr, int len) {
  if (len > 0)
  {

/*    while (!MY_SOC_UART->SR)
      ;
    *ptr = (char)(MY_SOC_UART->DR);*/
    return 1;
  }
  return 0;
}

int _write(int file, char *ptr, int len) {
  for (size_t i = 0; i < len; i++)
  {
    ITM_SendChar(*(ptr+i));
  }
  return len;
}

void _exit(int status)
{
  for (volatile unsigned int i;;)
    ;
}

int _getpid()
{
  return 0;
}

void _kill(int pid)
{}
