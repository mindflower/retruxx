#pragma once
#include <core/console/console.h>

namespace m3d
{
    class SoundConHandler :  public IConHandler
    {
    public:
        virtual void HandleCommand(int,CConsoleParams const &);
        virtual bool HandleCVar(CVar const *,CConsoleParams const &);
        SoundConHandler();
    };
}
