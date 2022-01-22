#include "cabinwnd.h"
#include "gadgetwnd.h"

RT_CLASS_DEFINE(CabinWnd);

CabinWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CabinWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CabinWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CabinWnd::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

CabinWnd::~CabinWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CabinWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

CabinWnd::CabinWnd()
{
    throw std::logic_error("Not implemented");
}

CabinWnd::CabinWnd(CabinWnd const&)
{
    throw std::logic_error("Not implemented");
}

int CabinWnd::CreateGadgetWnd(int)
{
    throw std::logic_error("Not implemented");
}

InventoryWnd::VehicleType CabinWnd::GetVehicleTypeByGuiId(int) const
{
    throw std::logic_error("Not implemented");
}

int CabinWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

ref_ptr<GadgetWnd> CabinWnd::GetGadgetWndByGadgetSlotId(int) const
{
    throw std::logic_error("Not implemented");
}

void CabinWnd::SetupGadgets()
{
    throw std::logic_error("Not implemented");
}

void CabinWnd::SetupChildVehicleParts()
{
    throw std::logic_error("Not implemented");
}

void CabinWnd::UpdateOnMainPartChanged()
{
    throw std::logic_error("Not implemented");
}

int CabinWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}
