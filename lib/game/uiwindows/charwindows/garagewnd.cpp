#include "garagewnd.h"

#include "basketlist.h"
#include "cabinlist.h"
#include "rechargelist.h"
#include "refuellist.h"
#include "repairlist.h"
#include "skinswnd.h"

#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/uidefs.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/geomrepository.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/prototypemanager.h>
#include <server/objects/guns/compoundgun.h>
#include <server/objects/guns/gun.h>
#include <server/objects/physicbodies/vehiclepart.h>
#include <server/objects/player.h>
#include <server/resourcemanager.h>
#include <server/objects/town.h>
#include <server/objects/vehicle.h>
#include <server/server.h>
#include <ui/button.h>

RT_CLASS_EXPORTS_BEGIN(GarageWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GarageWnd);

GarageWnd::AuxInfo GarageWnd::m_aif;

namespace
{
    // Ids the XML gives the service buttons.
    unsigned const ID_BTN_CABINS = 300300;
    unsigned const ID_BTN_BASKETS = 300301;
    unsigned const ID_BTN_NEW_VEHICLE = 300302;
    unsigned const ID_BTN_SKIN = 300303;
    unsigned const ID_BTN_REFUEL_ALL = 300304;
    unsigned const ID_BTN_REFUEL_LIST = 300305;
    unsigned const ID_BTN_REPAIR_ALL = 300306;
    unsigned const ID_BTN_REPAIR_LIST = 300307;
    unsigned const ID_BTN_RECHARGE_ALL = 300308;
    unsigned const ID_BTN_RECHARGE_LIST = 300309;
    unsigned const ID_WND_SKIN_LIST = 300310;

    // The skin list reports a purchase with this CallParentNotify message.
    unsigned const MSG_BUY_SKIN = 21;

    // Below half of what is needed reads as red, all of it as green.
    int const VALUE_COLOR_YELLOW_THRESHOLD = 50;
    int const VALUE_COLOR_GREEN_THRESHOLD = 100;
}  // namespace

// ===========================================================================
//  GarageWnd::AuxInfo
// ===========================================================================

GarageWnd::AuxInfo::AuxInfo()
{
    // RVA 0x446840
    m_btnCabinsName = "btnCabins";
    m_btnBasketsName = "btnBaskets";
    m_btnNewVehicleName = "btnNewVehicle";
    m_btnSkinName = "btnSkin";
    m_btnRefuelAllName = "btnRefuelAll";
    m_btnRefuelListName = "btnRefuelList";
    m_wndRefuelPriceName = "wndRefuelPrice";
    m_btnRepairAllName = "btnRepairAll";
    m_btnRepairListName = "btnRepairList";
    m_wndRepairPriceName = "wndRepairPrice";
    m_btnRechargeAllName = "btnRechargeAll";
    m_btnRechargeListName = "btnRechargeList";
    m_wndRechargePriceName = "wndRechargePrice";

    m_btnBasketsTexName = "ServiceBtnBaskets_";
    m_btnCabinsTexName = "ServiceBtnCabins_";
    m_btnSkinTexName = "ServiceBtnSkin_";
    m_btnRefuelListTexName = "ServiceBtnRefuelList_";
    m_btnRepairListTexName = "ServiceBtnRepairList_";
    m_btnRechargeListTexName = "ServiceBtnRechargeList_";

    m_colorYellow = 0xFFFFFF00u;
    m_colorGreen = 0xFF00FF00u;
    m_colorRed = 0xFFFF0000u;
    m_colorTooltipRed = 0xFF904040u;
    m_colorNormal = 0u;

    m_strIdTooltipRefuel = "RefuelMax";
    m_strIdTooltipRefuelNotNeed = "RefuelNotNeed";
    m_strIdTooltipRefuelUnavailable = "RefuelUnavailable";
    m_strIdTooltipRefuelPartial = "RefuelPartial";
    m_strIdTooltipRefuelFull = "RefuelFull";

    m_strIdTooltipRepair = "RepairMax";
    m_strIdTooltipRepairNotNeed = "RepairNotNeed";
    m_strIdTooltipRepairUnavailable = "RepairUnavailable";
    m_strIdTooltipRepairPartial = "RepairPartial";
    m_strIdTooltipRepairFull = "RepairFull";

    m_strIdTooltipRecharge = "RechargeMax";
    m_strIdTooltipRechargeNotNeed = "RechargeNotNeed";
    m_strIdTooltipRechargeUnavailable = "RechargeUnavailable";
    m_strIdTooltipRechargePartial = "RechargePartial";
    m_strIdTooltipRechargeFull = "RechargeFull";

    m_strIdState = "State";
    m_strEnabled = "enabled";
    m_strOverrolled = "overrolled";
    m_strPressed = "pressed";
    m_strDisabled = "disabled";
    m_strSelected = "selected";
}

GarageWnd::AuxInfo::AuxInfo(GarageWnd::AuxInfo const& rhs) :
    m_btnCabinsName(rhs.m_btnCabinsName),
    m_btnBasketsName(rhs.m_btnBasketsName),
    m_btnNewVehicleName(rhs.m_btnNewVehicleName),
    m_btnSkinName(rhs.m_btnSkinName),
    m_btnRefuelAllName(rhs.m_btnRefuelAllName),
    m_btnRefuelListName(rhs.m_btnRefuelListName),
    m_wndRefuelPriceName(rhs.m_wndRefuelPriceName),
    m_btnRepairAllName(rhs.m_btnRepairAllName),
    m_btnRepairListName(rhs.m_btnRepairListName),
    m_wndRepairPriceName(rhs.m_wndRepairPriceName),
    m_btnRechargeAllName(rhs.m_btnRechargeAllName),
    m_btnRechargeListName(rhs.m_btnRechargeListName),
    m_wndRechargePriceName(rhs.m_wndRechargePriceName),
    m_btnBasketsTexName(rhs.m_btnBasketsTexName),
    m_btnCabinsTexName(rhs.m_btnCabinsTexName),
    m_btnSkinTexName(rhs.m_btnSkinTexName),
    m_btnRefuelListTexName(rhs.m_btnRefuelListTexName),
    m_btnRepairListTexName(rhs.m_btnRepairListTexName),
    m_btnRechargeListTexName(rhs.m_btnRechargeListTexName),
    m_colorYellow(rhs.m_colorYellow),
    m_colorGreen(rhs.m_colorGreen),
    m_colorRed(rhs.m_colorRed),
    m_colorTooltipRed(rhs.m_colorTooltipRed),
    m_colorNormal(rhs.m_colorNormal),
    m_strIdTooltipRefuel(rhs.m_strIdTooltipRefuel),
    m_strIdTooltipRefuelNotNeed(rhs.m_strIdTooltipRefuelNotNeed),
    m_strIdTooltipRefuelUnavailable(rhs.m_strIdTooltipRefuelUnavailable),
    m_strIdTooltipRefuelPartial(rhs.m_strIdTooltipRefuelPartial),
    m_strIdTooltipRefuelFull(rhs.m_strIdTooltipRefuelFull),
    m_strIdTooltipRepair(rhs.m_strIdTooltipRepair),
    m_strIdTooltipRepairNotNeed(rhs.m_strIdTooltipRepairNotNeed),
    m_strIdTooltipRepairUnavailable(rhs.m_strIdTooltipRepairUnavailable),
    m_strIdTooltipRepairPartial(rhs.m_strIdTooltipRepairPartial),
    m_strIdTooltipRepairFull(rhs.m_strIdTooltipRepairFull),
    m_strIdTooltipRecharge(rhs.m_strIdTooltipRecharge),
    m_strIdTooltipRechargeNotNeed(rhs.m_strIdTooltipRechargeNotNeed),
    m_strIdTooltipRechargeUnavailable(rhs.m_strIdTooltipRechargeUnavailable),
    m_strIdTooltipRechargePartial(rhs.m_strIdTooltipRechargePartial),
    m_strIdTooltipRechargeFull(rhs.m_strIdTooltipRechargeFull),
    m_strIdState(rhs.m_strIdState),
    m_strEnabled(rhs.m_strEnabled),
    m_strOverrolled(rhs.m_strOverrolled),
    m_strPressed(rhs.m_strPressed),
    m_strDisabled(rhs.m_strDisabled),
    m_strSelected(rhs.m_strSelected)
{
}

