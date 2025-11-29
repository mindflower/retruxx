#include "healthindicatorinmaininterfacewnd.h"

RT_CLASS_EXPORTS_BEGIN(HealthIndicatorInMainInterfaceWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HealthIndicatorInMainInterfaceWnd);

m3d::Object* HealthIndicatorInMainInterfaceWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::SetType(Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HealthIndicatorInMainInterfaceWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* HealthIndicatorInMainInterfaceWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* HealthIndicatorInMainInterfaceWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* HealthIndicatorInMainInterfaceWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

HealthIndicatorInMainInterfaceWnd::~HealthIndicatorInMainInterfaceWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::UpdateTooltip(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* HealthIndicatorInMainInterfaceWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HealthIndicatorInMainInterfaceWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::UpdateLowHpLamp(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HealthIndicatorInMainInterfaceWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

HealthIndicatorInMainInterfaceWnd::HealthIndicatorInMainInterfaceWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

HealthIndicatorInMainInterfaceWnd::HealthIndicatorInMainInterfaceWnd(HealthIndicatorInMainInterfaceWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::GetHp(float&, float&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::UpdateProgressBar(float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::UpdateValueWnd(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorInMainInterfaceWnd::FullUpdate(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}
