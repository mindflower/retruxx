#include "maingameinterfacewnd.h"
#include "radarwnd.h"
#include "igrokamochatwnd.h"
#include "damageinfownd.h"
#include "weaponinfolist.h"
#include "fadingmsg.h"
#include "counterwnd.h"
#include "targetinfownd.h"
#include "maincursorwnd.h"
#include "speedometerwnd.h"
#include "bossindicatorwnd.h"
#include "game/m3dgame.h"
#include "core/log.h"
#include "game/uimanager/uidefs.h"
#include "server/objects/base/complexphysicobj.h"
#include "server/objects/base/objcontainer.h"
#include "ui/image.h"

RT_CLASS_EXPORT_METHOD_DEFINE(MainGameInterfaceWnd, SetupForBoss)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(MainGameInterfaceWnd)
RT_CLASS_EXPORT(MainGameInterfaceWnd, m3d::METHOD, SetupForBoss, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MainGameInterfaceWnd);

MainGameInterfaceWnd::AuxInfo::AuxInfo()
{
    m_wndNearbyChestsIcoName = "wndNearbyChestsIco";
    m_wndNearbyTownIcoName = "wndNearbyTownIco";
}

m3d::Class* MainGameInterfaceWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* MainGameInterfaceWnd::GetClass() const
{
    return RT_CLASS_LOCAL(MainGameInterfaceWnd);
}

int MainGameInterfaceWnd::GetBossId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::CheckAndShowTargetInfoWnd(bool bForceRemove)
{
    if ((m_gameDataFlags & 1) != 0)
    {
        if (m_wndTargetInfo->GetTargetObjId() == -1)
        {
            if (IsDirectChild(m_wndTargetInfo))
            {
                if (bForceRemove)
                {
                    RemoveChildForce(m_wndTargetInfo);
                }
                else
                {
                    RemoveChild(m_wndTargetInfo);
                }
            }
        }
        else
        {
            if (!IsDirectChild(m_wndTargetInfo))
            {
                AddChild(m_wndTargetInfo);
            }
            MoveChildToFirstPosition(m_wndTargetInfo);
        }
    }
}

m3d::Object* MainGameInterfaceWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MainGameInterfaceWnd::CreateObject()
{
    return new MainGameInterfaceWnd;
}

ai::Obj const* MainGameInterfaceWnd::GetBoss() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::CheckAndShowCounterWnd()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        if (!m_wndCounter->NeedShow())
        {
            if (IsDirectChild(m_wndCounter))
            {
                RemoveChild(m_wndCounter);
            }
        }
        else
        {
            if (!IsDirectChild(m_wndCounter))
            {
                AddChild(m_wndCounter);
            }
        }
    }
}

void MainGameInterfaceWnd::CheckAndShowMainCursorWnd()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        if (m_wndMainCursor->NeedShow())
        {
            if (!IsDirectChild(m_wndMainCursor))
            {
                AddChild(m_wndMainCursor);
            }
            if (m_wndTargetInfo)
            {
                if (IsDirectChild(m_wndTargetInfo))
                {
                    MoveChildToFirstPosition(m_wndTargetInfo);
                }
            }
        }
        else
        {
            if (IsDirectChild(m_wndMainCursor))
            {
                RemoveChild(m_wndMainCursor);
            }
        }
    }
}

int MainGameInterfaceWnd::SetupForBoss(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MainGameInterfaceWnd::~MainGameInterfaceWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MainGameInterfaceWnd::MainGameInterfaceWnd()
{
    m_contouredInfoObjId = -1;
    m_contouredCapturedObjId = -1;
    m_nearbyTownId = -1;
    m_bossId = -1;
    m_wndNearbyChestsIco = 0;
    m_wndNearbyTownIco = 0;
    m_bNearbyChests = 0;
    m_oldInfoObjTolerance = ai::RS_MAX;
}

MainGameInterfaceWnd::MainGameInterfaceWnd(MainGameInterfaceWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::CheckAndShowNearbyTownIco()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        if (m_bossId != -1 || m_nearbyTownId == -1)
        {
            if (m_wndNearbyTownIco && IsDirectChild(m_wndNearbyTownIco))
                RemoveChild(m_wndNearbyTownIco);
        }
        else
        {
            if (m_wndNearbyTownIco)
            {
                if (!m_wndNearbyTownIco->GetParent())
                    AddChild(m_wndNearbyTownIco);
            }
        }
    }
}

