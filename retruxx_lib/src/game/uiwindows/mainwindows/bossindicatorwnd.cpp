#include "bossindicatorwnd.h"

#include "core/log.h"
#include "server/objects/base/obj.h"
#include "server/objects/base/prototypeinfo.h"
#include "ui/image.h"
#include "ui/progressbarwnd.h"
#include "game/m3dgame.h"
#include "game/uimisc/guihelper.h"
#include "server/server.h"
#include "server/objects/monsters/boss02.h"
#include "server/objects/monsters/boss03.h"
#include "server/objects/monsters/boss04.h"
#include "server/objects/monsters/bossmetalarm.h"

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
    if (m_bossId == -1)
    {
        return nullptr;
    }

    auto* boss = ai::theObjects->GetEntityByObjId(m_bossId);
    if (boss && help::IsBoss(boss))
    {
        auto const flags = boss->GetFlags();
        if ((flags & 8) == 0 && (flags & 2) == 0 && !boss->GetParentRepository())
        {
            return boss;
        }
    }
    return nullptr;
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
    if ((m_gameDataFlags & 1) != 0)
    {
        CStr name;
        auto* boss = GetBoss();
        if (boss)
        {
            auto* prototypeInfo = boss->GetPrototypeInfo();
            if (prototypeInfo)
            {
                name = prototypeInfo->m_prototypeName;
            }
        }

        m3d::rend::TexHandle ico;
        if (!name.empty())
        {
            ico = M3D_APP->m_pInterfaceManager->GetIcoByName(name, 0);
        }

        m_wndBossImage->SetImage(ico);
    }
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
    UpdateImage();
    UpdateName();
    if ((m_gameDataFlags & 1) != 0)
    {
        auto* boss = GetBoss();
        float maxHeath  = 0.0;
        float curHealth = 0.0;
        GetBossHealth(boss, maxHeath, curHealth);
        m_pbBossHealth->SetMaxValue(maxHeath);
        m_pbBossHealth->SetCurValue(curHealth);
    }
}

void BossIndicatorWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BossIndicatorWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

void BossIndicatorWnd::GetBossHealth(ai::Obj const* boss, float& maxHealth, float& curHealth) const
{
    using namespace ai;

    maxHealth = 0.0;
    curHealth = 0.0;
    if (boss)
    {
        if (IS_KIND_OF(boss, BossMetalArm))
        {
            auto* castedBoss = RT_DYNCAST(boss, const BossMetalArm);
            maxHealth = castedBoss->GetMaxHealth();
            curHealth = castedBoss->GetHealth();
        }
        else if (IS_KIND_OF(boss, Boss02))
        {
            auto* castedBoss = RT_DYNCAST(boss, const Boss02);
            maxHealth = castedBoss->GetMaxHealth();
            curHealth = castedBoss->GetHealth();
        }
        else if (IS_KIND_OF(boss, Boss03))
        {
            auto* castedBoss = RT_DYNCAST(boss, const Boss03);
            maxHealth = castedBoss->GetMaxHealth();
            curHealth = castedBoss->GetHealth();
        }
        else if (IS_KIND_OF(boss, Boss04))
        {
            auto* castedBoss = RT_DYNCAST(boss, const Boss04);
            maxHealth = castedBoss->GetMaxHealth();
            curHealth = castedBoss->GetHealth();
        }
    }
}

int BossIndicatorWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BossIndicatorWnd::GameDataSetup()
{
    using namespace m3d::ui;

    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        auto child = GetChildByName(m_aif.m_wndBossImageName);
        if (child && IS_KIND_OF(child, ImageWnd))
        {
            m_wndBossImage = RT_DYNCAST(child, ImageWnd);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndBossImageName + " is not found or incorrect type");
            res = 0;
        }

        child = GetChildByName(m_aif.m_wndBossNameName);
        if (child && IS_KIND_OF(child, Wnd))
        {
            m_wndBossName = RT_DYNCAST(child, Wnd);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndBossNameName + " is not found or incorrect type");
            res = 0;
        }

        child = GetChildByName(m_aif.m_pbBossHealthName);
        if (child && IS_KIND_OF(child, ProgressBarWnd))
        {
            m_pbBossHealth = RT_DYNCAST(child, ProgressBarWnd);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_pbBossHealthName + " is not found or incorrect type");
            res = 0;
        }

        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_INFO("BossIndicatorWnd: error - fail to init because of a bad resource");
}

int BossIndicatorWnd::OnBeforeAddToWndStation()
{
    FullUpdate();
    return Wnd::OnBeforeAddToWndStation();
}

void BossIndicatorWnd::UpdateHealth(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BossIndicatorWnd::UpdateName()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        CStr name;
        if (auto* boss = GetBoss())
        {
            name = ai::pServer->GetFullNameByObjID(boss->GetId());
        }
        m_wndBossName->SetText(name);
    }
}