// ===========================================================================
//  GarageWnd - construction
// ===========================================================================

GarageWnd::GarageWnd() :
    m_btnCabins(nullptr),
    m_btnBaskets(nullptr),
    m_btnNewVehicle(nullptr),
    m_btnSkin(nullptr),
    m_btnRefuelAll(nullptr),
    m_btnRefuelList(nullptr),
    m_wndRefuelPrice(nullptr),
    m_btnRepairAll(nullptr),
    m_btnRepairList(nullptr),
    m_wndRepairPrice(nullptr),
    m_btnRechargeAll(nullptr),
    m_btnRechargeList(nullptr),
    m_wndRechargePrice(nullptr),
    m_curListButton(nullptr),
    m_curAllButton(nullptr),
    m_workshopId(-1),
    m_unitsToRepair(0),
    m_maxPossibleUnitsToRepair(0),
    m_repairPrice(0),
    m_prevUnitsToRepair(0),
    m_prevMaxPossibleUnitsToRepair(0),
    m_prevRepairPrice(0),
    m_unitsToRefuel(0),
    m_maxPossibleUnitsToRefuel(0),
    m_refuelPrice(0),
    m_prevUnitsToRefuel(0),
    m_prevMaxPossibleUnitsToRefuel(0),
    m_prevRefuelPrice(0),
    m_unitsToRecharge(0),
    m_maxPossibleUnitsToRecharge(0),
    m_rechargePrice(0),
    m_prevUnitsToRecharge(0),
    m_prevMaxPossibleUnitsToRecharge(0),
    m_prevRechargePrice(0)
{
}

GarageWnd::GarageWnd(GarageWnd const&) : GarageWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x4475F0) only builds the base, nulls the
    // seven list ref_ptrs and resets the two ChildPanel ids; everything else is
    // left uninitialised and nothing is copied from rhs.
}

GarageWnd::~GarageWnd() = default;

m3d::Object* GarageWnd::Clone()
{
    // RVA 0x4461F0
    return new GarageWnd(*this);
}

m3d::Object* GarageWnd::CreateObject()
{
    return new GarageWnd;
}

m3d::Class* GarageWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Class* GarageWnd::GetClass() const
{
    return RT_CLASS_LOCAL(GarageWnd);
}

// ===========================================================================
//  Setup
// ===========================================================================

int GarageWnd::GameDataSetup()
{
    // RVA 0x447710 - collects the thirteen service controls from the XML, then
    // borrows the six list windows from the interface manager by id.
    int res = 1;

    auto const findButton = [&](CStr const& name, m3d::ui::ButtonWnd*& out)
    {
        m3d::Object* child = GetChildByName(name);
        if (child && child->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            out = static_cast<m3d::ui::ButtonWnd*>(child);
            return;
        }
        M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
        res = 0;
    };
    auto const findWnd = [&](CStr const& name, m3d::ui::Wnd*& out)
    {
        m3d::Object* child = GetChildByName(name);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            out = static_cast<m3d::ui::Wnd*>(child);
            return;
        }
        M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
        res = 0;
    };

    findButton(m_aif.m_btnCabinsName, m_btnCabins);
    findButton(m_aif.m_btnBasketsName, m_btnBaskets);
    findButton(m_aif.m_btnNewVehicleName, m_btnNewVehicle);
    findButton(m_aif.m_btnSkinName, m_btnSkin);
    findButton(m_aif.m_btnRefuelAllName, m_btnRefuelAll);
    findButton(m_aif.m_btnRefuelListName, m_btnRefuelList);
    findWnd(m_aif.m_wndRefuelPriceName, m_wndRefuelPrice);
    findButton(m_aif.m_btnRepairAllName, m_btnRepairAll);
    findButton(m_aif.m_btnRepairListName, m_btnRepairList);
    findWnd(m_aif.m_wndRepairPriceName, m_wndRepairPrice);
    findButton(m_aif.m_btnRechargeAllName, m_btnRechargeAll);
    findButton(m_aif.m_btnRechargeListName, m_btnRechargeList);
    findWnd(m_aif.m_wndRechargePriceName, m_wndRechargePrice);

    if (res)
    {
        // The lists live in the interface manager rather than under this window;
        // they are only parented in while their service is open.
        auto const getList = [&](int wndId, m3d::Class const* cls) -> m3d::ui::Wnd*
        {
            ref_ptr<m3d::ui::Wnd> wnd = M3D_APP->m_pInterfaceManager->GetWindow(wndId);
            if (wnd && wnd->IsKindOf(cls))
            {
                return wnd.get();
            }
            res = 0;
            return nullptr;
        };

        m_wndRefuelList = static_cast<RefuelList*>(getList(IW_WND_REFUEL_LIST, &RefuelList::m_classRefuelList));
        m_wndRepairList = static_cast<RepairList*>(getList(IW_WND_REPAIR_LIST, &RepairList::m_classRepairList));
        m_wndRechargeList =
            static_cast<RechargeList*>(getList(IW_WND_RECHARGE_LIST, &RechargeList::m_classRechargeList));
        m_wndCabinsList = static_cast<CabinList*>(getList(IW_WND_CABIN_LIST, &CabinList::m_classCabinList));
        m_wndBasketsList = static_cast<BasketList*>(getList(IW_WND_BASKET_LIST, &BasketList::m_classBasketList));
        m_wndSkinList = static_cast<SkinsWnd*>(getList(IW_WND_SKINS, &SkinsWnd::m_classSkinsWnd));
    }

    if (res)
    {
        m_gameDataFlags |= 1u;
        return 1;
    }

    M3D_LOG_INFO("GarageWnd: error - fail to init because of a bad resource");
    return 0;
}

