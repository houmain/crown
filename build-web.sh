#!/bin/bash
cmake -B build-web -DCMAKE_TOOLCHAIN_FILE=cmake/clang-wajic.cmake
cmake --build build-web -j8

