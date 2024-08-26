#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

using namespace std;

void PatchMemory()
{
    //default effect technique null dereference patch
    //JmpShort(0x00A51AC3, 0x00A51AE4);
    ////JmpShort(0x00A514A3, 0x00A514D0);
    ////PatchVtableEntry(0x00BFA87C, TypeCast<uint32_t>(&Effect::MB_ApplyEffectPatched));

    //4gb patch
    //JmpShort(0x00AD8270, 0x00AD8290);
    //CondJumpToJmp(0x00AD8270);
    //CondJumpToJmp(0x00AC830D);
    //CondJumpToJmp(0x00AD8F6E);
    //CondJumpToJmp(0x00AD9031);
    //CondJumpToJmp(0x00AD90A7);
    //CondJumpToJmp(0x00AD93A7);
    //CondJumpToJmp(0x00AD94A6);
    //CondJumpToJmp(0x00AD95A5);
    //CondJumpToJmp(0x00AD96A4);
    //CondJumpToJmp(0x00AD979F);
    //CondJumpToJmp(0x00AD987F);
    //CondJumpToJmp(0x00AD996F);
    //CondJumpToJmp(0x00AD9E52);
    //CondJumpToJmp(0x00ADA3F0);
    //CondJumpToJmp(0x00ADA76D);
    //CondJumpToJmp(0x00ADA8F3);
    //CondJumpToJmp(0x00ADAF15);
    //CondJumpToJmp(0x00ADB537);
    //CondJumpToJmp(0x00ADBB6B);
    //JmpShort(0x00AD8894, 0x00AD88AD);

   // const uint32_t poolSizeInBytes = 256 * 1024 * 1024;
   // g_allocator.reset(_sm_allocator_create(5, poolSizeInBytes), [](sm_allocator allocator) {_sm_allocator_destroy(allocator); });
    //PatchVtableEntry(0x005C3F50, TypeCast<uint32_t>(&Allocate));
    //PatchVtableEntry(0x005C3F57, TypeCast<uint32_t>(&Reallocate));
    //PatchVtableEntry(0x005C3F5E, TypeCast<uint32_t>(&Free));
}

HINSTANCE g_hInst;
DWORD	  g_amPlatform;		// VER_PLATFORM_WIN32_WINDOWS etc... (from GetVersionEx)
OSVERSIONINFO g_osInfo;

DWORD WINAPI MainThread(LPVOID param)
{
    return 0;
}

void Init()
{
    MessageBox(NULL, "Dll injected!", "ProjectApocalypse", MB_OK);
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Init();
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport) void __cdecl Function1(void)
{
}
