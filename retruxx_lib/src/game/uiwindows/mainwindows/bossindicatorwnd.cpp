#include "bossindicatorwnd.h"

RT_CLASS_EXPORTS_BEGIN(BossIndicatorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BossIndicatorWnd);

BossIndicatorWnd::AuxInfo::AuxInfo()
{
    m_wndBossImageName = "wndBossImage";
    m_wndBossNameName = "wndBossName";
    m_pbBossHealthName = "pbBossHealth";
}

BossIndicatorWnd::~BossIndicatorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* BossIndicatorWnd::GetClass() const
{
    return RT_CLASS_LOCAL(BossIndicatorWnd);
}

ai::Obj const* BossIndicatorWnd::GetBoss() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BossIndicatorWnd::SetupForBoss(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BossIndicatorWnd::CreateObject()
{
    return new BossIndicatorWnd;
}

m3d::Class* BossIndicatorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int BossIndicatorWnd::GetBossId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* BossIndicatorWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BossIndicatorWnd::UpdateImage()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BossIndicatorWnd::BossIndicatorWnd(BossIndicatorWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BossIndicatorWnd::BossIndicatorWnd()
{
    m_wndBossImage = 0;
    m_wndBossName = 0;
    m_pbBossHealth = 0;
    m_bossId = -1;
}

void BossIndicatorWnd::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BossIndicatorWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BossIndicatorWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BossIndicatorWnd::GetBossHealth(ai::Obj const*, float&, float&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BossIndicatorWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BossIndicatorWnd::GameDataSetup()
{
    // TODO: implement BossIndicatorWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int BossIndicatorWnd::OnBeforeAddToWndStation()
{
    // TODO: implement BossIndicatorWnd::OnBeforeAddToWndStation
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void BossIndicatorWnd::UpdateHealth(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BossIndicatorWnd::UpdateName()
{
    RETRUXX_NOT_IMPLEMENTED;
}
