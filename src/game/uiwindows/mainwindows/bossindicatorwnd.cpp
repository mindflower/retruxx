#include "bossindicatorwnd.h"

RT_CLASS_EXPORTS_BEGIN(BossIndicatorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BossIndicatorWnd);

BossIndicatorWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

BossIndicatorWnd::~BossIndicatorWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BossIndicatorWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

ai::Obj const* BossIndicatorWnd::GetBoss() const
{
    throw std::logic_error("Not implemented");
}

int BossIndicatorWnd::SetupForBoss(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BossIndicatorWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* BossIndicatorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int BossIndicatorWnd::GetBossId() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* BossIndicatorWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

void BossIndicatorWnd::UpdateImage()
{
    throw std::logic_error("Not implemented");
}

BossIndicatorWnd::BossIndicatorWnd(BossIndicatorWnd const&)
{
    throw std::logic_error("Not implemented");
}

BossIndicatorWnd::BossIndicatorWnd()
{
    throw std::logic_error("Not implemented");
}

void BossIndicatorWnd::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

void BossIndicatorWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int BossIndicatorWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void BossIndicatorWnd::GetBossHealth(ai::Obj const*, float&, float&) const
{
    throw std::logic_error("Not implemented");
}

int BossIndicatorWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int BossIndicatorWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int BossIndicatorWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void BossIndicatorWnd::UpdateHealth(bool)
{
    throw std::logic_error("Not implemented");
}

void BossIndicatorWnd::UpdateName()
{
    throw std::logic_error("Not implemented");
}
