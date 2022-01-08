#include "vivisectionblock.h"
#include <config.h>
#include <stdexcept>
#include <core/kernel.h>

Vivisector* g_Vivisector = nullptr;

bool Vivisector::init()
{
    throw std::logic_error("Not implemented");
}

void Vivisector::HandleCommand(int, m3d::CConsoleParams const&)
{
    throw std::logic_error("Not implemented");
}

bool Vivisector::HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&)
{
    throw std::logic_error("Not implemented");
}

Vivisector::~Vivisector()
{
    throw std::logic_error("Not implemented");
}

bool Vivisector::shutdown()
{
    throw std::logic_error("Not implemented");
}

bool initVivisectionBlock()
{
    if (g_Vivisector != nullptr)
    {
        M3D_LOG_WARN("Vivisector is already initialized...", LOG_WARN);
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
