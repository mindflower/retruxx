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
    return RT_CLASS_LOCAL(ChildPanel);
}

int GarageWnd::SetupForWorkshop(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool GarageWnd::IsListOpen() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Workshop* GarageWnd::GetWorkshop() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

help::Color GarageWnd::GetValueColor(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* GarageWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::GetWorkshopId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

GarageWnd::~GarageWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* GarageWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* GarageWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnRepairList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateAllPricePrevValues()
{
    RETRUXX_NOT_IMPLEMENTED;
}

unsigned GarageWnd::EnumColor2Color(help::Color) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::GetUnitsToRecharge() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnCabins()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::Refuel(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::MaxRefuel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnRepairAll()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateRefuelAllTooltip()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnPlayerVehicleChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateRefuelAllPriceControls(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnRepositoryChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateNewVehicleActiveState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateRefuelActiveState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnBaskets()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::MaxRecharge()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnSkin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OpenList(ref_ptr<m3d::ui::Wnd>, m3d::ui::ButtonWnd*, m3d::ui::ButtonWnd*, ai::eGameEvent)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnRechargeList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnFinishTrade(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateServiceActiveState(std::vector<m3d::ui::Wnd*, std::allocator<m3d::ui::Wnd*>> const&, bool, ref_ptr<m3d::ui::Wnd>)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepository* GarageWnd::GetWorkshopRepositoryByType(ai::WorkshopRepositoryType) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateRechargeAllTooltip()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::GetPossibleRepair(int&, int&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnRechargeAll()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::GetUnitsToRepair() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::Recharge(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

float GarageWnd::GetHealthPriceForOneUnit() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::MaxRepair()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateSkinActiveState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnRefuelList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

GarageWnd::GarageWnd(GarageWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

GarageWnd::GarageWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::GetShellPrice(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::Repair(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::GetPossibleRecharge(int&, int&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateActiveStates()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateRepairActiveState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateRepairAllPriceControls(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateRepairAllTooltip()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnRefuelAll()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateAllPriceControls(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnBuySkin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::GetPossibleRefuel(int&, int&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::GetTownId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::CloseCurrentList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::GetUnitsToRefuel() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateBasketsActiveState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::GetFuelPriceForOneUnit() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateServiceSelectState(std::vector<m3d::ui::Wnd*, std::allocator<m3d::ui::Wnd*>> const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateRechargeAllPriceControls(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GarageWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateCabinsActiveState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::OnNewVehicle()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateRechargeActiveState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GarageWnd::UpdateAllPriceValues()
{
    RETRUXX_NOT_IMPLEMENTED;
}
