#include "garagewnd.h"
#include "rechargelist.h"
#include "repairlist.h"
#include "refuellist.h"
#include "skinswnd.h"
#include "basketwnd.h"
#include "basketlist.h"
#include "cabinlist.h"

RT_CLASS_EXPORTS_BEGIN(GarageWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GarageWnd);

m3d::Class* GarageWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::SetupForWorkshop(int)
{
    throw std::logic_error("Not implemented");
}

bool GarageWnd::IsListOpen() const
{
    throw std::logic_error("Not implemented");
}

ai::Workshop* GarageWnd::GetWorkshop() const
{
    throw std::logic_error("Not implemented");
}

help::Color GarageWnd::GetValueColor(int, int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* GarageWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::GetWorkshopId() const
{
    throw std::logic_error("Not implemented");
}

GarageWnd::~GarageWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* GarageWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* GarageWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnRepairList()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateAllPricePrevValues()
{
    throw std::logic_error("Not implemented");
}

unsigned GarageWnd::EnumColor2Color(help::Color) const
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::GetUnitsToRecharge() const
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnCabins()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::Refuel(int)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::MaxRefuel()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnRepairAll()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateRefuelAllTooltip()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnPlayerVehicleChanged()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateRefuelAllPriceControls(bool)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnRepositoryChanged(void*)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateNewVehicleActiveState()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateRefuelActiveState()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnBaskets()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::MaxRecharge()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnSkin()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OpenList(ref_ptr<m3d::ui::Wnd>, m3d::ui::ButtonWnd*, m3d::ui::ButtonWnd*, ai::eGameEvent)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnRechargeList()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnFinishTrade(void*)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateServiceActiveState(std::vector<m3d::ui::Wnd*, std::allocator<m3d::ui::Wnd*>> const&, bool, ref_ptr<m3d::ui::Wnd>)
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepository* GarageWnd::GetWorkshopRepositoryByType(ai::WorkshopRepositoryType) const
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateRechargeAllTooltip()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::GetPossibleRepair(int&, int&) const
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnRechargeAll()
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::GetUnitsToRepair() const
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::Recharge(int)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

float GarageWnd::GetHealthPriceForOneUnit() const
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::MaxRepair()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateSkinActiveState()
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnRefuelList()
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

GarageWnd::GarageWnd(GarageWnd const&)
{
    throw std::logic_error("Not implemented");
}

GarageWnd::GarageWnd()
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::GetShellPrice(int) const
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::Repair(int)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::GetPossibleRecharge(int&, int&) const
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateActiveStates()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateRepairActiveState()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateRepairAllPriceControls(bool)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateRepairAllTooltip()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnRefuelAll()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateAllPriceControls(bool)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnBuySkin()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::GetPossibleRefuel(int&, int&) const
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::GetTownId() const
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::CloseCurrentList()
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::GetUnitsToRefuel() const
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateBasketsActiveState()
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::GetFuelPriceForOneUnit() const
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateServiceSelectState(std::vector<m3d::ui::Wnd*, std::allocator<m3d::ui::Wnd*>> const&, bool)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateRechargeAllPriceControls(bool)
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int GarageWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateCabinsActiveState()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::OnNewVehicle()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateRechargeActiveState()
{
    throw std::logic_error("Not implemented");
}

void GarageWnd::UpdateAllPriceValues()
{
    throw std::logic_error("Not implemented");
}
