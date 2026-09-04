#include "localmapwnd.h"
#include "saveselllist.h"
#include "mapinfopanel.h"
#include "localchartwnd.h"
#include "mapcombobox.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include "game/uimanager/truxxuimanager.h"
#include "game/uimisc/guihelper.h"
#include "game/uimisc/levelinfo.h"
#include "game/uimisc/navpoint.h"
#include "game/uimisc/visibilitymap.h"

#include <ui/button.h>
#include <ui/msgbox.h>

#include "server/objects/player.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(LocalMapWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(LocalMapWnd);

namespace
{
    // RVA 0xEB4B0 - orders two ObjectInfos so the one nearer the player's
    // vehicle comes first.
    bool ObjectInfoLessByDistanceFromPlayer(ObjectInfo const* oi1, ObjectInfo const* oi2)
    {
        if (!oi1 || !oi2 || !ai::thePlayer)
        {
            return false;
        }
        ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
        if (!vehicle)
        {
            return false;
        }
        CVector const playerPos = vehicle->GetPosition();
        CVector const d1 = oi1->GetPosition() - playerPos;
        CVector const d2 = oi2->GetPosition() - playerPos;
        return std::sqrt(d2.x * d2.x + d2.y * d2.y + d2.z * d2.z) > std::sqrt(d1.x * d1.x + d1.y * d1.y + d1.z * d1.z);
    }
}  // namespace

LocalMapWnd::AuxInfo::AuxInfo()
{
    m_cbMapsName = "cbMaps";
    m_wndChartName = "wndChart";
    m_wndInfoName = "wndInfo";
    m_wndSaveSellListName = "wndSaveSellList";
    m_wndChartBgName = "wndChartBg";
    m_addNpDlgStr = "AddNavPointMsg";
    m_deleteNpDlgStr = "DeleteNavPointMsg";
    m_btnNavPointName = "btnNavPoint";
    m_btnEnableObjectNamesName = "btnEnableObjectNames";
    m_btnAddTownToSavedListName = "btnSaveList";
    m_wndEmbossBtnNavPointName = "emboss_btnNavPoint";
    m_wndEmbossBtnEnableObjectNamesName = "emboss_btnEnableObjectNames";
    m_wndEmbossBtnAddTownToSavedListName = "emboss_btnSaveList";
    m_strBtnAddTownToSavedList = "BtnSavePrices";
    m_strActive = "active";
    m_strInactive = "inactive";
    m_strOut = "out";
    m_strIn = "in";
    m_strDown = "down";
    m_strIdTooltipSavePrices = "SavePrices";
    m_strIdTooltipRemovePrices = "RemovePrices";
    m_strIdTooltipAddNavPoint = "AddNavPoint";
    m_strIdTooltipDeleteNavPoint = "DeleteNavPoint";
    m_strBtnNavPoint = "NavPointBtn_userLocation";
    m_strEnableObjectNames = "BtnEnableObjectNames";
    m_strIdTooltipShowObjectNames = "ShowObjectNames";
    m_strIdTooltipHideObjectNames = "HideObjectNames";
}

// --- lifecycle -----------------------------------------------------------------------------------

LocalMapWnd::LocalMapWnd()
{
    m_btnAddTownToSavedList = nullptr;
    m_btnNavPoint = nullptr;
    m_btnEnableObjectNames = nullptr;
    m_wndEmbossBtnAddTownToSavedList = nullptr;
    m_wndEmbossBtnNavPoint = nullptr;
    m_wndEmbossBtnEnableObjectNames = nullptr;
}

LocalMapWnd::LocalMapWnd(LocalMapWnd const& rhs) : ScreenWnd(rhs)
{
    // RVA 0x4E7810: fresh window + AuxInfo; every child pointer null.
    m_btnNavPoint = nullptr;
    m_btnAddTownToSavedList = nullptr;
    m_btnEnableObjectNames = nullptr;
    m_wndEmbossBtnNavPoint = nullptr;
    m_wndEmbossBtnAddTownToSavedList = nullptr;
    m_wndEmbossBtnEnableObjectNames = nullptr;
}

LocalMapWnd::~LocalMapWnd()
{
    // RVA 0x4E7860: the ref_ptr members and m_aif release themselves.
}

m3d::Object* LocalMapWnd::Clone()
{
    // RVA 0x4E6B50
    return new LocalMapWnd(*this);
}

