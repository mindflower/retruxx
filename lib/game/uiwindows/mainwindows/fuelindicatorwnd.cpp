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
    return RT_CLASS_LOCAL(FuelIndicatorWnd);
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

int FuelIndicatorWnd::GameDataUpdate(void*, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 89)
    {
        UpdateProgressBar();
        UpdateLowFuelIco();
    }
    return 1;
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