int GarageWnd::SetupForWorkshop(int workshopId)
{
    // RVA 0x44A080 - hands the workshop down to all five stock lists.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    m_workshopId = workshopId;
    int res = m_wndRefuelList->SetupForWorkshop(m_workshopId) & 1;
    res &= m_wndRepairList->SetupForWorkshop(m_workshopId);
    res &= m_wndRechargeList->SetupForWorkshop(m_workshopId);
    res &= m_wndCabinsList->SetupForWorkshop(m_workshopId);
    res &= m_wndBasketsList->SetupForWorkshop(m_workshopId);

    if (m_workshopId != -1 && !GetWorkshop())
    {
        return 0;
    }
    if (IsChildOf(M3D_APP))
    {
        FullUpdate();
    }
    return res;
}

int GarageWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x448C50
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    switch (dataType)
    {
    case 34:
        OnFinishTrade(data);
        break;
    case 58:
        OnRepositoryChanged(data);
        break;
    case 64:
        OnPlayerVehicleChanged();
        break;
    case 89:
        OnNewFrame();
        break;
    }
    return 1;
}

int GarageWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x448D10
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (ChildPanel::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }

    // Every service button answers a plain click; only the skin list reports a
    // purchase, and it does so with its own message.
    if (id == ID_WND_SKIN_LIST)
    {
        if (msg != MSG_BUY_SKIN)
        {
            return 0;
        }
        OnBuySkin();
        return 1;
    }
    if (msg != 1)
    {
        return 0;
    }

    switch (id)
    {
    case ID_BTN_CABINS:
        OnCabins();
        return 1;
    case ID_BTN_BASKETS:
        OnBaskets();
        return 1;
    case ID_BTN_NEW_VEHICLE:
        OnNewVehicle();
        return 1;
    case ID_BTN_SKIN:
        OnSkin();
        return 1;
    case ID_BTN_REFUEL_ALL:
        OnRefuelAll();
        return 1;
    case ID_BTN_REFUEL_LIST:
        OnRefuelList();
        return 1;
    case ID_BTN_REPAIR_ALL:
        OnRepairAll();
        return 1;
    case ID_BTN_REPAIR_LIST:
        OnRepairList();
        return 1;
    case ID_BTN_RECHARGE_ALL:
        OnRechargeAll();
        return 1;
    case ID_BTN_RECHARGE_LIST:
        OnRechargeList();
        return 1;
    default:
        return 0;
    }
}

int GarageWnd::OnBeforeAddToWndStation()
{
    // RVA 0x449750
    CloseCurrentList();
    UpdateActiveStates();
    return ChildPanel::OnBeforeAddToWndStation();
}

int GarageWnd::OnAfterAddToWndStation()
{
    // RVA 0x44C630
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, CStr(), m3d::AIParam());
    return res;
}

int GarageWnd::OnAfterRemoveFromWndStation()
{
    // RVA 0x4497A0
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    CloseCurrentList();
    return res;
}

void GarageWnd::FullUpdate()
{
    // RVA 0x4497C0
    CloseCurrentList();
    UpdateActiveStates();
}

// ===========================================================================
//  Service lists
// ===========================================================================

void GarageWnd::OpenList(
    ref_ptr<m3d::ui::Wnd> list,
    m3d::ui::ButtonWnd* listButton,
    m3d::ui::ButtonWnd* allButton,
    ai::eGameEvent eventId)
{
    // RVA 0x449480
    if ((m_gameDataFlags & 1) == 0 || !list || !listButton || list == m_curList)
    {
        return;
    }

    CloseCurrentList();
    if (!IsDirectChild(list))
    {
        AddChild(list);
    }

    m_curList = list;
    m_curAllButton = allButton;
    m_curListButton = listButton;

    std::vector<m3d::ui::Wnd*> controls;
    controls.push_back(listButton);
    UpdateServiceSelectState(controls, true);

    if (eventId != ai::GE_NUM_EVENTS)
    {
        ai::pServer->PostPlayerEvent(eventId);
    }
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, CStr(), m3d::AIParam());
}

void GarageWnd::CloseCurrentList()
{
    // RVA 0x449610
    if ((m_gameDataFlags & 1) == 0 || !m_curList)
    {
        return;
    }

    if (IsDirectChild(m_curList))
    {
        RemoveChild(m_curList);
    }
    if (!m_curListButton)
    {
        return;
    }

    std::vector<m3d::ui::Wnd*> controls;
    controls.push_back(m_curListButton);
    UpdateServiceSelectState(controls, false);

    m_curList = nullptr;
    m_curListButton = nullptr;
    m_curAllButton = nullptr;
}

bool GarageWnd::IsListOpen() const
{
    // RVA 0x44C620
    return m_curList != nullptr;
}

void GarageWnd::OnCabins()
{
    // RVA 0x448E80 - the service buttons toggle their list.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (m_curList && m_curList.get() == static_cast<m3d::ui::Wnd*>(m_wndCabinsList.get()))
    {
        CloseCurrentList();
        return;
    }
    OpenList(m_wndCabinsList.get(), m_btnCabins, nullptr, ai::GE_TUTORIAL_CABINS);
}

void GarageWnd::OnBaskets()
{
    // RVA 0x448F20
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (m_curList && m_curList.get() == static_cast<m3d::ui::Wnd*>(m_wndBasketsList.get()))
    {
        CloseCurrentList();
        return;
    }
    OpenList(m_wndBasketsList.get(), m_btnBaskets, nullptr, ai::GE_TUTORIAL_BASKETS);
}

void GarageWnd::OnNewVehicle()
{
    // RVA 0x448FC0 - buying a whole vehicle is handled by another window.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    M3D_APP->EnqueueMessage(65674, 73, 0, 0, 0, CStr(), m3d::AIParam());
    ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_NEW_VEHICLE);
}

void GarageWnd::OnSkin()
{
    // RVA 0x449070
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (m_curList && m_curList.get() == static_cast<m3d::ui::Wnd*>(m_wndSkinList.get()))
    {
        CloseCurrentList();
        return;
    }
    OpenList(m_wndSkinList.get(), m_btnSkin, nullptr, ai::GE_TUTORIAL_SKIN);
}

void GarageWnd::OnRefuelAll()
{
    // RVA 0x449110
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    CloseCurrentList();
    MaxRefuel();
}

void GarageWnd::OnRefuelList()
{
    // RVA 0x449130
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (m_curList && m_curList.get() == static_cast<m3d::ui::Wnd*>(m_wndRefuelList.get()))
    {
        CloseCurrentList();
        return;
    }
    OpenList(m_wndRefuelList.get(), m_btnRefuelList, m_btnRefuelAll, ai::GE_TUTORIAL_REFUEL);
}

