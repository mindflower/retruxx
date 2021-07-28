#pragma once
#include <core/console/console.h>

namespace m3d
{
    class SoundConHandler :  public IConHandler
    {
    public:
        virtual ~SoundConHandler() = default;
        virtual void HandleCommand(int cmdId,CConsoleParams const& params);
        virtual bool HandleCVar(CVar const* var,CConsoleParams const& params);
    };
}
