add_library(FreeRTOS STATIC
  ./modules/FreeRTOS/croutine.c
  ./modules/FreeRTOS/event_groups.c
  ./modules/FreeRTOS/list.c
  ./modules/FreeRTOS/queue.c
  #  ./modules/freertos/stream_buffer.c
  ./modules/FreeRTOS/tasks.c
  ./modules/FreeRTOS/timers.c
)

target_include_directories(FreeRTOS PUBLIC
  ./modules/FreeRTOS/include
)

