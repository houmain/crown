# The Crown And The Bombs
A work in progress Jump'n'Run game.

## Preview
Try out the [latest version](https://houmain.github.io/crown) online.

It is still just a tech demo. Use cursor keys to run, CTRL to hammer, R to regenerate level.

## Dependencies

A C++17 conforming compiler is required. Scripts for the
[xmake](https://xmake.io) and [CMake](https://cmake.org) build systems are provided.

The web version is powerd by [WAjic](https://github.com/schellingb/wajic). Only [LLVM](https://releases.llvm.org/download.html) and [nodejs](https://nodejs.org) need to be installed (no Emscripten).

## Building

### For your desktop platform:
  ```xmake```

### For your web browser:

  ```xmake f -p web && xmake```