m3d::Object* LocalMapWnd::CreateObject()
{
    return new LocalMapWnd;
}

m3d::Class* LocalMapWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Class* LocalMapWnd::GetClass() const
{
    return RT_CLASS_LOCAL(LocalMapWnd);
}

bool LocalMapWnd::IsObjectInfoValidForPrices(ObjectInfo* objectInfo)
{
    // RVA 0x4E9B80 - a non-ruined town.
    return objectInfo && objectInfo->IsTown() && !objectInfo->IsRuined();
}

// --- accessors -----------------------------------------------------------------------------------

ObjectInfo* LocalMapWnd::GetCurrentObjectInfo() const
{
    // RVA 0x4EB380
    if (!m_gameDataFlags)
    {
        return nullptr;
    }
    return m_wndInfo->GetObjectInfo();
}

LocalMapWnd::Mode LocalMapWnd::GetCurrentMode() const
{
    // RVA 0x4E92D0
    if ((m_gameDataFlags & 1) == 0)
    {
        return MODE_TRADE;
    }
    return m_wndInfo->GetCurrentMode();
}

CStr LocalMapWnd::GetCurrentLevelName() const
{
    // RVA 0x4E9310
    if ((m_gameDataFlags & 1) == 0)
    {
        return {};
    }
    return m_cbMaps->GetSelectedMapName();
}

bool LocalMapWnd::IsObjectNamesEnabled() const
{
    // RVA 0x4EAD40
    if ((m_gameDataFlags & 1) == 0)
    {
        return false;
    }
    return m_wndChart->IsObjectNamesEnabled();
}

bool LocalMapWnd::IsObjectInfoSavedList(ObjectInfo* objectInfo) const
{
    // RVA 0x4E9BD0
    if ((m_gameDataFlags & 1) == 0 || !objectInfo)
    {
        return false;
    }
    return m_wndSaveSellList->GetSaveButtonIdByObjectInfo(objectInfo) != -1;
}

// --- map / level -------------------------------------------------------------------------------

int LocalMapWnd::SetUpForLevel(CStr const& levelName)
{
    // RVA 0x4E9360
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    int const levelInfoId = M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelInfoId(levelName);
    if (levelInfoId == -1)
    {
        return 0;
    }
    int const count = m_cbMaps->GetCount();
    for (int i = 0; i < count; ++i)
    {
        if (m_cbMaps->GetItemData(i) == levelInfoId)
        {
            m_cbMaps->SetCurSel(i);
            return 1;
        }
    }
    return 0;
}

int LocalMapWnd::ChangeMap()
{
    // RVA 0x4E9250
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    m_wndChart->SetUpForMap(GetCurrentLevelName());
    UpdateBtnNavPointState();
    return 1;
}

// --- nav points -----------------------------------------------------------------------------

int LocalMapWnd::GetUserLocationNavPointId() const
{
    // RVA 0x4E9800
    CStr const levelName = help::GetCurrentLevelName();
    std::vector<int> const ids = M3D_APP->m_pInterfaceManager->GetNavPointManager()->GetNavPointsByType(
        levelName, NavPoint::NAVPOINT_TYPE_USER_LOCATION);
    return ids.empty() ? -1 : ids[0];
}

bool LocalMapWnd::CanUserLocationNavPointBeAddedFromBtn() const
{
    // RVA 0x4EA2B0
    if ((m_gameDataFlags & 1) == 0)
    {
        return false;
    }
    CStr const curLevel = help::GetCurrentLevelName();
    if (GetCurrentLevelName() != curLevel || GetUserLocationNavPointId() != -1)
    {
        return false;
    }
    ObjectInfo* oi = m_wndInfo->GetObjectInfo();
    return oi && oi->GetLevelName() == curLevel;
}

bool LocalMapWnd::CanUserLocationNavPointBeDeletedFromBtn() const
{
    // RVA 0x4EA3F0
    if ((m_gameDataFlags & 1) == 0)
    {
        return false;
    }
    return GetCurrentLevelName() == help::GetCurrentLevelName() && GetUserLocationNavPointId() != -1;
}

