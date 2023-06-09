cmake_minimum_required (VERSION 3.7)
set(CMAKE_TOOLCHAIN_FILE "Toolchain.cmake")
SET(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project (app)

add_compile_definitions(XIP_EXTERNAL_FLASH=1)
add_compile_definitions(SKIP_SYSCLK_INIT=1)

SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -DDEBUG")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -g")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -D__STARTUP_CLEAR_BSS")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -D__STARTUP_INITIALIZE_NONCACHEDATA")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=cortex-m7")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wall")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mfloat-abi=hard")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mfpu=fpv5-d16")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mthumb")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -fno-common")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -ffunction-sections")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -fdata-sections")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -ffreestanding")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -fno-builtin")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mapcs")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -std=gnu99")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCPU_MIMXRT1062DVL6A")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFSL_RTOS_FREE_RTOS")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m7")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=fpv5-d16")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -MMD")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -MP")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-common")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mapcs")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCPU_MIMXRT1062DVL6A")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFSL_RTOS_FREE_RTOS")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-m7")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=hard")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=fpv5-d16")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthumb")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MMD")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdata-sections")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffreestanding")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mapcs")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu99")

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mcpu=cortex-m7")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wall")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mfloat-abi=hard")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mfpu=fpv5-d16")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nano.specs")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nosys.specs")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-common")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ffunction-sections")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fdata-sections")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ffreestanding")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-builtin")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mthumb")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mapcs")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --gc-sections")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -z")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} muldefs")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Map=output.map")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _printf_float")

set(LINKER_SCRIPT "system/device.ld")

# -Xlinker --wrap=malloc -Xlinker --wrap=free
set (CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> ${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT} <OBJECTS> <LINK_LIBRARIES> -lm -o <TARGET>")
set (CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> ${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT} <OBJECTS> <LINK_LIBRARIES> -lm -o <TARGET>")

include_directories(
  ./include
  ./include/net/lwip
  ./include/net/emac
  ./include/net
  ./include/libiec61850
  ./board
  ./system/components/lan8720
  ./include/FreeRTOS
  ./modules/FreeRTOS/portable/GCC/ARM_CM4F
)

include(device.cmake)
include(FreeRTOS.cmake)
include(lwIP.cmake)
include(lib_iec61850.cmake)

add_executable(${PROJECT_NAME}.elf
  ./board/clock_config.c
  ./board/peripherals.c
  ./board/pin_mux.c
  ./board/board.c

  ./system/startup.S
  ./system/syscalls.c
  ./system/handlers.c
  ./system/components/lan8720/fsl_phy.c

  #./source/FreeRTOS/freertos_heap.c
  ./source/FreeRTOS/freertos_hooks.c
  ./modules/FreeRTOS/portable/GCC/ARM_CM4F/port.c
  ./modules/FreeRTOS/portable/MemMang/heap_4.c

  ./source/net/rb_tree.c
  ./source/net/emac/rx.c
  ./source/net/emac/tx.c
  ./source/net/emac/input.c
  ./source/net/emac/emac.c
  ./source/net/interface.c
  ./source/net/netif/ethernet.c
  ./source/net/sys_arch_freertos.c
  ./source/net/debug.c

  ./source/libiec61850/hal/time.c
  ./source/libiec61850/hal/lib_memory.c
  ./source/libiec61850/hal/socket.c
  ./source/libiec61850/hal/thread.c
  #  ./source/libiec61850/hal/ethernet.c
  #  ./source/libiec61850/hal/tls.c
  #  ./source/libiec61850/hal/file_provider.c

  ./source/example3/server.c
  ./source/example3/static_model.c

  ./source/app.c
  ./source/enet.c
  ./source/temp.c
  ./source/heartbeat.c
  ./source/swo.c
  ./source/main.c
)

set_property(SOURCE ./system/startup.S PROPERTY LANGUAGE C)

target_link_libraries(${PROJECT_NAME}.elf
  device
  FreeRTOS
  lwIP
  iec61850_client
  iec61850_server
)

target_include_directories(${PROJECT_NAME}.elf PUBLIC
)

set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT})

add_custom_command(TARGET ${PROJECT_NAME}.elf
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
)