void GarageWnd::OnRepairAll()
{
    // RVA 0x4491D0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    CloseCurrentList();
    MaxRepair();
}

void GarageWnd::OnRepairList()
{
    // RVA 0x4491F0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (m_curList && m_curList.get() == static_cast<m3d::ui::Wnd*>(m_wndRepairList.get()))
    {
        CloseCurrentList();
        return;
    }
    OpenList(m_wndRepairList.get(), m_btnRepairList, m_btnRepairAll, ai::GE_TUTORIAL_REPAIR);
}

void GarageWnd::OnRechargeAll()
{
    // RVA 0x449290
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    CloseCurrentList();
    MaxRecharge();
}

void GarageWnd::OnRechargeList()
{
    // RVA 0x4492B0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (m_curList && m_curList.get() == static_cast<m3d::ui::Wnd*>(m_wndRechargeList.get()))
    {
        CloseCurrentList();
        return;
    }
    OpenList(m_wndRechargeList.get(), m_btnRechargeList, m_btnRechargeAll, ai::GE_TUTORIAL_RECHARGE);
}

// ===========================================================================
//  Active / selected state
// ===========================================================================

void GarageWnd::UpdateServiceActiveState(
    std::vector<m3d::ui::Wnd*, std::allocator<m3d::ui::Wnd*>> const& controls,
    bool bActive,
    ref_ptr<m3d::ui::Wnd> list)
{
    // RVA 0x4496D0 - a service that has just become unavailable also has its list
    // closed if that list happens to be the one on screen.
    if (!bActive && list && list == m_curList)
    {
        CloseCurrentList();
    }
    for (auto* control : controls)
    {
        if (control)
        {
            control->EnableWindow(bActive);
        }
    }
}

void GarageWnd::UpdateServiceSelectState(
    std::vector<m3d::ui::Wnd*, std::allocator<m3d::ui::Wnd*>> const& controls,
    bool bSelect)
{
    // RVA 0x44B7F0 - a selected service button collapses to a single "selected"
    // image; an unselected one gets the usual four-state set back.
    for (auto* control : controls)
    {
        if (!control || !control->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            continue;
        }

        CStr texId;
        switch (control->GetId())
        {
        case ID_BTN_CABINS:
            texId = m_aif.m_btnCabinsTexName;
            break;
        case ID_BTN_BASKETS:
            texId = m_aif.m_btnBasketsTexName;
            break;
        case ID_BTN_SKIN:
            texId = m_aif.m_btnSkinTexName;
            break;
        case ID_BTN_REFUEL_LIST:
            texId = m_aif.m_btnRefuelListTexName;
            break;
        case ID_BTN_REPAIR_LIST:
            texId = m_aif.m_btnRepairListTexName;
            break;
        case ID_BTN_RECHARGE_LIST:
            texId = m_aif.m_btnRechargeListTexName;
            break;
        default:
            continue;
        }

        auto* btn = static_cast<m3d::ui::ButtonWnd*>(control);
        auto const ico = [&](CStr const& suffix)
        { return M3D_APP->m_pInterfaceManager->GetIcoByName(texId + suffix, 0); };

        if (bSelect)
        {
            btn->SetImaged(ico(m_aif.m_strSelected), {}, {}, {});
        }
        else
        {
            btn->SetImaged(
                ico(m_aif.m_strEnabled),
                ico(m_aif.m_strOverrolled),
                ico(m_aif.m_strPressed),
                ico(m_aif.m_strDisabled));
        }
    }
}

void GarageWnd::UpdateActiveStates()
{
    // RVA 0x449810
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    UpdateCabinsActiveState();
    UpdateBasketsActiveState();
    UpdateNewVehicleActiveState();
    UpdateSkinActiveState();
    UpdateRefuelActiveState();
    UpdateRepairActiveState();
    UpdateRechargeActiveState();
}

void GarageWnd::UpdateCabinsActiveState()
{
    // RVA 0x449850 - the service is offered only when the workshop actually has a
    // compatible cabin in stock.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    std::vector<m3d::ui::Wnd*> controls;
    controls.push_back(m_btnCabins);

    ai::Vehicle const* vehicle = help::GetPlayerVehicle();
    std::vector<int> compatibleIds;
    help::GetCompatibleVehiclePartsFromWorkshop(
        m_workshopId,
        ai::theResourceManager->GetResourceId(CStr("CABIN")),
        vehicle ? vehicle->GetId() : -1,
        compatibleIds);

    UpdateServiceActiveState(controls, !compatibleIds.empty(), m_wndCabinsList.get());
}

void GarageWnd::UpdateBasketsActiveState()
{
    // RVA 0x449980
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    std::vector<m3d::ui::Wnd*> controls;
    controls.push_back(m_btnBaskets);

    ai::Vehicle const* vehicle = help::GetPlayerVehicle();
    std::vector<int> compatibleIds;
    help::GetCompatibleVehiclePartsFromWorkshop(
        m_workshopId,
        ai::theResourceManager->GetResourceId(CStr("BASKET")),
        vehicle ? vehicle->GetId() : -1,
        compatibleIds);

    UpdateServiceActiveState(controls, !compatibleIds.empty(), m_wndBasketsList.get());
}

void GarageWnd::UpdateNewVehicleActiveState()
{
    // RVA 0x449AB0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    std::vector<m3d::ui::Wnd*> controls;
    controls.push_back(m_btnNewVehicle);

    bool bActive = false;
    if (help::GetPlayerVehicle())
    {
        if (ai::Workshop* workshop = GetWorkshop())
        {
            ai::GeomRepository const* repository = workshop->GetRepositoryByType(ai::WORKSHOP_VEHICLES);
            bActive = repository && repository->GetNumItems() != 0;
        }
    }

    UpdateServiceActiveState(controls, bActive, nullptr);
}

void GarageWnd::UpdateSkinActiveState()
{
    // RVA 0x449B70 - the button lights up only when at least one skin the vehicle
    // does not already wear can be applied to it.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    std::vector<m3d::ui::Wnd*> controls;
    controls.push_back(m_btnSkin);

    ai::Vehicle* vehicle = help::GetPlayerVehicle();
    if (!vehicle)
    {
        UpdateServiceActiveState(controls, false, m_wndSkinList.get());
        return;
    }

    bool bNewSkinFound = false;
    int const maxSkinCount = SkinSwitcher::GetMaxSkinCount(vehicle);
    for (int i = 0; i < maxSkinCount; ++i)
    {
        if (SkinSwitcher::IsSkinApplicable(i, vehicle))
        {
            bNewSkinFound = true;
            break;
        }
    }
    UpdateServiceActiveState(controls, bNewSkinFound, m_wndSkinList.get());
}

