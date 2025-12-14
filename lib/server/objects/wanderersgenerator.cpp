#include "wanderersgenerator.h"
#include "core/log.h"

namespace ai
{
    WanderersGeneratorPrototypeInfo::VehiclePartDescription::VehiclePartDescription()
    {
        m_present = true;
    }

    void WanderersGeneratorPrototypeInfo::VehiclePartDescription::LoadFromXML(m3d::cmn::XmlFile* xmlfile, m3d::cmn::XmlNode const* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersGeneratorPrototypeInfo::VehiclePartDescription::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool WanderersGeneratorPrototypeInfo::VehiclePartDescription::HasPrototypes() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool WanderersGeneratorPrototypeInfo::VehiclePartDescription::DoesPresent() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& WanderersGeneratorPrototypeInfo::VehiclePartDescription::GetRandomPrototypeName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WanderersGeneratorPrototypeInfo::VehiclePartDescription::GetRandomPrototypeId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersGeneratorPrototypeInfo::VehicleDescription::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersGeneratorPrototypeInfo::VehicleDescription::LoadPartFromXML(
        ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& part,
        char const* partName,
        m3d::cmn::XmlFile* xmlFile,
        m3d::cmn::XmlNode const* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersGeneratorPrototypeInfo::VehicleDescription::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetCabinDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetCabinSmallGunDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetCabinBigGunDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetCabinSpecialWeaponDescription()
        const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketSmallGun0Description()
        const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketSmallGun1Description()
        const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketBigGun0Description() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketBigGun1Description() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketSideGunDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetPrototypeName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WanderersGeneratorPrototypeInfo::VehicleDescription::GetPrototypeId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WanderersGeneratorPrototypeInfo::WanderersGeneratorPrototypeInfo()
    {
        m_desiredCountLow = -1;
        m_desiredCountHigh = -1;
    }

    bool WanderersGeneratorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m_desiredCountLow = -1;
            m_desiredCountHigh = -1;

            CStr desiredCount;
            m3d::SafeStrAttrib(desiredCount, xmlNode, "DesiredCount");

            retruxx::vector<CStr> tknsDesiredCount;
            m3d::Tokenize(desiredCount, tknsDesiredCount, "-");

            if (!tknsDesiredCount.empty())
            {
                m_desiredCountLow = std::atoi(tknsDesiredCount.front().c_str());
            }
            m_desiredCountHigh = m_desiredCountLow;

            if (tknsDesiredCount.size() > 1)
            {
                m_desiredCountHigh = std::atoi(tknsDesiredCount.back().c_str());
            }

            if (m_desiredCountHigh < m_desiredCountLow)
            {
                m_desiredCountHigh = m_desiredCountLow;
            }

            ref_ptr vehiclesNode = xmlFile->CreateNode();
            for (xmlNode->GetFirstChild(vehiclesNode, "Vehicles"); !vehiclesNode->IsEmpty(); vehiclesNode->GetNextSibling(vehiclesNode, "Vehicles"))
            {
                VehicleDescription desc;
                desc.LoadFromXML(xmlFile, vehiclesNode);
                m_vehicleDescriptions.push_back(std::move(desc));
            }
        }
        return result;
    }

    ai::Obj* WanderersGeneratorPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersGeneratorPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
