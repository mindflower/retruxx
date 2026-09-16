#include "weapongroup.h"

#include "guihelper.h"

#include <stdexcept>
#include <server/objects/player.h>

#include "m3dapp.h"
#include "core/kernel.h"
#include "core/ini.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include "game/uiwindows/miscwindows/bindkeyswnd.h"
#include "impulses/i_impulses.h"
#include "server/objects/vehicle.h"
#include "server/objects/guns/compoundgun.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "server/objects/base/objcontainer.h"
#include "server/utils.h"
#include <server/resourcemanager.h>

RT_CLASS_EXPORT_METHOD_DEFINE(WeaponGroupManager, SaveWeaponGroups)
{
    // RVA 0x586060
    auto* weaponGroupManager = (WeaponGroupManager*)context->asObject(0, "WeaponGroupManager");
    weaponGroupManager->SaveWeaponGroups();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(WeaponGroupManager, RestoreWeaponGroups)
{
    // RVA 0x586080
    auto* weaponGroupManager = (WeaponGroupManager*)context->asObject(0, "WeaponGroupManager");
    weaponGroupManager->RestoreWeaponGroups();
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(WeaponGroupManager)
    RT_CLASS_EXPORT(WeaponGroupManager, m3d::METHOD, SaveWeaponGroups, "", "", "")
    RT_CLASS_EXPORT(WeaponGroupManager, m3d::METHOD, RestoreWeaponGroups, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponGroupManager);

RT_CLASS_EXPORTS_BEGIN(WeaponGroup)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponGroup);

int WeaponGroupManager::DeleteWeaponGroup(int groupId)
{
    // RVA 0x586680
    auto const it = m_weaponGroups.find(groupId);
    if (it == m_weaponGroups.end())
    {
        return 0;
    }

    delete it->second;
    it->second = nullptr;
    m_weaponGroups.erase(it);
    return 1;
}

void WeaponGroupManager::ClearSavedGroups()
{
    // RVA 0x586490
    for (auto& group : m_savedWeaponGroups)
    {
        delete group.second;
        group.second = nullptr;
    }
    m_savedWeaponGroups.clear();
}

int WeaponGroupManager::ValidateWeaponGroups()
{
    // RVA 0x587430
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

    for (int i = 0; i < static_cast<int>(curWeapon.size()); ++i)
    {
        auto* gun = curWeapon[i];
        if (gun && IS_KIND_OF(gun, VehiclePart))
        {
            const auto& partName = static_cast<VehiclePart*>(gun)->GetPartName();
            if (GetWeaponGroupIdForWeapon(partName) == -1)
            {
                // Guns not assigned to any group go to their default group.
                AddWeaponToWeaponGroup(partName, -1);
                res = 0;
            }
        }
    }

    return res;
}

void WeaponGroupManager::GetAllWeapons(retruxx::set<CStr>& allWeapons) const
{
    // RVA 0x587690
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
    // RVA 0x587730
    ClearSavedGroups();
    for (const auto& [groupId, group] : m_weaponGroups)
    {
        if (group && m_savedWeaponGroups.find(groupId) == m_savedWeaponGroups.end())
        {
            auto* savedGroup = static_cast<WeaponGroup*>(M3D_KERNEL->New("WeaponGroup"));
            if (savedGroup)
            {
                *savedGroup = *group;
                m_savedWeaponGroups.insert({groupId, savedGroup});
            }
        }
    }

    M3D_LOG_INFO("Weapon groups were successfully saved");
}

m3d::Object* WeaponGroupManager::Clone()
{
    // RVA 0x5854D0
    return new WeaponGroupManager(*this);
}

WeaponGroup* WeaponGroupManager::GetWeaponGroupById(int groupId) const
{
    // RVA 0x586570
    if (groupId == -1)
    {
        return nullptr;
    }

    auto const it = m_weaponGroups.find(groupId);
    if (it == m_weaponGroups.end())
    {
        return nullptr;
    }
    return it->second;
}

void WeaponGroupManager::KeepFire()
{
    // RVA 0x5866E0
    auto vehicle = ai::thePlayer->GetVehicle();
    if (vehicle)
    {
        if (!vehicle->bIsMovingAlongExternalPath())
        {
            for (auto& [id, group] : this->m_weaponGroups)
            {
                if (group)
                {
                    group->KeepFire();
                }
            }
        }
    }
}

int WeaponGroupManager::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x5873F0
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
    // RVA 0x5865A0
    if (static_cast<unsigned int>(groupId) > 4)
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
        delete group;
        return nullptr;
    }
    return group;
}

