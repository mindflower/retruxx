#include "wanderersgenerator.h"

#include <stdexcept>

namespace ai
{
    WanderersGeneratorPrototypeInfo::VehiclePartDescription::VehiclePartDescription(
        const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& __that)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WanderersGeneratorPrototypeInfo::VehiclePartDescription::VehiclePartDescription()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersGeneratorPrototypeInfo::VehiclePartDescription::LoadFromXML(m3d::cmn::XmlFile* xmlfile,
        const m3d::cmn::XmlNode* xmlNode)
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

    const CStr& WanderersGeneratorPrototypeInfo::VehiclePartDescription::GetRandomPrototypeName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WanderersGeneratorPrototypeInfo::VehiclePartDescription::GetRandomPrototypeId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersGeneratorPrototypeInfo::VehicleDescription::LoadFromXML(m3d::cmn::XmlFile* xmlFile,
        const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersGeneratorPrototypeInfo::VehicleDescription::LoadPartFromXML(
        ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& part, const char* partName,
        m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersGeneratorPrototypeInfo::VehicleDescription::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& WanderersGeneratorPrototypeInfo::
    VehicleDescription::GetCabinDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& WanderersGeneratorPrototypeInfo::
    VehicleDescription::GetBasketDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& WanderersGeneratorPrototypeInfo::
    VehicleDescription::GetCabinSmallGunDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& WanderersGeneratorPrototypeInfo::
    VehicleDescription::GetCabinBigGunDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& WanderersGeneratorPrototypeInfo::
    VehicleDescription::GetCabinSpecialWeaponDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& WanderersGeneratorPrototypeInfo::
    VehicleDescription::GetBasketSmallGun0Description() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& WanderersGeneratorPrototypeInfo::
    VehicleDescription::GetBasketSmallGun1Description() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& WanderersGeneratorPrototypeInfo::
    VehicleDescription::GetBasketBigGun0Description() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& WanderersGeneratorPrototypeInfo::
    VehicleDescription::GetBasketBigGun1Description() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& WanderersGeneratorPrototypeInfo::
    VehicleDescription::GetBasketSideGunDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const CStr& WanderersGeneratorPrototypeInfo::VehicleDescription::GetPrototypeName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WanderersGeneratorPrototypeInfo::VehicleDescription::GetPrototypeId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WanderersGeneratorPrototypeInfo::WanderersGeneratorPrototypeInfo()
    {
        this->m_desiredCountLow = -1;
        this->m_desiredCountHigh = -1;
    }

    bool WanderersGeneratorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Obj* WanderersGeneratorPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersGeneratorPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
