#include "damageinfownd.h"
#include "fuelindicatorinmaininterfacewnd.h"
#include "durabilityindicatorinmaininterfacewnd.h"
#include "healthindicatorinmaininterfacewnd.h"

RT_CLASS_EXPORTS_BEGIN(DamageInfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DamageInfoWnd);

DamageInfoWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* DamageInfoWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* DamageInfoWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* DamageInfoWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* DamageInfoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

DamageInfoWnd::~DamageInfoWnd()
{
    throw std::logic_error("Not implemented");
}

int DamageInfoWnd::UpdateOnPlayerVehicleChanged()
{
    throw std::logic_error("Not implemented");
}

int DamageInfoWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int DamageInfoWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

DamageInfoWnd::DamageInfoWnd()
{
    throw std::logic_error("Not implemented");
}

DamageInfoWnd::DamageInfoWnd(DamageInfoWnd const&)
{
    throw std::logic_error("Not implemented");
}