int WeaponGroupManager::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    // RVA 0x586730
    ClearGroups();
    ClearSavedGroups();

    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("WeaponGroupManager::LoadFromXml error - null xmlFile or xmlNode");
        return 0;
    }

    int res = 1;

    ref_ptr curGroupsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    xmlNode->GetFirstChild(curGroupsNode, "CurrentWeaponGroups");
    if (!curGroupsNode->IsEmpty())
    {
        ref_ptr wgNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        for (curGroupsNode->GetFirstChild(wgNode, "WeaponGroup"); !wgNode->IsEmpty(); wgNode->GetNextSibling(wgNode, "WeaponGroup"))
        {
            auto* group = static_cast<WeaponGroup*>(M3D_KERNEL->New("WeaponGroup"));
            if (!group)
            {
                continue;
            }

            if (!group->LoadFromXml(xmlFile, wgNode) || !AddWeaponGroup(group))
            {
                delete group;
                res = 0;
            }
        }
    }

    ref_ptr savedGroupsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    xmlNode->GetFirstChild(savedGroupsNode, "SavedWeaponGroups");
    if (!savedGroupsNode->IsEmpty())
    {
        ref_ptr wgNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        for (savedGroupsNode->GetFirstChild(wgNode, "WeaponGroup"); !wgNode->IsEmpty(); wgNode->GetNextSibling(wgNode, "WeaponGroup"))
        {
            auto* group = static_cast<WeaponGroup*>(M3D_KERNEL->New("WeaponGroup"));
            if (!group)
            {
                continue;
            }

            // NOTE: saved groups are not validated with AddWeaponGroup, only checked for a duplicate id.
            if (!group->LoadFromXml(xmlFile, wgNode) || m_savedWeaponGroups.find(group->m_groupId) != m_savedWeaponGroups.end())
            {
                delete group;
                res = 0;
                continue;
            }

            m_savedWeaponGroups.insert({group->m_groupId, group});
        }
    }

    if (!res)
    {
        M3D_LOG_INFO("WeaponGroupManager was loaded with errors");
    }
    return res;
}

int WeaponGroupManager::GetWeaponGroupsMaxCount()
{
    // RVA 0x586560
    return 5;
}

int WeaponGroupManager::GetDefaultWeaponGroupIdForWeapon(const CStr& gunPartName)
{
    // RVA 0x586F30
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

    // NOTE: a gun of no known subtype falls into the BIG_GUN group rather than none.
    return 1;
}

m3d::Object* WeaponGroupManager::CreateObject()
{
    // RVA 0x5860C0
    return new WeaponGroupManager;
}

m3d::Class* WeaponGroupManager::GetBaseClass()
{
    // RVA 0x5860A0
    return RT_CLASS_LOCAL(Object);
}

int WeaponGroupManager::AddWeaponGroup(WeaponGroup* wg)
{
    // RVA 0x5865F0
    if (!wg)
    {
        return 0;
    }

    const auto id = wg->GetGroupId();
    if (id < 0 || id >= 5 || wg->GetImpulseId() == IM_ERROR)
    {
        return 0;
    }

    auto it = m_weaponGroups.find(id);
    if (it != m_weaponGroups.end() && it->second)
    {
        return it->second == wg;
    }

    // NOTE: if the id is present with a null group, the insert is a no-op but success is still reported.
    m_weaponGroups.insert({id, wg});
    return 1;
}

void WeaponGroupManager::ReloadAllWeapon()
{
    // RVA 0x5876F0
    for (auto& [id, group] : m_weaponGroups)
    {
        if (group)
        {
            group->Reload();
        }
    }
}

void WeaponGroupManager::OnPlayerVehicleChanged()
{
    // RVA 0x587610
    ValidateWeaponGroups();
}

int WeaponGroupManager::Init()
{
    // RVA 0x586500
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
                delete group;
            }
        }
    }
    return 1;
}

