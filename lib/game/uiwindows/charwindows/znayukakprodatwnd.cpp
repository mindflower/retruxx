#include "znayukakprodatwnd.h"

#include "skinswnd.h"

#include <core/aiparam.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/geomrepository.h>
#include <server/geomrepositoryitem.h>
#include <server/izvratrepository.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/chassis.h>
#include <server/objects/gadget.h>
#include <server/objects/guns/compoundgun.h>
#include <server/objects/guns/gun.h>
#include <server/objects/physicbodies/vehiclepart.h>
#include <server/objects/player.h>
#include <server/objects/town.h>
#include <server/objects/vehicle.h>
#include <server/objects/workshop.h>
#include <server/resourcemanager.h>
#include <ui/button.h>
#include <ui/msgbox.h>
#include <ui/ui_srv.h>
#include <ui/wndstation.h>

#include <vector>

namespace
{
    // The two vehicle parts a trade can hang off. The shipped build keeps these
    // as file scope CStr globals (CABIN_15 / BASKET_15 in the map file).
    CStr const CABIN_PART_NAME("CABIN");
    CStr const BASKET_PART_NAME("BASKET");

    // Control ids of the four buttons this panel reacts to.
    unsigned const ID_BTN_TRANSFER_FROM_PLAYER_TO_WORKSHOP = 800000;
    unsigned const ID_BTN_TRANSFER_FROM_WORKSHOP_TO_PLAYER = 800001;
    unsigned const ID_BTN_CANCEL = 800002;
    unsigned const ID_BTN_BUY = 800003;

    // GameDataUpdate data types the panel listens to.
    int const DATATYPE_REPOSITORY_CHANGED = 58;
    int const DATATYPE_VEHICLE_PART_CHANGED = 65;
    int const DATATYPE_GADGET_CHANGED = 69;

    // Application messages: "the trade finished" (arg0 = bought/cancelled,
    // arg1 = trade type) and the generic "refresh the character screen" one.
    int const UM_TRADE_FINISHED = 65672;
    int const UM_REFRESH_CHAR_SCREEN = 65691;

    bool IsResourceKindOf(int resourceId, char const* resourceName)
    {
        return ai::theResourceManager->bResourceIsKindOf(
            resourceId, ai::theResourceManager->GetResourceId(CStr(resourceName)));
    }

    ai::Vehicle* GetVehicleById(int objId)
    {
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
        if (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle))
        {
            return static_cast<ai::Vehicle*>(obj);
        }
        return nullptr;
    }

    bool IsGun(ai::VehiclePart const* part)
    {
        return part->IsKindOf(&ai::Gun::m_classGun) || part->IsKindOf(&ai::CompoundGun::m_classCompoundGun);
    }
}  // namespace

RT_CLASS_EXPORTS_BEGIN(ZnayuKakProdatWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ZnayuKakProdatWnd);

ZnayuKakProdatWnd::AuxInfo::AuxInfo()
{
    // RVA 0x48C220
    m_wndTradeCostName = "wndTradeCost";
    m_wndPlayerItemsCostName = "wndPlayerItemsCost";
    m_wndWorkshopItemsCostName = "wndWorkshopItemsCost";
    m_wndBottomPanelName = "wndBottomPanel";
    m_btnTransferItemsFromPlayerToWorkshopName = "btnMoveAllFromPlayerToWorkshop";
    m_btnTransferItemsFromWorkshopToPlayerName = "btnMoveAllFromWorkshopToPlayer";
    m_strIdLeftItemsInCabin = "LeftItemsInCabin";
    m_strIdLeftItemsInBasket = "LeftItemsInBasket";
    m_strIdLeftItemsInVehicle = "LeftItemsInVehicle";
    m_strIdTooltipTransferItemsFromPlayerToWorkshop[TRADETYPE_VEHICLE] = "MoveVehicleItemsFromPlayer";
    m_strIdTooltipTransferItemsFromPlayerToWorkshop[TRADETYPE_CABIN] = "MoveCabinItemsFromPlayer";
    m_strIdTooltipTransferItemsFromPlayerToWorkshop[TRADETYPE_BASKET] = "MoveBasketItemsFromPlayer";
    m_strIdTooltipTransferItemsFromWorkshopToPlayer[TRADETYPE_VEHICLE] = "MoveVehicleItemsToPlayer";
    m_strIdTooltipTransferItemsFromWorkshopToPlayer[TRADETYPE_CABIN] = "MoveCabinItemsToPlayer";
    m_strIdTooltipTransferItemsFromWorkshopToPlayer[TRADETYPE_BASKET] = "MoveBasketItemsToPlayer";
}

// ---------------------------------------------------------------------------
// ZnayuKakProdatWnd::CurValues
// ---------------------------------------------------------------------------

ZnayuKakProdatWnd::CurValues::CurValues()
{
    // RVA 0x48C850
    m_hp = 0.0;
    m_fuel = 0.0;
}

void ZnayuKakProdatWnd::CurValues::Clear()
{
    // RVA 0x48C920
    m_hp = 0.0;
    m_fuel = 0.0;
    m_durability.clear();
    m_shellsInCurCharge.clear();
    m_shellsInPool.clear();
}

// ---------------------------------------------------------------------------
// ZnayuKakProdatWnd::ItemSaveInfo
// ---------------------------------------------------------------------------

ZnayuKakProdatWnd::ItemSaveInfo::ItemSaveInfo()
{
    // RVA 0x48C5E0
    m_itemType = ITEMTYPE_INVALID;
    m_gadgetSlotId = -1;
    m_repositoryPos.x = -1;
    m_repositoryPos.y = -1;
    m_objId = -1;
    m_belong = BELONG_INVALID;
}

ZnayuKakProdatWnd::ItemSaveInfo::~ItemSaveInfo()
{
    // The shipped destructor only runs the CStr member's own destructor.
}

ZnayuKakProdatWnd::ItemType ZnayuKakProdatWnd::ItemSaveInfo::GetItemType() const
{
    // RVA 0x48C6A0
    return m_itemType;
}

int ZnayuKakProdatWnd::ItemSaveInfo::GetObjId() const
{
    // RVA 0x48C6B0
    return m_objId;
}

ZnayuKakProdatWnd::Belong ZnayuKakProdatWnd::ItemSaveInfo::GetBelong() const
{
    // RVA 0x48C6C0
    return m_belong;
}

bool ZnayuKakProdatWnd::ItemSaveInfo::IsValid() const
{
    // RVA 0x48C610
    if (m_belong == BELONG_INVALID || m_objId == -1)
    {
        return false;
    }

    switch (m_itemType)
    {
    case ITEMTYPE_GADGET:
        return m_gadgetSlotId != -1;
    case ITEMTYPE_VEHICLE_PART_CABIN:
    case ITEMTYPE_VEHICLE_PART_BASKET:
        return !m_vehiclePartName.empty();
    case ITEMTYPE_REPOSITORY_ITEM:
        return m_repositoryPos.x != -1 || m_repositoryPos.y != -1;
    case ITEMTYPE_MAIN_ITEM:
        return true;
    default:
        return false;
    }
}

