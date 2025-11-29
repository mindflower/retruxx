#include "playermoneywnd.h"

RT_CLASS_EXPORTS_BEGIN(PlayerMoneyWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(PlayerMoneyWnd);

PlayerMoneyWnd::AuxInfo::AuxInfo()
{
    m_wndValueName = "wndValue";
    m_strIdMoney = "Money";
}

m3d::Class* PlayerMoneyWnd::GetClass() const
{
    return RT_CLASS_LOCAL(PlayerMoneyWnd);
}

m3d::Object* PlayerMoneyWnd::CreateObject()
{
    return new PlayerMoneyWnd;
}

m3d::Class* PlayerMoneyWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

PlayerMoneyWnd::~PlayerMoneyWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* PlayerMoneyWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerMoneyWnd::OnStartLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

PlayerMoneyWnd::PlayerMoneyWnd()
{
    m_wndValue = 0;
}

PlayerMoneyWnd::PlayerMoneyWnd(PlayerMoneyWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void PlayerMoneyWnd::UpdateValue()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int PlayerMoneyWnd::GameDataSetup()
{
    // TODO: implement PlayerMoneyWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int PlayerMoneyWnd::GameDataUpdate(void*, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 75 || dataType == 85)
    {
        UpdateValue();
    }
    return 1;
}

void PlayerMoneyWnd::OnPlayerMoneyChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}