int MainGameInterfaceWnd::GameDataSetup()
{
    using namespace m3d::ui;

    if ((m_gameDataFlags & 2) != 0)
    {
        if ((m_gameDataFlags & 1) != 0)
        {
            return 1;
        }
        M3D_LOG_ERR("MainGameInterfaceWnd: error - fail to init because of a bad resource");
        return 0;
    }

    int res = 1;

    auto wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_RADAR);
    if (wnd && IS_KIND_OF(wnd, RadarWnd))
    {
        m_wndRadar = RT_DYNCAST(wnd.get(), RadarWnd);
        AddChild(m_wndRadar);
    }
    else
    {
        res = 0;
    }

    wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_DAMAGEINFO);
    if (wnd && IS_KIND_OF(wnd, DamageInfoWnd))
    {
        m_wndDamageInfo = RT_DYNCAST(wnd.get(), DamageInfoWnd);
        AddChild(m_wndDamageInfo);
    }
    else
    {
        res = 0;
    }

    wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_WEAPON_INFO_LIST);
    if (wnd && IS_KIND_OF(wnd, WeaponInfoList))
    {
        m_wndWeaponInfoList = RT_DYNCAST(wnd.get(), WeaponInfoList);
        AddChild(m_wndWeaponInfoList);
    }
    else
    {
        res = 0;
    }

    wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_FADING_MSG_LIST);
    if (wnd && IS_KIND_OF(wnd, FadingMsgList))
    {
        m_wndFadingMsgList = RT_DYNCAST(wnd.get(), FadingMsgList);
        AddChild(m_wndFadingMsgList);
    }
    else
    {
        res = 0;
    }

    wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_IMPORTANT_FADING_MSG_LIST);
    if (wnd && IS_KIND_OF(wnd, FadingMsgList))
    {
        m_wndImportantFadingMsgList = RT_DYNCAST(wnd.get(), FadingMsgList);
        AddChild(m_wndImportantFadingMsgList);
    }
    else
    {
        res = 0;
    }

    wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_COUNTER);
    if (wnd && IS_KIND_OF(wnd, CounterWnd))
    {
        m_wndCounter = RT_DYNCAST(wnd.get(), CounterWnd);
        AddChild(m_wndCounter);
    }
    else
    {
        res = 0;
    }

    wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_TARGET_INFO_IN_MAIN_INTERFACE);
    if (wnd && IS_KIND_OF(wnd, TargetInfoWnd))
    {
        m_wndTargetInfo = RT_DYNCAST(wnd.get(), TargetInfoWnd);
        AddChild(m_wndTargetInfo);
    }
    else
    {
        res = 0;
    }

    wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_CURSOR_MAIN);
    if (wnd && IS_KIND_OF(wnd, MainCursorWnd))
    {
        m_wndMainCursor = RT_DYNCAST(wnd.get(), MainCursorWnd);
        AddChild(m_wndMainCursor);
    }
    else
    {
        res = 0;
    }

    wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_IGROKA_MOCHAT);
    if (wnd && IS_KIND_OF(wnd, IgrokaMochatWnd))
    {
        m_wndIgrokaMochat = RT_DYNCAST(wnd.get(), IgrokaMochatWnd);
        AddChild(m_wndIgrokaMochat);
    }
    else
    {
        res = 0;
    }

    wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_SPEEDOMETER);
    if (wnd && IS_KIND_OF(wnd, SpeedometerWnd))
    {
        m_wndSpeedometer = RT_DYNCAST(wnd.get(), SpeedometerWnd);
        AddChild(m_wndSpeedometer);
    }
    else
    {
        res = 0;
    }

    wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_BOSS_INDICATOR);
    if (wnd && IS_KIND_OF(wnd, BossIndicatorWnd))
    {
        m_wndBossIndicator = RT_DYNCAST(wnd.get(), BossIndicatorWnd);
        AddChild(m_wndBossIndicator);
    }
    else
    {
        res = 0;
    }

    auto child = GetChildByName(m_aif.m_wndNearbyChestsIcoName);
    if (child && IS_KIND_OF(child, ImageWnd))
    {
        m_wndNearbyChestsIco = RT_DYNCAST(child, ImageWnd);
    }
    else
    {
        M3D_LOG_INFO(
            "Get control error: control " + m_aif.m_wndNearbyChestsIcoName + " is not found or incorrect type");
        res = 0;
    }

    child = GetChildByName(m_aif.m_wndNearbyTownIcoName);
    if (child && IS_KIND_OF(child, ImageWnd))
    {
        m_wndNearbyTownIco = RT_DYNCAST(child, ImageWnd);
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndNearbyTownIcoName + " is not found or incorrect type");
        res = 0;
    }

    if (res)
    {
        m_gameDataFlags |= 1u;
        CheckAndShowNearbyChestsIco();
        CheckAndShowNearbyTownIco();
        CheckAndShowBossIndicator();
        return 1;
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_ERR("MainGameInterfaceWnd: error - fail to init because of a bad resource");
    return 0;
}

