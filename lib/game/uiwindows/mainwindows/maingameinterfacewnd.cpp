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
#include "game/uimisc/guihelper.h"
#include "i_event.h"
#include "server/objects/base/complexphysicobj.h"
#include "server/objects/base/globalproperties.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"
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
    // RVA 0x12B4B0
    return m_bossId;
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
    // RVA 0x1292F0
    return new MainGameInterfaceWnd(*this);
}

m3d::Object* MainGameInterfaceWnd::CreateObject()
{
    return new MainGameInterfaceWnd;
}

ai::Obj const* MainGameInterfaceWnd::GetBoss() const
{
    // RVA 0x12B5E0
    if (m_bossId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_bossId);
    if (obj && help::IsBoss(obj))
    {
        unsigned int const flags = obj->GetFlags();
        if ((flags & 8) == 0 && (flags & 2) == 0 && !obj->GetParentRepository())
        {
            return obj;
        }
    }
    return nullptr;
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

int MainGameInterfaceWnd::SetupForBoss(int bossId)
{
    // RVA 0x12B4C0
    m_bossId = bossId;
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    bool const result = m_wndBossIndicator->SetupForBoss(bossId) != 0;
    CheckAndShowNearbyChestsIco();
    CheckAndShowNearbyTownIco();
    CheckAndShowBossIndicator();
    return result;
}

MainGameInterfaceWnd::~MainGameInterfaceWnd()
{
    // RVA 0x1297E0 - m_aif's CStr members and the ref_ptr<...> members
    // (which release their reference) clean up automatically; the two raw
    // ImageWnd* icons need an explicit release since they are not always a
    // current child (CheckAndShowNearby*Ico adds/removes them based on game
    // state) yet the class still holds an owning reference to them.
    if (m_wndNearbyChestsIco)
    {
        m_wndNearbyChestsIco->DecRef();
    }
    if (m_wndNearbyTownIco)
    {
        m_wndNearbyTownIco->DecRef();
    }
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

MainGameInterfaceWnd::MainGameInterfaceWnd(MainGameInterfaceWnd const&) : MainGameInterfaceWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x129770) default-constructs the
    // base, null ref_ptrs, and re-inits the AuxInfo, but leaves
    // m_contouredInfoObjId/m_contouredCapturedObjId/m_oldInfoObjTolerance/
    // m_wndNearbyChestsIco/m_wndNearbyTownIco/m_nearbyTownId/
    // m_bNearbyChests/m_bossId uninitialized; delegating to the default ctor
    // here avoids reading uninitialized pointers while still copying nothing
    // from the source.
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
    // RVA 0x12B5C0
    CheckAndShowNearbyChestsIco();
    CheckAndShowNearbyTownIco();
    CheckAndShowBossIndicator();
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

int MainGameInterfaceWnd::GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0x12B040
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("MainGameInterfaceWnd::GameDataSave error - MainGameInterfaceWnd has been not properly inited");
        return 0;
    }
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("MainGameInterfaceWnd::GameDataSave error - invalid params");
        return 0;
    }

    ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "MainGameInterface");
    guiNode->AddChild(node);
    node->SetAttribute("nearTownId", CStr(m_nearbyTownId).c_str());
    node->SetAttribute("nearChests", CStr(static_cast<int>(m_bNearbyChests)).c_str());
    node->SetAttribute("bossId", CStr(m_bossId).c_str());
    return 1;
}

void MainGameInterfaceWnd::OnTownRuined(void* data)
{
    // RVA 0x12B470
    if (data)
    {
        auto const* ev = static_cast<m3d::Event const*>(data);
        if (ev->m_intEv[0] == m_nearbyTownId)
        {
            if (ev->m_intEv[1])
            {
                m_nearbyTownId = -1;
            }
            CheckAndShowNearbyTownIco();
        }
    }
}

