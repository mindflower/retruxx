#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

HINSTANCE g_hInst;
DWORD	  g_amPlatform;		// VER_PLATFORM_WIN32_WINDOWS etc... (from GetVersionEx)
OSVERSIONINFO g_osInfo;

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD dwReasonForCall,
                      LPVOID
)
{
    switch (dwReasonForCall)
    {
    case DLL_PROCESS_ATTACH:
        MessageBox(NULL, "Dll injected!", "retruxx", MB_OK);
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