int WeaponGroupManager::AddWeaponToWeaponGroup(const CStr& gunPartName, int groupId)
{
    // RVA 0x586E50
    int targetGroupId = groupId;
    if (groupId == -1)
    {
        targetGroupId = GetDefaultWeaponGroupIdForWeapon(gunPartName);
        if (targetGroupId == -1)
        {
            return 0;
        }
    }

    auto const curGroupId = GetWeaponGroupIdForWeapon(gunPartName);
    if (curGroupId != -1)
    {
        // NOTE: compared against the requested id, not the resolved default one, so a weapon
        // added with -1 is always removed and re-added even when it is already in its default group.
        if (curGroupId == groupId)
        {
            return 1;
        }
        RemoveWeaponFromWeaponGroup(gunPartName);
    }

    WeaponGroup* group = nullptr;
    auto const it = m_weaponGroups.find(targetGroupId);
    if (it != m_weaponGroups.end())
    {
        group = it->second;
    }

    if (!group)
    {
        group = CreateWeaponGroup(targetGroupId);
        if (!group)
        {
            return 0;
        }
    }

    return group->AddWeapon(gunPartName);
}

int WeaponGroupManager::AddWeaponToWeaponGroup(int gunId, int groupId)
{
    // RVA 0x586EF0
    CStr const gunPartName = GetGunPartNameByGunId(gunId);
    return AddWeaponToWeaponGroup(gunPartName, groupId);
}

CStr WeaponGroupManager::GetGunPartNameByGunId(int gunId)
{
    // RVA 0x5872E0
    using namespace ai;

    if (gunId == -1)
    {
        return CStr();
    }

    auto* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return CStr();
    }

    auto* gun = ai::theObjects->GetEntityByObjId(gunId);
    if (!gun || (!IS_KIND_OF(gun, Gun) && !IS_KIND_OF(gun, CompoundGun)))
    {
        return CStr();
    }

    CStr partName = static_cast<VehiclePart*>(gun)->GetPartName();
    if (!vehicle->GetPartByName(partName))
    {
        return CStr();
    }
    return partName;
}

void WeaponGroupManager::RestoreWeaponGroups()
{
    // RVA 0x587830
    if (!m_savedWeaponGroups.size())
    {
        M3D_LOG_INFO("WeaponGroupManager::RestoreWeaponGroups(): warning - weapon groups have not been saved");
        return;
    }

    ClearGroups();
    for (const auto& [groupId, savedGroup] : m_savedWeaponGroups)
    {
        if (savedGroup)
        {
            auto* group = static_cast<WeaponGroup*>(M3D_KERNEL->New("WeaponGroup"));
            if (group)
            {
                *group = *savedGroup;
                if (!AddWeaponGroup(group))
                {
                    delete group;
                }
            }
        }
    }

    if (!ValidateWeaponGroups())
    {
        M3D_LOG_INFO("WeaponGroupManager::RestoreWeaponGroups(): warning - saved weapon groups not match current vehicle. Some corrections have been done");
    }
    else
    {
        M3D_LOG_INFO("Weapon groups were successfully restored");
    }
}

WeaponGroupManager::~WeaponGroupManager()
{
    // RVA 0x586220
    ClearGroups();
    ClearSavedGroups();
}

void WeaponGroupManager::OnVehiclePartChanged(void* data)
{
    // RVA 0x587620
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

int WeaponGroupManager::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
{
    // RVA 0x586B20
    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("WeaponGroupManager::SaveToXml error - null xnlFile or xmlNode");
        return 0;
    }

    int res = 1;

    ref_ptr curGroupsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "CurrentWeaponGroups");
    xmlNode->AddChild(curGroupsNode);
    for (const auto& [groupId, group] : m_weaponGroups)
    {
        if (!group)
        {
            res = 0;
            continue;
        }

        ref_ptr wgNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "WeaponGroup");
        curGroupsNode->AddChild(wgNode);
        res &= group->SaveToXml(xmlFile, wgNode);
    }

    if (m_savedWeaponGroups.size())
    {
        ref_ptr savedGroupsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "SavedWeaponGroups");
        xmlNode->AddChild(savedGroupsNode);
        for (const auto& [groupId, group] : m_savedWeaponGroups)
        {
            if (!group)
            {
                res = 0;
                continue;
            }

            ref_ptr wgNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "WeaponGroup");
            savedGroupsNode->AddChild(wgNode);
            res &= group->SaveToXml(xmlFile, wgNode);
        }
    }

    if (!res)
    {
        M3D_LOG_INFO("WeaponGroupManager saved with errors");
    }
    return res;
}

