#include "znayukakprodatwnd.h"

RT_CLASS_EXPORTS_BEGIN(ZnayuKakProdatWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ZnayuKakProdatWnd);

ZnayuKakProdatWnd::AuxInfo::AuxInfo()
{
    m_wndTradeCostName = "wndTradeCost";
    m_wndPlayerItemsCostName = "wndPlayerItemsCost";
    m_wndWorkshopItemsCostName = "wndWorkshopItemsCost";
    m_wndBottomPanelName = "wndBottomPanel";
    m_btnTransferItemsFromPlayerToWorkshopName = "btnMoveAllFromPlayerToWorkshop";
    m_btnTransferItemsFromWorkshopToPlayerName = "LeftItemsInCabin";
    m_strIdLeftItemsInCabin = "LeftItemsInBasket";
    m_strIdLeftItemsInBasket = "LeftItemsInVehicle";
    m_strIdLeftItemsInVehicle = "MoveVehicleItemsFromPlayer";
    m_strIdTooltipTransferItemsFromPlayerToWorkshop[0] = "MoveCabinItemsFromPlayer";
    m_strIdTooltipTransferItemsFromPlayerToWorkshop[1] = "MoveBasketItemsFromPlayer";
    m_strIdTooltipTransferItemsFromPlayerToWorkshop[2] = "MoveVehicleItemsToPlayer";
    m_strIdTooltipTransferItemsFromWorkshopToPlayer[0] = "MoveCabinItemsToPlayer";
    m_strIdTooltipTransferItemsFromWorkshopToPlayer[1] = "MoveBasketItemsToPlayer";
    m_strIdTooltipTransferItemsFromWorkshopToPlayer[2] = "MoveBasketItemsToPlayer";
}

void ZnayuKakProdatWnd::CurValues::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ZnayuKakProdatWnd::CurValues::CurValues()
{
    m_hp = 0.0;
    m_fuel = 0.0;
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsGadget(int, Belong, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ItemSaveInfo::GetGadgetSlotId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ZnayuKakProdatWnd::Belong ZnayuKakProdatWnd::ItemSaveInfo::GetBelong() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ZnayuKakProdatWnd::ItemType ZnayuKakProdatWnd::ItemSaveInfo::GetItemType() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ZnayuKakProdatWnd::ItemSaveInfo::ItemSaveInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

PointBase<int> ZnayuKakProdatWnd::ItemSaveInfo::GetRepositoryPos() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ZnayuKakProdatWnd::ItemSaveInfo::~ItemSaveInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ItemSaveInfo::GetObjId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsVehiclePart(int, Belong, CStr const&, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ZnayuKakProdatWnd::ItemSaveInfo::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsMainItem(int, Belong)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::ItemSaveInfo::Invalidate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsRepositoryItem(int, Belong, PointBase<int> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr ZnayuKakProdatWnd::ItemSaveInfo::GetVehiclePartName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::GetWorkshopItemsCostToBuy() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ZnayuKakProdatWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

ZnayuKakProdatWnd::Belong ZnayuKakProdatWnd::GetItemBelong(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::GetPlayerItemsCostToSell() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle* ZnayuKakProdatWnd::GetWorkshopVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ZnayuKakProdatWnd::TradeType ZnayuKakProdatWnd::GetTradeType() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ZnayuKakProdatWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::SetupForTrade(int, int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::GetItemCost(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ZnayuKakProdatWnd::~ZnayuKakProdatWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::GetCurTradeCost() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ZnayuKakProdatWnd::CreateObject()
{
    return new ZnayuKakProdatWnd;
}

m3d::Class* ZnayuKakProdatWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ZnayuKakProdatWnd);
}

int ZnayuKakProdatWnd::ClearCabinFromItems(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::SaveGadgetsState(int, Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::OnRepositoryChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ZnayuKakProdatWnd::CabinHasItems(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::RestoreCurValues(int, CurValues const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::UpdateItemsCost()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ClearVehicleFromItems(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::BuyCabin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ZnayuKakProdatWnd::IsItemRelevantForCurTradeState(ItemSaveInfo const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::Buy()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::SaveVehicleItemsState(Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::ClearSaveInfo(std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::GetVehicleId(Belong) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::UpdateOnCurTradeStateChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::TransferBasketItems(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::FinishTrade(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::RestoreTradeState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::BuyVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::SaveCurValues(int, CurValues&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::GetBasketId(Belong) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::TransferVehicleParts(int, int, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::SaveMainObject(int, Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::CalcOneItemCost(int, ai::Workshop const*, Belong) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::RestoreGadgetsState(int, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ClearTradeState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Workshop* ZnayuKakProdatWnd::GetWorkshop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ClearBasketFromItems(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ZnayuKakProdatWnd::BasketHasItems(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ClearFromRepositoryItems(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ZnayuKakProdatWnd::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::UpdateTransferButtonsTooltips()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::SaveBasketItemsState(Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::AddItemToWorkshop(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ZnayuKakProdatWnd::VehicleHasItems(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::RestoreVehicleItemsState(int, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ZnayuKakProdatWnd::RemoveItemFromWorkshop(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::TransferAll(Belong, Belong)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::SaveTradeState(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::CheckLeftItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ZnayuKakProdatWnd::Belong ZnayuKakProdatWnd::GetBelongByVehicleId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::TransferGadgets(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::RestoreRepositoryItemsState(int, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::TransferRepositoryItems(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::OnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::GetCabinId(Belong) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::UpdatePlayerItemsCostWnd(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::OnGadgetChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ReplaceParentPart(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::RestoreBasketItemsState(int, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ClearFromVehicleParts(int, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

std::vector<CStr, std::allocator<CStr>> ZnayuKakProdatWnd::GetChildVehiclePartNamesByParentPart(int, CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::UpdateTradeCostWnd(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::RepairThing(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::RestoreVehiclePartsState(int, CStr const&, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::CancelTradeState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::ClearFromGadgets(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::Cancel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::SellRepository()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::TransferVehicleItems(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ZnayuKakProdatWnd::ZnayuKakProdatWnd(ZnayuKakProdatWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ZnayuKakProdatWnd::ZnayuKakProdatWnd()
{
    m_wndTradeCost = 0;
    m_wndPlayerItemsCost = 0;
    m_wndWorkshopItemsCost = 0;
    m_btnTransferItemsFromPlayerToWorkshop = 0;
    m_btnTransferItemsFromWorkshopToPlayer = 0;
    m_objectToTradeId = -1;
    m_workshopId = -1;
    m_playerItemsCostToSell = -1;
    m_workshopItemsCostToBuy = -1;
    m_tradeType = TRADETYPE_NUM_TRADETYPES;
}

int ZnayuKakProdatWnd::RestoreCabinItemsState(int, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ZnayuKakProdatWnd::UpdateWorkshopItemsCostWnd(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::CheckGold(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::SaveCabinItemsState(Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::MakeBottomPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::TransferCabinItems(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::GameDataSetup()
{
    // TODO: implement ZnayuKakProdatWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int ZnayuKakProdatWnd::SaveVehiclePartsState(int, CStr const&, Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ZnayuKakProdatWnd::TradeType ZnayuKakProdatWnd::GetTradeTypeByObjectId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::BuyBasket()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::AddItem(ItemSaveInfo*, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::SaveRepositoryItemsState(int, Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ZnayuKakProdatWnd::CalcItemsCostByBelong(Belong) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
