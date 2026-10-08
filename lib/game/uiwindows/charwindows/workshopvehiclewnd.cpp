#include "workshopvehiclewnd.h"

#include "skinswnd.h"

#include <algorithm>

#include <core/aiparam.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimanager/uidefs.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/geomrepository.h>
#include <server/geomrepositoryitem.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/town.h>
#include <server/objects/vehicle.h>
#include <server/objects/workshop.h>
#include <server/server.h>
#include <ui/button.h>
#include <game/uiwindows/commonwindows/complexmodelwnd.h>
#include <ui/modelwnd.h>

namespace
{
    // Control ids of the six buttons this window reacts to.
    unsigned const ID_BTN_NEXT = 300300;
    unsigned const ID_BTN_PREV = 300301;
    unsigned const ID_BTN_BUY = 300302;
    unsigned const ID_BTN_EXIT = 300303;
    unsigned const ID_BTN_SKIN_NEXT = 300304;
    unsigned const ID_BTN_SKIN_PREV = 300305;

    // GameDataUpdate data types this window listens to.
    int const DATATYPE_FINISH_TRADE = 34;
    int const DATATYPE_REPOSITORY_CHANGED = 58;

    // Application messages: open the new vehicle's inventory, hide this window,
    // add an object to the encyclopaedia, and the generic character-screen
    // refresh.
    int const UM_SHOW_INVENTORY = 65671;
    int const UM_HIDE_WINDOW = 65674;
    int const UM_ADD_TO_ENCYCLOPAEDIA = 66557;
    int const UM_REFRESH_CHAR_SCREEN = 65691;

    // Argument of UM_HIDE_WINDOW for this window.
    int const HIDE_WINDOW_ARG = 67;

    // A skin number at or above this is not one of the three common paint jobs.
    unsigned const SKIN_LIMIT = 8;
}  // namespace

RT_CLASS_EXPORTS_BEGIN(WorkshopVehicleWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WorkshopVehicleWnd);

WorkshopVehicleWnd::WAuxInfo::WAuxInfo()
{
    // RVA 0x48ACF0
    m_btnPrevName = "btnPrev";
    m_btnNextName = "btnNext";
    m_btnSkinNextName = "btnSkinNext";
    m_btnSkinPrevName = "btnSkinPrev";
    m_wndPriceName = "wndPrice";
}

// ---------------------------------------------------------------------------
// construction / RTTI
// ---------------------------------------------------------------------------

WorkshopVehicleWnd::WorkshopVehicleWnd()
{
    // RVA 0x48AE40
    m_btnPrev = nullptr;
    m_btnNext = nullptr;
    m_btnSkinNext = nullptr;
    m_btnSkinPrev = nullptr;
    m_skinSwitcher = nullptr;
    m_wndPrice = nullptr;
    m_workshopId = -1;
}

WorkshopVehicleWnd::WorkshopVehicleWnd(WorkshopVehicleWnd const&)
{
    // RVA 0x48AF60 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves the child window pointers, the skin
    // switcher and m_workshopId uninitialised.
}

WorkshopVehicleWnd::~WorkshopVehicleWnd()
{
    // RVA 0x48AFA0
    delete m_skinSwitcher;
    m_skinSwitcher = nullptr;
    m_vehicleObjIdList.clear();
}

m3d::Object* WorkshopVehicleWnd::Clone()
{
    // RVA 0x48AC40
    return new WorkshopVehicleWnd(*this);
}

m3d::Object* WorkshopVehicleWnd::CreateObject()
{
    return new WorkshopVehicleWnd;
}

m3d::Class* WorkshopVehicleWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(VehicleWnd);
}

m3d::Class* WorkshopVehicleWnd::GetClass() const
{
    return RT_CLASS_LOCAL(WorkshopVehicleWnd);
}

// ---------------------------------------------------------------------------
// resource setup
// ---------------------------------------------------------------------------

