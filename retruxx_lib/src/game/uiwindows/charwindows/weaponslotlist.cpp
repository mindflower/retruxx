#include "weaponslotlist.h"
#include "weaponslotwnd.h"

RT_CLASS_EXPORTS_BEGIN(WeaponSlotList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponSlotList);

WeaponSlotList::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponSlotList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

WeaponSlotList::~WeaponSlotList()
{
    throw std::logic_error("Not implemented");
}

int WeaponSlotList::SetupForVehicle(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponSlotList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* WeaponSlotList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponSlotList::Clone()
{
    throw std::logic_error("Not implemented");
}

void WeaponSlotList::OnVehiclePartChanged(void*)
{
    throw std::logic_error("Not implemented");
}

int WeaponSlotList::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

WeaponSlotList::WeaponSlotList()
{
    throw std::logic_error("Not implemented");
}

WeaponSlotList::WeaponSlotList(WeaponSlotList const&)
{
    throw std::logic_error("Not implemented");
}

void WeaponSlotList::GetGunPartNames(int, std::vector<CStr, std::allocator<CStr>>&) const
{
    throw std::logic_error("Not implemented");
}

int WeaponSlotList::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int WeaponSlotList::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void WeaponSlotList::ClearItems()
{
    throw std::logic_error("Not implemented");
}

int WeaponSlotList::AddItem(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void WeaponSlotList::CreateItems()
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle const* WeaponSlotList::GetVehicle() const
{
    throw std::logic_error("Not implemented");
}

void WeaponSlotList::SortGunPartNames(std::vector<CStr, std::allocator<CStr>>&, ai::Vehicle const*) const
{
    throw std::logic_error("Not implemented");
}

int WeaponSlotList::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

void WeaponSlotList::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

int WeaponSlotList::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void WeaponSlotList::RecalcLayot()
{
    throw std::logic_error("Not implemented");
}