void GarageWnd::UpdateRefuelActiveState()
{
    // RVA 0x449CC0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    std::vector<m3d::ui::Wnd*> controls;
    controls.push_back(m_btnRefuelAll);
    controls.push_back(m_btnRefuelList);

    bool const bActive = help::GetPlayerVehicle() != nullptr && GetUnitsToRefuel() != 0;
    UpdateServiceActiveState(controls, bActive, m_wndRefuelList.get());
}

void GarageWnd::UpdateRepairActiveState()
{
    // RVA 0x449DD0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    std::vector<m3d::ui::Wnd*> controls;
    controls.push_back(m_btnRepairAll);
    controls.push_back(m_btnRepairList);

    bool const bActive = help::GetPlayerVehicle() != nullptr && GetUnitsToRepair() != 0;
    UpdateServiceActiveState(controls, bActive, m_wndRepairList.get());
}

void GarageWnd::UpdateRechargeActiveState()
{
    // RVA 0x449EE0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    std::vector<m3d::ui::Wnd*> controls;
    controls.push_back(m_btnRechargeAll);
    controls.push_back(m_btnRechargeList);

    bool const bActive = help::GetPlayerVehicle() != nullptr && GetUnitsToRecharge() != 0;
    UpdateServiceActiveState(controls, bActive, m_wndRechargeList.get());
}

// ===========================================================================
//  Workshop accessors
// ===========================================================================

int GarageWnd::GetWorkshopId() const
{
    // RVA 0x449FF0
    return m_workshopId;
}

ai::Workshop* GarageWnd::GetWorkshop() const
{
    // RVA 0x44A000
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

ai::GeomRepository* GarageWnd::GetWorkshopRepositoryByType(ai::WorkshopRepositoryType repositoryType) const
{
    // RVA 0x44A060
    ai::Workshop* workshop = GetWorkshop();
    return workshop ? workshop->GetRepositoryByType(repositoryType) : nullptr;
}

int GarageWnd::GetTownId() const
{
    // RVA 0x44BB20
    ai::Workshop* workshop = GetWorkshop();
    return workshop ? workshop->GetParentId() : -1;
}

// ===========================================================================
//  How much work each service has to do
// ===========================================================================

int GarageWnd::GetUnitsToRefuel() const
{
    // RVA 0x44A1D0
    ai::Vehicle* vehicle = help::GetPlayerVehicle();
    if (!vehicle)
    {
        return 0;
    }
    return static_cast<int>(vehicle->Fuel().maxValue().get() - vehicle->Fuel().value().get());
}

int GarageWnd::GetUnitsToRepair() const
{
    // RVA 0x44A230 - the vehicle's own health plus every part except the chassis
    // (the chassis is what the health value already stands for).
    ai::Vehicle* vehicle = help::GetPlayerVehicle();
    if (!vehicle)
    {
        return 0;
    }

    int unitsToRepair = static_cast<int>(vehicle->Health().maxValue().get() - vehicle->Health().value().get());
    for (auto it = vehicle->begin(); it != vehicle->end(); ++it)
    {
        auto const& [partName, part] = *it;
        if (partName == CStr("CHASSIS") || !part)
        {
            continue;
        }
        unitsToRepair += static_cast<int>(part->Durability().maxValue().get() - part->Durability().value().get());
    }
    return unitsToRepair;
}

int GarageWnd::GetUnitsToRecharge() const
{
    // RVA 0x44A360
    ai::Vehicle* vehicle = help::GetPlayerVehicle();
    if (!vehicle)
    {
        return 0;
    }

    int units = 0;
    for (auto it = vehicle->begin(); it != vehicle->end(); ++it)
    {
        auto const& [partName, part] = *it;
        if (!part || (!part->IsKindOf(&ai::Gun::m_classGun) && !part->IsKindOf(&ai::CompoundGun::m_classCompoundGun)))
        {
            continue;
        }
        if (!help::CanGunBeReloaded(part))
        {
            continue;
        }
        units += static_cast<int>(help::GetGunShellsPoolSize(part)) +
                 static_cast<int>(help::GetGunChargeSize(part)) -
                 static_cast<int>(help::GetGunShellsInPool(part)) -
                 static_cast<int>(help::GetGunShellsInCurrentCharge(part));
    }
    return units;
}

// ===========================================================================
//  Prices
// ===========================================================================

float GarageWnd::GetHealthPriceForOneUnit() const
{
    // RVA 0x44A990
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return 0.0f;
    }
    float const price = workshop->GetHealthPriceForOneUnit();
    return price >= 0.0f ? price : 0.0f;
}

int GarageWnd::GetFuelPriceForOneUnit() const
{
    // RVA 0x44A9C0
    ai::Vehicle* vehicle = help::GetPlayerVehicle();
    if (!vehicle)
    {
        return 0;
    }
    int const price = help::GetFuelPriceForOneUnit(vehicle->GetId(), GetTownId());
    return price < 0 ? 0 : price;
}

int GarageWnd::GetShellPrice(int shellPrototypeId) const
{
    // RVA 0x44AA10
    if (shellPrototypeId == -1)
    {
        return 0;
    }
    ai::PrototypeInfo const* prototypeInfo = ai::thePrototypeManager->GetPrototypeInfo(shellPrototypeId);
    if (!prototypeInfo)
    {
        return 0;
    }
    int const price = static_cast<int>(prototypeInfo->GetBasePrice());
    return price < 0 ? 0 : price;
}

void GarageWnd::GetPossibleRefuel(int& units, int& price) const
{
    // RVA 0x44A400 - how much of the missing fuel the player can actually pay for.
    units = 0;
    price = 0;

    ai::Vehicle* vehicle = help::GetPlayerVehicle();
    if (vehicle)
    {
        int const money = static_cast<int>(ai::thePlayer->GetMoney());
        int pricePerUnit = GetFuelPriceForOneUnit();
        if (!pricePerUnit)
        {
            pricePerUnit = 1;
        }

        units = GetUnitsToRefuel();
        if (units)
        {
            price = pricePerUnit * units;
            if (money < price)
            {
                units = money / pricePerUnit;
                price = pricePerUnit * units;
            }
        }
    }

    // NOTE: a broke player buys nothing at all, and any non-zero amount always
    // costs at least one coin.
    if (!ai::thePlayer->GetMoney())
    {
        units = 0;
        price = 0;
    }
    else if (units && !price)
    {
        price = 1;
    }
}

