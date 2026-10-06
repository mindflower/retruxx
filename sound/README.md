# sound

`sound/` is the sound driver of the engine, built as `sound.dll`. The executable loads it at run
time, looks up the `createISound(m3d::Kernel*)` export and then talks to it only through
`snd::ISound` (`lib/include/iface.h`).

The code is a restoration of the original sound driver (project `sound`), taken from a build in
which it was statically linked into the executable. The manager, its groups and its log strings
are the original's; the FMOD underneath is not: the original used FMOD 3 through `fmod.dll` 3.74,
this build uses the FMOD Core SDK (see Adaptations).

## What it does

`CM3DSoundManager` keeps a table of sounds (samples for short effects, streams for music and long
effects) keyed by the id it hands back from `AddSound`, a tree of sound groups loaded from the
sound groups XML (`MUSIC`, `SOUND2D`, `SOUND3D` and their children, each with a volume multiplier
and a 3D distance range), and a table that records which sound every mixer channel plays. Music
changes fade the music group out over `m_musicFadeTime` and then start the next track; the engine's
end-of-music callback is invoked from FMOD's stream end callback under a critical section.

## Layout

| file | contents |
|---|---|
| `CM3DSoundManager.cpp` | the whole manager, as in the original: the class, the FMOD file callbacks, `m3d::SoundFactory` |
| `sounditem.h`, `soundgroup.h`, `SoundGroup.cpp` | `CSoundItem` and `SoundGroup` |
| `fmod/fsound.h` | the FMOD 3 API the driver calls: types, modes, callbacks and the `FMOD_INSTANCE` table |
| `fmod/fsound_errors.h` | the FMOD 3 error codes and texts the driver logs |
| `fmod/fsound_compat.cpp` | `retruxx adaptation`: those calls implemented over the FMOD Core API |
| `main.cpp` | the `createISound` export, `operator new/delete` over the kernel's allocator, the DLL's `g_kernel` and the engine's `m3d::g_Kernel` |
| `engine_helpers.cpp` | `m3d::ReadXmlFile`, `SafeStrAttrib`, `SafeFloatAttrib`, copied from `lib/engine/core/ini.cpp` (which cannot be compiled into the DLL: TinyXML) |

## Building

The target is `sound` in the root CMake project; `retruxx` depends on it, so a normal build of the
executable also produces `build/bin/<Config>/sound.dll` and copies the SDK's `fmod.dll` next to it.

Requirements:

- The FMOD Core SDK (FMOD Studio API, folder `api/core`). CMake reads `FMOD_SDK_DIR` from the
  environment and falls back to the default install path. The SDK's own `CMakeLists.txt` is not
  used: it describes the x64 libraries only; `sound/CMakeLists.txt` imports `lib/x86` (or `lib/x64`
  for a 64-bit build) itself.
- A 32-bit (`win32`) build, like the rest of the project.

The DLL links `fmod_vc.lib` and stays independent of `retruxx_lib`: the engine value types it uses
(`CStr`, `CVar`, `CriticalSection`, the math classes) are compiled into it from `lib/engine`.

At run time `fmod.dll` must be the FMOD Core one from the SDK. The game folder ships `fmod.dll`
3.74 under the same name; when the executable runs from there, that file has to be replaced.

The DLL's `operator new` and `operator delete` go through the kernel's allocator once `createISound`
has run and through the CRT heap before that, so no static object of the DLL may allocate in its
constructor: it would be freed through the wrong allocator when the executable unloads the DLL
(`FreeLibrary` after the manager is gone). The compat layer keeps everything that allocates in a
state created by `FSOUND_Init` and destroyed by `FSOUND_Close`.

## Source conventions

The same as `dxrender9/README.md`: `// orig 0x<rva> <file>:<line>` before each restored definition,
`// <n>` for the original line of a statement, original strings byte for byte, deviations marked
`retruxx adaptation`.

## Interface: Hard Truck Apocalypse's vtable vs. the original