int LocalMapWnd::AddNavPoint(ObjectInfo* objectInfo, bool bWithUserWarning)
{
    // RVA 0x4EA4C0
    // NOTE: the shipped build only proceeds when bWithUserWarning is set; the
    // nav-point button path passes false, so it is effectively a no-op there.
    if (!bWithUserWarning || objectInfo->GetLevelName() != help::GetCurrentLevelName())
    {
        return 0;
    }
    if (M3D_APP->RunMsgBoxDlg({}, M3D_APP->GetStringByStringId0(m_aif.m_addNpDlgStr), 2, false) != m3d::ui::MBX_RET_YES)
    {
        return 1;
    }
    return M3D_APP->m_pInterfaceManager->GetNavPointManager()->AddNavPointUserLocation(
               objectInfo->GetLevelName(), objectInfo->GetPosition()) != -1;
}

int LocalMapWnd::AddNavPoint(CVector const& worldOrigin, bool bWithUserWarning)
{
    // RVA 0x4E94E0
    CStr selLevelName;
    if ((m_gameDataFlags & 1) != 0)
    {
        selLevelName = m_cbMaps->GetSelectedMapName();
    }
    if (selLevelName != help::GetCurrentLevelName())
    {
        return 0;
    }
    if (bWithUserWarning)
    {
        if (M3D_APP->RunMsgBoxDlg({}, M3D_APP->GetStringByStringId0(m_aif.m_addNpDlgStr), 2, false) !=
            m3d::ui::MBX_RET_YES)
        {
            return 1;
        }
    }
    return M3D_APP->m_pInterfaceManager->GetNavPointManager()->AddNavPointUserLocation(selLevelName, worldOrigin) != -1;
}

int LocalMapWnd::DeleteNavPoint(int npId, bool bWithUserWarning)
{
    // RVA 0x4E96E0
    if (bWithUserWarning)
    {
        if (M3D_APP->RunMsgBoxDlg({}, M3D_APP->GetStringByStringId0(m_aif.m_deleteNpDlgStr), 2, false) !=
            m3d::ui::MBX_RET_YES)
        {
            return 1;
        }
    }
    return M3D_APP->m_pInterfaceManager->GetNavPointManager()->RemoveNavPointById(npId) != 0;
}

void LocalMapWnd::OnNavPointAdded(void* data)
{
    // RVA 0x4E97C0
    if (data && static_cast<int*>(data)[16] == 2)
    {
        UpdateBtnNavPointState();
    }
}

void LocalMapWnd::OnNavPointDeleted(void* data)
{
    // RVA 0x4E97E0
    if (data && static_cast<int*>(data)[16] == 2)
    {
        UpdateBtnNavPointState();
    }
}

// --- saved price list ------------------------------------------------------------------------

int LocalMapWnd::SavePricesForCurrentObjectInfo()
{
    // RVA 0x4E91A0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    ObjectInfo* oi = m_wndInfo->GetObjectInfo();
    if (!oi)
    {
        return 0;
    }
    oi->SetSavedPricesPersistant(true);
    int const res = m_wndSaveSellList->AddSaveButton(oi);
    UpdateBtnAddTownToSavedListState();
    UpdateSavedListSelection();
    return res;
}

int LocalMapWnd::RemoveSavedPricesForCurrentObjectInfo()
{
    // RVA 0x4E9110
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    ObjectInfo* oi = m_wndInfo->GetObjectInfo();
    if (!oi)
    {
        return 0;
    }
    int const res = m_wndSaveSellList->RemoveSaveButton(oi) & 1;
    oi->SetSavedPricesPersistant(false);
    UpdateBtnAddTownToSavedListState();
    UpdateSavedListSelection();
    return res;
}

void LocalMapWnd::UpdateSavedListSelection()
{
    // RVA 0x4EA1A0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    int const buttonId = m_wndSaveSellList->GetSaveButtonIdByObjectInfo(m_wndInfo->GetObjectInfo());
    m_wndSaveSellList->SetCurSel(buttonId);
}

void LocalMapWnd::ValidateSavedObjectInfos()
{
    // RVA 0x4EA0D0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    std::vector<ObjectInfo*> savedObjectInfos;
    m_wndSaveSellList->GetSavedObjectInfos(savedObjectInfos);
    for (ObjectInfo* oi : savedObjectInfos)
    {
        if (oi && !IsObjectInfoValidForPrices(oi))
        {
            m_wndSaveSellList->RemoveSaveButton(oi);
            oi->SetSavedPricesPersistant(false);
        }
    }
}

// --- info panel ----------------------------------------------------------------------------

void LocalMapWnd::ShowInfo(ObjectInfo* objectInfo, bool bFromSaveList)
{
    // RVA 0x4E9FF0
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndInfo->SetUpForObject(objectInfo, bFromSaveList);
    }
}

