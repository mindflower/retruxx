#include "vivisectionblock.h"
#include <config.h>
#include <core/kernel.h>

Vivisector* g_Vivisector = nullptr;

bool initVivisectionBlock()
{
    if (g_Vivisector != nullptr)
    {
        LOG("Vivisector is already initialized...", LOG_WARN);
        return false;
    }
    auto* vivisector = new Vivisector{};
    if (vivisector != nullptr)
    {
        g_Vivisector = vivisector;
        m3d::g_Kernel->GetEngineCfg().m_console->RegisterCommand("vivisect", 0, g_Vivisector);
        return true;
    }
    g_Vivisector = nullptr;
    return false;
}
