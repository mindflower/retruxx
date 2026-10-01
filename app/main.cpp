#include "game\game.h"

namespace
{
    // The shipped executable reserves 50,000,000 bytes of stack in its PE header (0x2FAF080): ODE's collision code
    // allocas per-geom data and an n*n bit matrix, which overflows the default 1 MB. A PE stack size is also the
    // default for every other thread in the process (driver, audio, input and thread-pool threads), and on a current
    // Windows there are enough of those to use up the 2 GB address space with 48 MB reservations. So the executable
    // keeps the default 1 MB, and the game runs on a thread of its own with the shipped 50 MB stack.
    constexpr SIZE_T GAME_THREAD_STACK_SIZE = 50000000;

    struct GameThreadArgs
    {
        HINSTANCE hInstance;
        HINSTANCE hPrevInstance;
        LPSTR lpCmdLine;
        int nCmdShow;
    };

    DWORD WINAPI GameThreadProc(LPVOID param)
    {
        auto const* args = static_cast<GameThreadArgs const*>(param);
        return static_cast<DWORD>(mainImpl(args->hInstance, args->hPrevInstance, args->lpCmdLine, args->nCmdShow));
    }
}  // namespace

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    GameThreadArgs args{hInstance, hPrevInstance, lpCmdLine, nCmdShow};
    HANDLE const thread =
        CreateThread(nullptr, GAME_THREAD_STACK_SIZE, GameThreadProc, &args, STACK_SIZE_PARAM_IS_A_RESERVATION, nullptr);
    if (!thread)
    {
        return mainImpl(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    }
    WaitForSingleObject(thread, INFINITE);
    DWORD exitCode = 0;
    GetExitCodeThread(thread, &exitCode);
    CloseHandle(thread);
    return static_cast<int>(exitCode);
}
