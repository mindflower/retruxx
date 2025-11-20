#include "weapongroup.h"
#include <stdexcept>
#include <server/objects/player.h>

#include "m3dapp.h"
#include "core/kernel.h"
#include "game/uiwindows/miscwindows/bindkeyswnd.h"
#include "impulses/i_impulses.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORT_METHOD_DEFINE(WeaponGroupManager, SaveWeaponGroups)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(WeaponGroupManager, RestoreWeaponGroups)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupManager::ClearSavedGroups()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupManager::ValidateWeaponGroups()
{
    // TODO: implement WeaponGroupManager::ValidateWeaponGroups
    RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void WeaponGroupManager::GetAllWeapons(retruxx::set<CStr>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupManager::SaveWeaponGroups()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroupManager::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroup* WeaponGroupManager::GetWeaponGroupById(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupManager::KeepFire()
{
    auto vehicle = ai::thePlayer->GetVehicle();
    if (vehicle)
    {
        if (!vehicle->bIsMovingAlongExternalPath())
        {
            for (auto& [id, group] : this->m_weaponGroups)
            {
                group->KeepFire();
            }
        }
    }
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
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupManager::GetWeaponGroupsMaxCount()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupManager::GetDefaultWeaponGroupIdForWeapon(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroupManager::CreateObject()
{
    return new WeaponGroupManager;
}

m3d::Class* WeaponGroupManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

int WeaponGroupManager::AddWeaponGroup(WeaponGroup* wg)
{
    if (!wg)
    {
        return 0;
    }

    const auto id = wg->GetGroupId();
    if (id > 4 || wg->GetImpulseId() == IM_ERROR)
    {
        return 0;
    }

    auto it = m_weaponGroups.find(id);
    if (it != m_weaponGroups.end())
    {
        return it->second == wg;
    }

    m_weaponGroups.emplace(id, wg);
    return 1;
}

void WeaponGroupManager::ReloadAllWeapon()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupManager::OnPlayerVehicleChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupManager::Init()
{
    ClearGroups();
    for (int i = 0; i < 5; ++i)
    {
        auto* group = M3D_KERNEL->New("WeaponGroup");
        if (group)
        {
            auto* weaponGroup = static_cast<WeaponGroup*>(group);
            weaponGroup->SetGroupId(i);
            if (!AddWeaponGroup(weaponGroup))
            {
                // TODO: check this
                delete group;
            }
        }
    }
    return 1;
}

int WeaponGroupManager::AddWeaponToWeaponGroup(CStr const&, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupManager::AddWeaponToWeaponGroup(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr WeaponGroupManager::GetGunPartNameByGunId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupManager::RestoreWeaponGroups()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupManager::~WeaponGroupManager()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupManager::OnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupManager::ClearGroups()
{
    for (auto& group : m_weaponGroups)
    {
        delete group.second;
    }
    m_weaponGroups.clear();

    M3D_APP->ImmediateMessage(UM_WEAPONGROUP_CHANGED, -1, 0, 0, 0, {}, {});
}

int WeaponGroupManager::RemoveWeaponFromWeaponGroup(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupManager::RemoveWeaponFromWeaponGroup(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponGroupManager::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponGroupManager);
}

int WeaponGroupManager::GetWeaponGroupIdForWeapon(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupManager::Clear()
{
    for (auto& group : m_weaponGroups)
    {
        auto* weaponGroup = group.second;
        if (weaponGroup)
        {
            weaponGroup->ClearWeapons();
        }
    }
    // TODO: clear?

    M3D_APP->ImmediateMessage(65668, -1, 0, 0, 0, {}, {});
}

WeaponGroupManager::WeaponGroupManager(WeaponGroupManager const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupManager::WeaponGroupManager() = default;

void WeaponGroup::Reload()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponGroup::IsEmpty() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroup::ClearWeapons()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponGroup::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroup::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponGroup::CanFire() const
{
    if (m_groupId <= 4)
    {
        if (m_impulseId != IM_ERROR &&
            (M3D_APP->m_pImpulses->GetImpulseState(m_impulseId) || M3D_APP->m_pImpulses->GetImpulseState(IM_CAR_FIRE_ALL)))
        {
            return true;
        }
    }
    return false;
}

m3d::Class* WeaponGroup::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

m3d::Object* WeaponGroup::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

Impulse WeaponGroup::GetImpulseByGroupId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroup::CreateObject()
{
    return new WeaponGroup;
}

int WeaponGroup::AddWeapon(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroup::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroup::GetGroupId() const
{
    return m_groupId;
}

m3d::Class* WeaponGroup::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

Impulse WeaponGroup::GetImpulseId() const
{
    return m_impulseId;
}

void WeaponGroup::KeepFire()
{
    auto* vehicle = ai::thePlayer->GetVehicle();
    if (vehicle)
    {
        const auto canFire = CanFire();
        for (const auto& gunName : m_gunPartNames)
        {
            vehicle->FireFromWeaponByGunPartName(gunName, canFire);
        }
    }
}

int WeaponGroup::RemoveWeapon(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroup::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponGroup::IncludesWeapon(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroup& WeaponGroup::operator=(WeaponGroup const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroup::SetGroupId(int groupId)
{
    m_groupId = groupId;
    switch (groupId)
    {
    case 0u: m_impulseId = IM_CAR_FIRE_0; break;
    case 1u: m_impulseId = IM_CAR_FIRE_1; break;
    case 2u: m_impulseId = IM_CAR_FIRE_2; break;
    case 3u: m_impulseId = IM_CAR_FIRE_3; break;
    case 4u: m_impulseId = IM_CAR_FIRE_4; break;
    default: m_impulseId = IM_ERROR; break;
    }
}

WeaponGroup::~WeaponGroup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>> const& WeaponGroup::GetWeapons() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroup::WeaponGroup()
{
    m_groupId = -1;
    m_impulseId = IM_ERROR;
}

WeaponGroup::WeaponGroup(WeaponGroup const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
