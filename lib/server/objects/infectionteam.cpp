#include "infectionteam.h"
#include <stdexcept>

#include "core/ini.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(InfectionTeam)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(InfectionTeam);

    void InfectionTeam::SetCriticalDistAndTime(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionTeam::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    InfectionTeam::InfectionTeam(InfectionTeamPrototypeInfo const& prototypeInfo) : Team(prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionTeam::SetBlindDistAndTime(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* InfectionTeam::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Team);
    }

    void InfectionTeam::Generate(std::vector<CVector, std::allocator<CVector>> const&, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* InfectionTeam::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    InfectionTeamPrototypeInfo const* InfectionTeam::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionTeam::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionTeam::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    InfectionTeamPrototypeInfo::Item::Item(const CStr& protoName, int count) : m_protoName(protoName), m_count(count)
    {
    }

    InfectionTeamPrototypeInfo::InfectionTeamPrototypeInfo()
    {
    }

    bool InfectionTeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        auto result = ai::TeamPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeStrAttrib(m_vehiclesGeneratorProtoName, xmlNode, "VehiclesGenerator");
            m_items.clear();

            ref_ptr vehiclesNode = xmlFile->CreateNode();
            xmlNode->GetFirstChild(vehiclesNode, "Vehicles");

            ref_ptr node = xmlFile->CreateNode();
            for (vehiclesNode->GetFirstChild(node, "Vehicle"); !node->IsEmpty(); node->GetNextSibling(node, "Vehicle"))
            {
                CStr protoName;
                unsigned count = 0;
                m3d::SafeStrAttrib(protoName, node, "PrototypeName");
                m3d::SafeUintAttrib(count, node, "Count");
                m_items.emplace_back(protoName, count);
            }
        }
        return result;
    }

    void InfectionTeamPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Obj* InfectionTeamPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    InfectionTeam::~InfectionTeam()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionTeam::_TeamUpdate(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* InfectionTeam::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* InfectionTeam::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
