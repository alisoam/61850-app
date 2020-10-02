/*
 *  thread_linux.c
 *
 *  Copyright 2013 Michael Zillgith
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

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define IEC61850_DEBUG_DOMAIN IEC61850_DEFAULT_DEBUG_DOMAIN"THREAD "

#include "hal_debug.h"

#if DEBUG_HAL_THREAD
# define DEBUGF IEC61850_DEBUGF
#else
# define DEBUGF(...)
#endif

#include "hal_thread.h"
#include "lib_memory.h"

#define  SEM_VALUE_MAX    6500
#define TASK_STACK_SIZE   512
#define TASK_PRIORITY     tskIDLE_PRIORITY + 1

struct sThread {
  ThreadExecutionFunction function;
  void* parameter;
  TaskHandle_t thread;
  SemaphoreHandle_t sem;
  int state;
  bool autodestroy;
};

Semaphore
Semaphore_create(int initialValue)
{
  Semaphore sem = xSemaphoreCreateCounting(SEM_VALUE_MAX, initialValue);
  DEBUGF("Semaphore_create(%d) -> 0x%x\n", initialValue, sem);
  return sem;
}

void
Semaphore_wait(Semaphore self)
{
  while (xSemaphoreTake(self, portMAX_DELAY) != pdTRUE)
    ;
}

void
Semaphore_post(Semaphore self)
{
  xSemaphoreGive(self);
}

void
Semaphore_destroy(Semaphore self)
{
  DEBUGF("Semaphore_destroy(0x%x)\n", self);
  vSemaphoreDelete(self);
}

static void Free_sthread(struct sThread* thread)
{
  DEBUGF("Free_sthread(0x%x)\n", thread);
  if (thread != NULL)
  {
    TaskHandle_t xHandle = thread->thread;
    if (NULL != thread->sem)
      vSemaphoreDelete(thread->sem);
    GLOBAL_FREEMEM(thread);
    DEBUGF("Deleting Thread (0x%x)\n", xHandle);
    vTaskDelete(xHandle);
  }
}

static void
Thread_function(void* arg)
{
  DEBUGF("Thread_function(%x)\n", arg);
  struct sThread* thread = (struct sThread*)arg;

  while (xSemaphoreTake(thread->sem, portMAX_DELAY) != pdTRUE)
    ;
  thread->state = 1;

  DEBUGF("Thread_function call (0x%x)(0x%x)\n", thread->function, thread->parameter);
  if (NULL != thread->function)
    thread->function(thread->parameter);
  DEBUGF("Thread_function finished (0x%x)(0x%x)\n", thread->function, thread->parameter);

  if (thread->autodestroy)
    Free_sthread(thread);
  else
    xSemaphoreGive(thread->sem);

  while (true)
    vTaskDelay(configTICK_RATE_HZ * 100);
}

Thread
Thread_create(ThreadExecutionFunction function, void* parameter, bool autodestroy)
{
  DEBUGF("Thread_create(0x%x, 0x%x, %s)\n", function, parameter, (autodestroy? "true": "false"));
  SemaphoreHandle_t sem = xSemaphoreCreateBinary();
  if (NULL == sem)
    goto exit1;

  Thread thread = (Thread) GLOBAL_MALLOC(sizeof(struct sThread));
  if (thread != NULL) {
    thread->parameter = parameter;
    thread->function = function;
    thread->state = 0;
    thread->autodestroy = autodestroy;
    thread->thread = NULL;
    thread->sem = sem;
  }
  else
    goto exit2;

  TaskHandle_t xHandle = NULL;
  if (xTaskCreate(Thread_function, "61850T", 2*1024, (void*)thread, tskIDLE_PRIORITY, &xHandle) != pdPASS)
    goto exit3;
  thread->thread = xHandle;
  DEBUGF("xTaskCreate(0x%x, \"61850T\", 1024, 0x%x, tskIDLE_PRIORITY, &(0x%x)) -> pdPASS\n", function, thread, xHandle);

  DEBUGF("Thread_create(0x%x, 0x%x, %s) -> 0x%x\n", function, parameter, (autodestroy? "true": "false"), thread);
  return thread;

exit3:
  GLOBAL_FREEMEM(thread);
exit2:
  vSemaphoreDelete(sem);
exit1:
  return NULL;
}


void
Thread_start(Thread thread)
{
  DEBUGF("Thread_start(0x%x)\n", thread);
  xSemaphoreGive(thread->sem);
}

void
Thread_destroy(Thread thread)
{
  DEBUGF("Thread_destroy(0x%x)\n", thread);

  while (thread->state == 0)
    vTaskDelay(500 / portTICK_PERIOD_MS);

  while (xSemaphoreTake(thread->sem, portMAX_DELAY) != pdTRUE)
    ;

  Free_sthread(thread);
}

void
Thread_sleep(int millies)
{
  TickType_t t = millies / portTICK_PERIOD_MS;
  if (t == 0)
    t = 1;
  vTaskDelay(t);
}
