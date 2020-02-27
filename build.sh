#!/bin/bash
mkdir build_cmake/unix
cd build_cmake/unix
CC=clang CXX=clang++ CMAKE_PREFIX_PATH=/usr/lib/llvm-9/ cmake -DCMAKE_BUILD_TYPE=MinSizeRel ../..
make -j8 LiteCore
