#include "playermoneywnd.h"

#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>

#include <server/objects/player.h>

RT_CLASS_EXPORTS_BEGIN(PlayerMoneyWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(PlayerMoneyWnd);

PlayerMoneyWnd::AuxInfo::AuxInfo()
{
    // RVA 0x658A0
    m_wndValueName = "wndValue";
    m_strIdMoney = "Money";
}

m3d::Class* PlayerMoneyWnd::GetClass() const
{
    // RVA 0x65830
    return RT_CLASS_LOCAL(PlayerMoneyWnd);
}

m3d::Class* PlayerMoneyWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* PlayerMoneyWnd::CreateObject()
{
    // RVA 0x65840
    return new PlayerMoneyWnd;
}

m3d::Object* PlayerMoneyWnd::Clone()
{
    // RVA 0x657C0
    return new PlayerMoneyWnd(*this);
}

PlayerMoneyWnd::PlayerMoneyWnd()
{
    // RVA 0x65930
    m_wndValue = nullptr;
}

PlayerMoneyWnd::PlayerMoneyWnd(PlayerMoneyWnd const& rhs) : m3d::ui::Wnd(rhs)
{
    // RVA 0x659F0 - fresh window; m_strMoney / m_aif are re-defaulted.
    m_wndValue = nullptr;
}

PlayerMoneyWnd::~PlayerMoneyWnd()
{
    // RVA 0x65A30 - m_strMoney and m_aif release themselves.
}

int PlayerMoneyWnd::GameDataSetup()
{
    // RVA 0x65AD0
    int res = 1;

    if ((m_gameDataFlags & 2) == 0)
    {
        m3d::Object* child = GetChildByName(m_aif.m_wndValueName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndValue = static_cast<m3d::ui::Wnd*>(child);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndValueName + " is not found or incorrect type");
            res = 0;
        }

        m_strMoney = M3D_APP->GetStringByStringId0(m_aif.m_strIdMoney) + CStr(": ");

        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("PlayerMoneyWnd: error - fail to init because of a bad resource");
    return 0;
}

int PlayerMoneyWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x65D40
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
    // RVA 0x65D70
    UpdateValue();
}

void PlayerMoneyWnd::OnStartLevel()
{
    // RVA 0x65D80
    UpdateValue();
}

void PlayerMoneyWnd::UpdateValue()
{
    // RVA 0x65D90
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (ai::thePlayer)
    {
        m_wndValue->SetText(m_strMoney + CStr(ai::thePlayer->GetMoney()));
    }
    else
    {
        m_wndValue->SetText(m_strMoney);
    }
}