void LocalMapWnd::EnsureShowInfo()
{
    // RVA 0x4EB430
    if ((m_gameDataFlags & 1) != 0 && m_wndInfo->GetObjectInfo() != nullptr)
    {
        return;
    }
    ObjectInfo* nearest = GetInfoForNearestVisibleObject();
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndInfo->SetUpForObject(nearest, false);
    }
}

ObjectInfo* LocalMapWnd::GetInfoForNearestVisibleObject() const
{
    // RVA 0x4EB590
    if (!ai::thePlayer || !ai::thePlayer->GetVehicle())
    {
        return nullptr;
    }
    CStr const curLevel = help::GetCurrentLevelName();
    LevelInfoManager* lim = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();
    VisibilityMap* vm = lim->GetVisibilityMapForLevel(curLevel);
    if (!vm)
    {
        return nullptr;
    }
    auto* objects = lim->GetObjectsForLevel(curLevel);
    if (!objects)
    {
        return nullptr;
    }

    std::vector<ObjectInfo*> list;
    for (auto const& kv : *objects)
    {
        list.push_back(kv.second);
    }
    std::sort(list.begin(), list.end(), ObjectInfoLessByDistanceFromPlayer);
    for (ObjectInfo* oi : list)
    {
        if (oi && vm->IsWorldPositionVisible(oi->GetPosition()))
        {
            return oi;
        }
    }
    return nullptr;
}

void LocalMapWnd::OnObjectInfoChanged()
{
    // RVA 0x4EA040
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    UpdateSavedListSelection();
    UpdateBtnAddTownToSavedListState();
    UpdateBtnNavPointState();
    m_wndChart->SelectMapMarkIcoForObjectInfo(m_wndInfo->GetObjectInfo());
}

// --- object-name toggle -----------------------------------------------------------------

void LocalMapWnd::EnableObjectNames(bool bEnable)
{
    // RVA 0x4EAD00
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndChart->EnableObjectNames(bEnable);
    }
}

// --- button visuals -----------------------------------------------------------------------

void LocalMapWnd::GetBtnAddTownToSavedListTexNames(bool bActive, CStr& icoIdOut, CStr& icoIdIn, CStr& icoIdDown) const
{
    // RVA 0x4E9C20
    CStr const& stateStr = bActive ? m_aif.m_strActive : m_aif.m_strInactive;
    CStr const nameBase = m_aif.m_strBtnAddTownToSavedList + "_" + stateStr + "_";
    icoIdOut = nameBase + m_aif.m_strOut;
    icoIdIn = nameBase + m_aif.m_strIn;
    icoIdDown = nameBase + m_aif.m_strDown;
}

void LocalMapWnd::GetBtnEnableObjectNamesTexNames(bool bActive, CStr& icoIdOut, CStr& icoIdIn, CStr& icoIdDown) const
{
    // RVA 0x4EAFB0
    CStr const& stateStr = bActive ? m_aif.m_strActive : m_aif.m_strInactive;
    CStr const nameBase = m_aif.m_strEnableObjectNames + "_" + stateStr + "_";
    icoIdOut = nameBase + m_aif.m_strOut;
    icoIdIn = nameBase + m_aif.m_strIn;
    icoIdDown = nameBase + m_aif.m_strDown;
}

void LocalMapWnd::GetBtnNavPointTexNames(bool bActive, CStr& icoIdOut, CStr& icoIdIn, CStr& icoIdDown) const
{
    // RVA 0x4EA8B0
    CStr const& stateStr = bActive ? m_aif.m_strActive : m_aif.m_strInactive;
    CStr const nameBase = m_aif.m_strBtnNavPoint + "_" + stateStr + "_";
    icoIdOut = nameBase + m_aif.m_strOut;
    icoIdIn = nameBase + m_aif.m_strIn;
    icoIdDown = nameBase + m_aif.m_strDown;
}