void GarageWnd::GetPossibleRepair(int& units, int& price) const
{
    // RVA 0x44A4A0 - the vehicle's own health first, then part by part until the
    // money runs out; the part that the money runs out on is repaired pro rata.
    units = 0;
    price = 0;

    ai::Vehicle* vehicle = help::GetPlayerVehicle();
    ai::Workshop* workshop = GetWorkshop();
    if (vehicle && workshop)
    {
        int const money = static_cast<int>(ai::thePlayer->GetMoney());
        float pricePerUnit = workshop->GetHealthPriceForOneUnit();
        if (pricePerUnit <= 0.0f)
        {
            pricePerUnit = 1.0f;
        }

        int const healthUnits =
            static_cast<int>(vehicle->Health().maxValue().get() - vehicle->Health().value().get());
        units = healthUnits;
        price = static_cast<int>(static_cast<float>(healthUnits) * pricePerUnit);

        if (units && money < price)
        {
            units = static_cast<int>(static_cast<float>(money) / pricePerUnit);
            price = static_cast<int>(static_cast<float>(units) * pricePerUnit);
        }
        else
        {
            for (auto it = vehicle->begin(); it != vehicle->end(); ++it)
            {
                auto const& [partName, part] = *it;
                if (!part)
                {
                    continue;
                }
                int const partPrice = workshop->GetObjectRepairPrice(part);
                int const moneyLeft = money - price;
                int const partUnits =
                    static_cast<int>(part->Durability().maxValue().get() - part->Durability().value().get());

                if (moneyLeft >= partPrice)
                {
                    if (partUnits)
                    {
                        units += partUnits;
                        price += partPrice;
                    }
                }
                else
                {
                    if (partUnits)
                    {
                        int const affordableUnits = static_cast<int>(
                            static_cast<float>(partUnits) * static_cast<float>(moneyLeft) /
                            static_cast<float>(partPrice));
                        if (affordableUnits)
                        {
                            units += affordableUnits;
                            price += affordableUnits * (partPrice / partUnits);
                        }
                        break;
                    }
                }
            }
        }
    }

    if (!ai::thePlayer->GetMoney())
    {
        units = 0;
        price = 0;
    }
    else if (units && !price)
    {
        price = 1;
    }
}

void GarageWnd::GetPossibleRecharge(int& units, int& price) const
{
    // RVA 0x44A6B0 - gun by gun, stopping at the one the money runs out on.
    units = 0;
    price = 0;

    ai::Vehicle* vehicle = help::GetPlayerVehicle();
    if (vehicle)
    {
        int const money = static_cast<int>(ai::thePlayer->GetMoney());

        retruxx::vector<ai::Obj*> guns;
        help::GetGunsForVehicle(vehicle->GetId(), guns);

        for (auto* gun : guns)
        {
            if (!help::CanGunBeReloaded(gun))
            {
                continue;
            }

            int const shellsNeeded = static_cast<int>(help::GetGunChargeSize(gun)) -
                                     static_cast<int>(help::GetGunShellsInCurrentCharge(gun)) +
                                     static_cast<int>(help::GetGunShellsPoolSize(gun)) -
                                     static_cast<int>(help::GetGunShellsInPool(gun));
            if (!shellsNeeded)
            {
                continue;
            }

            int shellPrototypeId = -1;
            if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
            {
                shellPrototypeId = g->GetShellPrototypeId();
            }
            else if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
            {
                shellPrototypeId = cg->GetShellPrototypeId();
            }

            int const shellPrice = GetShellPrice(shellPrototypeId);
            if (shellPrice <= 0)
            {
                units += shellsNeeded;
                continue;
            }

            int const moneyLeft = money - price;
            if (moneyLeft < shellsNeeded * shellPrice)
            {
                units += moneyLeft / shellPrice;
                price += shellPrice * units;
                break;
            }
            units += shellsNeeded;
            price += shellsNeeded * shellPrice;
        }
    }

    if (!ai::thePlayer->GetMoney())
    {
        units = 0;
        price = 0;
    }
    else if (units && !price)
    {
        price = 1;
    }
}

// ===========================================================================
//  Doing the work
// ===========================================================================

void GarageWnd::MaxRefuel()
{
    // RVA 0x449350
    if (!help::GetPlayerVehicle())
    {
        return;
    }
    int units = 0;
    int price = 0;
    GetPossibleRefuel(units, price);
    if (!units)
    {
        return;
    }
    Refuel(units);
    ai::thePlayer->AddMoney(-price);
}

void GarageWnd::MaxRepair()
{
    // RVA 0x4493C0
    if (!help::GetPlayerVehicle())
    {
        return;
    }
    int units = 0;
    int price = 0;
    GetPossibleRepair(units, price);
    if (!units)
    {
        return;
    }
    Repair(units);
    ai::thePlayer->AddMoney(-price);
}

void GarageWnd::MaxRecharge()
{
    // RVA 0x449420
    if (!help::GetPlayerVehicle())
    {
        return;
    }
    int units = 0;
    int price = 0;
    GetPossibleRecharge(units, price);
    if (!units)
    {
        return;
    }
    Recharge(units);
    ai::thePlayer->AddMoney(-price);
}

void GarageWnd::Refuel(int units)
{
    // RVA 0x44BB40
    ai::Vehicle* vehicle = help::GetPlayerVehicle();
    if (!vehicle)
    {
        return;
    }
    vehicle->Fuel().value().set(static_cast<float>(units) + vehicle->Fuel().value().get());
}

void GarageWnd::Repair(int units)
{
    // RVA 0x44BBD0 - the vehicle's own health is topped up first; whatever is
    // left over is spent on the parts in map order, skipping the chassis.
    ai::Vehicle* vehicle = help::GetPlayerVehicle();
    if (!vehicle)
    {
        return;
    }

    int const healthUnits =
        static_cast<int>(vehicle->Health().maxValue().get() - vehicle->Health().value().get());
    if (healthUnits > units)
    {
        vehicle->Health().value().set(vehicle->Health().value().get() + static_cast<float>(units));
        return;
    }

    vehicle->Health().value().set(vehicle->Health().value().get() + static_cast<float>(healthUnits));
    int unitsLeft = units - healthUnits;

    for (auto it = vehicle->begin(); it != vehicle->end(); ++it)
    {
        auto const& [partName, part] = *it;
        if (!part || partName == CStr("CHASSIS"))
        {
            continue;
        }
        int const partUnits =
            static_cast<int>(part->Durability().maxValue().get() - part->Durability().value().get());
        if (partUnits > unitsLeft)
        {
            part->Durability().value().set(part->Durability().value().get() + static_cast<float>(unitsLeft));
            return;
        }
        part->Durability().value().set(part->Durability().value().get() + static_cast<float>(partUnits));
        unitsLeft -= partUnits;
    }
}

