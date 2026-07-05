#!/bin/bash
set -euo pipefail

echo "[*] Cloning LLVM Project (shallow clone for speed)..."
# We put this in /tmp so it doesn't clutter your project folder
if [ ! -d "/tmp/llvm-project" ]; then
    git clone --depth 1 https://github.com/llvm/llvm-project.git /tmp/llvm-project
fi

mkdir -p /tmp/llvm-project/build
cd /tmp/llvm-project/build

# The destination for your new standard library!
SYSROOT_DIR="/home/lodestone/projects/agy/tmp/sysroot"

echo "[*] Configuring libc++ for baremetal i686-elf..."
cmake -G Ninja ../runtimes \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_C_COMPILER_TARGET=i686-elf \
  -DCMAKE_CXX_COMPILER_TARGET=i686-elf \
  -DCMAKE_SYSTEM_NAME=Generic \
  -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
  -DLIBCXX_ENABLE_EXCEPTIONS=OFF \
  -DLIBCXX_ENABLE_RTTI=OFF \
  -DLIBCXX_ENABLE_THREADS=OFF \
  -DLIBCXX_ENABLE_FILESYSTEM=OFF \
  -DLIBCXX_ENABLE_LOCALIZATION=OFF \
  -DLIBCXX_ENABLE_RANDOM_DEVICE=OFF \
  -DLIBCXX_ENABLE_WIDE_CHARACTERS=OFF \
  -DLIBCXX_ENABLE_MONOTONIC_CLOCK=OFF \
  -DLIBCXXABI_ENABLE_EXCEPTIONS=OFF \
  -DLIBCXXABI_ENABLE_THREADS=OFF \
  -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi" \
  -DCMAKE_INSTALL_PREFIX="$SYSROOT_DIR"

echo "[*] Building and installing sysroot..."
ninja install-cxx install-cxxabi

echo "[+] Success! Your baremetal sysroot is ready at $SYSROOT_DIR"