void LocalMapWnd::UpdateBtnAddTownToSavedListState()
{
    // RVA 0x4E9920
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ObjectInfo* oi = m_wndInfo->GetObjectInfo();
    if (!IsObjectInfoValidForPrices(oi))
    {
        m_btnAddTownToSavedList->ShowWindow(false);
        m_wndEmbossBtnAddTownToSavedList->ShowWindow(false);
        return;
    }
    m_btnAddTownToSavedList->ShowWindow(true);
    m_wndEmbossBtnAddTownToSavedList->ShowWindow(true);

    bool const bIsTownInSavedList = IsObjectInfoSavedList(oi);
    CStr icoIdOut, icoIdIn, icoIdDown;
    GetBtnAddTownToSavedListTexNames(bIsTownInSavedList, icoIdOut, icoIdIn, icoIdDown);
    auto* mgr = M3D_APP->m_pInterfaceManager;
    // NOTE: the shipped build passes the down/in textures in swapped order.
    m_btnAddTownToSavedList->SetImaged(
        mgr->GetIcoByName(icoIdOut, 0), mgr->GetIcoByName(icoIdDown, 0), mgr->GetIcoByName(icoIdIn, 0), {});

    CStr const& strIdTooltip = bIsTownInSavedList ? m_aif.m_strIdTooltipRemovePrices : m_aif.m_strIdTooltipSavePrices;
    CStr tooltip = M3D_APP->GetStringByStringId0(strIdTooltip);
    m_btnAddTownToSavedList->SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

void LocalMapWnd::UpdateBtnEnableObjectNamesState()
{
    // RVA 0x4EAD80
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    bool const namesOn = m_wndChart->IsObjectNamesEnabled();

    CStr icoIdOut, icoIdIn, icoIdDown;
    GetBtnEnableObjectNamesTexNames(!namesOn, icoIdOut, icoIdIn, icoIdDown);
    auto* mgr = M3D_APP->m_pInterfaceManager;
    m_btnEnableObjectNames->SetImaged(
        mgr->GetIcoByName(icoIdOut, 0), mgr->GetIcoByName(icoIdDown, 0), mgr->GetIcoByName(icoIdIn, 0), {});

    CStr const& strIdTooltip = namesOn ? m_aif.m_strIdTooltipHideObjectNames : m_aif.m_strIdTooltipShowObjectNames;
    CStr tooltip = M3D_APP->GetStringByStringId0(strIdTooltip);
    m_btnEnableObjectNames->SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

void LocalMapWnd::UpdateBtnNavPointState()
{
    // RVA 0x4EA650
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    bool canAdd = false;
    if (CanUserLocationNavPointBeDeletedFromBtn())
    {
        m_btnNavPoint->ShowWindow(true);
        m_wndEmbossBtnNavPoint->ShowWindow(true);
    }
    else if (CanUserLocationNavPointBeAddedFromBtn())
    {
        m_btnNavPoint->ShowWindow(true);
        m_wndEmbossBtnNavPoint->ShowWindow(true);
        canAdd = true;
    }
    else
    {
        m_btnNavPoint->ShowWindow(false);
        m_wndEmbossBtnNavPoint->ShowWindow(false);
        return;
    }

    CStr icoIdOut, icoIdIn, icoIdDown;
    GetBtnNavPointTexNames(!canAdd, icoIdOut, icoIdIn, icoIdDown);
    auto* mgr = M3D_APP->m_pInterfaceManager;
    m_btnNavPoint->SetImaged(
        mgr->GetIcoByName(icoIdOut, 0), mgr->GetIcoByName(icoIdDown, 0), mgr->GetIcoByName(icoIdIn, 0), {});

    CStr const& strIdTooltip = canAdd ? m_aif.m_strIdTooltipAddNavPoint : m_aif.m_strIdTooltipDeleteNavPoint;
    CStr tooltip = M3D_APP->GetStringByStringId0(strIdTooltip);
    m_btnNavPoint->SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

// --- button clicks -------------------------------------------------------------------------

void LocalMapWnd::OnBtnAddTownToSavedListClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    // RVA 0x4E98C0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ObjectInfo* oi = m_wndInfo->GetObjectInfo();
    if (IsObjectInfoValidForPrices(oi))
    {
        if (IsObjectInfoSavedList(oi))
        {
            RemoveSavedPricesForCurrentObjectInfo();
        }
        else
        {
            SavePricesForCurrentObjectInfo();
        }
    }
}

void LocalMapWnd::OnBtnNavPointClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    // RVA 0x4EA230
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (CanUserLocationNavPointBeDeletedFromBtn())
    {
        M3D_APP->m_pInterfaceManager->GetNavPointManager()->RemoveNavPointById(GetUserLocationNavPointId());
    }
    else if (CanUserLocationNavPointBeAddedFromBtn())
    {
        AddNavPoint(m_wndInfo->GetObjectInfo(), false);
    }
}

void LocalMapWnd::OnBtnEnableObjectNamesClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    // RVA 0x4EAC80
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndChart->EnableObjectNames(!m_wndChart->IsObjectNamesEnabled());
}