void WeaponGroupManager::ClearGroups()
{
    // RVA 0x5863A0
    for (auto& group : m_weaponGroups)
    {
        delete group.second;
        group.second = nullptr;
    }
    m_weaponGroups.clear();

    M3D_APP->ImmediateMessage(UM_WEAPONGROUP_CHANGED, -1, 0, 0, 0, {}, {});
}

int WeaponGroupManager::RemoveWeaponFromWeaponGroup(int gunId)
{
    // RVA 0x5872A0
    CStr const partName = GetGunPartNameByGunId(gunId);
    return RemoveWeaponFromWeaponGroup(partName);
}

int WeaponGroupManager::RemoveWeaponFromWeaponGroup(CStr const& gunPartName)
{
    // RVA 0x587240
    auto const id = GetWeaponGroupIdForWeapon(gunPartName);
    if (id == -1)
    {
        return 0;
    }

    auto const it = m_weaponGroups.find(id);
    if (it != m_weaponGroups.end() && it->second)
    {
        return it->second->RemoveWeapon(gunPartName);
    }
    return 0;
}

m3d::Class* WeaponGroupManager::GetClass() const
{
    // RVA 0x5860B0
    return RT_CLASS_LOCAL(WeaponGroupManager);
}

int WeaponGroupManager::GetWeaponGroupIdForWeapon(const CStr& gunPartName) const
{
    // RVA 0x5871B0
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
    // RVA 0x5862C0
    for (auto& group : m_weaponGroups)
    {
        auto* weaponGroup = group.second;
        if (weaponGroup)
        {
            weaponGroup->ClearWeapons();
        }
    }

    M3D_APP->ImmediateMessage(UM_WEAPONGROUP_CHANGED, -1, 0, 0, 0, {}, {});
}

WeaponGroupManager::WeaponGroupManager(WeaponGroupManager const&)
{
    // RVA 0x5861C0
    // NOTE: the copy constructor copies nothing; the copy starts with no groups.
}

WeaponGroupManager::WeaponGroupManager() = default;

void WeaponGroup::Reload()
{
    // RVA 0x585F60
    using namespace ai;

    auto* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return;
    }

    for (const auto& gunName : m_gunPartNames)
    {
        auto* part = vehicle->GetPartByName(gunName);
        if (part)
        {
            if (IS_KIND_OF(part, Gun))
            {
                static_cast<Gun*>(part)->Recharge();
            }
            else if (IS_KIND_OF(part, CompoundGun))
            {
                static_cast<CompoundGun*>(part)->Recharge();
            }
        }
    }
}

bool WeaponGroup::IsEmpty() const
{
    // RVA 0x585710
    return m_gunPartNames.size() == 0;
}

void WeaponGroup::ClearWeapons()
{
    // RVA 0x585F30
    m_gunPartNames.clear();
}

bool WeaponGroup::IsValid() const
{
    // RVA 0x5856F0
    return static_cast<unsigned int>(m_groupId) <= 4 && m_impulseId != IM_ERROR;
}

int WeaponGroup::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
{
    // RVA 0x585DE0
    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("WeaponGroup::SaveToXml error - null xmlFile or xmlNode");
        return 0;
    }

    xmlNode->SetAttribute("groupId", CStr(m_groupId).c_str());

    retruxx::vector<CStr> lstWeapons;
    lstWeapons.assign(m_gunPartNames.begin(), m_gunPartNames.end());
    CStr const strWeapons = ai::StringVectorToStr(lstWeapons);
    xmlNode->SetAttribute("weaponParts", strWeapons.c_str());
    return 1;
}

bool WeaponGroup::CanFire() const
{
    // RVA 0x585A00
    if (static_cast<unsigned int>(m_groupId) <= 4)
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
    // RVA 0x585500
    return RT_CLASS_LOCAL(Object);
}

m3d::Object* WeaponGroup::Clone()
{
    // RVA 0x585470
    return new WeaponGroup(*this);
}

