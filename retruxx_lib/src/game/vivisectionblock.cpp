#include "vivisectionblock.h"
#include <config.h>
#include <stdexcept>
#include <core/kernel.h>
#include <core/log.h>

Vivisector* g_Vivisector = nullptr;

bool Vivisector::init()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Vivisector::HandleCommand(int, m3d::CConsoleParams const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool Vivisector::HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

Vivisector::~Vivisector()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool Vivisector::shutdown()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool initVivisectionBlock()
{
    if (g_Vivisector != nullptr)
    {
        M3D_LOG_WARN("Vivisector is already initialized...");
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
