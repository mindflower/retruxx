#include "damageinfownd.h"
#include "fuelindicatorinmaininterfacewnd.h"
#include "durabilityindicatorinmaininterfacewnd.h"
#include "healthindicatorinmaininterfacewnd.h"

RT_CLASS_EXPORTS_BEGIN(DamageInfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DamageInfoWnd);

DamageInfoWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* DamageInfoWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* DamageInfoWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* DamageInfoWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* DamageInfoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

DamageInfoWnd::~DamageInfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DamageInfoWnd::UpdateOnPlayerVehicleChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DamageInfoWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int DamageInfoWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

DamageInfoWnd::DamageInfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

DamageInfoWnd::DamageInfoWnd(DamageInfoWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
