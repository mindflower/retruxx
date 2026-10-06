# dxrender9

`dxrender9/` is the Direct3D 9 renderer of the engine, built as `dxrender9.dll`. The executable
loads it at run time, looks up the `createIRenderer(m3d::Kernel*)` export and then talks to it only
through `m3d::rend::IRenderer` (`lib/include/renderer/i_renderer.h`).

The code is a restoration of the original renderer (project `dxrender9`), taken from a build in
which it was statically linked into the executable, so the behaviour, log strings and quirks of
the original are kept on purpose. It is a drop-in replacement for the `dxrender9.dll` shipped with
Hard Truck Apocalypse: it implements that game's vtable layout of `IRenderer`, so both `hta.exe` and
`retruxx.exe` can load it.

## Origin

| what | where |
|---|---|
| Reference | the original renderer, statically linked into an executable with full symbols and line information |
| Target interface | `lib/include/renderer/i_renderer.h`: Hard Truck Apocalypse's `IRenderer`, 345 virtual slots, declared under the original method names |
| Original-only interface pieces the engine headers lack | `i_renderer_orig.h` |

The original source layout is kept: one `.cpp` per original compiland, the same file names, the
original global-namespace class names (`CDevice`, `EffectImpl`, `HlslShaderImpl`, `AsmShaderImpl`,
`Query`, `StateManager`, `nShaderArg`, `nShaderParams`, `auxShaderInclude`), and inside each file
the functions in the order of their original source lines.

## Layout

| file | contents |
|---|---|
| `device.h` | `CDevice`, the `IRenderer` implementation, and the internal texture / buffer / handle types |
| `device.cpp` | device creation and reset, caps, render and texture stage state, fixed-function setup, cursor, resource reports |
| `matrices.cpp`, `lights.cpp`, `clipplanes.cpp` | matrix stacks and texture-matrix generation, lights, clip planes |
| `drawcalls.cpp`, `fsrt.cpp`, `rendertarget.cpp` | draw calls, the full-screen quad, render-to-texture |
| `texture_man*.cpp`, `vb_man*.cpp`, `ib_man*.cpp` | texture, vertex buffer and index buffer managers and their statistics |
| `mesh.cpp`, `query.cpp` / `query.h`, `screenshot.cpp` | meshes, D3D queries, TGA writers and screenshots |
| `shaders/shaders.cpp`, `shaders/shader_include.cpp`, `shaders/nshaderarg.h` | shader bookkeeping, the `#include` handler, shader argument packing |
| `shaders/hlsl/`, `shaders/assembly/`, `shaders/effects/` | HLSL shaders, assembly shaders, D3DX effects with their state manager and parameter name table |
| `shaders/shader_compat.h` | compiling the original shaders with a current D3DX (see Adaptations) |

The glue that the original got from being linked into the executable:

| file | contents |
|---|---|
| `main.cpp` | the `createIRenderer` export, the log callback, `operator new/delete` over the kernel's allocator (`g_mar`, so objects can be freed on either side), the DLL's `g_kernel` and the engine's `m3d::g_Kernel` |
| `i_renderer_impl.cpp` | the inline members of the original interface headers that the engine's headers only declare (`IRenderResource`, `QueryReturnValue`, `ShaderMacro`, interface destructors) |
| `engine_helpers.cpp` | `m3d::ReadXmlFile`, `SafeStrAttrib`, `SafeClrAttrib`, `Tokenize`, copied from `lib/engine/core/ini.cpp`, which cannot be compiled into the DLL because it carries the TinyXML-based XML implementation |
| `log.h` | `LogMsg`, the log callback the executable passes to `Create` |

## Building

The target is `dxrender9` in the root CMake project; `retruxx` depends on it, so a normal build of
the executable also produces `build/bin/<Config>/dxrender9.dll` next to it.

Requirements:

- The DirectX SDK (June 2010). CMake reads `DXSDK_DIR` from the environment and falls back to the
  default install path. Its headers are added after the Windows SDK so the Windows SDK wins where
  both declare something.
- A 32-bit (`win32`) build, like the rest of the project.

