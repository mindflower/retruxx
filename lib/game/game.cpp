#include "m3dgame.h"

namespace
{
    HANDLE g_SentinelMutex = NULL;

    HANDLE CreateSentinelMutex(TCHAR const* name)
    {
        HANDLE mutex = ::CreateMutex(0, 1, name);
        if (::GetLastError() == ERROR_ALREADY_EXISTS)
        {
            mutex = NULL;
        }
        return mutex;
    }
}  // namespace

int APIENTRY mainImpl(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);

    g_SentinelMutex = CreateSentinelMutex(TEXT("TargemExMachina_SentinelMutex"));
    if (g_SentinelMutex != NULL)
    {
        int result = 0;
        HICON hIcon = ::LoadIcon(hInstance, TEXT("MAINICON"));

        CMiracle3d game;
        if (game.init(hInstance, hIcon, "data\\config.cfg", NULL, lpCmdLine) != 0)
        {
            result = game.run();
            game.done();
        }

        ::ReleaseMutex(g_SentinelMutex);
        return result;
    }
    ::MessageBox(0, TEXT("Another instance already exists!"), TEXT("Error"), MB_ICONHAND);
    return 0;
}
