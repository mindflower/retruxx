#include "healthindicatorwnd.h"

RT_CLASS_EXPORTS_BEGIN(HealthIndicatorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HealthIndicatorWnd);

HealthIndicatorWnd::~HealthIndicatorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorWnd::SetType(Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* HealthIndicatorWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HealthIndicatorWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* HealthIndicatorWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* HealthIndicatorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ProgressBarWnd);
}

void HealthIndicatorWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* HealthIndicatorWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HealthIndicatorWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorWnd::UpdateBarTexture()
{
    RETRUXX_NOT_IMPLEMENTED;
}

HealthIndicatorWnd::HealthIndicatorWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

HealthIndicatorWnd::HealthIndicatorWnd(HealthIndicatorWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle* HealthIndicatorWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorWnd::SetBarTextures(CStr const&, CStr const&, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int HealthIndicatorWnd::UpdateOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorWnd::UpdateValue()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void HealthIndicatorWnd::UpdateTooltip()
{
    RETRUXX_NOT_IMPLEMENTED;
}