void MainGameInterfaceWnd::UpdateInfoContour()
{
    // RVA 0x12A6E0
    int const oldInfoObjId = m_contouredInfoObjId;
    int const infoObjId = GetInfoObjId();
    ai::eTolerance const tolerance = help::GetObjTolerance(oldInfoObjId);
    bool const changed = (oldInfoObjId != infoObjId) || (tolerance != m_oldInfoObjTolerance);
    m_oldInfoObjTolerance = tolerance;
    if (changed)
    {
        if (oldInfoObjId != -1)
        {
            HideContour(oldInfoObjId);
            m_contouredInfoObjId = -1;
            m_oldInfoObjTolerance = ai::RS_MAX;
        }
        if (infoObjId != -1 && infoObjId != m_contouredCapturedObjId)
        {
            ShowInfoContour(infoObjId);
        }
    }
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

void MainGameInterfaceWnd::ShowContour(int targetObjId, unsigned int color, float size)
{
    // RVA 0x12A860
    if (targetObjId < 0)
    {
        return;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(targetObjId);
    if (obj && obj->IsKindOf(&ai::ComplexPhysicObj::m_classComplexPhysicObj))
    {
        auto* complexObj = static_cast<ai::ComplexPhysicObj*>(obj);
        complexObj->SetContourColor(color);
        complexObj->SetContourWidth(size);
        complexObj->PutContour();
    }
}

void MainGameInterfaceWnd::UpdateContours()
{
    // RVA 0x12A6C0
    UpdateCapturedContour();
    UpdateInfoContour();
}

unsigned int MainGameInterfaceWnd::GetColorForInfoContour(int infoObjId) const
{
    // RVA 0x12AEF0
    ai::eTolerance const tolerance = help::GetObjTolerance(infoObjId);
    if (tolerance == ai::RS_ENEMY)
    {
        return ai::theGlobProp.m_colorEnemy;
    }
    if (tolerance > ai::RS_ENEMY && tolerance <= ai::RS_OWN)
    {
        return ai::theGlobProp.m_colorFriend;
    }
    return ai::theGlobProp.m_colorEnemy;
}

void MainGameInterfaceWnd::ShowCapturedContour(int capturedObjId)
{
    // RVA 0x12A830 (decompile unavailable - body inferred from
    // UpdateCapturedContour's inlined equivalent call to ShowContour).
    ShowContour(capturedObjId, ai::theGlobProp.m_colorTargetCaptured, ai::theGlobProp.m_targetCapturedContourWidth);
    m_contouredCapturedObjId = capturedObjId;
}

void MainGameInterfaceWnd::OnNearbyChests(void* data)
{
    // RVA 0x12AFF0
    if (data)
    {
        auto const* ev = static_cast<m3d::Event const*>(data);
        m_bNearbyChests = ev->m_intEv[0] != 0;
        CheckAndShowNearbyChestsIco();
    }
}

void MainGameInterfaceWnd::ClearCapturedContour()
{
    // RVA 0x12A990
    HideContour(m_contouredCapturedObjId);
    m_contouredCapturedObjId = -1;
}

void MainGameInterfaceWnd::OnApproachTown(void* data)
{
    // RVA 0x12B010
    if (data)
    {
        auto const* ev = static_cast<m3d::Event const*>(data);
        m_nearbyTownId = ev->m_intEv[1] ? ev->m_intEv[0] : -1;
        CheckAndShowNearbyTownIco();
    }
}

int MainGameInterfaceWnd::GetCapturedObjId() const
{
    // RVA 0x12AE60
    if (!ai::thePlayer)
    {
        return -1;
    }
    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return -1;
    }
    int const lockedObjId = vehicle->GetLockedObjId();
    if (lockedObjId == -1)
    {
        return -1;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(lockedObjId);
    if (obj)
    {
        unsigned int const flags = obj->GetFlags();
        if ((flags & 8) == 0 && (flags & 2) == 0 && !obj->GetParentRepository() &&
            obj->IsKindOf(&ai::ComplexPhysicObj::m_classComplexPhysicObj))
        {
            return lockedObjId;
        }
    }
    return -1;
}

void MainGameInterfaceWnd::ClearContours()
{
    // RVA 0x12A930
    HideContour(m_contouredInfoObjId);
    m_contouredInfoObjId = -1;
    m_oldInfoObjTolerance = ai::RS_MAX;
    HideContour(m_contouredCapturedObjId);
    m_contouredCapturedObjId = -1;
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

int MainGameInterfaceWnd::GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0x12B240
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("MainGameInterfaceWnd::GameDataLoad error - MainGameInterfaceWnd has been not properly inited");
        return 0;
    }
    GameDataClear(false);
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("MainGameInterfaceWnd::GameDataLoad error - invalid params");
        return 0;
    }

    ref_ptr node = xmlFile->CreateNode();
    guiNode->GetFirstChild(node, "MainGameInterface");
    if (node->IsEmpty())
    {
        M3D_LOG_INFO("MainGameInterfaceWnd::GameDataLoad error - cannot find journal node");
        return 0;
    }

    m3d::SafeIntAttrib(m_nearbyTownId, node, "nearTownId");
    m3d::SafeBoolAttrib(m_bNearbyChests, node, "nearChests");
    m3d::SafeIntAttrib(m_bossId, node, "bossId");
    SetupForBoss(m_bossId);
    return 1;
}

int MainGameInterfaceWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x12A5F0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    switch (dataType)
    {
    case 78:  // 'N' - town ruined
        OnTownRuined(data);
        break;
    case 82:  // 'R' - nearby chests
        OnNearbyChests(data);
        break;
    case 83:  // 'S' - approach town
        OnApproachTown(data);
        break;
    case 89:  // 'Y' - new frame
        OnNewFrame();
        break;
    default:
        break;
    }
    return 1;
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
    // RVA 0x12B650
    if (m_bossId != -1 && !GetBoss())
    {
        SetupForBoss(-1);
    }
}

int MainGameInterfaceWnd::GameDataClear(bool)
{
    // RVA 0x12A5B0
    m_bNearbyChests = false;
    m_nearbyTownId = -1;
    m_bossId = -1;
    CheckAndShowNearbyChestsIco();
    CheckAndShowNearbyTownIco();
    CheckAndShowBossIndicator();
    return 1;
}

void MainGameInterfaceWnd::UpdateCapturedContour()
{
    // RVA 0x12A750
    int const oldCapturedObjId = m_contouredCapturedObjId;
    int const capturedObjId = GetCapturedObjId();
    if (oldCapturedObjId != capturedObjId)
    {
        if (oldCapturedObjId != -1)
        {
            HideContour(oldCapturedObjId);
            m_contouredCapturedObjId = -1;
        }
        if (capturedObjId != -1)
        {
            if (capturedObjId == m_contouredInfoObjId)
            {
                HideContour(m_contouredInfoObjId);
                m_contouredInfoObjId = -1;
                m_oldInfoObjTolerance = ai::RS_MAX;
            }
            ShowContour(
                capturedObjId, ai::theGlobProp.m_colorTargetCaptured, ai::theGlobProp.m_targetCapturedContourWidth);
            m_contouredCapturedObjId = capturedObjId;
        }
    }
}

int MainGameInterfaceWnd::GetInfoObjId() const
{
    // RVA 0x12ADD0
    if (!ai::thePlayer)
    {
        return -1;
    }
    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return -1;
    }
    int const infoObjId = vehicle->GetInfoObjId();
    if (infoObjId == -1)
    {
        return -1;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(infoObjId);
    if (obj)
    {
        unsigned int const flags = obj->GetFlags();
        if ((flags & 8) == 0 && (flags & 2) == 0 && !obj->GetParentRepository() &&
            obj->IsKindOf(&ai::ComplexPhysicObj::m_classComplexPhysicObj))
        {
            return infoObjId;
        }
    }
    return -1;
}

void MainGameInterfaceWnd::ShowInfoContour(int infoObjId)
{
    // RVA 0x12A7D0 (decompile unavailable - body inferred from
    // UpdateInfoContour's call site: it shows the contour with the
    // tolerance-based color and marks infoObjId as the contoured info obj).
    ShowContour(infoObjId, GetColorForInfoContour(infoObjId), ai::theGlobProp.m_targetInfoContourWidth);
    m_contouredInfoObjId = infoObjId;
}

void MainGameInterfaceWnd::ClearInfoContour()
{
    // RVA 0x12A970
    HideContour(m_contouredInfoObjId);
    m_contouredInfoObjId = -1;
    m_oldInfoObjTolerance = ai::RS_MAX;
}

bool MainGameInterfaceWnd::IsInBossMode() const
{
    // RVA 0x12B4A0
    return m_bossId != -1;
}

void MainGameInterfaceWnd::OnNewFrame()
{
    // RVA 0x12A680
    UpdateCapturedContour();
    UpdateInfoContour();
    if (m_bossId != -1 && !GetBoss())
    {
        SetupForBoss(-1);
    }
}
