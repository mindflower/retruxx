#include "vagabondteam.h"

#include <cstdlib>
#include <stdexcept>

#include "core/ini.h"
#include <core/kernel.h>
#include <core/log.h>
#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "vehicle.h"
#include "wanderersgenerator.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VagabondTeam)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VagabondTeam);

    std::vector<CStr, std::allocator<CStr>> const& VagabondTeamPrototypeInfo::GetWaresPrototypes() const
    {
        // RVA 0x82FB50
        return m_waresPrototypes;
    }

    VagabondTeamPrototypeInfo::VagabondTeamPrototypeInfo()
    {
        m_bRemoveWhenChildrenDead = 1;
    }

    Obj* VagabondTeamPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x830040
        return new VagabondTeam(*this);
    }

    CStr const& VagabondTeamPrototypeInfo::GetVehiclesGeneratorPrototypeName() const
    {
        // RVA 0x82FB40
        return m_vehiclesGeneratorPrototype;
    }

    bool VagabondTeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = TeamPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeStrAttrib(m_vehiclesGeneratorPrototype, xmlNode, "VehicleGeneratorPrototype");

            CStr strWares;
            m3d::SafeStrAttrib(strWares, xmlNode, "WaresPrototypes");
            m3d::Tokenize(strWares, m_waresPrototypes, "(), ;\t");
        }
        return result;
    }

    VagabondTeam::VagabondTeam(VagabondTeamPrototypeInfo const& prototype) : Team(prototype)
    {
        // RVA 0x82FF70 - an unknown generator is only logged; GenerateAndPlace fails on it later.
        m_vehiclesGeneratorPrototypeId =
            thePrototypeManager->GetPrototypeId(prototype.GetVehiclesGeneratorPrototypeName());
        if (m_vehiclesGeneratorPrototypeId == -1)
        {
            M3D_LOG_INFO(CStr("Unknown VehiclesGenerator ") + prototype.GetVehiclesGeneratorPrototypeName());
        }
    }

    VagabondTeamPrototypeInfo const* VagabondTeam::GetPrototypeInfo() const
    {
        // RVA 0x830080 - NOTE: the prototype is cast without a type check.
        return static_cast<VagabondTeamPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void VagabondTeam::GenerateAndPlace(CVector const& start)
    {
        // RVA 0x8300B0 - a vagabond team is a single vehicle carrying one to three of a random ware.
        // NOTE: the prototype is cast to a wanderers generator without a type check, and a missing
        // one is only reported.
        auto const* generator = static_cast<WanderersGeneratorPrototypeInfo const*>(
            thePrototypeManager->GetPrototypeInfo(m_vehiclesGeneratorPrototypeId));
        if (!generator)
        {
            SYS_ERROR("proto");
        }
        retruxx::vector<int> const vehicleIds = generator->GenerateAndPlace(start, 1);
        retruxx::vector<CStr> const& waresPrototypes = GetPrototypeInfo()->GetWaresPrototypes();

        // NOTE: only the first generated vehicle joins the team (a generator without DesiredCount
        // ignores the count and makes one of each), and neither an empty result, a missing vehicle
        // nor an empty ware list is checked.
        auto* vehicle = static_cast<Vehicle*>(theObjects->GetEntityByObjId(vehicleIds[0]));
        vehicle->SetGamePositionOnGround(vehicle->GetPosition(), true, false);
        vehicle->SetRandomSkin();
        AddChild(vehicle);

        int const wareIndex = rand() % static_cast<int>(waresPrototypes.size());
        int const amount = rand() % 3 + 1;
        vehicle->AddItemsToRepository(waresPrototypes[wareIndex].c_str(), amount);
    }

    m3d::Class* VagabondTeam::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Team);
    }

    void VagabondTeam::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x82FB80
        Team::LoadFromXML(xmlFile, xmlNode);
    }

    void VagabondTeam::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x82FB90
        Team::SaveToXML(xmlFile, xmlNode);
    }

    m3d::Class* VagabondTeam::GetClass() const
    {
        // RVA 0x82FB30
        return RT_CLASS_LOCAL(VagabondTeam);
    }

    VagabondTeam::~VagabondTeam() = default;

    m3d::Object* VagabondTeam::Clone()
    {
        // RVA 0x82FBF0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* VagabondTeam::CreateObject()
    {
        // RVA 0x82FDB0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void VagabondTeam::_DoNoticeEnemy(int)
    {
        // RVA 0x82FB70 - vagabonds ignore enemies they notice.
    }
}  // namespace ai
