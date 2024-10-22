# The Crown And The Bombs
A work in progress Jump'n'Run game.

## Preview
Try out the [latest version](https://houmain.github.io/crown) online.

It is still just a tech demo. Use cursor keys to run, CTRL to hammer, R to regenerate level.

## Dependencies

A C++17 conforming compiler is required. A scripts for the [CMake](https://cmake.org) build systems is provided.

## Building 

**Checking out the source:**

```
git clone --recurse-submodules https://github.com/houmain/crown
```

**Building for your desktop platform:**

```
cd crown
cmake -B build
cmake --build build
```

**Building for the web:**

The web version is powerd by [WAjic](https://github.com/schellingb/wajic). Only [LLVM](https://releases.llvm.org/download.html) and [nodejs](https://nodejs.org) need to be installed (no Emscripten).

```
cd crown
cmake -B build-web -DCMAKE_TOOLCHAIN_FILE=cmake/clang-wajic.cmake
cmake --build build-web
```

