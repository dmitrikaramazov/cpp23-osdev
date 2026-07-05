set -euo pipefail

# llvm installation path
LIBCXX_INC="/usr/include/c++/v1"

clang++ -std=c++23 --target=i686-elf \
	    -ffreestanding \
		-fno-exceptions \
	    -fno-rtti \
		-nostdinc++ \
		-isystem "$LIBCXX_INC" \
		-Wall -Wextra \
		-c test.cpp -o test.o
clang++ --target=i686-elf test.o -o test.bin \
	    -nostdlib \
		-static \
		-Wall -Wextra \
		-T linker.ld

qemu-system-x86_64 -kernel test.bin
