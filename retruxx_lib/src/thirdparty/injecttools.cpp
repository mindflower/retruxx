#ifdef RETRUXX_DLL

#include "injecttools.h"

namespace
{
    const uint8_t JmpCmd = 0xe9;
    const uint8_t JmpShortCmd = 0xe9;
}

namespace inject
{
    bool injectFunctionCall(uint32_t address, uint32_t newFunctionAddress)
    {
        MessageBox(nullptr, "", "", MB_OK);
        const auto currentProcess = ::GetCurrentProcess();
        ::WriteProcessMemory(
            currentProcess,
            reinterpret_cast<void*>(address),
            &JmpCmd,
            sizeof(JmpCmd),
            0
        );

        const auto offset = newFunctionAddress - (address + 5);
       :: WriteProcessMemory(
            currentProcess,
            reinterpret_cast<void*>(address + 1),
            &offset,
            sizeof(offset),
            0
        );

       return true;
    }

    bool injectJmpShort(uint32_t from, uint32_t to)
    {
        const auto currentProcess = ::GetCurrentProcess();
        ::WriteProcessMemory(
            currentProcess,
            reinterpret_cast<void*>(from),
            &JmpShortCmd,
            sizeof(JmpShortCmd),
            0
        );

        const auto toOffset = to - from - 2;
        ::WriteProcessMemory(
            currentProcess,
            reinterpret_cast<void*>(from + 1),
            &toOffset,
            sizeof(toOffset),
            0
        );

        return true;
    }

    bool replaceVtableEntry(uint32_t entryAddress, uint32_t newFunctionAddress)
    {
        const auto currentProcess = ::GetCurrentProcess();
        ::WriteProcessMemory(
            currentProcess,
            reinterpret_cast<void*>(entryAddress),
            &newFunctionAddress,
            sizeof(newFunctionAddress),
            0
        );

        return true;
    }

    bool replaceCondJmpToJmp(uint32_t address)
    {
        const auto currentProcess = ::GetCurrentProcess();
        ::WriteProcessMemory(
            currentProcess,
            reinterpret_cast<void*>(address),
            &JmpShortCmd,
            sizeof(JmpShortCmd),
            0
        );

        return true;
    }
}
#endif //RETRUXX_DLL
