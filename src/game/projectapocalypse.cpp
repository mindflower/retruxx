#include "m3dgame.h"

namespace
{
    HANDLE g_SentinelMutex = NULL;
}

HINSTANCE g_hInst;
DWORD g_amPlatform;		// VER_PLATFORM_WIN32_WINDOWS etc... (from GetVersionEx)
OSVERSIONINFO g_osInfo;

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);

    //TODO: g_hInst, g_osInfo and g_amPlatform
    g_hInst = hInstance;
    g_amPlatform = VER_PLATFORM_WIN32_WINDOWS; // win95 assumed in case GetVersionEx fails
    g_osInfo.dwOSVersionInfoSize = sizeof(g_osInfo);
    if (GetVersionEx(&g_osInfo))
    {
        g_amPlatform = g_osInfo.dwPlatformId;
    }

    g_SentinelMutex = ::CreateMutex(0, 1, TEXT("TargemExMachina_SentinelMutex"));
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        g_SentinelMutex = NULL;
    }

    if (g_SentinelMutex != NULL)
    {
        CMiracle3d app;
        //TODO:: load icon from resources
        auto hIcon = ::LoadIcon(hInstance, TEXT("icon.ico"));
        auto result = 0;
        if (app.init(hInstance, hIcon, "data\\config.cfg", NULL, lpCmdLine))
        {
            result = app.run();
            app.done();
        }
        ::ReleaseMutex(g_SentinelMutex);
        return result;
    }
    else
    {
        ::MessageBox(0, TEXT("Another instance already exists!"), TEXT("Error"), MB_ICONHAND);
        return 0;
    }
}
