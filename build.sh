set -euo pipefail

# llvm installation path
#LIBCXX_INC="/usr/include/c++/v1"
LIBCXX_INC="./sysroot/include/c++/v1"


COMP_FLAGS=" \
		-std=c++23 --target=i686-elf \
	    -ffreestanding \
		-fno-exceptions \
	    -fno-rtti \
		-nostdinc++ \
		-isystem "$LIBCXX_INC" \
		-I./include \
		-Wall -Wextra \
		"


#		-isystem "$LIBCXX_INC" \
clang++ $COMP_FLAGS	-c vga.cpp -o vga.o
clang++ $COMP_FLAGS	-c main.cpp -o main.o
clang++ --target=i686-elf main.o vga.o -o main.bin \
	    -nostdlib \
		-static \
		-Wall -Wextra \
		-T linker.ld

qemu-system-x86_64 -kernel main.bin
