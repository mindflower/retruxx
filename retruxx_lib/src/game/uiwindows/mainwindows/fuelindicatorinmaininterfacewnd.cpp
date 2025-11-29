#include "fuelindicatorinmaininterfacewnd.h"

RT_CLASS_EXPORTS_BEGIN(FuelIndicatorInMainInterfaceWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FuelIndicatorInMainInterfaceWnd);

void FuelIndicatorInMainInterfaceWnd::SetType(Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* FuelIndicatorInMainInterfaceWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* FuelIndicatorInMainInterfaceWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* FuelIndicatorInMainInterfaceWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* FuelIndicatorInMainInterfaceWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorInMainInterfaceWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

FuelIndicatorInMainInterfaceWnd::~FuelIndicatorInMainInterfaceWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FuelIndicatorInMainInterfaceWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorInMainInterfaceWnd::UpdateProgressBar(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorInMainInterfaceWnd::GetFuel(float&, float&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorInMainInterfaceWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorInMainInterfaceWnd::FullUpdate(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorInMainInterfaceWnd::UpdateValueWnd(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* FuelIndicatorInMainInterfaceWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorInMainInterfaceWnd::UpdateTooltip(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

FuelIndicatorInMainInterfaceWnd::FuelIndicatorInMainInterfaceWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

FuelIndicatorInMainInterfaceWnd::FuelIndicatorInMainInterfaceWnd(FuelIndicatorInMainInterfaceWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FuelIndicatorInMainInterfaceWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FuelIndicatorInMainInterfaceWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

void FuelIndicatorInMainInterfaceWnd::UpdateLowFuelLamp(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}
