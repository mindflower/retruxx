#include "damageinfownd.h"
#include "fuelindicatorinmaininterfacewnd.h"
#include "durabilityindicatorinmaininterfacewnd.h"
#include "healthindicatorinmaininterfacewnd.h"

RT_CLASS_EXPORTS_BEGIN(DamageInfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DamageInfoWnd);

DamageInfoWnd::AuxInfo::AuxInfo()
{
    m_wndHealthName = "wndHealth";
    m_wndCabinDurabilityName = "wndCabinDurability";
    m_wndBasketDurabilityName = "wndBasketDurability";
    m_wndFuelName = "wndFuel";
}

m3d::Object* DamageInfoWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* DamageInfoWnd::GetClass() const
{
    return RT_CLASS_LOCAL(DamageInfoWnd);
}

m3d::Object* DamageInfoWnd::CreateObject()
{
    return new DamageInfoWnd;
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
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int DamageInfoWnd::GameDataSetup()
{
    // TODO: implement DamageInfoWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

DamageInfoWnd::DamageInfoWnd() = default;

DamageInfoWnd::DamageInfoWnd(DamageInfoWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