The DLL links `d3d9`, `d3dx9` (d3dx9_43), `dxguid` and `winmm`. It stays independent of
`retruxx_lib`: the engine value types it shares with the executable (`CStr`, the math classes,
`CVar`, `Timer`, the renderer colour and vertex helpers) are compiled into it from
`lib/engine` and `lib/include/renderer`.

At run time the DLL needs `d3dx9_43.dll` and, for correct shaders, `d3dx9_31.dll`. Both come with
the DirectX end-user runtime the game installs.

## Source conventions

- **The original binary is the specification.** Each restored definition is preceded by
  `// orig 0x<rva> <file>:<line>`: the function's RVA in the original binary and its original
  source location. Inside a function, `// <n>` comments give the original line of the statement
  that follows.
- Log and message strings are byte for byte the originals. Identifiers are the original ones.
- No behaviour was "improved". Where the port had to deviate from the original the code is marked
  `retruxx adaptation` and the reason is in the comment; the complete list is below.
- Entry points that exist in Hard Truck Apocalypse's vtable but had no body in the original are
  marked `// HTA-only wrapper, no original body` and forward to the original overload with the
  obvious default. They are `DrawFullScreenQuad()`, `SetAlphaTest(int)`,
  `RenderToTexStart(tex, wantZBuffer)`, `TexCopy(dest, src)`, `NewHlslShader(file, entry, profile)`
  and `NewEffect(file, applyGlobals)`.
- Style is the project's: four spaces, braces on their own lines, `Type const&`.

## Interface: Hard Truck Apocalypse's vtable vs. the original

`CDevice` overrides HTA's `IRenderer`. The engine header spells the overloads the original way
(`PushX()` / `PushX(c)`, `MatPush()` / `MatPush(m)`, `SetIndices`, `SetToStream0`, `SetToStream`,
`DownloadTexImageRgba8888`, `DrawFullScreenQuad(IEffect*)`, `AddTextureFromBackBuffer(TexHandle)`),
so the original names are used as is. What differs:

| original | HTA (`i_renderer.h`) |
|---|---|
| `PushAlphaTest/PopAlphaTest/PushAlphaTest(int)/SetAlphaTest(int,bool)`, `Push/Pop/SetDithering` | not virtual; HTA has only `SetAlphaTest(int)` |
| `SetToStreams01`, `SetStreamFrequency`, `SetTexAnimStart`, `CreateMips`, `TexCopy(…,bool)`, `DrawFullScreenQuad(bool)`, `SaveTextureToFile`, `RenderToTexStart(3 args)`, `NewHlslShader/NewEffect(…, compileParams)`, `CompileParamToString`, `StringToCompileParam`, `SetRenderThreadId`, `EnableThreadSafeQuard`, `AddMesh/ReleaseMesh/RenderMesh/…`, `SetupDXCursorForce`, `TgEnableSetMatrixStrReflection`, `EnableFastClipPlane`, `SetFastClipPlane`, `GetInvViewMatrix`, `GetModelMatrix` | plain members, not in HTA's vtable |
| `SetTexture(int, TexHandle const&, float tsc)` | `… long double tsc`, converted to `float` inside |
| `IsMultiSamplingSupported(int)` (plain) | virtual |
| `IEffect::Parameter` has 49 values | HTA's 31, in a different order; the effect code indexes by HTA's enum and the name table uses HTA's names |
| `DeviceFeature` has 27 values | HTA's 25 plus `FEATURE_HARDWARE_INSTANCING` and `FEATURE_DEPTH_TEXTURES` from `i_renderer_orig.h` |
| `TexDynFormat` adds `TM_DTF_COMPRESSED_RGBA_FORMAT/RGB_FORMAT/DEPTH_TARGET` | from `i_renderer_orig.h` |
| `TextureState` has `TS_ITEX = 14` | not in HTA; every later value is one lower there. `SetStageState` switches by name, so the executable's values are honoured; the `TS_ITEX` case is kept as a comment |
| `TexCreateFlags` has no `TM_WANT_ALPHA` | HTA has `TM_WANT_ALPHA = 8`; the original flag handling is ported as is, and the engine never passes that bit |