void ZnayuKakProdatWnd::ItemSaveInfo::Invalidate()
{
    // RVA 0x48C680 - note that the shipped code deliberately leaves
    // m_vehiclePartName alone.
    m_itemType = ITEMTYPE_INVALID;
    m_gadgetSlotId = -1;
    m_repositoryPos.x = -1;
    m_repositoryPos.y = -1;
    m_objId = -1;
    m_belong = BELONG_INVALID;
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsGadget(int objId, Belong belong, int gadgetSlotId)
{
    // RVA 0x48C6D0
    m_repositoryPos.x = -1;
    m_repositoryPos.y = -1;
    m_objId = objId;
    m_itemType = ITEMTYPE_GADGET;
    m_belong = belong;
    m_gadgetSlotId = gadgetSlotId;
    return IsValid();
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsVehiclePart(
    int objId,
    Belong belong,
    CStr const& vehiclePartName,
    CStr const& parentPartName)
{
    // RVA 0x48C700 - the item type comes from the *parent* part: anything that
    // does not hang off the cabin or the basket stays ITEMTYPE_INVALID, so the
    // IsValid() below fails for it.
    Invalidate();
    if (CStr::my_strcmp(parentPartName.c_str(), CABIN_PART_NAME.c_str()) == 0)
    {
        m_itemType = ITEMTYPE_VEHICLE_PART_CABIN;
    }
    else if (CStr::my_strcmp(parentPartName.c_str(), BASKET_PART_NAME.c_str()) == 0)
    {
        m_itemType = ITEMTYPE_VEHICLE_PART_BASKET;
    }
    m_objId = objId;
    m_belong = belong;
    m_vehiclePartName = vehiclePartName;
    return IsValid();
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsRepositoryItem(int objId, Belong belong, PointBase<int> const& repositoryPos)
{
    // RVA 0x48C780
    m_gadgetSlotId = -1;
    m_repositoryPos.x = -1;
    m_repositoryPos.y = -1;
    m_objId = objId;
    m_itemType = ITEMTYPE_REPOSITORY_ITEM;
    m_belong = belong;
    m_repositoryPos = repositoryPos;
    return IsValid();
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsMainItem(int objId, Belong belong)
{
    // RVA 0x48C7C0
    m_gadgetSlotId = -1;
    m_repositoryPos.x = -1;
    m_repositoryPos.y = -1;
    m_itemType = ITEMTYPE_MAIN_ITEM;
    m_objId = objId;
    m_belong = belong;
    return IsValid();
}

int ZnayuKakProdatWnd::ItemSaveInfo::GetGadgetSlotId() const
{
    // RVA 0x48C7F0
    return m_itemType == ITEMTYPE_GADGET ? m_gadgetSlotId : -1;
}

CStr ZnayuKakProdatWnd::ItemSaveInfo::GetVehiclePartName() const
{
    // RVA 0x48C800
    if (m_itemType == ITEMTYPE_VEHICLE_PART_CABIN || m_itemType == ITEMTYPE_VEHICLE_PART_BASKET)
    {
        return m_vehiclePartName;
    }
    return CStr();
}

PointBase<int> ZnayuKakProdatWnd::ItemSaveInfo::GetRepositoryPos() const
{
    // RVA 0x48C830
    if (m_itemType == ITEMTYPE_REPOSITORY_ITEM)
    {
        return m_repositoryPos;
    }
    return PointBase<int>{-1, -1};
}

// ---------------------------------------------------------------------------
// construction / RTTI
// ---------------------------------------------------------------------------

ZnayuKakProdatWnd::ZnayuKakProdatWnd()
{
    // RVA 0x48C9B0
    // NOTE: m_workshopVehicleId is deliberately left out here - the shipped
    // constructor never initialises it. It only becomes defined once
    // SetupForTrade() runs (GameDataClear() does not reset it either).
    m_vehicleId = -1;
    m_wndTradeCost = nullptr;
    m_wndPlayerItemsCost = nullptr;
    m_wndWorkshopItemsCost = nullptr;
    m_btnTransferItemsFromPlayerToWorkshop = nullptr;
    m_btnTransferItemsFromWorkshopToPlayer = nullptr;
    m_objectToTradeId = -1;
    m_workshopId = -1;
    m_playerItemsCostToSell = -1;
    m_workshopItemsCostToBuy = -1;
    m_tradeType = TRADETYPE_NUM_TRADETYPES;
}

ZnayuKakProdatWnd::ZnayuKakProdatWnd(ZnayuKakProdatWnd const&)
{
    // RVA 0x48CD80 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, does not even null the child window pointers or
    // reset the trade ids; all it does is default-construct the members with
    // constructors of their own and reset the ChildPanel vehicle id.
    m_vehicleId = -1;
}

ZnayuKakProdatWnd::~ZnayuKakProdatWnd()
{
    // RVA 0x48CE80
    ZnayuKakProdatWnd::GameDataClear(false);
}

m3d::Object* ZnayuKakProdatWnd::Clone()
{
    // RVA 0x48C160 - note this uses the default constructor, not the copy one.
    return new ZnayuKakProdatWnd;
}

m3d::Object* ZnayuKakProdatWnd::CreateObject()
{
    return new ZnayuKakProdatWnd;
}

m3d::Class* ZnayuKakProdatWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Class* ZnayuKakProdatWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ZnayuKakProdatWnd);
}

// ---------------------------------------------------------------------------
// resource setup
// ---------------------------------------------------------------------------

int ZnayuKakProdatWnd::GameDataSetup()
{
    // RVA 0x48CF90
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        m3d::Object* wndTradeCost = GetChildByName(m_aif.m_wndTradeCostName);
        if (wndTradeCost && wndTradeCost->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndTradeCost = static_cast<m3d::ui::Wnd*>(wndTradeCost);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndTradeCostName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* wndPlayerItemsCost = GetChildByName(m_aif.m_wndPlayerItemsCostName);
        if (wndPlayerItemsCost && wndPlayerItemsCost->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndPlayerItemsCost = static_cast<m3d::ui::Wnd*>(wndPlayerItemsCost);
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_aif.m_wndPlayerItemsCostName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* wndWorkshopItemsCost = GetChildByName(m_aif.m_wndWorkshopItemsCostName);
        if (wndWorkshopItemsCost && wndWorkshopItemsCost->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndWorkshopItemsCost = static_cast<m3d::ui::Wnd*>(wndWorkshopItemsCost);
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_aif.m_wndWorkshopItemsCostName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* btnToWorkshop = GetChildByName(m_aif.m_btnTransferItemsFromPlayerToWorkshopName);
        if (btnToWorkshop && btnToWorkshop->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnTransferItemsFromPlayerToWorkshop = static_cast<m3d::ui::ButtonWnd*>(btnToWorkshop);
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_aif.m_btnTransferItemsFromPlayerToWorkshopName +
                " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* btnToPlayer = GetChildByName(m_aif.m_btnTransferItemsFromWorkshopToPlayerName);
        if (btnToPlayer && btnToPlayer->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnTransferItemsFromWorkshopToPlayer = static_cast<m3d::ui::ButtonWnd*>(btnToPlayer);
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_aif.m_btnTransferItemsFromWorkshopToPlayerName +
                " is not found or incorrect type");
            res = 0;
        }

        // MakeBottomPanel() always runs - it is not short-circuited by res.
        if ((MakeBottomPanel() & res) != 0)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_INFO("ZnayuKakProdatWnd: error - fail to init because of a bad resource");
    return 0;
}

int ZnayuKakProdatWnd::MakeBottomPanel()
{
    // RVA 0x48D5B0 - every direct child window that overlaps the bottom panel is
    // reparented into it, keeping the position it had on screen.
    m3d::Object* bottomPanelObj = GetChildByName(m_aif.m_wndBottomPanelName);
    if (!bottomPanelObj || !bottomPanelObj->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        return 0;
    }
    auto* bottomPanel = static_cast<m3d::ui::Wnd*>(bottomPanelObj);

    int res = 1;
    for (m3d::Object* child = GetFirstChild(); child;)
    {
        m3d::Object* nextChild = child->GetNextSibling();
        if (!child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            res = 0;
            child = nextChild;
            continue;
        }

        if (child != bottomPanel)
        {
            auto* childWnd = static_cast<m3d::ui::Wnd*>(child);
            BoundsBase<float> const overlap = childWnd->GetBounds().Intersect(bottomPanel->GetBounds());
            if (!overlap.Empty())
            {
                PointBase<float> const onScreen = ToScreen(childWnd->GetOrigin());
                PointBase<float> const panelOrigin = bottomPanel->ToScreen(PointBase<float>{0.0f, 0.0f});
                childWnd->SetOrigin(PointBase<float>{onScreen.x - panelOrigin.x, onScreen.y - panelOrigin.y});
                RemoveChild(childWnd);
                bottomPanel->AddChild(childWnd);
            }
        }

        child = nextChild;
    }
    return res;
}

int ZnayuKakProdatWnd::GameDataClear(bool)
{
    // RVA 0x48D7F0 - note m_workshopVehicleId is *not* reset here.
    ClearSaveInfo(m_originalPlayerItems);
    ClearSaveInfo(m_originalWorkshopItems);
    ClearSaveInfo(m_curPlayerItems);
    ClearSaveInfo(m_curWorkshopItems);
    m_vehicleId = -1;
    m_objectToTradeId = -1;
    m_workshopId = -1;
    m_playerItemsCostToSell = -1;
    m_workshopItemsCostToBuy = -1;
    m_tradeType = TRADETYPE_NUM_TRADETYPES;
    return 1;
}

void ZnayuKakProdatWnd::ClearSaveInfo(ItemSaveInfoMap& items)
{
    // RVA 0x48D860
    for (auto& si : items)
    {
        delete si.second;
        si.second = nullptr;
    }
    items.clear();
}

int ZnayuKakProdatWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x48D930
    if ((m_gameDataFlags & 1) == 0 || m_workshopId == -1 || m_objectToTradeId == -1 || m_workshopVehicleId == -1)
    {
        return 0;
    }

    switch (dataType)
    {
    case DATATYPE_REPOSITORY_CHANGED:
        OnRepositoryChanged(data);
        break;
    case DATATYPE_VEHICLE_PART_CHANGED:
        OnVehiclePartChanged(data);
        break;
    case DATATYPE_GADGET_CHANGED:
        OnGadgetChanged(data);
        break;
    default:
        break;
    }
    return 1;
}

int ZnayuKakProdatWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x48D9B0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (ChildPanel::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }

    int srcVehicleId = -1;
    int dstVehicleId = -1;
    switch (id)
    {
    case ID_BTN_TRANSFER_FROM_PLAYER_TO_WORKSHOP:
        if (msg != 1)
        {
            return 0;
        }
        srcVehicleId = GetVehicleId(BELONG_PLAYER);
        dstVehicleId = m_workshopVehicleId;
        break;
    case ID_BTN_TRANSFER_FROM_WORKSHOP_TO_PLAYER:
        if (msg != 1)
        {
            return 0;
        }
        srcVehicleId = m_workshopVehicleId;
        dstVehicleId = GetVehicleId(BELONG_PLAYER);
        break;
    case ID_BTN_CANCEL:
        if (msg != 1)
        {
            return 0;
        }
        Cancel();
        return 1;
    case ID_BTN_BUY:
        if (msg != 1)
        {
            return 0;
        }
        Buy();
        return 1;
    default:
        return 0;
    }

    switch (m_tradeType)
    {
    case TRADETYPE_VEHICLE:
        TransferVehicleItems(srcVehicleId, dstVehicleId);
        break;
    case TRADETYPE_CABIN:
        TransferCabinItems(srcVehicleId, dstVehicleId);
        break;
    case TRADETYPE_BASKET:
        TransferBasketItems(srcVehicleId, dstVehicleId);
        break;
    default:
        break;
    }
    return 1;
}

int ZnayuKakProdatWnd::OnAfterAddToWndStation()
{
    // RVA 0x491B80
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(UM_REFRESH_CHAR_SCREEN, 0, 0, 0, 0, CStr(), m3d::AIParam());
    return res;
}

int ZnayuKakProdatWnd::OnAfterRemoveFromWndStation()
{
    // RVA 0x491580
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    if (m_objectToTradeId != -1)
    {
        Cancel();
    }
    return res;
}

// ---------------------------------------------------------------------------
// trade setup
// ---------------------------------------------------------------------------

int ZnayuKakProdatWnd::SetupForTrade(int workshopId, int objectToTradeId, int workshopVehicleId)
{
    // RVA 0x48DC10
    GameDataClear(false);
    m_workshopId = workshopId;
    m_objectToTradeId = objectToTradeId;
    m_workshopVehicleId = workshopVehicleId;
    m_tradeType = GetTradeTypeByObjectId(objectToTradeId);

    if (!IsValid())
    {
        return 0;
    }
    if (!SaveTradeState(true))
    {
        GameDataClear(false);
        return 0;
    }
    UpdateOnCurTradeStateChanged();
    UpdateTransferButtonsTooltips();
    return 1;
}

ZnayuKakProdatWnd::TradeType ZnayuKakProdatWnd::GetTradeTypeByObjectId(int objectToTradeId) const
{
    // RVA 0x48DAE0
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(objectToTradeId);
    if (!obj)
    {
        return TRADETYPE_INVALID;
    }
    ai::PrototypeInfo const* proto = obj->GetPrototypeInfo();
    if (!proto)
    {
        return TRADETYPE_INVALID;
    }

    int const resourceId = proto->m_resourceId;
    if (IsResourceKindOf(resourceId, "VEHICLE"))
    {
        return TRADETYPE_VEHICLE;
    }
    if (IsResourceKindOf(resourceId, "CABIN"))
    {
        return TRADETYPE_CABIN;
    }
    return IsResourceKindOf(resourceId, "BASKET") ? TRADETYPE_BASKET : TRADETYPE_INVALID;
}

bool ZnayuKakProdatWnd::IsValid() const
{
    // RVA 0x48F920
    return (m_gameDataFlags & 1) != 0 && m_workshopId != -1 && m_objectToTradeId != -1 && m_workshopVehicleId != -1;
}

int ZnayuKakProdatWnd::GetVehicleId(Belong belong) const
{
    // RVA 0x48F7C0
    if (belong == BELONG_PLAYER)
    {
        ai::Vehicle const* vehicle = ai::thePlayer->GetVehicle();
        return vehicle ? vehicle->GetId() : -1;
    }
    if (belong == BELONG_WORKSHOP)
    {
        return m_workshopVehicleId;
    }
    return -1;
}

int ZnayuKakProdatWnd::GetCabinId(Belong belong) const
{
    // RVA 0x48F800
    if (belong == BELONG_PLAYER)
    {
        ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
        if (!vehicle)
        {
            return -1;
        }
        ai::VehiclePart const* cabin = vehicle->GetPartByName(CABIN_PART_NAME);
        return cabin ? cabin->GetId() : -1;
    }
    if (belong != BELONG_WORKSHOP)
    {
        return -1;
    }

    // For a cabin trade the traded object *is* the cabin; for a whole vehicle
    // trade it is the cabin mounted on the traded vehicle.
    if (m_tradeType == TRADETYPE_CABIN)
    {
        return m_objectToTradeId;
    }
    if (m_tradeType != TRADETYPE_VEHICLE)
    {
        return -1;
    }
    ai::Vehicle* tradedVehicle = GetVehicleById(m_objectToTradeId);
    if (!tradedVehicle)
    {
        return -1;
    }
    ai::VehiclePart const* cabin = tradedVehicle->GetPartByName(CABIN_PART_NAME);
    return cabin ? cabin->GetId() : -1;
}

int ZnayuKakProdatWnd::GetBasketId(Belong belong) const
{
    // RVA 0x48F890
    if (belong == BELONG_PLAYER)
    {
        ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
        if (!vehicle)
        {
            return -1;
        }
        ai::VehiclePart const* basket = vehicle->GetPartByName(BASKET_PART_NAME);
        return basket ? basket->GetId() : -1;
    }
    if (belong != BELONG_WORKSHOP)
    {
        return -1;
    }

    if (m_tradeType == TRADETYPE_BASKET)
    {
        return m_objectToTradeId;
    }
    if (m_tradeType != TRADETYPE_VEHICLE)
    {
        return -1;
    }
    ai::Vehicle* tradedVehicle = GetVehicleById(m_objectToTradeId);
    if (!tradedVehicle)
    {
        return -1;
    }
    ai::VehiclePart const* basket = tradedVehicle->GetPartByName(BASKET_PART_NAME);
    return basket ? basket->GetId() : -1;
}

ZnayuKakProdatWnd::Belong ZnayuKakProdatWnd::GetBelongByVehicleId(int vehicleId) const
{
    // RVA 0x491480
    if (vehicleId == GetVehicleId(BELONG_PLAYER))
    {
        return BELONG_PLAYER;
    }
    return vehicleId == m_workshopVehicleId ? BELONG_WORKSHOP : BELONG_INVALID;
}

ai::Vehicle* ZnayuKakProdatWnd::GetWorkshopVehicle() const
{
    // RVA 0x491430
    return GetVehicleById(m_workshopVehicleId);
}

ai::Workshop* ZnayuKakProdatWnd::GetWorkshop() const
{
    // RVA 0x4914C0
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

ZnayuKakProdatWnd::TradeType ZnayuKakProdatWnd::GetTradeType() const
{
    // RVA 0x491AC0
    return m_tradeType;
}

// ---------------------------------------------------------------------------
// saving the trade state
// ---------------------------------------------------------------------------

int ZnayuKakProdatWnd::SaveTradeState(bool bOriginaltState)
{
    // RVA 0x48DCA0
    ItemSaveInfoMap& playerItems = bOriginaltState ? m_originalPlayerItems : m_curPlayerItems;
    ItemSaveInfoMap& workshopItems = bOriginaltState ? m_originalWorkshopItems : m_curWorkshopItems;
    ClearSaveInfo(playerItems);
    ClearSaveInfo(workshopItems);

    int res;
    switch (m_tradeType)
    {
    case TRADETYPE_VEHICLE:
        res = SaveVehicleItemsState(BELONG_PLAYER, playerItems)
            ? SaveVehicleItemsState(BELONG_WORKSHOP, workshopItems)
            : 0;
        break;
    case TRADETYPE_CABIN:
        res = SaveVehicleItemsState(BELONG_PLAYER, playerItems) ? SaveCabinItemsState(BELONG_WORKSHOP, workshopItems)
                                                                : 0;
        break;
    case TRADETYPE_BASKET:
        res = SaveVehicleItemsState(BELONG_PLAYER, playerItems) ? SaveBasketItemsState(BELONG_WORKSHOP, workshopItems)
                                                                : 0;
        break;
    default:
        // The current values are not saved at all for an unknown trade type.
        return 0;
    }

    if (bOriginaltState)
    {
        int const playerValuesRes = SaveCurValues(GetVehicleId(BELONG_PLAYER), m_playerCurValues);
        res = playerValuesRes & res;
        res = SaveCurValues(m_workshopVehicleId, m_workshopCurValues) & res;
    }
    return res;
}

int ZnayuKakProdatWnd::SaveVehicleItemsState(Belong belong, ItemSaveInfoMap& containerToSave)
{
    // RVA 0x48DF90
    int const vehicleId = GetVehicleId(belong);
    if (vehicleId == -1)
    {
        return 0;
    }

    int res = SaveMainObject(vehicleId, belong, containerToSave) & 1;
    res = SaveCabinItemsState(belong, containerToSave) & res;
    if (GetBasketId(belong) != -1)
    {
        res = SaveBasketItemsState(belong, containerToSave) & res;
    }
    return res;
}

int ZnayuKakProdatWnd::SaveCabinItemsState(Belong belong, ItemSaveInfoMap& containerToSave)
{
    // RVA 0x48E010
    int const cabinId = GetCabinId(belong);
    if (cabinId == -1)
    {
        return 0;
    }
    int const vehicleId = GetVehicleId(belong);
    if (vehicleId == -1)
    {
        return 0;
    }

    int res = SaveMainObject(cabinId, belong, containerToSave) & 1;
    res = SaveVehiclePartsState(vehicleId, CABIN_PART_NAME, belong, containerToSave) & res;
    return res & SaveGadgetsState(vehicleId, belong, containerToSave);
}

int ZnayuKakProdatWnd::SaveBasketItemsState(Belong belong, ItemSaveInfoMap& containerToSave)
{
    // RVA 0x48E0A0
    int const basketId = GetBasketId(belong);
    if (basketId == -1)
    {
        return 0;
    }
    int const vehicleId = GetVehicleId(belong);
    if (vehicleId == -1)
    {
        return 0;
    }

    int res = SaveMainObject(basketId, belong, containerToSave) & 1;
    res = SaveVehiclePartsState(vehicleId, BASKET_PART_NAME, belong, containerToSave) & res;
    return res & SaveRepositoryItemsState(vehicleId, belong, containerToSave);
}

int ZnayuKakProdatWnd::SaveMainObject(int objId, Belong belong, ItemSaveInfoMap& containerToSave)
{
    // RVA 0x48E130
    auto* item = new ItemSaveInfo;
    if (!item)
    {
        return 0;
    }

    if (GetTradeTypeByObjectId(objId) == TRADETYPE_VEHICLE)
    {
        // A whole vehicle is represented by its chassis.
        // NOTE: every bail-out in this block leaks `item` in the shipped build -
        // only the setup/AddItem failure below frees it. Preserved as shipped.
        ai::Vehicle* vehicle = GetVehicleById(objId);
        if (!vehicle)
        {
            return 0;
        }
        ai::Chassis* chassis = vehicle->GetChassis();
        if (!chassis)
        {
            return 0;
        }
        objId = chassis->GetId();
    }

    if (!item->SetupAsMainItem(objId, belong) || !AddItem(item, containerToSave))
    {
        delete item;
        return 0;
    }
    return 1;
}

int ZnayuKakProdatWnd::SaveGadgetsState(int vehicleId, Belong belong, ItemSaveInfoMap& containerToSave)
{
    // RVA 0x48E260
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return 0;
    }

    auto const& gadgets = vehicle->GetGadgets();
    for (auto it = gadgets.begin(); it != gadgets.end(); ++it)
    {
        ai::Gadget* gadget = it->second;
        if (!gadget)
        {
            return 0;
        }

        auto* item = new ItemSaveInfo;
        if (!item)
        {
            return 0;
        }
        if (!item->SetupAsGadget(gadget->GetId(), belong, gadget->GetSlotNum()) || !AddItem(item, containerToSave))
        {
            delete item;
            return 0;
        }
    }
    return 1;
}

int ZnayuKakProdatWnd::SaveVehiclePartsState(
    int vehicleId,
    CStr const& parentPartName,
    Belong belong,
    ItemSaveInfoMap& containerToSave)
{
    // RVA 0x48E3C0
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return 0;
    }
    if (parentPartName.empty())
    {
        return 0;
    }
    ai::VehiclePrototypeInfo const* vehicleInfo = vehicle->GetPrototypeInfo();
    if (!vehicleInfo)
    {
        return 0;
    }

    for (auto partIt = vehicle->begin(); partIt != vehicle->end(); ++partIt)
    {
        ai::VehiclePart* part = partIt->second;
        if (!part)
        {
            return 0;
        }
        ai::ComplexPhysicObjPartDescription const* desc = vehicleInfo->GetPartDescriptionByName(partIt->first);
        if (!desc)
        {
            return 0;
        }
        ai::ComplexPhysicObjPartDescription const* parent = desc->GetParent();
        if (!parent || CStr::my_strcmp(parent->GetName(), parentPartName.c_str()) != 0)
        {
            continue;
        }

        auto* item = new ItemSaveInfo;
        if (!item)
        {
            return 0;
        }
        if (!item->SetupAsVehiclePart(part->GetId(), belong, partIt->first, parentPartName) ||
            !AddItem(item, containerToSave))
        {
            delete item;
            return 0;
        }
    }
    return 1;
}

int ZnayuKakProdatWnd::SaveRepositoryItemsState(int vehicleId, Belong belong, ItemSaveInfoMap& containerToSave)
{
    // RVA 0x48E540
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return 0;
    }
    ai::IzvratRepository* repository = vehicle->GetRepository();
    if (!repository)
    {
        return 0;
    }

    int const numItems = static_cast<int>(repository->GetNumItems());
    for (int i = 0; i < numItems; ++i)
    {
        ai::GeomRepositoryItem repositoryItem = repository->GetItem(i);
        if (!repositoryItem.IsValid())
        {
            continue;
        }

        auto* item = new ItemSaveInfo;
        if (!item)
        {
            return 0;
        }
        // NOTE: unlike the other Save* helpers, the shipped code leaks `item`
        // when the setup or the insertion fails. Preserved as shipped.
        if (!item->SetupAsRepositoryItem(repositoryItem.GetObjId(), belong, repositoryItem.m_origin) ||
            !AddItem(item, containerToSave))
        {
            return 0;
        }
    }
    return 1;
}

int ZnayuKakProdatWnd::SaveCurValues(int vehicleId, CurValues& curValues)
{
    // RVA 0x48E670
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return 0;
    }

    curValues.Clear();
    curValues.m_hp = vehicle->Health().value().get();
    curValues.m_fuel = vehicle->Fuel().value().get();

    for (auto it = vehicle->begin(); it != vehicle->end(); ++it)
    {
        ai::VehiclePart* part = it->second;
        if (!part)
        {
            continue;
        }

        curValues.m_durability.insert(PartFloatValuePair(it->first, part->Durability().value().get()));
        if (IsGun(part))
        {
            curValues.m_shellsInCurCharge.insert(
                PartIntValuePair(it->first, static_cast<int>(help::GetGunShellsInCurrentCharge(part))));
            curValues.m_shellsInPool.insert(
                PartIntValuePair(it->first, static_cast<int>(help::GetGunShellsInPool(part))));
        }
    }
    return 1;
}

