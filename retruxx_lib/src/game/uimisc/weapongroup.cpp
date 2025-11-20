#include "weapongroup.h"

#include "guihelper.h"

#include <stdexcept>
#include <server/objects/player.h>

#include "m3dapp.h"
#include "core/kernel.h"
#include "game/uiwindows/miscwindows/bindkeyswnd.h"
#include "impulses/i_impulses.h"
#include "server/objects/vehicle.h"
#include "server/objects/guns/compoundgun.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include <server/resourcemanager.h>

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
    using namespace ai;

    if (!ai::thePlayer)
    {
        return 0;
    }

    auto* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return 0;
    }

    int res = 1;

    retruxx::set<CStr> allWeapons;
    GetAllWeapons(allWeapons);
    for (auto& weapon : allWeapons)
    {
        if (!vehicle->GetPartByName(weapon))
        {
            RemoveWeaponFromWeaponGroup(weapon);
            res = 0;
        }
    }

    std::vector<ai::Obj*> curWeapon;
    help::GetGunsForVehicle(vehicle->GetId(), curWeapon);

    for (auto& gun : curWeapon)
    {
        if (IS_KIND_OF(gun, VehiclePart))
        {
            auto* vehPart = RT_DYNCAST(gun, VehiclePart);
            const auto& partName = vehPart->GetPartName();
            AddWeaponToWeaponGroup(partName, GetWeaponGroupIdForWeapon(partName));
            res = 0;
        }
    }

    return res;
}

void WeaponGroupManager::GetAllWeapons(retruxx::set<CStr>& allWeapons) const
{
    allWeapons.clear();
    for (const auto& group : m_weaponGroups)
    {
        if (group.second)
        {
            allWeapons.insert(group.second->m_gunPartNames.begin(), group.second->m_gunPartNames.end());
        }
    }
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

WeaponGroup* WeaponGroupManager::CreateWeaponGroup(int groupId)
{
    if (groupId > 4)
    {
        return nullptr;
    }

    auto* group = (WeaponGroup*)M3D_KERNEL->New("WeaponGroup");
    if (!group)
    {
        return nullptr;
    }

    group->SetGroupId(groupId);
    if (!AddWeaponGroup(group))
    {
        // TODO: check this
        delete group;
        return nullptr;
    }
    return group;
}

int WeaponGroupManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupManager::GetWeaponGroupsMaxCount()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupManager::GetDefaultWeaponGroupIdForWeapon(const CStr& gunPartName)
{
    if (gunPartName.empty())
    {
        return -1;
    }

    const auto gunResName = ai::theResourceManager->GetResourceNameByVehiclePartName(gunPartName);
    const auto gunResId = ai::theResourceManager->GetResourceId(gunResName);
    if (gunResId == -1)
    {
        return -1;
    }

    if (!ai::theResourceManager->bResourceIsKindOf(gunResId, ai::theResourceManager->GetResourceId("GUN")))
    {
        return -1;
    }

    if (ai::theResourceManager->bResourceIsKindOf(gunResId, ai::theResourceManager->GetResourceId("SMALL_GUN")))
    {
        return 0;
    }

    if (ai::theResourceManager->bResourceIsKindOf(gunResId, ai::theResourceManager->GetResourceId("BIG_GUN")))
    {
        return 1;
    }

    if (ai::theResourceManager->bResourceIsKindOf(gunResId, ai::theResourceManager->GetResourceId("GIANT_GUN")))
    {
        return 2;
    }

    if (ai::theResourceManager->bResourceIsKindOf(gunResId, ai::theResourceManager->GetResourceId("SIDE_GUN")))
    {
        return 3;
    }

    if (ai::theResourceManager->bResourceIsKindOf(gunResId, ai::theResourceManager->GetResourceId("SPECIAL_WEAPON")))
    {
        return 4;
    }

    return -1;
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

int WeaponGroupManager::AddWeaponToWeaponGroup(const CStr& gunPartName, int groupId)
{
    // TOOD: check this
    if (groupId == -1)
    {
        groupId = GetDefaultWeaponGroupIdForWeapon(gunPartName);
        if (groupId == -1)
        {
            return 0;
        }
    }

    auto weaponGroup = GetWeaponGroupIdForWeapon(gunPartName);
    if (weaponGroup != -1)
    {
        if (weaponGroup == groupId)
        {
            return 1;
        }
        RemoveWeaponFromWeaponGroup(gunPartName);
    }

    auto it = m_weaponGroups.find(groupId);
    if (it != m_weaponGroups.end())
    {
        return it->second->AddWeapon(gunPartName);
    }
    else
    {
        auto* group = CreateWeaponGroup(groupId);
        if (group)
        {
            return group->AddWeapon(gunPartName);
        }
        return 0;
    }

    return 1;
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

void WeaponGroupManager::OnVehiclePartChanged(void* data)
{
    const auto event = static_cast<m3d::Event*>(data);
    if (ai::thePlayer)
    {
        auto* vehicle = ai::thePlayer->GetVehicle();
        if (vehicle && event->m_uintEv[0] == vehicle->GetId())
        {
            CStr& partName = event->m_strEv;
            if (!partName.empty())
            {
                if (vehicle->GetPartByName(partName))
                {
                    const auto weaponGroup = GetWeaponGroupIdForWeapon(partName);
                    AddWeaponToWeaponGroup(partName, weaponGroup);
                }
                else
                {
                    RemoveWeaponFromWeaponGroup(partName);
                }
            }
        }
    }
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

int WeaponGroupManager::GetWeaponGroupIdForWeapon(const CStr& gunPartName) const
{
    if (gunPartName.empty())
    {
        return -1;
    }

    for (auto& group : m_weaponGroups)
    {
        auto* weaponGroup = group.second;
        if (weaponGroup)
        {
            const auto& weapons = weaponGroup->GetWeapons();
            const auto it = weapons.find(gunPartName);
            if (it != weapons.end())
            {
                return weaponGroup->GetGroupId();
            }
        }
    }
    return -1;
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
    m_gunPartNames.clear();
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

int WeaponGroup::AddWeapon(const CStr& gunPartName)
{
    using namespace ai;

    if (gunPartName.empty())
    {
        return 0;
    }

    auto* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return 0;
    }

    auto* part = vehicle->GetPartByName(gunPartName);
    if (!part || (!IS_KIND_OF(part, Gun) && !IS_KIND_OF(part, CompoundGun)))
    {
        return 0;
    }

    auto it = m_gunPartNames.find(gunPartName);
    if (it == m_gunPartNames.end())
    {
        m_gunPartNames.insert(gunPartName);
        M3D_APP->EnqueueMessage(UM_WEAPONGROUP_CHANGED, m_groupId, 0, 0, 0, {}, {});
    }
    return 1;
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

WeaponGroup::~WeaponGroup() = default;

retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>> const& WeaponGroup::GetWeapons() const
{
    return m_gunPartNames;
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
