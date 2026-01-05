#include "soundconhandler.h"
#include <config.h>
#include <m3dapp.h>
#include <core/kernel.h>

namespace m3d
{
    void SoundConHandler::HandleCommand(int cmdId, CConsoleParams const& params)
    {
    }

    bool SoundConHandler::HandleCVar(CVar const* var, CConsoleParams const& params)
    {
        if (var == nullptr ||
            var != &g_Kernel->GetEngineCfg().m_mus_Volume &&
            var != &g_Kernel->GetEngineCfg().m_snd_2dVolume &&
            var != &g_Kernel->GetEngineCfg().m_snd_3dVolume)
        {
            return true;
        }
        if (params.NumOfTokens(' ') <= 1)
        {
            return true;
        }
        auto const* token = params.UnsafeStringToken(1, ' ');
        int intToken = 0;
        if (token == nullptr || sscanf_s(token, "%d", &intToken) == 0)
        {
            intToken = 0;
        }

        if (var == &g_Kernel->GetEngineCfg().m_mus_Volume)
        {
            if (g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
            {
                Application::g_pApp->m_sound->SetGroupVolume(0, intToken);
                Application::g_pApp->m_sound->SetMaxVolume(intToken);
            }
        }
        else if (var == &g_Kernel->GetEngineCfg().m_snd_2dVolume)
        {
            if (g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
            {
                Application::g_pApp->m_sound->SetGroupVolume(1, intToken);
            }
        }
        else if (var == &g_Kernel->GetEngineCfg().m_snd_3dVolume)
        {
            if (g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
            {
                Application::g_pApp->m_sound->SetGroupVolume(2, intToken);
            }
        }
        return true;
    }
}
