#include "weapongroup.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(WeaponGroupManager, SaveWeaponGroups)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(WeaponGroupManager, RestoreWeaponGroups)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(WeaponGroupManager)
    RT_CLASS_EXPORT(WeaponGroupManager, m3d::METHOD, SaveWeaponGroups, "", "", "")
    RT_CLASS_EXPORT(WeaponGroupManager, m3d::METHOD, RestoreWeaponGroups, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponGroupManager);

RT_CLASS_EXPORTS_BEGIN(WeaponGroup)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponGroup);

int WeaponGroupManager::DeleteWeaponGroup(int)
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroupManager::ClearSavedGroups()
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroupManager::ValidateWeaponGroups()
{
    // TODO: implement WeaponGroupManager::ValidateWeaponGroups
    //throw retruxx::logic_error("Not implemented");
    return 1;
}

void WeaponGroupManager::GetAllWeapons(retruxx::set<CStr>&) const
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroupManager::SaveWeaponGroups()
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Object* WeaponGroupManager::Clone()
{
    throw retruxx::logic_error("Not implemented");
}

WeaponGroup* WeaponGroupManager::GetWeaponGroupById(int) const
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroupManager::KeepFire()
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroupManager::GameDataUpdate(void* data, int dataType)
{
    auto v3 = dataType - 11;
    if (v3)
    {
        auto v4 = v3 - 53;
        if (!v4)
        {
            WeaponGroupManager::ValidateWeaponGroups();
            return 1;
        }
        if (v4 == 1)
        {
            WeaponGroupManager::OnVehiclePartChanged(data);
            return 1;
        }
    }
    else
    {
        WeaponGroupManager::ReloadAllWeapon();
    }
    return 1;
}

WeaponGroup* WeaponGroupManager::CreateWeaponGroup(int)
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroupManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroupManager::GetWeaponGroupsMaxCount()
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroupManager::GetDefaultWeaponGroupIdForWeapon(CStr const&)
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Object* WeaponGroupManager::CreateObject()
{
    return new WeaponGroupManager;
}

m3d::Class* WeaponGroupManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

int WeaponGroupManager::AddWeaponGroup(WeaponGroup*)
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroupManager::ReloadAllWeapon()
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroupManager::OnPlayerVehicleChanged()
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroupManager::Init()
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroupManager::AddWeaponToWeaponGroup(CStr const&, int)
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroupManager::AddWeaponToWeaponGroup(int, int)
{
    throw retruxx::logic_error("Not implemented");
}

CStr WeaponGroupManager::GetGunPartNameByGunId(int)
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroupManager::RestoreWeaponGroups()
{
    throw retruxx::logic_error("Not implemented");
}

WeaponGroupManager::~WeaponGroupManager()
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroupManager::OnVehiclePartChanged(void*)
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroupManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroupManager::ClearGroups()
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroupManager::RemoveWeaponFromWeaponGroup(int)
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroupManager::RemoveWeaponFromWeaponGroup(CStr const&)
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Class* WeaponGroupManager::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponGroupManager);
}

int WeaponGroupManager::GetWeaponGroupIdForWeapon(CStr const&) const
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroupManager::Clear()
{
    throw retruxx::logic_error("Not implemented");
}

WeaponGroupManager::WeaponGroupManager(WeaponGroupManager const&)
{
    throw retruxx::logic_error("Not implemented");
}

WeaponGroupManager::WeaponGroupManager()
{
}

void WeaponGroup::Reload()
{
    throw retruxx::logic_error("Not implemented");
}

bool WeaponGroup::IsEmpty() const
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroup::ClearWeapons()
{
    throw retruxx::logic_error("Not implemented");
}

bool WeaponGroup::IsValid() const
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroup::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw retruxx::logic_error("Not implemented");
}

bool WeaponGroup::CanFire() const
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Class* WeaponGroup::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

m3d::Object* WeaponGroup::Clone()
{
    throw retruxx::logic_error("Not implemented");
}

Impulse WeaponGroup::GetImpulseByGroupId(int)
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Object* WeaponGroup::CreateObject()
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroup::AddWeapon(CStr const&)
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroup::Clear()
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroup::GetGroupId() const
{
    throw retruxx::logic_error("Not implemented");
}

m3d::Class* WeaponGroup::GetClass() const
{
    throw retruxx::logic_error("Not implemented");
}

Impulse WeaponGroup::GetImpulseId() const
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroup::KeepFire()
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroup::RemoveWeapon(CStr const&)
{
    throw retruxx::logic_error("Not implemented");
}

int WeaponGroup::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw retruxx::logic_error("Not implemented");
}

bool WeaponGroup::IncludesWeapon(CStr const&) const
{
    throw retruxx::logic_error("Not implemented");
}

WeaponGroup& WeaponGroup::operator=(WeaponGroup const&)
{
    throw retruxx::logic_error("Not implemented");
}

void WeaponGroup::SetGroupId(int)
{
    throw retruxx::logic_error("Not implemented");
}

WeaponGroup::~WeaponGroup()
{
    throw retruxx::logic_error("Not implemented");
}

retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>> const& WeaponGroup::GetWeapons() const
{
    throw retruxx::logic_error("Not implemented");
}

WeaponGroup::WeaponGroup()
{
    throw retruxx::logic_error("Not implemented");
}

WeaponGroup::WeaponGroup(WeaponGroup const&)
{
    throw retruxx::logic_error("Not implemented");
}
