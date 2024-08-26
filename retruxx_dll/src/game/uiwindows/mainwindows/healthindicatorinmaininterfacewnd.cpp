#include "healthindicatorinmaininterfacewnd.h"

RT_CLASS_EXPORTS_BEGIN(HealthIndicatorInMainInterfaceWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HealthIndicatorInMainInterfaceWnd);

m3d::Object* HealthIndicatorInMainInterfaceWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorInMainInterfaceWnd::SetType(Type)
{
    throw std::logic_error("Not implemented");
}

int HealthIndicatorInMainInterfaceWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* HealthIndicatorInMainInterfaceWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* HealthIndicatorInMainInterfaceWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* HealthIndicatorInMainInterfaceWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

HealthIndicatorInMainInterfaceWnd::~HealthIndicatorInMainInterfaceWnd()
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorInMainInterfaceWnd::SetVehicleId(int)
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorInMainInterfaceWnd::UpdateTooltip(float, float)
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle const* HealthIndicatorInMainInterfaceWnd::GetVehicle() const
{
    throw std::logic_error("Not implemented");
}

int HealthIndicatorInMainInterfaceWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorInMainInterfaceWnd::UpdateLowHpLamp(float, float)
{
    throw std::logic_error("Not implemented");
}

int HealthIndicatorInMainInterfaceWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

HealthIndicatorInMainInterfaceWnd::HealthIndicatorInMainInterfaceWnd()
{
    throw std::logic_error("Not implemented");
}

HealthIndicatorInMainInterfaceWnd::HealthIndicatorInMainInterfaceWnd(HealthIndicatorInMainInterfaceWnd const&)
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorInMainInterfaceWnd::GetHp(float&, float&) const
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorInMainInterfaceWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorInMainInterfaceWnd::UpdateProgressBar(float, float)
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorInMainInterfaceWnd::UpdateValueWnd(float)
{
    throw std::logic_error("Not implemented");
}

void HealthIndicatorInMainInterfaceWnd::FullUpdate(bool)
{
    throw std::logic_error("Not implemented");
}
