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
            var != &M3D_ENGINE_CFG.m_mus_Volume && var != &M3D_ENGINE_CFG.m_snd_2dVolume &&
                var != &M3D_ENGINE_CFG.m_snd_3dVolume)
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

        if (var == &M3D_ENGINE_CFG.m_mus_Volume)
        {
            if (M3D_ENGINE_CFG.m_mus_Enable.GetB())
            {
                M3D_SOUND->SetGroupVolume(0, intToken);
                M3D_SOUND->SetMaxVolume(intToken);
            }
        }
        else if (var == &M3D_ENGINE_CFG.m_snd_2dVolume)
        {
            if (M3D_ENGINE_CFG.m_snd_Enable.GetB())
            {
                M3D_SOUND->SetGroupVolume(1, intToken);
            }
        }
        else if (var == &M3D_ENGINE_CFG.m_snd_3dVolume)
        {
            if (M3D_ENGINE_CFG.m_snd_Enable.GetB())
            {
                M3D_SOUND->SetGroupVolume(2, intToken);
            }
        }
        return true;
    }
}  // namespace m3d