Impulse WeaponGroup::GetImpulseByGroupId(int groupId)
{
    // RVA 0x585790
    switch (groupId)
    {
    case 0: return IM_CAR_FIRE_0;
    case 1: return IM_CAR_FIRE_1;
    case 2: return IM_CAR_FIRE_2;
    case 3: return IM_CAR_FIRE_3;
    case 4: return IM_CAR_FIRE_4;
    default: return IM_ERROR;
    }
}

m3d::Object* WeaponGroup::CreateObject()
{
    // RVA 0x585520
    return new WeaponGroup;
}

int WeaponGroup::AddWeapon(const CStr& gunPartName)
{
    // RVA 0x585800
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
    if (!part || (!IS_KIND_OF(part, CompoundGun) && !IS_KIND_OF(part, Gun)))
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
    // RVA 0x5856B0
    m_groupId = -1;
    m_impulseId = IM_ERROR;
    m_gunPartNames.clear();
}

int WeaponGroup::GetGroupId() const
{
    // RVA 0x585720
    return m_groupId;
}

m3d::Class* WeaponGroup::GetClass() const
{
    // RVA 0x585510
    return RT_CLASS_LOCAL(WeaponGroup);
}

Impulse WeaponGroup::GetImpulseId() const
{
    // RVA 0x5857E0
    return m_impulseId;
}

void WeaponGroup::KeepFire()
{
    // RVA 0x585A50
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

int WeaponGroup::RemoveWeapon(CStr const& gunPartName)
{
    // RVA 0x585920
    if (gunPartName.empty())
    {
        return 0;
    }

    auto const it = m_gunPartNames.find(gunPartName);
    if (it == m_gunPartNames.end())
    {
        return 0;
    }

    m_gunPartNames.erase(it);
    M3D_APP->ImmediateMessage(UM_WEAPONGROUP_CHANGED, m_groupId, 0, 0, 0, {}, {});
    return 1;
}

int WeaponGroup::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    // RVA 0x585AB0
    Clear();

    if (!xmlFile || !xmlNode)
    {
        M3D_LOG_INFO("WeaponGroup::LoadFromXml error - null xmlFile or xmlNode");
        return 0;
    }

    int groupId = -1;
    if (!xmlNode->IsEmpty())
    {
        if (auto const* attr = xmlNode->GetAttribute("groupId"))
        {
            groupId = atoi(attr);
        }
    }
    SetGroupId(groupId);

    if (!IsValid())
    {
        M3D_LOG_INFO("WeaponGroup::LoadFromXml error - invalid group: id - " + CStr(m_groupId) + CStr(", impulse - ") +
                     M3D_APP->m_pImpulses->GetImpulseNameById(m_impulseId));
        return 0;
    }

    CStr strWeapons;
    retruxx::vector<CStr> lstWeapons;
    m3d::SafeStrAttrib(strWeapons, xmlNode, "weaponParts");
    ai::StrToStringVector(strWeapons, lstWeapons);
    for (int i = 0; i < static_cast<int>(lstWeapons.size()); ++i)
    {
        m_gunPartNames.insert(lstWeapons[i]);
    }
    return 1;
}

bool WeaponGroup::IncludesWeapon(CStr const& gunPartName) const
{
    // RVA 0x585F10
    return m_gunPartNames.find(gunPartName) != m_gunPartNames.end();
}

void WeaponGroup::SetGroupId(int groupId)
{
    // RVA 0x585730
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
    // RVA 0x585610
    Clear();
}

retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>> const& WeaponGroup::GetWeapons() const
{
    // RVA 0x5857F0
    return m_gunPartNames;
}

WeaponGroup::WeaponGroup()
{
    // RVA 0x585550
    m_groupId = -1;
    m_impulseId = IM_ERROR;
}

WeaponGroup::WeaponGroup(WeaponGroup const&)
{
    // RVA 0x5855D0
    // NOTE: the copy constructor copies nothing and leaves m_groupId and m_impulseId uninitialized
    // (Clone goes through it too); copying state is done with operator= instead.
}

WeaponGroup& WeaponGroup::operator=(WeaponGroup const& rhs)
{
    // RVA 0x585FE0
    // Copies only the group state, not the m3d::Object part.
    if (&rhs != this)
    {
        m_groupId = rhs.m_groupId;
        m_impulseId = rhs.m_impulseId;
        if (&m_gunPartNames != &rhs.m_gunPartNames)
        {
            m_gunPartNames = rhs.m_gunPartNames;
        }
    }
    return *this;
}