int ZnayuKakProdatWnd::AddItem(ItemSaveInfo* item, ItemSaveInfoMap& containerToSave)
{
    // RVA 0x48F6F0 - an entry with the same object id is replaced.
    if (!item || !item->IsValid())
    {
        return 0;
    }

    auto existingItemIt = containerToSave.find(item->GetObjId());
    if (existingItemIt != containerToSave.end())
    {
        delete existingItemIt->second;
        existingItemIt->second = nullptr;
        containerToSave.erase(existingItemIt);
    }
    containerToSave.insert(ItemSaveInfoPair(item->GetObjId(), item));
    return 1;
}

// ---------------------------------------------------------------------------
// clearing the trade state
// ---------------------------------------------------------------------------

int ZnayuKakProdatWnd::ClearTradeState()
{
    // RVA 0x48EA00 - the shipped code inlines ClearCabinFromItems /
    // ClearBasketFromItems into the cabin and basket branches.
    int const playerVehicleId = GetVehicleId(BELONG_PLAYER);
    switch (m_tradeType)
    {
    case TRADETYPE_VEHICLE:
    {
        int const playerRes = ClearVehicleFromItems(playerVehicleId) & 1;
        return ClearVehicleFromItems(m_workshopVehicleId) & playerRes;
    }
    case TRADETYPE_CABIN:
    {
        int const playerRes = ClearVehicleFromItems(playerVehicleId) & 1;
        return ClearCabinFromItems(m_workshopVehicleId) & playerRes;
    }
    case TRADETYPE_BASKET:
    {
        int const playerRes = ClearVehicleFromItems(playerVehicleId) & 1;
        return ClearBasketFromItems(m_workshopVehicleId) & playerRes;
    }
    default:
        return 0;
    }
}