void GarageWnd::Recharge(int units)
{
    // RVA 0x44BD50 - shells go into the current charge first, then the pool.
    ai::Vehicle* vehicle = help::GetPlayerVehicle();
    if (!vehicle)
    {
        return;
    }

    retruxx::vector<ai::Obj*> guns;
    help::GetGunsForVehicle(vehicle->GetId(), guns);

    int unitsLeft = units;
    for (auto* gun : guns)
    {
        if (unitsLeft <= 0)
        {
            break;
        }
        if (!help::CanGunBeReloaded(gun))
        {
            continue;
        }

        int const chargeNeeded = static_cast<int>(help::GetGunChargeSize(gun)) -
                                 static_cast<int>(help::GetGunShellsInCurrentCharge(gun));
        int const poolNeeded =
            static_cast<int>(help::GetGunShellsPoolSize(gun)) - static_cast<int>(help::GetGunShellsInPool(gun));

        int const intoCharge = (chargeNeeded > unitsLeft) ? unitsLeft : chargeNeeded;
        if (intoCharge > 0)
        {
            unsigned const shells =
                static_cast<unsigned>(static_cast<int>(help::GetGunShellsInCurrentCharge(gun)) + intoCharge);
            if (auto* g = RT_DYNCAST(gun, ai::Gun))
            {
                g->SetShellsInCurrentCharge(shells);
            }
            else if (auto* cg = RT_DYNCAST(gun, ai::CompoundGun))
            {
                cg->SetShellsInCurrentCharge(shells);
            }
            unitsLeft -= intoCharge;
        }

        int const intoPool = (poolNeeded > unitsLeft) ? unitsLeft : poolNeeded;
        if (intoPool > 0)
        {
            unsigned const shells =
                static_cast<unsigned>(static_cast<int>(help::GetGunShellsInPool(gun)) + intoPool);
            if (auto* g = RT_DYNCAST(gun, ai::Gun))
            {
                g->SetShellsInPool(shells);
            }
            else if (auto* cg = RT_DYNCAST(gun, ai::CompoundGun))
            {
                cg->SetShellsInPool(shells);
            }
            unitsLeft -= intoPool;
        }
    }
}

// ===========================================================================
//  Price labels and tooltips
// ===========================================================================

help::Color GarageWnd::GetValueColor(int value, int maxValue)
{
    // RVA 0x44AA70
    if (!maxValue)
    {
        return help::COLOR_WHITE;
    }
    int const percent = static_cast<int>(static_cast<float>(value) / static_cast<float>(maxValue) * 100.0f);
    if (percent < VALUE_COLOR_YELLOW_THRESHOLD)
    {
        return help::COLOR_RED;
    }
    return (percent < VALUE_COLOR_GREEN_THRESHOLD) ? help::COLOR_YELLOW : help::COLOR_GREEN;
}

unsigned GarageWnd::EnumColor2Color(help::Color enumColor) const
{
    // RVA 0x44AAC0
    switch (enumColor)
    {
    case help::COLOR_RED:
        return m_aif.m_colorRed;
    case help::COLOR_GREEN:
        return m_aif.m_colorGreen;
    case help::COLOR_YELLOW:
        return m_aif.m_colorYellow;
    default:
        return m_aif.m_colorNormal;
    }
}

void GarageWnd::UpdateAllPriceValues()
{
    // RVA 0x44C390
    GetPossibleRefuel(m_maxPossibleUnitsToRefuel, m_refuelPrice);
    m_unitsToRefuel = GetUnitsToRefuel();
    GetPossibleRepair(m_maxPossibleUnitsToRepair, m_repairPrice);
    m_unitsToRepair = GetUnitsToRepair();
    GetPossibleRecharge(m_maxPossibleUnitsToRecharge, m_rechargePrice);
    m_unitsToRecharge = GetUnitsToRecharge();
}

void GarageWnd::UpdateAllPricePrevValues()
{
    // RVA 0x44C400
    m_prevUnitsToRepair = m_unitsToRepair;
    m_prevMaxPossibleUnitsToRepair = m_maxPossibleUnitsToRepair;
    m_prevRepairPrice = m_repairPrice;
    m_prevUnitsToRefuel = m_unitsToRefuel;
    m_prevMaxPossibleUnitsToRefuel = m_maxPossibleUnitsToRefuel;
    m_prevRefuelPrice = m_refuelPrice;
    m_prevUnitsToRecharge = m_unitsToRecharge;
    m_prevMaxPossibleUnitsToRecharge = m_maxPossibleUnitsToRecharge;
    m_prevRechargePrice = m_rechargePrice;
}

void GarageWnd::UpdateAllPriceControls(bool bForce)
{
    // RVA 0x44C470
    UpdateRefuelAllPriceControls(bForce);
    UpdateRepairAllPriceControls(bForce);
    UpdateRechargeAllPriceControls(bForce);
}

void GarageWnd::UpdateRefuelAllPriceControls(bool bForce)
{
    // RVA 0x44AB00 - the label is tinted by how much of the needed work the
    // player can actually afford.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (!bForce && m_unitsToRefuel == m_prevUnitsToRefuel &&
        m_maxPossibleUnitsToRefuel == m_prevMaxPossibleUnitsToRefuel && m_refuelPrice == m_prevRefuelPrice)
    {
        return;
    }

    m_wndRefuelPrice->SetText(CStr(m_refuelPrice));
    m_wndRefuelPrice->SetTextColor(
        EnumColor2Color(GetValueColor(m_maxPossibleUnitsToRefuel, m_unitsToRefuel)));
}

void GarageWnd::UpdateRepairAllPriceControls(bool bForce)
{
    // RVA 0x44AC30
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (!bForce && m_unitsToRepair == m_prevUnitsToRepair &&
        m_maxPossibleUnitsToRepair == m_prevMaxPossibleUnitsToRepair && m_repairPrice == m_prevRepairPrice)
    {
        return;
    }

    m_wndRepairPrice->SetText(CStr(m_repairPrice));
    m_wndRepairPrice->SetTextColor(
        EnumColor2Color(GetValueColor(m_maxPossibleUnitsToRepair, m_unitsToRepair)));
}

void GarageWnd::UpdateRechargeAllPriceControls(bool bForce)
{
    // RVA 0x44AD60
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (!bForce && m_unitsToRecharge == m_prevUnitsToRecharge &&
        m_maxPossibleUnitsToRecharge == m_prevMaxPossibleUnitsToRecharge && m_rechargePrice == m_prevRechargePrice)
    {
        return;
    }

    m_wndRechargePrice->SetText(CStr(m_rechargePrice));
    m_wndRechargePrice->SetTextColor(
        EnumColor2Color(GetValueColor(m_maxPossibleUnitsToRecharge, m_unitsToRecharge)));
}

namespace
{
    // Shared tooltip shape: "<service>|<State>: <colour><state text>". The state
    // line is omitted entirely when there is nothing to say about it.
    CStr BuildServiceTooltip(CStr const& strIdService, CStr const& strIdState, CStr const& strIdBuyType,
                             CStr const& strStateColor)
    {
        CStr tooltip = M3D_APP->GetStringByStringId0(strIdService);
        if (!strIdBuyType.empty())
        {
            tooltip += CStr("|") + M3D_APP->GetStringByStringId0(strIdState) + ": " + strStateColor +
                       M3D_APP->GetStringByStringId0(strIdBuyType);
        }
        return tooltip;
    }
}  // namespace

