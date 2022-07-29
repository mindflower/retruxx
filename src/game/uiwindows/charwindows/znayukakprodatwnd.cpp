#include "znayukakprodatwnd.h"

RT_CLASS_EXPORTS_BEGIN(ZnayuKakProdatWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ZnayuKakProdatWnd);

ZnayuKakProdatWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::CurValues::Clear()
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::CurValues::CurValues()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsGadget(int, Belong, int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ItemSaveInfo::GetGadgetSlotId() const
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::Belong ZnayuKakProdatWnd::ItemSaveInfo::GetBelong() const
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::ItemType ZnayuKakProdatWnd::ItemSaveInfo::GetItemType() const
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::ItemSaveInfo::ItemSaveInfo()
{
    throw std::logic_error("Not implemented");
}

PointBase<int> ZnayuKakProdatWnd::ItemSaveInfo::GetRepositoryPos() const
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::ItemSaveInfo::~ItemSaveInfo()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ItemSaveInfo::GetObjId() const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsVehiclePart(int, Belong, CStr const&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

bool ZnayuKakProdatWnd::ItemSaveInfo::IsValid() const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsMainItem(int, Belong)
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::ItemSaveInfo::Invalidate()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ItemSaveInfo::SetupAsRepositoryItem(int, Belong, PointBase<int> const&)
{
    throw std::logic_error("Not implemented");
}

CStr ZnayuKakProdatWnd::ItemSaveInfo::GetVehiclePartName() const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::GetWorkshopItemsCostToBuy() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ZnayuKakProdatWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::Belong ZnayuKakProdatWnd::GetItemBelong(int) const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::GetPlayerItemsCostToSell() const
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle* ZnayuKakProdatWnd::GetWorkshopVehicle() const
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::TradeType ZnayuKakProdatWnd::GetTradeType() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ZnayuKakProdatWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::SetupForTrade(int, int, int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::GetItemCost(int) const
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::~ZnayuKakProdatWnd()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::GetCurTradeCost() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ZnayuKakProdatWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ZnayuKakProdatWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ClearCabinFromItems(int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::SaveGadgetsState(int, Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::OnRepositoryChanged(void*)
{
    throw std::logic_error("Not implemented");
}

bool ZnayuKakProdatWnd::CabinHasItems(int) const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::RestoreCurValues(int, CurValues const&)
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::UpdateItemsCost()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ClearVehicleFromItems(int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::BuyCabin()
{
    throw std::logic_error("Not implemented");
}

bool ZnayuKakProdatWnd::IsItemRelevantForCurTradeState(ItemSaveInfo const*) const
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::Buy()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::SaveVehicleItemsState(Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::ClearSaveInfo(std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::GetVehicleId(Belong) const
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::UpdateOnCurTradeStateChanged()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::TransferBasketItems(int, int)
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::FinishTrade(int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::RestoreTradeState()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::BuyVehicle()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::SaveCurValues(int, CurValues&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::GetBasketId(Belong) const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::TransferVehicleParts(int, int, CStr const&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::SaveMainObject(int, Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::CalcOneItemCost(int, ai::Workshop const*, Belong) const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::RestoreGadgetsState(int, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ClearTradeState()
{
    throw std::logic_error("Not implemented");
}

ai::Workshop* ZnayuKakProdatWnd::GetWorkshop() const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ClearBasketFromItems(int)
{
    throw std::logic_error("Not implemented");
}

bool ZnayuKakProdatWnd::BasketHasItems(int) const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ClearFromRepositoryItems(int)
{
    throw std::logic_error("Not implemented");
}

bool ZnayuKakProdatWnd::IsValid() const
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::UpdateTransferButtonsTooltips()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::SaveBasketItemsState(Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::AddItemToWorkshop(int)
{
    throw std::logic_error("Not implemented");
}

bool ZnayuKakProdatWnd::VehicleHasItems(int) const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::RestoreVehicleItemsState(int, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    throw std::logic_error("Not implemented");
}

bool ZnayuKakProdatWnd::RemoveItemFromWorkshop(int)
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::TransferAll(Belong, Belong)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::SaveTradeState(bool)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::CheckLeftItems()
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::Belong ZnayuKakProdatWnd::GetBelongByVehicleId(int) const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::TransferGadgets(int, int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::RestoreRepositoryItemsState(int, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::TransferRepositoryItems(int, int)
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::OnVehiclePartChanged(void*)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::GetCabinId(Belong) const
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::UpdatePlayerItemsCostWnd(int)
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::OnGadgetChanged(void*)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ReplaceParentPart(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::RestoreBasketItemsState(int, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ClearFromVehicleParts(int, CStr const&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

std::vector<CStr, std::allocator<CStr>> ZnayuKakProdatWnd::GetChildVehiclePartNamesByParentPart(int, CStr const&) const
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::UpdateTradeCostWnd(int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::RepairThing(int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::RestoreVehiclePartsState(int, CStr const&, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::CancelTradeState()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::ClearFromGadgets(int)
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::Cancel()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::SellRepository()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::TransferVehicleItems(int, int)
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::ZnayuKakProdatWnd(ZnayuKakProdatWnd const&)
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::ZnayuKakProdatWnd()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::RestoreCabinItemsState(int, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>> const&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

void ZnayuKakProdatWnd::UpdateWorkshopItemsCostWnd(int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::CheckGold(int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::SaveCabinItemsState(Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::MakeBottomPanel()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::TransferCabinItems(int, int)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::SaveVehiclePartsState(int, CStr const&, Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    throw std::logic_error("Not implemented");
}

ZnayuKakProdatWnd::TradeType ZnayuKakProdatWnd::GetTradeTypeByObjectId(int) const
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::BuyBasket()
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::AddItem(ItemSaveInfo*, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::SaveRepositoryItemsState(int, Belong, std::map<int, ItemSaveInfo*, std::less<int>, std::allocator<std::pair<int const, ItemSaveInfo*>>>&)
{
    throw std::logic_error("Not implemented");
}

int ZnayuKakProdatWnd::CalcItemsCostByBelong(Belong) const
{
    throw std::logic_error("Not implemented");
}
