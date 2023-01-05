#include "m3dgame.h"

namespace
{
    HANDLE g_SentinelMutex = nullptr;
}

HINSTANCE g_hInst = nullptr;
DWORD g_amPlatform = 0;
OSVERSIONINFO g_osInfo = {};

int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    try
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

        g_SentinelMutex = ::CreateMutex(nullptr, 1, TEXT("TargemExMachina_SentinelMutex"));
        if (::GetLastError() == ERROR_ALREADY_EXISTS)
        {
            g_SentinelMutex = nullptr;
        }

        if (g_SentinelMutex != nullptr)
        {
            //TODO:: load icon from resources
            auto const hIcon = ::LoadIcon(nullptr, IDI_WINLOGO);
            auto result = 0;
            if (CMiracle3d app; app.init(hInstance, hIcon, "data\\config.cfg", nullptr, lpCmdLine))
            {
                result = app.run();
                app.done();
            }
            ::ReleaseMutex(g_SentinelMutex);
            return result;
        }
        ::MessageBox(nullptr, TEXT("Another instance already exists!"), TEXT("Error"), MB_ICONHAND);
        return 0;
    }
    catch (const std::exception& ex)
    {
        ::MessageBox(nullptr, ex.what(), TEXT("Error"), MB_ICONHAND);
        return 0;
    }
}
