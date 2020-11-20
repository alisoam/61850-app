file build/com.elf
target remote localhost:2331

#monitor flash erase 
#monitor flash download = 1
monitor flash breakpoints = 1

define reset
	monitor SWO DisableTarget 1
	monitor reset

	load

#	monitor reg r13 = (0x00000000)
#	monitor reg pc = (0x00000004)
	break main
	continue
	continue

	monitor SWO EnableTarget 6000000 64000 1 0
end

reset
