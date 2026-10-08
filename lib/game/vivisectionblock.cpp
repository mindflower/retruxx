#include "vivisectionblock.h"
#include <config.h>
#include <stdexcept>
#include <core/kernel.h>
#include <core/log.h>
#include <client.h>
#include <landscape.h>
#include <world.h>

Vivisector* g_Vivisector = nullptr;

bool Vivisector::init()
{
    // RVA 0x42D7C0
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCommand("vivisect", 0, this);
    return true;
}

void Vivisector::HandleCommand(int cmdID, m3d::CConsoleParams const&)
{
    // RVA 0x42D800 - "vivisect" dumps the landscape grass to an XML file.
    if (cmdID == 0)
    {
        m3d::pClient->GetWorld().GetLandscape().WriteGrassToXmlFile("data/testGrass.xml");
    }
}

bool Vivisector::HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&)
{
    // RVA 0x42D830
    return true;
}

Vivisector::~Vivisector()
{
    // RVA 0x42D630 - only m_test is released.
}

bool Vivisector::shutdown()
{
    // RVA 0x42D7F0
    return true;
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
