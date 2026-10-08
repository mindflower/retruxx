Reverse engineered Hard Truck Apocalypse

## Building

The project builds on Windows only. The main target is **64-bit (x64)**. A **32-bit (x86)** build is still supported, and it is the configuration that matches the original game's memory layouts.

The renderer, input and sound drivers are separate DLLs that the game loads at run time. They are restored from source in `dxrender9/`, `input_di8/` and `sound/` and built along with the game, with the same bitness.

### Requirements

- Visual Studio Build Tools (or Visual Studio) with the **Desktop development with C++** workload, which provides MSVC, the Windows SDK, CMake and Ninja. The project is currently built with Visual Studio 2026 (MSVC 14.50).
- CMake 3.13 or newer, if you don't use the copy bundled with Visual Studio.
- Git.
- The **DirectX SDK (June 2010)**, for `dxrender9` (d3dx9) and `input_di8` (DirectInput 8). CMake reads its location from the `DXSDK_DIR` environment variable, which the SDK installer sets. Otherwise it looks in `C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)`.
- The **FMOD Core SDK** (part of the FMOD Studio API for Windows), for `sound`. Set `FMOD_SDK_DIR` to its `api/core` folder. Otherwise CMake looks in `C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows\api\core`.

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

The bitness of the build is the bitness of the compiler environment, so pick the matching prompt.

**64-bit (default):** open an **x64 Native Tools Command Prompt for VS**, or run `vcvarsall.bat x64` in a normal command prompt. From the repository root:

```
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

**32-bit:** open an **x86 Native Tools Command Prompt for VS**, or run `vcvarsall.bat x86`. Use a separate build directory, because a CMake build tree is tied to the compiler it was configured with:

```
cmake -S . -B build_x86 -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build_x86
```

The build writes these files to `bin/` in the build directory:

- `retruxx.exe`: the game
- `dxrender9.dll`, `input_di8.dll`, `sound.dll`: the renderer, input and sound drivers
- `fmod.dll`: the FMOD runtime, copied from the SDK
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

You need an installed copy of Ex Machina / Hard Truck Apocalypse for the game data. Start `retruxx.exe` with the game's install directory as the working directory, and put the driver DLLs and `fmod.dll` from the same build next to it.

The executable and the drivers must have the same bitness. A 64-bit `retruxx.exe` needs the 64-bit DLLs from its own build: it cannot load the 32-bit DLLs shipped with the game. A 32-bit build can use either its own DLLs or the original ones from the game.
