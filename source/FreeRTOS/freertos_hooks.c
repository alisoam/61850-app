#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board.h"

#if defined(configAPPLICATION_ALLOCATED_HEAP) && configAPPLICATION_ALLOCATED_HEAP == 1
  uint8_t ucHeap[configTOTAL_HEAP_SIZE];
#endif

#if defined(configSUPPORT_STATIC_ALLOCATION) && configSUPPORT_STATIC_ALLOCATION == 1
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
  static StaticTask_t xIdleTaskTCB;
  static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
  static StaticTask_t xTimerTaskTCB;
  static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif

void FreeRTOSDelay(uint32_t ms)
{
  vTaskDelay(ms);
}

void vApplicationMallocFailedHook()
{
  puts("Malloc Failed!\n");
  taskDISABLE_INTERRUPTS();
  for (;; )
    ;
}

void vApplicationIdleHook()
{
  if (xTaskGetTickCount() % 600 < 4)
    boardLedSet(BOARD_LED1, 1);

  if (xTaskGetTickCount() % 600 >= 4)
    boardLedSet(BOARD_LED1, 0);
//  __WFI();
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, signed char *pcTaskName)
{
  (void) pxTask;
  (void) pcTaskName;

  for (;; )
    ;
}

void vApplicationTickHook()
{
}
