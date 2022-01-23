#include "weapongroup.h"
#include <stdexcept>

RT_CLASS_DEFINE(WeaponGroupManager);

int WeaponGroupManager::DeleteWeaponGroup(int)
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupManager::ClearSavedGroups()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::ValidateWeaponGroups()
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupManager::GetAllWeapons(std::set<CStr>&) const
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupManager::SaveWeaponGroups()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponGroupManager::Clone()
{
    throw std::logic_error("Not implemented");
}

WeaponGroup* WeaponGroupManager::GetWeaponGroupById(int) const
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupManager::KeepFire()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

WeaponGroup* WeaponGroupManager::CreateWeaponGroup(int)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::GetWeaponGroupsMaxCount()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::GetDefaultWeaponGroupIdForWeapon(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponGroupManager::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponGroupManager::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::AddWeaponGroup(WeaponGroup*)
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupManager::ReloadAllWeapon()
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupManager::OnPlayerVehicleChanged()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::Init()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::AddWeaponToWeaponGroup(CStr const&, int)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::AddWeaponToWeaponGroup(int, int)
{
    throw std::logic_error("Not implemented");
}

CStr WeaponGroupManager::GetGunPartNameByGunId(int)
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupManager::RestoreWeaponGroups()
{
    throw std::logic_error("Not implemented");
}

WeaponGroupManager::~WeaponGroupManager()
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupManager::OnVehiclePartChanged(void*)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupManager::ClearGroups()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::RemoveWeaponFromWeaponGroup(int)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::RemoveWeaponFromWeaponGroup(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponGroupManager::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int WeaponGroupManager::GetWeaponGroupIdForWeapon(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

void WeaponGroupManager::Clear()
{
    throw std::logic_error("Not implemented");
}

WeaponGroupManager::WeaponGroupManager(WeaponGroupManager const&)
{
    throw std::logic_error("Not implemented");
}

WeaponGroupManager::WeaponGroupManager()
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_DEFINE(WeaponGroup);

void WeaponGroup::Reload()
{
    throw std::logic_error("Not implemented");
}

bool WeaponGroup::IsEmpty() const
{
    throw std::logic_error("Not implemented");
}

void WeaponGroup::ClearWeapons()
{
    throw std::logic_error("Not implemented");
}

bool WeaponGroup::IsValid() const
{
    throw std::logic_error("Not implemented");
}

int WeaponGroup::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

bool WeaponGroup::CanFire() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponGroup::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponGroup::Clone()
{
    throw std::logic_error("Not implemented");
}

Impulse WeaponGroup::GetImpulseByGroupId(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* WeaponGroup::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroup::AddWeapon(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void WeaponGroup::Clear()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroup::GetGroupId() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* WeaponGroup::GetClass() const
{
    throw std::logic_error("Not implemented");
}

Impulse WeaponGroup::GetImpulseId() const
{
    throw std::logic_error("Not implemented");
}

void WeaponGroup::KeepFire()
{
    throw std::logic_error("Not implemented");
}

int WeaponGroup::RemoveWeapon(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int WeaponGroup::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

bool WeaponGroup::IncludesWeapon(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

WeaponGroup& WeaponGroup::operator=(WeaponGroup const&)
{
    throw std::logic_error("Not implemented");
}

void WeaponGroup::SetGroupId(int)
{
    throw std::logic_error("Not implemented");
}

WeaponGroup::~WeaponGroup()
{
    throw std::logic_error("Not implemented");
}

std::set<CStr, std::less<CStr>, std::allocator<CStr>> const& WeaponGroup::GetWeapons() const
{
    throw std::logic_error("Not implemented");
}

WeaponGroup::WeaponGroup()
{
    throw std::logic_error("Not implemented");
}

WeaponGroup::WeaponGroup(WeaponGroup const&)
{
    throw std::logic_error("Not implemented");
}