int ZnayuKakProdatWnd::ClearVehicleFromItems(int vehicleId)
{
    // RVA 0x48EB00
    int const cabinRes = ClearCabinFromItems(vehicleId) & 1;
    return cabinRes & ClearBasketFromItems(vehicleId) & 1;
}

int ZnayuKakProdatWnd::ClearCabinFromItems(int vehicleId)
{
    // RVA 0x48EB50
    int const partsRes = ClearFromVehicleParts(vehicleId, CABIN_PART_NAME) & 1;
    return partsRes & ClearFromGadgets(vehicleId);
}

int ZnayuKakProdatWnd::ClearBasketFromItems(int vehicleId)
{
    // RVA 0x48EB70
    int const partsRes = ClearFromVehicleParts(vehicleId, BASKET_PART_NAME) & 1;
    return partsRes & ClearFromRepositoryItems(vehicleId);
}

int ZnayuKakProdatWnd::ClearFromVehicleParts(int vehicleId, CStr const& parentPartName)
{
    // RVA 0x48EB90
    if (vehicleId == -1 || parentPartName.empty())
    {
        return 0;
    }
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return 0;
    }

    std::vector<CStr> const childPartNames = GetChildVehiclePartNamesByParentPart(vehicleId, parentPartName);
    for (auto const& partName : childPartNames)
    {
        vehicle->SetPartByName(partName, nullptr, false);
    }
    return 1;
}

int ZnayuKakProdatWnd::ClearFromGadgets(int vehicleId)
{
    // RVA 0x48EC90
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return 0;
    }

    auto const& gadgets = vehicle->GetGadgets();
    for (auto it = gadgets.begin(); it != gadgets.end();)
    {
        auto nextIt = it;
        ++nextIt;
        vehicle->RemoveChild(it->second);
        it = nextIt;
    }
    return 1;
}

int ZnayuKakProdatWnd::ClearFromRepositoryItems(int vehicleId)
{
    // RVA 0x48ED20
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return 0;
    }
    ai::IzvratRepository* repository = vehicle->GetRepository();
    if (!repository)
    {
        return 0;
    }
    repository->Clear(false);
    return 1;
}

std::vector<CStr> ZnayuKakProdatWnd::GetChildVehiclePartNamesByParentPart(int vehicleId, CStr const& parentPartName)
    const
{
    // RVA 0x48ED80
    std::vector<CStr> childPartNames;
    if (vehicleId == -1 || parentPartName.empty())
    {
        return childPartNames;
    }
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return childPartNames;
    }
    ai::VehiclePrototypeInfo const* vehicleInfo = vehicle->GetPrototypeInfo();
    if (!vehicleInfo)
    {
        return childPartNames;
    }

    for (auto partIt = vehicle->begin(); partIt != vehicle->end(); ++partIt)
    {
        ai::ComplexPhysicObjPartDescription const* desc = vehicleInfo->GetPartDescriptionByName(partIt->first);
        if (!desc)
        {
            continue;
        }
        ai::ComplexPhysicObjPartDescription const* parent = desc->GetParent();
        if (parent && CStr::my_strcmp(parent->GetName(), parentPartName.c_str()) == 0)
        {
            childPartNames.push_back(partIt->first);
        }
    }
    return childPartNames;
}

// ---------------------------------------------------------------------------
// restoring the trade state
// ---------------------------------------------------------------------------

int ZnayuKakProdatWnd::RestoreTradeState()
{
    // RVA 0x48EF00 - the shipped code inlines RestoreVehicleItemsState for both
    // sides. Every step runs, none of them short-circuits.
    int const playerVehicleId = GetVehicleId(BELONG_PLAYER);
    int const workshopVehicleId = m_workshopVehicleId;

    int const playerRes = RestoreVehicleItemsState(playerVehicleId, m_originalPlayerItems) & 1;
    int const workshopRes = RestoreVehicleItemsState(workshopVehicleId, m_originalWorkshopItems) & 1;
    int const playerValuesRes = RestoreCurValues(playerVehicleId, m_playerCurValues) & 1;
    int const workshopValuesRes = RestoreCurValues(workshopVehicleId, m_workshopCurValues) & 1;
    return playerRes & workshopRes & playerValuesRes & workshopValuesRes & 1;
}

int ZnayuKakProdatWnd::RestoreVehicleItemsState(int vehicleId, ItemSaveInfoMap const& containerToRestoreFrom)
{
    // RVA 0x48F000
    int res = RestoreVehiclePartsState(vehicleId, CABIN_PART_NAME, containerToRestoreFrom) & 1;
    res = RestoreGadgetsState(vehicleId, containerToRestoreFrom) & res;
    int const basketRes = RestoreVehiclePartsState(vehicleId, BASKET_PART_NAME, containerToRestoreFrom) & 1;
    return res & (basketRes & RestoreRepositoryItemsState(vehicleId, containerToRestoreFrom)) & 1;
}

int ZnayuKakProdatWnd::RestoreCabinItemsState(int vehicleid, ItemSaveInfoMap const& containerToRestoreFrom)
{
    // RVA 0x48F050
    int const partsRes = RestoreVehiclePartsState(vehicleid, CABIN_PART_NAME, containerToRestoreFrom) & 1;
    return partsRes & RestoreGadgetsState(vehicleid, containerToRestoreFrom);
}

int ZnayuKakProdatWnd::RestoreBasketItemsState(int vehicleId, ItemSaveInfoMap const& containerToRestoreFrom)
{
    // RVA 0x48F080
    int const partsRes = RestoreVehiclePartsState(vehicleId, BASKET_PART_NAME, containerToRestoreFrom) & 1;
    return partsRes & RestoreRepositoryItemsState(vehicleId, containerToRestoreFrom);
}

