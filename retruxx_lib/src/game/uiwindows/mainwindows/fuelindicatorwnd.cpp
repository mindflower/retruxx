#include "fuelindicatorwnd.h"

RT_CLASS_EXPORTS_BEGIN(FuelIndicatorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FuelIndicatorWnd);

FuelIndicatorWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FuelIndicatorWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* FuelIndicatorWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

FuelIndicatorWnd::~FuelIndicatorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* FuelIndicatorWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* FuelIndicatorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int FuelIndicatorWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* FuelIndicatorWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

FuelIndicatorWnd::FuelIndicatorWnd(FuelIndicatorWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

FuelIndicatorWnd::FuelIndicatorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FuelIndicatorWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int FuelIndicatorWnd::UpdateProgressBar()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle* FuelIndicatorWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FuelIndicatorWnd::UpdateLowFuelIco()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FuelIndicatorWnd::UpdateOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}
