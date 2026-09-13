Reverse engineered Hard Truck Apocalypse

## Building

The project builds on Windows only, and only as a **32-bit (x86)** binary. The game loads the original 32-bit renderer, input and sound DLLs at runtime.

### Requirements

- Visual Studio Build Tools (or Visual Studio) with the **Desktop development with C++** workload, which provides MSVC, the Windows SDK, CMake and Ninja. The project is currently built with Visual Studio 2026 (MSVC 14.50).
- CMake 3.13 or newer, if you don't use the copy bundled with Visual Studio.
- Git.

### Get the sources

GoogleTest is a submodule, so clone with `--recursive`:

```
git clone --recursive <repository-url> retruxx
```

In an existing clone, run:

```
git submodule update --init
```

### Configure and build

Open an **x86 Native Tools Command Prompt for VS**, or run `vcvarsall.bat x86` in a normal command prompt. The environment must be x86. An x64 prompt produces a 64-bit build that doesn't match the original layouts. From the repository root:

```
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

The build writes these files to `build/bin/`:

- `retruxx.exe`: the game
- `retruxx_tests.exe`: unit tests

### Running the tests

```
cd build\bin
retruxx_tests.exe
```

To run one test suite:

```
retruxx_tests.exe --gtest_filter=CMatrixGetYPRTest.*
```

### Running the game

`retruxx.exe` is only the game executable. You need an installed copy of Ex Machina / Hard Truck Apocalypse for the game data and the driver DLLs (for example `dxrender9.dll`). Start `retruxx.exe` with the game's install directory as the working directory.

Much of the game is not reimplemented yet. Code that reaches an unimplemented function stops in the debugger and exits, so run it under a debugger.