int ZnayuKakProdatWnd::RestoreVehiclePartsState(
    int vehicleId,
    CStr const& parentPartName,
    ItemSaveInfoMap const& containerToRestoreFrom)
{
    // RVA 0x48F0B0
    if (vehicleId == -1 || parentPartName.empty())
    {
        return 0;
    }
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return 0;
    }
    ai::VehiclePrototypeInfo const* vehicleInfo = vehicle->GetPrototypeInfo();
    if (!vehicleInfo)
    {
        return 0;
    }

    int res = 1;
    for (auto iIt = containerToRestoreFrom.begin(); iIt != containerToRestoreFrom.end(); ++iIt)
    {
        ItemSaveInfo const* item = iIt->second;
        if (!item)
        {
            res = 0;
            continue;
        }
        if (item->GetItemType() != ITEMTYPE_VEHICLE_PART_CABIN && item->GetItemType() != ITEMTYPE_VEHICLE_PART_BASKET)
        {
            continue;
        }
        if (!item->IsValid())
        {
            res = 0;
            continue;
        }

        CStr const partName = item->GetVehiclePartName();
        ai::ComplexPhysicObjPartDescription const* desc = vehicleInfo->GetPartDescriptionByName(partName);
        if (!desc)
        {
            res = 0;
            continue;
        }
        ai::ComplexPhysicObjPartDescription const* parent = desc->GetParent();
        if (!parent || CStr::my_strcmp(parent->GetName(), parentPartName.c_str()) != 0)
        {
            continue;
        }

        ai::Obj* partObj = ai::theObjects->GetEntityByObjId(item->GetObjId());
        if (partObj && partObj->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
        {
            partObj->SetVisible();
            vehicle->SetPartByName(partName, static_cast<ai::VehiclePart*>(partObj), false);
        }
        else
        {
            res = 0;
        }
    }
    return res;
}

int ZnayuKakProdatWnd::RestoreGadgetsState(int vehicleId, ItemSaveInfoMap const& containerToRestoreFrom)
{
    // RVA 0x48F250
    if (vehicleId == -1)
    {
        return 0;
    }
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return 0;
    }

    int res = 1;
    for (auto iIt = containerToRestoreFrom.begin(); iIt != containerToRestoreFrom.end(); ++iIt)
    {
        ItemSaveInfo const* item = iIt->second;
        if (!item)
        {
            res = 0;
            continue;
        }
        if (item->GetItemType() != ITEMTYPE_GADGET)
        {
            continue;
        }
        if (!item->IsValid())
        {
            res = 0;
            continue;
        }

        ai::Obj* obj = ai::theObjects->GetEntityByObjId(item->GetObjId());
        if (!obj || !obj->IsKindOf(&ai::Gadget::m_classGadget))
        {
            res = 0;
            continue;
        }

        auto* gadget = static_cast<ai::Gadget*>(obj);
        gadget->SetSlotNum(item->GetGadgetSlotId());
        if (!vehicle->AddGadget(gadget))
        {
            res = 0;
        }
    }
    return res;
}

int ZnayuKakProdatWnd::RestoreRepositoryItemsState(int vehicleId, ItemSaveInfoMap const& containerToRestoreFrom)
{
    // RVA 0x48F340
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return 0;
    }
    ai::IzvratRepository* repository = vehicle->GetRepository();
    if (!repository)
    {
        return 0;
    }

    int res = 1;
    for (auto iIt = containerToRestoreFrom.begin(); iIt != containerToRestoreFrom.end(); ++iIt)
    {
        ItemSaveInfo const* item = iIt->second;
        if (!item)
        {
            res = 0;
            continue;
        }
        if (item->GetItemType() != ITEMTYPE_REPOSITORY_ITEM)
        {
            continue;
        }
        if (!item->IsValid())
        {
            res = 0;
            continue;
        }

        PointBase<int> const repositoryPos = item->GetRepositoryPos();
        ai::GeomRepositoryItem repositoryItem(item->GetObjId());
        if (!repository->AddThingToPlace(repositoryItem, repositoryPos))
        {
            res = 0;
        }
    }
    return res;
}

int ZnayuKakProdatWnd::RestoreCurValues(int vehicleId, CurValues const& curValues)
{
    // RVA 0x48F430
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle)
    {
        return 0;
    }

    int res = 1;
    vehicle->Health().value().set(curValues.m_hp);
    vehicle->Fuel().value().set(curValues.m_fuel);

    for (auto it = vehicle->begin(); it != vehicle->end(); ++it)
    {
        ai::VehiclePart* part = it->second;
        if (!part)
        {
            res = 0;
            continue;
        }

        auto const durabilityIt = curValues.m_durability.find(it->first);
        if (durabilityIt == curValues.m_durability.end())
        {
            res = 0;
        }
        else
        {
            part->Durability().value().set(durabilityIt->second);
        }

        if (!IsGun(part))
        {
            continue;
        }

        auto const chargeIt = curValues.m_shellsInCurCharge.find(it->first);
        if (chargeIt == curValues.m_shellsInCurCharge.end())
        {
            res = 0;
        }
        else
        {
            help::SetGunShellsInCurrentCharge(part, chargeIt->second);
        }

        auto const poolIt = curValues.m_shellsInPool.find(it->first);
        if (poolIt == curValues.m_shellsInPool.end())
        {
            res = 0;
        }
        else
        {
            help::SetGunShellsInPool(part, poolIt->second);
        }
    }
    return res;
}

int ZnayuKakProdatWnd::CancelTradeState()
{
    // RVA 0x48F950
    if (!IsValid())
    {
        return 0;
    }
    if (!ClearTradeState())
    {
        return 0;
    }
    return RestoreTradeState();
}

// ---------------------------------------------------------------------------
// finishing the trade
// ---------------------------------------------------------------------------

void ZnayuKakProdatWnd::Cancel()
{
    // RVA 0x48F990
    if (!IsValid())
    {
        return;
    }

    if (ClearTradeState())
    {
        RestoreTradeState();
    }

    // NOTE: the shipped code looks the workshop vehicle up without checking the
    // result (or its type) before calling SetPartByName on it, so a stale
    // m_workshopVehicleId dereferences a null pointer here. Preserved as
    // shipped; in practice IsValid() above guarantees the id is set.
    ai::Obj* workshopVehicleObj = ai::theObjects->GetEntityByObjId(m_workshopVehicleId);
    auto* workshopVehicle = static_cast<ai::Vehicle*>(workshopVehicleObj);
    if (m_tradeType == TRADETYPE_CABIN)
    {
        workshopVehicle->SetPartByName(CABIN_PART_NAME, nullptr, false);
    }
    else if (m_tradeType == TRADETYPE_BASKET)
    {
        workshopVehicle->SetPartByName(BASKET_PART_NAME, nullptr, false);
    }

    FinishTrade(0);
}

void ZnayuKakProdatWnd::Buy()
{
    // RVA 0x48FA60
    int const tradeCost = GetCurTradeCost();
    if (!CheckGold(tradeCost) || !CheckLeftItems())
    {
        return;
    }

    int bought;
    switch (m_tradeType)
    {
    case TRADETYPE_VEHICLE:
        bought = BuyVehicle();
        break;
    case TRADETYPE_CABIN:
        bought = BuyCabin();
        break;
    case TRADETYPE_BASKET:
        bought = BuyBasket();
        break;
    default:
        // NOTE: for an unknown trade type the shipped code jumps straight to the
        // money deduction without buying anything. Preserved as shipped.
        bought = 1;
        break;
    }

    if (bought)
    {
        ai::thePlayer->AddMoney(-tradeCost);
    }
    FinishTrade(1);
}

int ZnayuKakProdatWnd::BuyVehicle()
{
    // RVA 0x48FB10
    ai::Vehicle* playerVehicle = ai::thePlayer->GetVehicle();
    if (!playerVehicle)
    {
        return 0;
    }
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return 0;
    }
    if (m_workshopVehicleId == -1)
    {
        return 1;
    }

    // Anything left in the old vehicle's basket goes back to the workshop.
    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    if (vehicle)
    {
        ai::VehiclePart const* basket = vehicle->GetPartByName(BASKET_PART_NAME);
        if (basket && basket->GetId() != -1)
        {
            SellRepository();
        }
    }

    ai::GeomRepository* vehiclesRepository = workshop->GetRepositoryByType(ai::WORKSHOP_VEHICLES);
    vehiclesRepository->GiveUpThingByObjId(m_workshopVehicleId);
    ai::thePlayer->ChangeVehicleByExisting(m_workshopVehicleId, false);

    auto* newVehicle = static_cast<ai::Vehicle*>(ai::theObjects->GetEntityByObjId(m_workshopVehicleId));
    newVehicle->SetGamePositionOnGround(playerVehicle->GetPosition(), true, false);
    newVehicle->EnableSounds(false);
    AddItemToWorkshop(playerVehicle->GetId());
    return 1;
}

int ZnayuKakProdatWnd::BuyCabin()
{
    // RVA 0x48FAF0
    return ReplaceParentPart(CABIN_PART_NAME);
}

int ZnayuKakProdatWnd::BuyBasket()
{
    // RVA 0x48FB00
    return ReplaceParentPart(BASKET_PART_NAME);
}

