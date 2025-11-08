#include "weaponslotlist.h"
#include "weaponslotwnd.h"

RT_CLASS_EXPORTS_BEGIN(WeaponSlotList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponSlotList);

WeaponSlotList::AuxInfo::AuxInfo()
{
    m_space = -2.0;
}

m3d::Class* WeaponSlotList::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponSlotList);
}

WeaponSlotList::~WeaponSlotList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotList::SetupForVehicle(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponSlotList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* WeaponSlotList::CreateObject()
{
    return new WeaponSlotList;
}

m3d::Object* WeaponSlotList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotList::OnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponSlotList::WeaponSlotList()
{
    m_vehicleId = -1;
}

WeaponSlotList::WeaponSlotList(WeaponSlotList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotList::GetGunPartNames(int, std::vector<CStr, std::allocator<CStr>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotList::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotList::ClearItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotList::AddItem(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotList::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* WeaponSlotList::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotList::SortGunPartNames(std::vector<CStr, std::allocator<CStr>>&, ai::Vehicle const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotList::GameDataSetup()
{
    // TODO: implement WeaponSlotList::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void WeaponSlotList::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotList::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotList::RecalcLayot()
{
    RETRUXX_NOT_IMPLEMENTED;
}