| original | HTA (`iface.h`) |
|---|---|
| `Init(sampleRate, bitsPerSample, maxSounds, groupsFileName)`; the driver reached the kernel, the log and the asserts directly | `Init(logFunc, ...)`: the DLL stores the callback in the original's `l_log` static, as HTA's own `sound.dll` does, then runs the original body. `M3D_LOG_INFO` / `M3D_LOG_ERR` go through the callback, `M3D_ASSERT` reaches the kernel's `SysError(whence, descr)` with the original assertion text, file and line |
| `GetSoundIdByFilename`, `AddCustomMusic` are virtuals of `ISound` (slots 0x70, 0x74) | not in HTA's interface; plain members here |
| `SetEndMusicCallback(int, callback)`: the callback is invoked with the sound id in `ecx` | the same in HTA's own `sound.dll`. `iface.h` spells the pointer as a plain `void (*)(int)`; the engine passes a server function cast to that type (`servermusic.cpp`), so the manager stores and calls it as `__fastcall`. If the engine ever defines that function as cdecl, its argument will be wrong |

Everything else is identical, including the object size (0x64).

## Adaptations

`fmod/fsound_compat.cpp` is the one adaptation. The original loaded `fmod.dll` (FMOD 3.74) at run
time with its `fmoddyn.h` and called it through the `FMOD_INSTANCE` table; that API no longer
exists, so the table is filled with functions that translate the FMOD 3 semantics the driver
relies on to FMOD Core. Where the FMOD 3 documentation does not settle a behaviour, it was
measured on the game's own `fmod.dll` and reproduced:

- the driver's channels (`FSOUND_Init`'s count) are a table kept by the layer; FMOD Core gets more
  virtual voices than that so it never steals a channel on its own. `FSOUND_FREE` takes the next
  free channel round robin; when all are busy it steals the one with the lowest priority not above
  the new sound's, the oldest among equals, and the play fails without an error when there is none.
  The manager's channel table, its bare-index loops and the engine's stored handles depend on
  these rules: a looped sound whose handle dies is restarted by the sound server every frame;
- channel handles carry the channel index in the low 12 bits and a reuse count above, as FMOD 3's
  did (the driver masks with `0xfff`); a bare index addresses whatever the index currently holds;
- a stream plays on one channel at a time: `FSOUND_Stream_PlayEx` on a stream that is playing
  returns the handle it has and does not restart it (several scene nodes sharing one long 3D sound
  rely on this), `FSOUND_Stream_SetPosition` rewinds a playing stream;
- `FSOUND_Sample_SetMaxPlaybacks` (the `maxSounds` of `AddSound`): a play beyond the limit fails
  without an error;
- volumes 0..255 and priorities 0 (lowest) .. 255 become 0..1 and 256 (lowest) .. 0; a loaded
  sound's default priority is 255, as in FMOD 3;
- sounds are 3D unless opened with `FSOUND_2D`, and stereo data is played in 2D, as FMOD 3's
  software mixer did not position it (music streams rely on this);
- the stream end callback fires when the stream reaches its end, not when the driver stops it, and
  it is invoked from `FSOUND_Update` (the game thread), where FMOD 3 used its stream thread;
- after `FSOUND_Close` every call fails with `FMOD_ERR_UNINITIALIZED`, as in FMOD 3. The manager's
  destructor depends on it: it closes FMOD first and frees its samples and streams afterwards.

Known remaining difference: `FSOUND_SetLoopMode(FSOUND_LOOP_OFF)` on a playing stream lets the
current pass finish and then raises the end callback, where FMOD 3 stopped the stream within its
buffer and raised none. The engine only changes the loop mode of scene sounds, which are samples
unless longer than ten seconds, and only music streams have an end callback.

The original's `fmod_errors.h` is kept (`fsound_errors.h`) so the logged error texts are the same.

## Open items

- `MuteAllSounds`, `RestoreAllVolumes` and `SetChannelVolume` start with an assert the original
  left in (`!"obsolete"`, `!"not used"`); they reach the kernel's `SysError` when called.
