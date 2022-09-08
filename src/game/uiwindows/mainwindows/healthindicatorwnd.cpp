#include "healthindicatorwnd.h"

RT_CLASS_EXPORTS_BEGIN(HealthIndicatorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HealthIndicatorWnd);

HealthIndicatorWnd::~HealthIndicatorWnd()
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorWnd::SetType(Type)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* HealthIndicatorWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int HealthIndicatorWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* HealthIndicatorWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* HealthIndicatorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ProgressBarWnd);
}

void HealthIndicatorWnd::SetVehicleId(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* HealthIndicatorWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int HealthIndicatorWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorWnd::UpdateBarTexture()
{
    throw std::logic_error("Not implemented");
}

HealthIndicatorWnd::HealthIndicatorWnd()
{
    throw std::logic_error("Not implemented");
}

HealthIndicatorWnd::HealthIndicatorWnd(HealthIndicatorWnd const&)
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle* HealthIndicatorWnd::GetVehicle() const
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorWnd::SetBarTextures(CStr const&, CStr const&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

int HealthIndicatorWnd::UpdateOnNewFrame()
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorWnd::UpdateValue()
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorWnd::UpdateTooltip()
{
    throw std::logic_error("Not implemented");
}