int WorkshopVehicleWnd::GameDataSetup()
{
    // RVA 0x48B010
    if (!VehicleWnd::GameDataSetup())
    {
        return 0;
    }

    if ((m_gameDataFlags & 2) == 0)
    {
        // The shared vehicle model window is registered under its own gui id so
        // that other windows can pull it in front of this one.
        int res =
            M3D_APP->m_pInterfaceManager->AddWindowById(m_wndVehicleModel.get(), IW_WND_WORKSHOP_VEHICLE_MODEL, true, false) & 1;
        MoveChildToLastPosition(static_cast<m3d::ui::Wnd*>(m_wndVehicleModel.get()));

        m3d::Object* btnPrev = GetChildByName(m_waif.m_btnPrevName);
        if (btnPrev && btnPrev->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnPrev = static_cast<m3d::ui::ButtonWnd*>(btnPrev);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_waif.m_btnPrevName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* btnNext = GetChildByName(m_waif.m_btnNextName);
        if (btnNext && btnNext->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnNext = static_cast<m3d::ui::ButtonWnd*>(btnNext);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_waif.m_btnNextName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* btnSkinNext = GetChildByName(m_waif.m_btnSkinNextName);
        if (btnSkinNext && btnSkinNext->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnSkinNext = static_cast<m3d::ui::ButtonWnd*>(btnSkinNext);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_waif.m_btnSkinNextName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* btnSkinPrev = GetChildByName(m_waif.m_btnSkinPrevName);
        if (btnSkinPrev && btnSkinPrev->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnSkinPrev = static_cast<m3d::ui::ButtonWnd*>(btnSkinPrev);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_waif.m_btnSkinPrevName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* wndPrice = GetChildByName(m_waif.m_wndPriceName);
        if (wndPrice && wndPrice->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndPrice = static_cast<m3d::ui::Wnd*>(wndPrice);
            if (res)
            {
                m_skinSwitcher = new SkinSwitcher;
                ShowTab(TAB_ID_VEHICLE_CHARACTERISTICS);
                SetVehicleType(m_vehicleType);
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_waif.m_wndPriceName + " is not found or incorrect type");
            // NOTE: the shipped code clears the ready bit that VehicleWnd set
            // rather than tracking a local failure, so an earlier failed lookup
            // that still found wndPrice leaves the window marked ready.
            m_gameDataFlags &= ~1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_INFO("WorkshopVehicleWnd: error - fail to init because of a bad resource");
    return 0;
}

int WorkshopVehicleWnd::GameDataClear(bool)
{
    // RVA 0x48B520
    SetVehicleId(-1);
    UpdateNextPrevButtonsStates();
    m_vehicleObjIdList.clear();
    m_workshopId = -1;
    return 1;
}

int WorkshopVehicleWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x48B580
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    if (dataType == DATATYPE_FINISH_TRADE)
    {
        OnFinishTrade(data);
    }
    else if (dataType == DATATYPE_REPOSITORY_CHANGED)
    {
        if (data && IsChildOf(M3D_APP) &&
            static_cast<ai::GeomRepository**>(data)[13] == GetWorkshopRepository())
        {
            OnRepositoryChanged();
            return 1;
        }
    }
    else
    {
        return VehicleWnd::GameDataUpdate(data, dataType);
    }
    return 1;
}

int WorkshopVehicleWnd::OnBeforeAddToWndStation()
{
    // RVA 0x48B6E0
    OnRepositoryChanged();
    AddInfoToEncyclopaedia();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int WorkshopVehicleWnd::OnAfterAddToWndStation()
{
    // RVA 0x48BFB0
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(UM_REFRESH_CHAR_SCREEN, 0, 0, 0, 0, CStr(), m3d::AIParam());
    return res;
}

int WorkshopVehicleWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x48BC70
    if ((m_style & m3d::ui::WS_REFLECT_NOTIFY_MESSAGES_TO_PARENT) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }

    if (id == 1)
    {
        if (msg != 1)
        {
            return 0;
        }
        OnExit();
        return 1;
    }

    if (msg != 1)
    {
        return 0;
    }

    switch (id)
    {
    case ID_BTN_NEXT:
        ShowNextVehicle();
        break;
    case ID_BTN_PREV:
        ShowPrevVehicle();
        break;
    case ID_BTN_BUY:
        LaunchNewVehicleInventoryWnd();
        ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_NEW_VEHICLE_SELECT);
        break;
    case ID_BTN_EXIT:
        Hide();
        break;
    case ID_BTN_SKIN_NEXT:
        m_skinSwitcher->ShowNextSkin();
        break;
    case ID_BTN_SKIN_PREV:
        m_skinSwitcher->ShowPrevSkin();
        break;
    default:
        return 0;
    }
    return 1;
}

// ---------------------------------------------------------------------------
// workshop / vehicle list
// ---------------------------------------------------------------------------

int WorkshopVehicleWnd::GetWorkshopId() const
{
    // RVA 0x48B620
    return m_workshopId;
}

ai::Workshop* WorkshopVehicleWnd::GetWorkshop() const
{
    // RVA 0x48B630
    if (m_workshopId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_workshopId);
    if (obj && obj->IsKindOf(&ai::Workshop::m_classWorkshop))
    {
        return static_cast<ai::Workshop*>(obj);
    }
    return nullptr;
}

ai::GeomRepository* WorkshopVehicleWnd::GetWorkshopRepository() const
{
    // RVA 0x48B8A0
    ai::Workshop* workshop = GetWorkshop();
    return workshop ? workshop->GetRepositoryByType(ai::WORKSHOP_VEHICLES) : nullptr;
}

int WorkshopVehicleWnd::SetupForWorkshop(int workshopId)
{
    // RVA 0x48B690
    m_workshopId = workshopId;
    if (IsChildOf(M3D_APP))
    {
        OnRepositoryChanged();
    }
    if (m_workshopId == -1)
    {
        return 1;
    }
    return GetWorkshop() != nullptr;
}

int WorkshopVehicleWnd::MakeVehiclesList()
{
    // RVA 0x48B7F0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    m_vehicleObjIdList.clear();

    ai::Workshop* workshop = GetWorkshop();
    if (workshop)
    {
        if (ai::GeomRepository* repository = workshop->GetRepositoryByType(ai::WORKSHOP_VEHICLES))
        {
            int const numItems = static_cast<int>(repository->GetNumItems());
            for (int i = 0; i < numItems; ++i)
            {
                ai::GeomRepositoryItem const item = repository->GetItem(i);
                if (ai::Obj* obj = item.GetObj())
                {
                    m_vehicleObjIdList.push_back(obj->GetId());
                }
            }
        }
    }
    return 1;
}

void WorkshopVehicleWnd::AddInfoToEncyclopaedia()
{
    // RVA 0x48B700 - every vehicle on offer becomes a known object.
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return;
    }
    ai::GeomRepository* repository = workshop->GetRepositoryByType(ai::WORKSHOP_VEHICLES);
    if (!repository)
    {
        return;
    }

    int const numItems = static_cast<int>(repository->GetNumItems());
    for (int i = 0; i < numItems; ++i)
    {
        ai::GeomRepositoryItem const item = repository->GetItem(i);
        M3D_APP->EnqueueMessage(UM_ADD_TO_ENCYCLOPAEDIA, item.GetObjId(), 0, 0, 0, CStr(), m3d::AIParam());
    }
}

int WorkshopVehicleWnd::GetNextObjId(int curVehicleObjId) const
{
    // RVA 0x48B980
    if (curVehicleObjId == -1)
    {
        return -1;
    }
    auto it = std::find(begin(m_vehicleObjIdList), end(m_vehicleObjIdList), curVehicleObjId);
    if (it == end(m_vehicleObjIdList))
    {
        return -1;
    }
    ++it;
    return it == end(m_vehicleObjIdList) ? -1 : *it;
}

int WorkshopVehicleWnd::GetPrevObjId(int curVehicleObjId) const
{
    // RVA 0x48B9C0
    if (curVehicleObjId == -1)
    {
        return -1;
    }
    auto const it = std::find(begin(m_vehicleObjIdList), end(m_vehicleObjIdList), curVehicleObjId);
    if (it == end(m_vehicleObjIdList) || it == begin(m_vehicleObjIdList))
    {
        return -1;
    }
    return *(it - 1);
}

void WorkshopVehicleWnd::ShowNextVehicle()
{
    // RVA 0x48B8C0
    SetVehicleId(GetNextObjId(m_vehicleId));
}

void WorkshopVehicleWnd::ShowPrevVehicle()
{
    // RVA 0x48B920
    SetVehicleId(GetPrevObjId(m_vehicleId));
}

void WorkshopVehicleWnd::UpdateNextPrevButtonsStates()
{
    // RVA 0x48BA00
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_btnNext->EnableWindow(GetNextObjId(m_vehicleId) != -1);
    m_btnPrev->EnableWindow(GetPrevObjId(m_vehicleId) != -1);
}

void WorkshopVehicleWnd::OnRepositoryChanged()
{
    // RVA 0x48BB40 - the stock changed, so rebuild the list and try to stay on
    // the vehicle that was on show; failing that fall back to the one that would
    // have come next (or previous), and finally to the first one on offer.
    int const curVehicleObjId = m_vehicleId;
    int fallbackObjId = GetNextObjId(curVehicleObjId);
    if (fallbackObjId == -1)
    {
        fallbackObjId = GetPrevObjId(curVehicleObjId);
    }

    MakeVehiclesList();

    if (m_vehicleObjIdList.empty())
    {
        SetVehicleId(-1);
        return;
    }
    if (std::find(begin(m_vehicleObjIdList), end(m_vehicleObjIdList), curVehicleObjId) != end(m_vehicleObjIdList))
    {
        SetVehicleId(curVehicleObjId);
        return;
    }
    if (std::find(begin(m_vehicleObjIdList), end(m_vehicleObjIdList), fallbackObjId) == end(m_vehicleObjIdList))
    {
        SetVehicleId(m_vehicleObjIdList.front());
        return;
    }
    SetVehicleId(fallbackObjId);
}

void WorkshopVehicleWnd::FullUpdate()
{
    // RVA 0x48BC60 - folded onto OnRepositoryChanged in the shipped build.
    OnRepositoryChanged();
}

// ---------------------------------------------------------------------------
// the shown vehicle
// ---------------------------------------------------------------------------

void WorkshopVehicleWnd::SetVehicleId(int vehicleId)
{
    // RVA 0x48BD80
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    // Remember the skin of the vehicle that is on show so the next one comes up
    // in the same paint job.
    if (ai::Vehicle const* oldVehicle = GetVehicle())
    {
        int const skin = static_cast<int>(oldVehicle->GetSkin());
        if (skin != -1)
        {
            m_curSkin = skin;
        }
    }

    VehicleWnd::SetVehicleId(vehicleId);
    UpdateNextPrevButtonsStates();

    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        m_skinSwitcher->SetUp(-1, m_btnSkinNext, m_btnSkinPrev);
        m_wndPrice->SetText(CStr());
        return;
    }

    if (static_cast<unsigned>(m_curSkin) >= SKIN_LIMIT)
    {
        m_curSkin = 0;
        SkinSwitcher::ApplySkin(0, vehicle);
    }
    else
    {
        vehicle->SetSkin(vehicle->CheckSkin(m_curSkin));
    }
    m_skinSwitcher->SetUp(vehicle->GetId(), m_btnSkinNext, m_btnSkinPrev);

    ai::Town const* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (town)
    {
        m_wndPrice->SetText(CStr(help::GetBuyPriceByObjId(vehicleId, town->GetId())));
    }
    else
    {
        m_wndPrice->SetText(CStr());
    }
}

void WorkshopVehicleWnd::OnRestoreStyles()
{
    // RVA 0x48BEC0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    UpdateNextPrevButtonsStates();
    m_skinSwitcher->SetUp(m_vehicleId, m_btnSkinNext, m_btnSkinPrev);
}

void WorkshopVehicleWnd::LaunchNewVehicleInventoryWnd()
{
    // RVA 0x48BA90
    if (m_vehicleId == -1)
    {
        return;
    }
    M3D_APP->EnqueueMessage(
        UM_SHOW_INVENTORY, m_vehicleId, m_vehicleId, m_workshopId, 0, CStr(), m3d::AIParam());
}

void WorkshopVehicleWnd::Hide()
{
    // RVA 0x48BF00
    M3D_APP->EnqueueMessage(UM_HIDE_WINDOW, HIDE_WINDOW_ARG, 0, 0, 0, CStr(), m3d::AIParam());
}

void WorkshopVehicleWnd::OnFinishTrade(void* data)
{
    // RVA 0x48BF90 - a completed purchase closes the showroom; a cancelled one
    // leaves it up.
    if ((m_gameDataFlags & 1) != 0 && data && static_cast<int*>(data)[13] && !static_cast<int*>(data)[14])
    {
        Hide();
    }
}
