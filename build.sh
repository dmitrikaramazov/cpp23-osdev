#!/bin/bash
set -euo pipefail

mkdir -p build

# llvm installation path
#LIBCXX_INC="/usr/include/c++/v1"
#LIBCXX_INC="./sysroot/include/c++/v1"
#		-isystem "$LIBCXX_INC" \
#		-nostdinc++ \


COMP_FLAGS=" \
		-std=c++23 --target=i686-elf \
	    -ffreestanding \
		-fno-exceptions \
	    -fno-rtti \
		-I./include \
		-Wall -Wextra \
		-ffunction-sections \
		-fdata-sections \
		-mgeneral-regs-only \
		-DKERNEL_TEST
		"
# mgeneral-regs only allows saving of all regs with interrupts
# using [[gnu::interrupt]]


#		-Wl,--orphan-handling=error \
#		-isystem "$LIBCXX_INC" \
clang --target=i686-elf -c src/boot.s -o build/boot.o
clang++ $COMP_FLAGS	-c src/drivers/vga.cpp -o build/vga.o
clang++ $COMP_FLAGS	-c src/drivers/serial.cpp -o build/serial.o
clang++ $COMP_FLAGS	-c src/exceptions.cpp -o build/exceptions.o
clang++ $COMP_FLAGS	-c src/main.cpp -o build/main.o
clang++ --target=i686-elf build/main.o \
		build/boot.o \
		build/exceptions.o build/vga.o build/serial.o \
		-o build/main.bin \
	    -nostdlib \
		-static \
		-Wl,--gc-sections \
		-Wl,-z,noexecstack \
		-Wall -Wextra \
		-T linker.ld

qemu-system-x86_64 -serial stdio -device isa-debug-exit,iobase=0xf4,iosize=0x04 -kernel build/main.bin
