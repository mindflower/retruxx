#pragma once
#include <core/console/console.h>
#include <math/vector.h>

class Vivisector : public m3d::IConHandler
{
public:
    class TownPosInfo
    {
    private:
        int m_townID;
        CVector m_origin;
    };

public:
    bool init();
    Vivisector();
    virtual void HandleCommand(int, m3d::CConsoleParams const&);
    virtual bool HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&);
    virtual ~Vivisector();
    bool shutdown();

private:
    std::vector<TownPosInfo> m_test;
};