void LocalMapWnd::LaunchGlobalMap()
{
    // RVA 0x4E9450
    M3D_APP->EnqueueMessage(65677, 0, 0, 0, 0, {}, {});
}

// --- notifications / wnd-station hooks -------------------------------------------------

int LocalMapWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x4E8E30
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((dataType == 28 || dataType == 29) && data && static_cast<int*>(data)[16] == 2)
    {
        UpdateBtnNavPointState();
    }
    return 1;
}

int LocalMapWnd::OnAfterAddToWndStation()
{
    // RVA 0x4EB760
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    return res;
}

int LocalMapWnd::OnBeforeAddToWndStation()
{
    // RVA 0x4EB3C0
    int const res = m3d::ui::Wnd::OnBeforeAddToWndStation();
    ValidateSavedObjectInfos();
    if (m_gameDataFlags != 0 && m_wndInfo->GetObjectInfo() != nullptr)
    {
        OnObjectInfoChanged();
    }
    else
    {
        EnsureShowInfo();
    }
    return res;
}

int LocalMapWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0x4E8E70
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (ChildPanel::OnWndNotify(from, idFrom, message, data))
    {
        return 1;
    }

    switch (idFrom)
    {
    case 0x190:  // map combo box selection changed
        if (message == 5)
        {
            ChangeMap();
            return 1;
        }
        return 0;

    case 0x194:  // saved-price list
        if (from && from->IsKindOf(RT_CLASS_LOCAL(SaveSellButton)) && message == 21)
        {
            ShowInfo(static_cast<SaveSellButton*>(from)->GetObjectInfo(), true);
        }
        return 1;

    case 0x195:  // "save prices" button
        if (message != 1)
        {
            return 0;
        }
        OnBtnAddTownToSavedListClick(from, idFrom, data);
        return 1;

    case 0x196:  // nav-point button
        if (message != 1)
        {
            return 0;
        }
        OnBtnNavPointClick(from, idFrom, data);
        return 1;

    case 0x197:  // "global map" button
        if (message != 1)
        {
            return 0;
        }
        LaunchGlobalMap();
        return 1;

    case 0x199:  // chart
        switch (message)
        {
        case 0x17:  // right-click: add a user-location nav point at the world position
            if (m_wndChart)
            {
                CVector2 const range = data.GetAsRange();
                PointBase<float> const wndPt{range.x, range.y};
                CVector const worldPos = m_wndChart->WndPtToWorldPos(wndPt, GetCurrentLevelName());
                // NOTE: the shipped build passes an uninitialised bWithUserWarning here.
                AddNavPoint(worldPos, true);
            }
            return 1;

        case 0x18:  // delete a nav point by id (user-location only)
        {
            int const npId = data.GetAsID();
            NavPointManager* npm = M3D_APP->m_pInterfaceManager->GetNavPointManager();
            NavPoint const* np = npm->GetNavPointById(npId);
            if (np && np->GetNavPointType() == NavPoint::NAVPOINT_TYPE_USER_LOCATION)
            {
                npm->RemoveNavPointById(npId);
            }
            return 1;
        }

        case 0x19:  // show info for the clicked object
            ShowInfo(reinterpret_cast<ObjectInfo*>(data.GetAsID()), false);
            return 1;

        case 0x1B:  // object-name visibility toggled on the chart
            UpdateBtnEnableObjectNamesState();
            return 0;

        default:
            return 0;
        }

    case 0x19B:  // info panel
        if (message != 26)
        {
            return 0;
        }
        OnObjectInfoChanged();
        return 1;

    case 0x19C:  // "enable object names" button
        if (message != 1)
        {
            return 0;
        }
        OnBtnEnableObjectNamesClick(from, idFrom, data);
        return 1;

    default:
        return 0;
    }
}