int ZnayuKakProdatWnd::ReplaceParentPart(CStr const& parentPartName)
{
    // RVA 0x48FC30 - swaps the named part (and everything hanging off it)
    // between the player's vehicle and the workshop's one.
    ai::Vehicle* playerVehicle = ai::thePlayer->GetVehicle();
    if (!playerVehicle)
    {
        return 0;
    }
    ai::Vehicle* workshopVehicle = GetVehicleById(m_workshopVehicleId);
    if (!workshopVehicle)
    {
        return 0;
    }

    ai::VehiclePart* newParentPart = workshopVehicle->GetPartByName(parentPartName);
    if (!newParentPart)
    {
        return 0;
    }
    ai::VehiclePart* oldParentPart = playerVehicle->GetPartByName(parentPartName);
    if (!oldParentPart)
    {
        return 0;
    }
    ai::VehiclePrototypeInfo const* playerVehicleInfo = playerVehicle->GetPrototypeInfo();
    if (!playerVehicleInfo)
    {
        return 0;
    }
    ai::VehiclePrototypeInfo const* workshopVehicleInfo = workshopVehicle->GetPrototypeInfo();
    if (!workshopVehicleInfo)
    {
        return 0;
    }

    // Detach the guns mounted on the bought part so they survive the swap.
    std::map<CStr, ai::VehiclePart*> newGuns;
    for (auto ngIt = workshopVehicle->begin(); ngIt != workshopVehicle->end();)
    {
        auto nextIt = ngIt;
        ++nextIt;

        CStr const partName = ngIt->first;
        ai::ComplexPhysicObjPartDescription const* desc = workshopVehicleInfo->GetPartDescriptionByName(partName);
        if (!desc)
        {
            return 0;
        }
        ai::ComplexPhysicObjPartDescription const* parent = desc->GetParent();
        if (parent && CStr::my_strcmp(parent->GetName(), parentPartName.c_str()) == 0 && ngIt->second)
        {
            newGuns.insert(std::pair<CStr, ai::VehiclePart*>(partName, ngIt->second));
            workshopVehicle->SetPartByName(partName, nullptr, false);
        }
        ngIt = nextIt;
    }

    // Everything of the player's that hung off the old part is sold.
    for (auto it = playerVehicle->begin(); it != playerVehicle->end();)
    {
        auto nextIt = it;
        ++nextIt;

        CStr const partName = it->first;
        ai::ComplexPhysicObjPartDescription const* desc = playerVehicleInfo->GetPartDescriptionByName(partName);
        if (desc)
        {
            ai::ComplexPhysicObjPartDescription const* parent = desc->GetParent();
            if (parent && CStr::my_strcmp(parent->GetName(), parentPartName.c_str()) == 0)
            {
                ai::VehiclePart* part = it->second;
                if (part)
                {
                    playerVehicle->SetPartByName(partName, nullptr, false);
                    AddItemToWorkshop(part->GetId());
                }
            }
        }
        it = nextIt;
    }

    std::map<int, ai::Gadget*> newGadgets;
    std::map<int, ai::Gadget*> oldGadgets;
    std::vector<ai::GeomRepositoryItem> repositoryItems;

    if (CStr::my_strcmp(parentPartName.c_str(), CABIN_PART_NAME.c_str()) == 0)
    {
        newGadgets = workshopVehicle->GetGadgets();
        oldGadgets = playerVehicle->GetGadgets();

        for (auto ngIt = newGadgets.begin(); ngIt != newGadgets.end(); ++ngIt)
        {
            workshopVehicle->RemoveChild(ngIt->second);
        }
        for (auto ogIt = oldGadgets.begin(); ogIt != oldGadgets.end(); ++ogIt)
        {
            playerVehicle->RemoveChild(ogIt->second);
            if (ogIt->second)
            {
                AddItemToWorkshop(ogIt->second->GetId());
            }
        }
        oldGadgets.clear();
    }

    if (CStr::my_strcmp(parentPartName.c_str(), BASKET_PART_NAME.c_str()) == 0)
    {
        SellRepository();
        if (ai::IzvratRepository* workshopRepository = workshopVehicle->GetRepository())
        {
            int const numItems = static_cast<int>(workshopRepository->GetNumItems());
            for (int i = 0; i < numItems; ++i)
            {
                repositoryItems.push_back(workshopRepository->GetItem(i));
                workshopRepository->GiveUpThingFromSlotUnsafe(i, 1u);
            }
            workshopRepository->Purge();
        }
    }

    workshopVehicle->SetPartByName(parentPartName, nullptr, false);
    int const skin = static_cast<int>(playerVehicle->GetSkin());
    playerVehicle->SetPartByName(parentPartName, newParentPart, false);
    SkinSwitcher::ApplySkin(skin, playerVehicle);

    for (auto ngIt = newGuns.begin(); ngIt != newGuns.end(); ++ngIt)
    {
        playerVehicle->SetPartByName(ngIt->first, ngIt->second, false);
    }

    if (CStr::my_strcmp(parentPartName.c_str(), CABIN_PART_NAME.c_str()) == 0)
    {
        for (auto ngIt = newGadgets.begin(); ngIt != newGadgets.end(); ++ngIt)
        {
            playerVehicle->AddGadget(ngIt->second);
        }
    }

    if (CStr::my_strcmp(parentPartName.c_str(), BASKET_PART_NAME.c_str()) == 0)
    {
        if (ai::IzvratRepository* playerRepository = playerVehicle->GetRepository())
        {
            for (auto& item : repositoryItems)
            {
                if (!playerRepository->AddThingToPlace(item, item.m_origin))
                {
                    playerRepository->AddThing(item, 0);
                }
            }
        }
    }

    RemoveItemFromWorkshop(newParentPart->GetId());
    AddItemToWorkshop(oldParentPart->GetId());
    return 1;
}

int ZnayuKakProdatWnd::SellRepository()
{
    // RVA 0x490280 - everything the player brought along in their own repository
    // is handed over to the workshop, then the repository is emptied.
    ai::IzvratRepository* repository = ai::thePlayer->GetRepository();
    if (!repository)
    {
        return 0;
    }

    int res = 1;
    int const numRepositoryItems = static_cast<int>(repository->GetNumItems());
    for (int i = 0; i < numRepositoryItems; ++i)
    {
        ai::GeomRepositoryItem item = repository->GetItem(i);
        if (m_originalPlayerItems.find(item.GetObjId()) == m_originalPlayerItems.end())
        {
            continue;
        }
        if (!repository->GiveUpThingFromSlotUnsafe(i, 1u))
        {
            res = 0;
        }
        res &= AddItemToWorkshop(item.GetObjId());
    }
    repository->Clear(false);
    return res;
}

void ZnayuKakProdatWnd::FinishTrade(int ret)
{
    // RVA 0x4903A0
    TradeType const tradeType = m_tradeType;
    GameDataClear(false);
    M3D_APP->EnqueueMessage(UM_TRADE_FINISHED, ret, tradeType, 0, 0, CStr(), m3d::AIParam());
}

int ZnayuKakProdatWnd::CheckGold(int tradeCost)
{
    // RVA 0x4915A0
    if (tradeCost <= ai::thePlayer->GetMoney())
    {
        return 1;
    }
    M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("NotEnoughGold")), 1u, false);
    return 0;
}

int ZnayuKakProdatWnd::CheckLeftItems()
{
    // RVA 0x491680 - warn before a trade that would hand the player's own items
    // over along with the traded part.
    int const playerVehicleId = GetVehicleId(BELONG_PLAYER);

    CStr msgStrId;
    bool hasItems;
    switch (m_tradeType)
    {
    case TRADETYPE_VEHICLE:
        hasItems = VehicleHasItems(playerVehicleId);
        msgStrId = m_aif.m_strIdLeftItemsInVehicle;
        break;
    case TRADETYPE_CABIN:
        hasItems = CabinHasItems(playerVehicleId);
        msgStrId = m_aif.m_strIdLeftItemsInCabin;
        break;
    case TRADETYPE_BASKET:
        hasItems = BasketHasItems(playerVehicleId);
        msgStrId = m_aif.m_strIdLeftItemsInBasket;
        break;
    default:
        return 1;
    }

    if (!hasItems)
    {
        return 1;
    }
    return M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(msgStrId), 2u, false) == m3d::ui::MBX_RET_YES;
}

bool ZnayuKakProdatWnd::CabinHasItems(int vehicleId) const
{
    // RVA 0x491850
    if (vehicleId == -1)
    {
        return false;
    }
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle || !vehicle->GetCabin())
    {
        return false;
    }

    std::vector<CStr> const childPartNames = GetChildVehiclePartNamesByParentPart(vehicleId, CABIN_PART_NAME);
    return !childPartNames.empty() || vehicle->GetGadgets().size() != 0;
}

bool ZnayuKakProdatWnd::BasketHasItems(int vehicleId) const
{
    // RVA 0x491900
    if (vehicleId == -1)
    {
        return false;
    }
    ai::Vehicle* vehicle = GetVehicleById(vehicleId);
    if (!vehicle || !vehicle->GetBasket())
    {
        return false;
    }

    std::vector<CStr> const childPartNames = GetChildVehiclePartNamesByParentPart(vehicleId, BASKET_PART_NAME);
    if (!childPartNames.empty())
    {
        return true;
    }
    ai::IzvratRepository* repository = vehicle->GetRepository();
    return repository && repository->GetNumItems() != 0;
}

bool ZnayuKakProdatWnd::VehicleHasItems(int vehicleId) const
{
    // RVA 0x4919C0
    return CabinHasItems(vehicleId) || BasketHasItems(vehicleId);
}

// ---------------------------------------------------------------------------
// workshop bookkeeping
// ---------------------------------------------------------------------------

int ZnayuKakProdatWnd::AddItemToWorkshop(int itemId)
{
    // RVA 0x490A90
    ai::Obj* item = ai::theObjects->GetEntityByObjId(itemId);
    if (!item)
    {
        return 0;
    }
    ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (!town)
    {
        return 0;
    }
    ai::Workshop* workshop = town->GetWorkshopByObject(item);
    if (!workshop)
    {
        return 0;
    }
    ai::PrototypeInfo const* proto = item->GetPrototypeInfo();
    if (!proto)
    {
        return 0;
    }
    int const resourceId = proto->m_resourceId;
    if (resourceId == -1)
    {
        return 0;
    }

    if (IsResourceKindOf(resourceId, "GOODS"))
    {
        int const res = workshop->BuyArticle(item->GetId());
        item->Remove();
        return res;
    }

    ai::GeomRepository* repository =
        workshop->GetRepositoryByType(ai::Workshop::GetRepositoryTypeByResourceId(resourceId));
    if (!repository)
    {
        item->Remove();
        return 1;
    }

    int const repaired = RepairThing(itemId) & 1;
    return repository->AddThing(ai::GeomRepositoryItem(itemId), 0) & repaired;
}