int MainGameInterfaceWnd::OnBeforeAddToWndStation()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        CheckAndShowCounterWnd();
        CheckAndShowTargetInfoWnd(true);
        CheckAndShowMainCursorWnd();
        CheckAndShowNearbyChestsIco();
        CheckAndShowNearbyTownIco();
        CheckAndShowBossIndicator();
        MoveChildToFirstPosition(m_wndIgrokaMochat);
    }
    return Wnd::OnBeforeAddToWndStation();
}

void MainGameInterfaceWnd::CheckAndShowNearbyChestsIco()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        if (m_bossId == -1 && m_bNearbyChests)
        {
            if (m_wndNearbyChestsIco)
            {
                if (!m_wndNearbyChestsIco->GetParent())
                    AddChild(m_wndNearbyChestsIco);
            }
        }
        else if (m_wndNearbyChestsIco && IsDirectChild(m_wndNearbyChestsIco))
        {
            RemoveChild(m_wndNearbyChestsIco);
        }
    }
}

void MainGameInterfaceWnd::OnBossModeChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::OnAfterRemoveFromWndStation()
{
    auto res = Wnd::OnBeforeRemoveFromWndStation();

    HideContour(m_contouredInfoObjId);
    m_contouredInfoObjId = -1;
    m_oldInfoObjTolerance = ai::RS_MAX;

    HideContour(m_contouredCapturedObjId);
    m_contouredCapturedObjId = -1;
    return res;
}

int MainGameInterfaceWnd::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::OnTownRuined(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::UpdateInfoContour()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::HideContour(int objId)
{
    using namespace ai;
    auto* obj = ai::theObjects->GetEntityByObjId(objId);
    if (obj && IS_KIND_OF(obj, ComplexPhysicObj))
    {
        auto* physObj = RT_DYNCAST(obj, ComplexPhysicObj);
        physObj->RemoveContour();
    }
}

void MainGameInterfaceWnd::ShowContour(int, unsigned, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::UpdateContours()
{
    RETRUXX_NOT_IMPLEMENTED;
}

unsigned MainGameInterfaceWnd::GetColorForInfoContour(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::ShowCapturedContour(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::OnNearbyChests(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::ClearCapturedContour()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::OnApproachTown(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::GetCapturedObjId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::ClearContours()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::CheckAndShowBossIndicator()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        if (m_bossId == -1)
        {
            if (m_wndBossIndicator)
            {
                if (IsDirectChild(m_wndBossIndicator))
                {
                    RemoveChild(m_wndBossIndicator);
                }
            }
        }
        else if (m_wndBossIndicator)
        {
            if (!m_wndBossIndicator->GetParent())
            {
                AddChild(m_wndBossIndicator);
            }
        }
    }
}

int MainGameInterfaceWnd::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int MainGameInterfaceWnd::OnBeforeRemoveFromWndStation()
{
    if (m_wndTargetInfo && IsDirectChild(m_wndTargetInfo))
    {
        RemoveChildForce(m_wndTargetInfo);
    }
    return Wnd::OnBeforeRemoveFromWndStation();
}

void MainGameInterfaceWnd::UpdateBossMode()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::UpdateCapturedContour()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MainGameInterfaceWnd::GetInfoObjId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::ShowInfoContour(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::ClearInfoContour()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool MainGameInterfaceWnd::IsInBossMode() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void MainGameInterfaceWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}
