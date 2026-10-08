# input_di8

`input_di8/` is the DirectInput 8 input driver of the engine, built as `input_di8.dll`. The
executable loads it at run time, looks up the `createIInput(m3d::Kernel*)` export and then talks to
it only through `m3d::input::IInput` (`lib/include/iface.h`).

The code is a restoration of the original input driver (project `input_di8`), taken from a build
in which it was statically linked into the executable, so its behaviour and log strings are kept on
purpose. It is a drop-in replacement for the `input_di8.dll` shipped with Hard Truck Apocalypse:
same vtable, same object layout, same entry point.

## What it does

A worker thread (`CInput_di8::diThread`) owns the DirectInput keyboard, mouse and joystick devices
and waits on their event handles. Keyboard state changes become events in a ring buffer, with
autorepeat of the last key held; mouse deltas and buttons and the joystick axes are accumulated
into a parameter table that the engine reads once per frame through `NewFrame` / `GetParam`.
`GetLastKbdEvent` translates a scan code through the configured base or additional keyboard layout
(`input_baseKeyboardLayot`, `input_additionalKeyboardLayot`) and the `ui_codePageName` code page.

## Layout

| file | contents |
|---|---|
| `input_di8.cpp` | the whole driver, as in the original: `AutoLockMutex`, `CInput_di8`, the DirectInput callbacks, `m3d::InputFactory` |
| `main.cpp` | the `createIInput` export, the log callback, `operator new/delete` over the kernel's allocator, the DLL's `g_kernel` and the engine's `m3d::g_Kernel` |
| `log.h` | `LogMsg`, the log callback the executable passes to `Init` |

## Building

The target is `input_di8` in the root CMake project; `retruxx` depends on it, so a normal build of
the executable also produces `build/bin/<Config>/input_di8.dll` next to it. It needs the DirectX
SDK (June 2010) for `dinput.h`, `dinput8.lib` and `dxguid.lib`, found through `DXSDK_DIR` or the
default install path, and a 32-bit build like the rest of the project. The engine value types it
uses (`CStr`, `CVar`, `Timer`, the math classes) are compiled into the DLL from `lib/engine`.

## Source conventions

The same as `dxrender9/README.md`: `// orig 0x<rva> <file>:<line>` before each restored definition,
`// <n>` for the original line of a statement, original strings byte for byte, deviations marked
`retruxx adaptation` (there are none in this module).

## Interface: Hard Truck Apocalypse's vtable vs. the original

| original | HTA (`iface.h`) |
|---|---|
| `Init()` with no parameters; the driver reached the kernel, the log and the asserts directly | `Init(Kernel*, logFunc)`; the DLL stores both, as HTA's own `input_di8.dll` does, then runs the original body. The original's `M3D_LOG_INFO` / `M3D_LOG_ERR` calls go through the callback (the executable logs them as `input: ...` at its own level); `M3D_ASSERT` reaches the kernel's `SysError(whence, descr)` with the original assertion text, file and line |
| `IsKeyDown(int)` is a virtual of `IInput` | not in HTA's interface; a plain member here |
| `GetLastKbdEvent(..., double& time, ...)` | the same; the event time is a `double` |
| `JoystickAttached()` is the class's own virtual after the interface | the same, slot 18 in HTA's DLL |

Everything else is identical, including the object size (0x2a8) and the field offsets, which
`static_assert`s in `input_di8.cpp` pin down.

Checked against HTA's own `input_di8.dll` (no symbols, disassembled): vtable, object layout,
function sizes, every DirectInput constant (buffer sizes, cooperative levels, axis range, error
codes, autorepeat clamp, keyboard layout flags) and the scan-code table all match, with one
difference: HTA's DLL has no entry for `DIK_SYSRQ`, which the original maps to `0xfe`. The port
keeps the original's entry.

## Open items

- `GetCodePage` reads the `ui_codePageName` cvar, which this engine configures; the original's
  behaviour for an empty name (fall back to the ANSI code page) is kept.
- The worker thread's shutdown sequence is reached from three places in the original; the port
  uses a label for that, which the original may have written differently.