bool ZnayuKakProdatWnd::RemoveItemFromWorkshop(int itemId)
{
    // RVA 0x490C00
    if (itemId == -1)
    {
        return false;
    }
    ai::Obj* item = ai::theObjects->GetEntityByObjId(itemId);
    if (!item)
    {
        return false;
    }
    ai::Town* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (!town)
    {
        return false;
    }
    ai::Workshop* workshop = town->GetWorkshopByObject(item);
    if (!workshop)
    {
        return false;
    }
    ai::PrototypeInfo const* proto = item->GetPrototypeInfo();
    if (!proto)
    {
        return false;
    }
    int const resourceId = proto->m_resourceId;
    if (resourceId == -1)
    {
        return false;
    }
    if (IsResourceKindOf(resourceId, "GOODS"))
    {
        return false;
    }

    ai::GeomRepository* repository =
        workshop->GetRepositoryByType(ai::Workshop::GetRepositoryTypeByResourceId(resourceId));
    if (repository)
    {
        return repository->GiveUpThingByObjId(itemId);
    }
    item->Remove();
    return true;
}

int ZnayuKakProdatWnd::RepairThing(int itemId)
{
    // RVA 0x491520
    if (itemId == -1)
    {
        return 0;
    }
    ai::Obj* item = ai::theObjects->GetEntityByObjId(itemId);
    if (!item || !item->IsKindOf(&ai::Obj::m_classObj))
    {
        return 0;
    }
    help::RepairObj(item);
    return 1;
}

void ZnayuKakProdatWnd::DestroyHackedVehicle()
{
    // Declared by the shipped headers but completely absent from the shipped
    // binary - the compiler dropped it because nothing calls it, so there is no
    // ground truth to port. Left unimplemented on purpose rather than guessed.
    RETRUXX_NOT_IMPLEMENTED;
}

// ---------------------------------------------------------------------------
// transfers between the two vehicles
// ---------------------------------------------------------------------------

void ZnayuKakProdatWnd::TransferAll(Belong srcBelong, Belong dstBelong)
{
    // RVA 0x490340
    if (srcBelong == BELONG_INVALID || dstBelong == BELONG_INVALID || srcBelong == dstBelong)
    {
        return;
    }

    int const srcVehicleId = GetVehicleId(srcBelong);
    int const dstVehicleId = GetVehicleId(dstBelong);
    switch (m_tradeType)
    {
    case TRADETYPE_VEHICLE:
        TransferVehicleItems(srcVehicleId, dstVehicleId);
        break;
    case TRADETYPE_CABIN:
        TransferCabinItems(srcVehicleId, dstVehicleId);
        break;
    case TRADETYPE_BASKET:
        TransferBasketItems(srcVehicleId, dstVehicleId);
        break;
    default:
        break;
    }
}

int ZnayuKakProdatWnd::TransferVehicleItems(int srcVehicleId, int dstVehicleId)
{
    // RVA 0x490E90
    int res = TransferVehicleParts(srcVehicleId, dstVehicleId, CABIN_PART_NAME) & 1;
    res = res & TransferGadgets(srcVehicleId, dstVehicleId) & 1;

    if (GetBasketId(GetBelongByVehicleId(srcVehicleId)) == -1)
    {
        return res;
    }
    if (GetBasketId(GetBelongByVehicleId(dstVehicleId)) == -1)
    {
        return res;
    }

    int const basketRes = TransferVehicleParts(srcVehicleId, dstVehicleId, BASKET_PART_NAME) & 1;
    return basketRes & TransferRepositoryItems(srcVehicleId, dstVehicleId) & res;
}

int ZnayuKakProdatWnd::TransferCabinItems(int srcVehicleId, int dstVehicleId)
{
    // RVA 0x490F70
    int const partsRes = TransferVehicleParts(srcVehicleId, dstVehicleId, CABIN_PART_NAME) & 1;
    return partsRes & TransferGadgets(srcVehicleId, dstVehicleId);
}

int ZnayuKakProdatWnd::TransferBasketItems(int srcVehicleId, int dstVehicleId)
{
    // RVA 0x490FA0
    int const partsRes = TransferVehicleParts(srcVehicleId, dstVehicleId, BASKET_PART_NAME) & 1;
    return partsRes & TransferRepositoryItems(srcVehicleId, dstVehicleId);
}

int ZnayuKakProdatWnd::TransferVehicleParts(int srcVehicleId, int dstVehicleId, CStr const& parentPartName)
{
    // RVA 0x490FD0
    if (srcVehicleId == -1 || dstVehicleId == -1 || parentPartName.empty())
    {
        return 0;
    }
    ai::Vehicle* srcVehicle = GetVehicleById(srcVehicleId);
    if (!srcVehicle)
    {
        return 0;
    }
    ai::VehiclePrototypeInfo const* srcVehicleInfo = srcVehicle->GetPrototypeInfo();
    if (!srcVehicleInfo)
    {
        return 0;
    }
    ai::Vehicle* dstVehicle = GetVehicleById(dstVehicleId);
    if (!dstVehicle || !dstVehicle->GetPrototypeInfo())
    {
        return 0;
    }

    int res = 1;
    for (auto it = srcVehicle->begin(); it != srcVehicle->end();)
    {
        auto nextIt = it;
        ++nextIt;

        CStr const partName = it->first;
        ai::VehiclePart* part = it->second;
        if (!part)
        {
            res = 0;
            it = nextIt;
            continue;
        }

        ai::ComplexPhysicObjPartDescription const* desc = srcVehicleInfo->GetPartDescriptionByName(partName);
        if (!desc)
        {
            res = 0;
            it = nextIt;
            continue;
        }
        ai::ComplexPhysicObjPartDescription const* parent = desc->GetParent();
        if (!parent || CStr::my_strcmp(parent->GetName(), parentPartName.c_str()) != 0)
        {
            it = nextIt;
            continue;
        }

        if (dstVehicle->CanPartBeAttached(partName) && !dstVehicle->GetPartByName(partName))
        {
            srcVehicle->SetPartByName(partName, nullptr, false);
            dstVehicle->SetPartByName(partName, part, false);
        }
        else if (m_tradeType != TRADETYPE_CABIN)
        {
            // Not mountable: drop it into the destination repository instead.
            ai::IzvratRepository* repository = dstVehicle->GetRepository();
            if (!repository)
            {
                res = 0;
            }
            else
            {
                srcVehicle->SetPartByName(partName, nullptr, false);
                if (!repository->AddThing(ai::GeomRepositoryItem(part->GetId()), 0))
                {
                    srcVehicle->SetPartByName(partName, part, false);
                }
            }
        }

        it = nextIt;
    }
    return res;
}

int ZnayuKakProdatWnd::TransferGadgets(int srcVehicleId, int dstVehicleId)
{
    // RVA 0x4911D0
    if (srcVehicleId == -1 || dstVehicleId == -1)
    {
        return 0;
    }
    ai::Vehicle* srcVehicle = GetVehicleById(srcVehicleId);
    if (!srcVehicle)
    {
        return 0;
    }
    ai::Vehicle* dstVehicle = GetVehicleById(dstVehicleId);
    if (!dstVehicle)
    {
        return 0;
    }

    int res = 1;
    auto const& srcGadgets = srcVehicle->GetGadgets();
    for (auto it = srcGadgets.begin(); it != srcGadgets.end();)
    {
        auto nextIt = it;
        ++nextIt;

        ai::Gadget* gadget = it->second;
        if (!gadget)
        {
            res = 0;
            it = nextIt;
            continue;
        }

        int const oldSlotNum = gadget->GetSlotNum();
        gadget->SetSlotNum(-1);
        int const validSlotId = dstVehicle->GetValidSlotIdForGadget(gadget);
        gadget->SetSlotNum(oldSlotNum);

        if (validSlotId != -1)
        {
            srcVehicle->RemoveChild(gadget);
            gadget->SetSlotNum(validSlotId);
            if (!dstVehicle->AddGadget(gadget))
            {
                gadget->SetSlotNum(oldSlotNum);
                srcVehicle->AddGadget(gadget);
            }
        }
        else if (m_tradeType == TRADETYPE_VEHICLE)
        {
            // Only a whole-vehicle trade falls back to the repository.
            ai::IzvratRepository* repository = dstVehicle->GetRepository();
            if (!repository)
            {
                res = 0;
            }
            else if (repository->AddThing(ai::GeomRepositoryItem(gadget->GetId()), 0))
            {
                srcVehicle->RemoveChild(gadget);
            }
        }

        it = nextIt;
    }
    return res;
}

int ZnayuKakProdatWnd::TransferRepositoryItems(int srcVehicleId, int dstVehicleId)
{
    // RVA 0x491380
    if (srcVehicleId == -1 || dstVehicleId == -1)
    {
        return 0;
    }
    ai::Vehicle* srcVehicle = GetVehicleById(srcVehicleId);
    if (!srcVehicle)
    {
        return 0;
    }
    ai::Vehicle* dstVehicle = GetVehicleById(dstVehicleId);
    if (!dstVehicle)
    {
        return 0;
    }

    ai::IzvratRepository* srcRepository = srcVehicle->GetRepository();
    ai::IzvratRepository* dstRepository = dstVehicle->GetRepository();
    if (!srcRepository || !dstRepository)
    {
        return 0;
    }
    srcRepository->TransferToRepository(dstRepository);
    return 1;
}

// ---------------------------------------------------------------------------
// cost
// ---------------------------------------------------------------------------

int ZnayuKakProdatWnd::GetCurTradeCost() const
{
    // RVA 0x490450
    if (m_workshopItemsCostToBuy == -1 || m_playerItemsCostToSell == -1)
    {
        return -1;
    }
    return m_workshopItemsCostToBuy - m_playerItemsCostToSell;
}

int ZnayuKakProdatWnd::GetPlayerItemsCostToSell() const
{
    // RVA 0x490A70
    return m_playerItemsCostToSell;
}

int ZnayuKakProdatWnd::GetWorkshopItemsCostToBuy() const
{
    // RVA 0x490A80
    return m_workshopItemsCostToBuy;
}

