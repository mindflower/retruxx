#include "weaponslotwnd.h"
#include "weapongroupbuttonlist.h"

RT_CLASS_EXPORTS_BEGIN(WeaponSlotWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponSlotWnd);

m3d::Class* WeaponSlotWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponSlotWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponSlotWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotWnd::SetupForGunPart(CStr const&, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponSlotWnd::~WeaponSlotWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponSlotWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int WeaponSlotWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponSlotWnd::IsSetForGunPart() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotWnd::CreateFromPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponSlotWnd::NeedShowWeaponGroups() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotWnd::UpdateActiveState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotWnd::UpdateGunName(ai::VehiclePart const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotWnd::OnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotWnd::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotWnd::UpdateGunPartIco(ai::VehiclePart const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotWnd::UpdateOnGunChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* WeaponSlotWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotWnd::LoadPattern(m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotWnd::UpdateOnGunPartChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::VehiclePart const* WeaponSlotWnd::GetGun() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotWnd::UpdateGunIco(ai::VehiclePart const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponSlotWnd::WeaponSlotWnd(WeaponSlotWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponSlotWnd::WeaponSlotWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle WeaponSlotWnd::GetIcoByGunPartName(CStr const&, bool) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotWnd::ClearWeaponGroups()
{
    RETRUXX_NOT_IMPLEMENTED;
}
