#include "game\game.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    return mainImpl(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