Handle index access is the original way: `InternalHandle<T>` plus `TexId/VbId/IbId/MeshId` in `device.h`.
Engine services are reached as in the original: `g_kernel->GetEngineCfg().m_r_xxx`, `GetFileServer()`,
`CreateIniFile()`, `GetTimer()`, `g_mar.AllocMem/FreeMem`.

## Adaptations

Everything marked `retruxx adaptation` in the code:

1. **Shader compilation** (`shaders/shader_compat.h`, `shaders/hlsl/hlsl_shader.cpp`,
   `shaders/effects/effect.cpp`). The original compiled its shaders with the D3DX of its own SDK.
   The compiler in d3dx9_43 rejects the ps_1_x profiles the shaders use (`error X3539`), and its
   backwards-compatibility mode compiles them as ps_2_0 with different semantics: no [0,1] clamp on
   texture coordinates read as values, exact compares instead of `cnd`, samplers bound in order of
   first use with unused ones dropped. The visible result was a stretched shadow edge from
   `lsdetailedshadows.fx` and wrong sampler stages in `landscapefp_ps11.ps`. The port therefore
   passes `D3DXSHADER_USE_LEGACY_D3DX9_31_DLL`, which makes d3dx9_43 delegate to the older compiler
   in `d3dx9_31.dll`; its output was verified identical to that of the D3DX the original used. When
   `d3dx9_31.dll` is not installed, the fallback is the compatibility mode with the preprocessed
   source rewritten in memory so that samplers get their registers in declaration order. The
   shader files are never modified.
2. **Bounds-checked containers.** The original relied on an STL that did not check. `~CDevice`
   releases the index-buffer pool bounded by `m_IbPoolBuffers.size()` where the original used the
   vertex pool's size and read past the end (the Debug STL aborted at shutdown), and
   `UnregisterResetCallback` / `ReleaseTexture` skip the erase when the element is not found instead
   of calling `erase(end())`. The observable behaviour is unchanged.

## Differences forced by the engine headers

These are places where the original and the retruxx headers disagree. They are implemented the
original way as far as the headers allow and are listed here rather than fixed in `lib/include`:

- `EngineConfig` has no `r_screenshotTGA` and no `m_mt_render_in_separate_thread` cvar; the
  renderer uses the original defaults (TGA screenshots on, thread-safe guard reduced to the
  `GetCurrentThreadId()` call that is left of it in the original release build).
- `QueryReturnValue` keeps its type and result union private in `i_renderer_query.h`; `query.cpp`
  reaches them through the accessors' return types.
- `m3d::rend::VertexXYZT1` and the converting `InvalidHandle<>` of the original do not exist;
  `fsrt.cpp` and `vb_man.cpp` define local equivalents.
- `device.h` declares `getD3dFmtStr`, `getD3dErrorStr`, `SaveSurfaceToTGAFile`,
  `SaveSurfaceToGrayscaleTGAFile` and `DXRENDER9_BUILD_STRING`, the build stamp of the resource
  reports. The original embedded a compile time in each report, so the stamp is assembled from
  `__DATE__` / `__TIME__`; its text follows the stamp of Hard Truck Apocalypse's own DLL without the
  version number.

## Engine-side changes made for the DLL

- `lib/engine/math/matrix.cpp` defines `CMatrix::calcDeterminantSimple`, `getInverseSimple`,
  `transposeInplace` and `createPlaneTransform`, which `matrix.h` declared but nothing defined;
  they are restored from the original inlines.
- `lib/engine/core/timer.cpp` includes `<mmsystem.h>` explicitly so it compiles under the DLL's
  `WIN32_LEAN_AND_MEAN`.
- The root `CMakeLists.txt` adds the subdirectory and makes `retruxx` depend on `dxrender9`.

## Validating a function against the original

Open the reference binary with its symbols in any disassembler that reads PDB symbols and line
information (IDA, Ghidra, x64dbg, WinDbg) and go to the RVA from the function's `orig` marker (the
virtual address is the RVA plus the image base). The symbols give the original parameter and local
names and the source line of every statement, which the `// <n>` comments in the port refer to.
When resolving a virtual call, establish the type of the object first: `CDevice` is not
`IDirect3DDevice9`, and a slot in the COM interfaces is read from the DirectX SDK headers, a slot
in the engine's own classes from that class's vtable in the binary.
