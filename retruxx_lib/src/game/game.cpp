#include "game.h"
#include "m3dgame.h"

namespace
{
    HANDLE g_SentinelMutex = NULL;

    HANDLE CreateSentinelMutex(TCHAR const* name)
    {
        auto mutex = ::CreateMutex(0, 1, name);
        if (::GetLastError() == ERROR_ALREADY_EXISTS)
        {
            mutex = NULL;
        }
        return mutex;
    }

    void injectRetruxxAddresses()
    {
        for (const auto& [address, newAddress] : inject::InjectAddresses)
        {
            inject::injectMemoryAddress(address, newAddress);
        }
    }
}

RETRUXX_DLL_INJECT_FUNCTION(0x00414C80, mainImpl)
int APIENTRY mainImpl(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);

    injectRetruxxAddresses();

    g_SentinelMutex = CreateSentinelMutex(TEXT("TargemExMachina_SentinelMutex"));
    if (g_SentinelMutex != NULL)
    {
        CMiracle3d game;
        //TODO:: load icon from resources
        auto hIcon = ::LoadIcon(hInstance, TEXT("icon.ico"));
        auto result = 0;
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