int ZnayuKakProdatWnd::CalcItemsCostByBelong(Belong belong) const
{
    // RVA 0x490470 - only the items that are still where they started count: for
    // the player that is what is sold along with the traded object, for the
    // workshop what is bought along with it.
    if (belong == BELONG_INVALID)
    {
        return -1;
    }
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return -1;
    }

    ItemSaveInfoMap const& curItems = belong == BELONG_PLAYER ? m_curPlayerItems : m_curWorkshopItems;
    ItemSaveInfoMap const& originalItems = belong == BELONG_PLAYER ? m_originalPlayerItems : m_originalWorkshopItems;

    int cost = 0;
    for (auto curItemIt = curItems.begin(); curItemIt != curItems.end(); ++curItemIt)
    {
        if (originalItems.find(curItemIt->first) == originalItems.end())
        {
            continue;
        }
        if (!IsItemRelevantForCurTradeState(curItemIt->second))
        {
            continue;
        }

        int const itemId = curItemIt->first;
        if (itemId == -1)
        {
            return -1;
        }
        int const itemCost = belong == BELONG_PLAYER ? help::GetSellPriceByObjId(itemId, workshop->GetParentId())
                                                     : help::GetBuyPriceByObjId(itemId, workshop->GetParentId());
        if (itemCost == -1)
        {
            return -1;
        }
        cost += itemCost;
    }
    return cost;
}

int ZnayuKakProdatWnd::CalcOneItemCost(int itemId, ai::Workshop const* workshop, Belong belong) const
{
    // RVA 0x4905B0
    if (itemId == -1 || !workshop || belong == BELONG_INVALID)
    {
        return -1;
    }
    return belong == BELONG_PLAYER ? help::GetSellPriceByObjId(itemId, workshop->GetParentId())
                                   : help::GetBuyPriceByObjId(itemId, workshop->GetParentId());
}

bool ZnayuKakProdatWnd::IsItemRelevantForCurTradeState(ItemSaveInfo const* item) const
{
    // RVA 0x490D20 - a cabin trade never involves basket contents, and the other
    // way round.
    if (!item)
    {
        return false;
    }

    switch (item->GetItemType())
    {
    case ITEMTYPE_GADGET:
    case ITEMTYPE_VEHICLE_PART_CABIN:
        return m_tradeType != TRADETYPE_BASKET;
    case ITEMTYPE_VEHICLE_PART_BASKET:
    case ITEMTYPE_REPOSITORY_ITEM:
        return m_tradeType != TRADETYPE_CABIN;
    case ITEMTYPE_MAIN_ITEM:
        break;
    default:
        return false;
    }

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(item->GetObjId());
    if (!obj)
    {
        return false;
    }
    ai::PrototypeInfo const* proto = obj->GetPrototypeInfo();
    if (!proto)
    {
        return false;
    }

    int const resourceId = proto->m_resourceId;
    if (IsResourceKindOf(resourceId, "CHASSIS"))
    {
        return m_tradeType == TRADETYPE_VEHICLE;
    }
    if (IsResourceKindOf(resourceId, "CABIN"))
    {
        return m_tradeType != TRADETYPE_BASKET;
    }
    return IsResourceKindOf(resourceId, "BASKET") && m_tradeType != TRADETYPE_CABIN;
}

void ZnayuKakProdatWnd::UpdateItemsCost()
{
    // RVA 0x490A50
    m_playerItemsCostToSell = CalcItemsCostByBelong(BELONG_PLAYER);
    m_workshopItemsCostToBuy = CalcItemsCostByBelong(BELONG_WORKSHOP);
}

void ZnayuKakProdatWnd::UpdateTradeCost()
{
    // NOTE: declared by the shipped headers but not present in the shipped
    // binary - the compiler dropped it because nothing calls it. Reconstructed
    // from its name and from UpdateOnCurTradeStateChanged (RVA 0x4909E0), which
    // is the caller-side equivalent minus the state save.
    UpdateItemsCost();
    UpdateTradeCostWnd(GetCurTradeCost());
    UpdatePlayerItemsCostWnd(m_playerItemsCostToSell);
    UpdateWorkshopItemsCostWnd(m_workshopItemsCostToBuy);
}

void ZnayuKakProdatWnd::UpdateOnCurTradeStateChanged()
{
    // RVA 0x4909E0
    SaveTradeState(false);
    m_playerItemsCostToSell = CalcItemsCostByBelong(BELONG_PLAYER);
    m_workshopItemsCostToBuy = CalcItemsCostByBelong(BELONG_WORKSHOP);

    if (m_workshopItemsCostToBuy == -1 || m_playerItemsCostToSell == -1)
    {
        UpdateTradeCostWnd(-1);
    }
    else
    {
        UpdateTradeCostWnd(m_workshopItemsCostToBuy - m_playerItemsCostToSell);
    }
    UpdatePlayerItemsCostWnd(m_playerItemsCostToSell);
    UpdateWorkshopItemsCostWnd(m_workshopItemsCostToBuy);
}

void ZnayuKakProdatWnd::UpdateTradeCostWnd(int summaryCost)
{
    // RVA 0x4905E0 - the label shows what the deal is worth to the player, so a
    // positive cost (money leaving the player) is displayed negated.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    CStr const sign = summaryCost < 0 ? CStr("+") : CStr();
    m_wndTradeCost->SetText(sign + CStr(-summaryCost));
}

void ZnayuKakProdatWnd::UpdatePlayerItemsCostWnd(int)
{
    // RVA 0x4906B0 - the shipped code ignores the argument and always formats
    // m_playerItemsCostToSell.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndPlayerItemsCost->SetText(CStr("+") + CStr(m_playerItemsCostToSell));
}

void ZnayuKakProdatWnd::UpdateWorkshopItemsCostWnd(int)
{
    // RVA 0x490770 - as above, the argument is ignored and
    // m_workshopItemsCostToBuy is formatted instead.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndWorkshopItemsCost->SetText(CStr("-") + CStr(m_workshopItemsCostToBuy));
}

int ZnayuKakProdatWnd::GetItemCost(int itemId) const
{
    // RVA 0x491AD0 - CalcOneItemCost inlined against this window's workshop.
    Belong const belong = GetItemBelong(itemId);
    ai::Workshop* workshop = GetWorkshop();
    if (itemId == -1 || !workshop || belong == BELONG_INVALID)
    {
        return -1;
    }
    return belong == BELONG_PLAYER ? help::GetSellPriceByObjId(itemId, workshop->GetParentId())
                                   : help::GetBuyPriceByObjId(itemId, workshop->GetParentId());
}

ZnayuKakProdatWnd::Belong ZnayuKakProdatWnd::GetItemBelong(int itemId) const
{
    // RVA 0x491B10 - an item that is in both original sets (or in neither) has
    // no well-defined owner.
    if (itemId == -1)
    {
        return BELONG_INVALID;
    }

    bool const inPlayerItems = m_originalPlayerItems.find(itemId) != m_originalPlayerItems.end();
    bool const inWorkshopItems = m_originalWorkshopItems.find(itemId) != m_originalWorkshopItems.end();
    if (inPlayerItems == inWorkshopItems)
    {
        return BELONG_INVALID;
    }
    return inPlayerItems ? BELONG_PLAYER : BELONG_WORKSHOP;
}

// ---------------------------------------------------------------------------
// game data notifications
// ---------------------------------------------------------------------------

void ZnayuKakProdatWnd::OnVehiclePartChanged(void* data)
{
    // RVA 0x490830
    if (!IsChildOf(M3D_APP) || !data || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    int const vehicleId = static_cast<int*>(data)[13];
    if (vehicleId == -1)
    {
        return;
    }
    if (vehicleId == GetVehicleId(BELONG_PLAYER) || vehicleId == m_workshopVehicleId)
    {
        UpdateOnCurTradeStateChanged();
    }
}

void ZnayuKakProdatWnd::OnGadgetChanged(void* data)
{
    // RVA 0x4908A0
    if (!IsChildOf(M3D_APP) || !data || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    int const vehicleId = static_cast<int*>(data)[13];
    if (vehicleId == -1)
    {
        return;
    }
    if (vehicleId == GetVehicleId(BELONG_PLAYER) || vehicleId == m_workshopVehicleId)
    {
        UpdateOnCurTradeStateChanged();
    }
}

void ZnayuKakProdatWnd::OnRepositoryChanged(void* data)
{
    // RVA 0x490910
    if (!IsChildOf(M3D_APP) || !data || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    auto const* repository = static_cast<ai::IzvratRepository const*>(static_cast<void**>(data)[13]);
    if (!repository)
    {
        return;
    }

    ai::Vehicle* playerVehicle = GetVehicleById(GetVehicleId(BELONG_PLAYER));
    if (!playerVehicle)
    {
        return;
    }

    if (playerVehicle->GetRepository() == repository)
    {
        UpdateOnCurTradeStateChanged();
        return;
    }

    ai::Vehicle* workshopVehicle = GetVehicleById(m_workshopVehicleId);
    if (workshopVehicle && workshopVehicle->GetRepository() == repository)
    {
        UpdateOnCurTradeStateChanged();
    }
}

void ZnayuKakProdatWnd::UpdateTransferButtonsTooltips()
{
    // RVA 0x4919F0
    if ((m_gameDataFlags & 1) == 0 || m_tradeType == TRADETYPE_NUM_TRADETYPES)
    {
        return;
    }

    CStr tipFromPlayer =
        M3D_APP->GetStringByStringId0(m_aif.m_strIdTooltipTransferItemsFromPlayerToWorkshop[m_tradeType]);
    CStr tipFromWorkshop =
        M3D_APP->GetStringByStringId0(m_aif.m_strIdTooltipTransferItemsFromWorkshopToPlayer[m_tradeType]);

    m_btnTransferItemsFromPlayerToWorkshop->SetProperty(
        PROP_WND_TOOLTIP, const_cast<char*>(tipFromPlayer.c_str()));
    m_btnTransferItemsFromWorkshopToPlayer->SetProperty(
        PROP_WND_TOOLTIP, const_cast<char*>(tipFromWorkshop.c_str()));
}