void GarageWnd::UpdateRefuelAllTooltip()
{
    // RVA 0x44AE90
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    CStr strIdBuyType;
    CStr strStateColor;
    if (!m_unitsToRefuel)
    {
        strIdBuyType = m_aif.m_strIdTooltipRefuelNotNeed;
    }
    else if (!m_maxPossibleUnitsToRefuel)
    {
        strIdBuyType = m_aif.m_strIdTooltipRefuelUnavailable;
        strStateColor = help::Color2Str(m_aif.m_colorTooltipRed);
    }
    else if (m_maxPossibleUnitsToRefuel < m_unitsToRefuel)
    {
        strIdBuyType = m_aif.m_strIdTooltipRefuelPartial;
    }
    else if (m_maxPossibleUnitsToRefuel == m_unitsToRefuel)
    {
        strIdBuyType = m_aif.m_strIdTooltipRefuelFull;
    }

    CStr tooltip =
        BuildServiceTooltip(m_aif.m_strIdTooltipRefuel, m_aif.m_strIdState, strIdBuyType, strStateColor);
    m_btnRefuelAll->SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

void GarageWnd::UpdateRepairAllTooltip()
{
    // RVA 0x44B1B0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    CStr strIdBuyType;
    CStr strStateColor;
    if (!m_unitsToRepair)
    {
        strIdBuyType = m_aif.m_strIdTooltipRepairNotNeed;
    }
    else if (!m_maxPossibleUnitsToRepair)
    {
        strIdBuyType = m_aif.m_strIdTooltipRepairUnavailable;
        strStateColor = help::Color2Str(m_aif.m_colorTooltipRed);
    }
    else if (m_maxPossibleUnitsToRepair < m_unitsToRepair)
    {
        strIdBuyType = m_aif.m_strIdTooltipRepairPartial;
    }
    else if (m_maxPossibleUnitsToRepair == m_unitsToRepair)
    {
        strIdBuyType = m_aif.m_strIdTooltipRepairFull;
    }

    CStr tooltip =
        BuildServiceTooltip(m_aif.m_strIdTooltipRepair, m_aif.m_strIdState, strIdBuyType, strStateColor);
    m_btnRepairAll->SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

void GarageWnd::UpdateRechargeAllTooltip()
{
    // RVA 0x44B4D0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    CStr strIdBuyType;
    CStr strStateColor;
    if (!m_unitsToRecharge)
    {
        strIdBuyType = m_aif.m_strIdTooltipRechargeNotNeed;
    }
    else if (!m_maxPossibleUnitsToRecharge)
    {
        strIdBuyType = m_aif.m_strIdTooltipRechargeUnavailable;
        strStateColor = help::Color2Str(m_aif.m_colorTooltipRed);
    }
    else if (m_maxPossibleUnitsToRecharge < m_unitsToRecharge)
    {
        strIdBuyType = m_aif.m_strIdTooltipRechargePartial;
    }
    else if (m_maxPossibleUnitsToRecharge == m_unitsToRecharge)
    {
        strIdBuyType = m_aif.m_strIdTooltipRechargeFull;
    }

    CStr tooltip =
        BuildServiceTooltip(m_aif.m_strIdTooltipRecharge, m_aif.m_strIdState, strIdBuyType, strStateColor);
    m_btnRechargeAll->SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

// ===========================================================================
//  Notifications
// ===========================================================================

void GarageWnd::OnNewFrame()
{
    // RVA 0x44C040 - recomputes the three services' prices and, for each one that
    // moved, refreshes its label, button state and tooltip.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    UpdateAllPriceValues();

    if (m_unitsToRefuel != m_prevUnitsToRefuel ||
        m_maxPossibleUnitsToRefuel != m_prevMaxPossibleUnitsToRefuel || m_refuelPrice != m_prevRefuelPrice)
    {
        UpdateRefuelAllPriceControls(true);
        UpdateRefuelActiveState();
        UpdateRefuelAllTooltip();
    }
    if (m_unitsToRepair != m_prevUnitsToRepair ||
        m_maxPossibleUnitsToRepair != m_prevMaxPossibleUnitsToRepair || m_repairPrice != m_prevRepairPrice)
    {
        UpdateRepairAllPriceControls(true);
        UpdateRepairActiveState();
        UpdateRepairAllTooltip();
    }
    if (m_unitsToRecharge != m_prevUnitsToRecharge ||
        m_maxPossibleUnitsToRecharge != m_prevMaxPossibleUnitsToRecharge ||
        m_rechargePrice != m_prevRechargePrice)
    {
        UpdateRechargeAllPriceControls(true);
        UpdateRechargeActiveState();
        UpdateRechargeAllTooltip();
    }

    UpdateAllPricePrevValues();
}

void GarageWnd::OnRepositoryChanged(void* data)
{
    // RVA 0x44C4A0 - only the workshop's own cabin/basket stock affects this
    // window's buttons.
    if ((m_gameDataFlags & 1) == 0 || !IsChildOf(M3D_APP))
    {
        return;
    }

    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return;
    }
    if (workshop->GetRepositoryByType(ai::WORKSHOP_CABINS_AND_BASKETS) !=
        reinterpret_cast<ai::GeomRepository*>(static_cast<int*>(data)[13]))
    {
        return;
    }

    UpdateCabinsActiveState();
    UpdateBasketsActiveState();
}

void GarageWnd::OnPlayerVehicleChanged()
{
    // RVA 0x44C4F0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ai::Vehicle const* vehicle = help::GetPlayerVehicle();
    m_wndSkinList->SetupForVehicle(vehicle ? vehicle->GetId() : -1);
}

void GarageWnd::OnBuySkin()
{
    // RVA 0x44C550 - buying a skin closes the list it was bought from.
    if (m_curList && m_curList.get() == static_cast<m3d::ui::Wnd*>(m_wndSkinList.get()))
    {
        CloseCurrentList();
    }
}

void GarageWnd::OnFinishTrade(void* data)
{
    // RVA 0x44C5A0 - a completed cabin or basket trade closes whichever of those
    // two lists is on screen.
    if ((m_gameDataFlags & 1) == 0 || !data || !static_cast<int*>(data)[13])
    {
        return;
    }

    switch (static_cast<int*>(data)[14])
    {
    case 1:
        if (m_curList && m_curList.get() == static_cast<m3d::ui::Wnd*>(m_wndCabinsList.get()))
        {
            CloseCurrentList();
        }
        break;
    case 2:
        if (m_curList && m_curList.get() == static_cast<m3d::ui::Wnd*>(m_wndBasketsList.get()))
        {
            CloseCurrentList();
        }
        break;
    default:
        break;
    }
}
