#!/bin/bash
set -euo pipefail

LLVM_SRC_DIR="/tmp/llvm-project"
SYSROOT_DIR="$(pwd)/sysroot"

BUILD_DIR="${LLVM_SRC_DIR}/build_baremetal"

if [ ! -d "$LLVM_SRC_DIR" ]; then

    echo "Cloning LLVM 18..."
    git clone --depth 1 -b release/18.x https://github.com/llvm/llvm-project.git "$LLVM_SRC_DIR"
fi

echo "Configuring baremetal libc++ headers..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake -G Ninja ../runtimes -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER_TARGET=i686-elf -DCMAKE_CXX_COMPILER_TARGET=i686-elf -DCMAKE_SYSTEM_NAME=Generic -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY -DCMAKE_C_FLAGS="-ffreestanding" -DCMAKE_CXX_FLAGS="-ffreestanding" -DCMAKE_INSTALL_PREFIX="$SYSROOT_DIR" -DLLVM_ENABLE_RUNTIMES="libcxx" -DLIBCXX_ENABLE_SHARED=OFF -DLIBCXX_ENABLE_STATIC=OFF -DLIBCXX_ENABLE_THREADS=OFF -DLIBCXX_ENABLE_MONOTONIC_CLOCK=OFF -DLIBCXX_ENABLE_FILESYSTEM=OFF -DLIBCXX_ENABLE_LOCALIZATION=OFF -DLIBCXX_ENABLE_RANDOM_DEVICE=OFF -DLIBCXX_ENABLE_EXCEPTIONS=OFF -DLIBCXX_ENABLE_RTTI=OFF -DLIBCXX_ENABLE_WIDE_CHARACTERS=OFF -DLIBCXX_CXX_ABI=none

echo "Installing freestanding headers..."
ninja install-cxx-headers

echo "Done! The freestanding headers are ready at $SYSROOT_DIR/include/c++/v1"
