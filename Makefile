default: all
all: build
.PHONY: modules patch-lwip patch-libiec61850 cmake

init: modules | patch

modules:
	git submodule update --init --depth 1 --recommend-shallow --single-branch

patch: patch-lwip patch-libiec61850

patch-lwip:
	git -C modules/lwip apply ../../patch/lwip.patch

patch-libiec61850:
	git -C modules/libiec61850 apply ../../patch/libiec61850.patch

cmake:
	mkdir -p build/
	cmake -S . -B build

build: cmake
	make -C build

clean: 
	make -C build clean

program: build
	./tools/program.sh

gdb-srv: program
	./tools/debug.sh

gdb-swo:
	while true; do echo "connecting..."; nc localhost 2332; sleep 1; done