int LocalMapWnd::GameDataSetup()
{
    // RVA 0x4E78F0
    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        // The three tool buttons and their emboss backgrounds are plain children.
        auto getButton = [&](CStr const& name, m3d::ui::ButtonWnd*& out)
        {
            out = RT_DYNCAST(GetChildByName(name), m3d::ui::ButtonWnd);
            if (!out)
            {
                M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
                res = 0;
            }
        };
        auto getWnd = [&](CStr const& name, m3d::ui::Wnd*& out)
        {
            out = RT_DYNCAST(GetChildByName(name), m3d::ui::Wnd);
            if (!out)
            {
                M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
                res = 0;
            }
        };
        getButton(m_aif.m_btnAddTownToSavedListName, m_btnAddTownToSavedList);
        getButton(m_aif.m_btnNavPointName, m_btnNavPoint);
        getButton(m_aif.m_btnEnableObjectNamesName, m_btnEnableObjectNames);
        getWnd(m_aif.m_wndEmbossBtnAddTownToSavedListName, m_wndEmbossBtnAddTownToSavedList);
        getWnd(m_aif.m_wndEmbossBtnNavPointName, m_wndEmbossBtnNavPoint);
        getWnd(m_aif.m_wndEmbossBtnEnableObjectNamesName, m_wndEmbossBtnEnableObjectNames);

        // The chart / info panel / save-sell list / map combo box are all RTTI
        // widgets built from a template child.
        if (auto* child = RT_DYNCAST(GetChildByName(m_aif.m_wndChartName), m3d::ui::Wnd))
        {
            m_wndChart = static_cast<LocalChartWnd*>(M3D_KERNEL->New("LocalChartWnd"));
            if (m_wndChart)
            {
                if (!m_wndChart->CreateFromPattern(child, true))
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndChartName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndChartName +
                    " - cannot find rtti class LocalChartWnd");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_wndChartName + " is not found or incorrect type");
            res = 0;
        }

        if (auto* child = RT_DYNCAST(GetChildByName(m_aif.m_wndInfoName), m3d::ui::Wnd))
        {
            m_wndInfo = static_cast<MapInfoPanel*>(M3D_KERNEL->New("MapInfoPanel"));
            if (m_wndInfo)
            {
                if (!m_wndInfo->CreateFromPattern(child, true))
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndInfoName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndInfoName +
                    " - cannot find rtti class MapInfoPanel");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_wndInfoName + " is not found or incorrect type");
            res = 0;
        }

        if (auto* child = RT_DYNCAST(GetChildByName(m_aif.m_wndSaveSellListName), m3d::ui::Wnd))
        {
            m_wndSaveSellList = static_cast<SaveSellList*>(M3D_KERNEL->New("SaveSellList"));
            if (m_wndSaveSellList)
            {
                if (!m_wndSaveSellList->CreateFromPattern(child, true))
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + m_aif.m_wndSaveSellListName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndSaveSellListName +
                    " - cannot find rtti class SaveSellList");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Make control error: control " + m_aif.m_wndSaveSellListName + " is not found or incorrect type");
            res = 0;
        }

        if (auto* child = RT_DYNCAST(GetChildByName(m_aif.m_cbMapsName), m3d::ui::ComboBoxWnd))
        {
            m_cbMaps = static_cast<MapComboBox*>(M3D_KERNEL->New("MapComboBox"));
            if (m_cbMaps)
            {
                if (!m_cbMaps->CreateFromPattern(child, true))
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_cbMapsName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_cbMapsName +
                    " - cannot find rtti class MapComboBox");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_cbMapsName + " is not found or incorrect type");
            res = 0;
        }

        // NOTE: the shipped code asserts m_cbMaps is non-null here even when its
        // creation failed above.
        MoveChildToFirstPosition(m_cbMaps.get());

        auto* chartBg = RT_DYNCAST(GetChildByName(m_aif.m_wndChartBgName), m3d::ui::Wnd);
        if (chartBg)
        {
            MoveChildToLastPosition(chartBg);
            if (res)
            {
                M3D_APP->m_pInterfaceManager->AddWindowById(m_cbMaps.get(), 81, true, true);
                M3D_APP->m_pInterfaceManager->AddWindowById(m_wndChart.get(), 83, true, true);
                M3D_APP->m_pInterfaceManager->AddWindowById(m_wndInfo.get(), 85, true, true);
                M3D_APP->m_pInterfaceManager->AddWindowById(m_wndSaveSellList.get(), 86, true, true);
                m_gameDataFlags |= 1u;
                ShowInfo(nullptr, false);
                UpdateBtnEnableObjectNamesState();
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndChartBgName + " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("LocalMapWnd: error - fail to init because of a bad resource");
    return 0;
}
