file build/com.elf
target remote localhost:2331

monitor SWO DisableTarget 1
monitor reset
monitor flash download = 1
monitor flash breakpoints = 1

load

monitor reg r13 = (0x00000000)
monitor reg pc = (0x00000004)

break main
continue

monitor SWO EnableTarget 6000000 64000 1 0
