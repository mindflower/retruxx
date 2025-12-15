#include "boss04part.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss04Part)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss04Part);

    Obj* Boss04PartPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04PartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    m3d::Class* Boss04Part::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    Boss04Part::Boss04Part(Boss04PartPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Part::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04PartPrototypeInfo const* Boss04Part::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Boss04Part::CollideBoss04PartAndVehiclePart(Boss04Part*, VehiclePart*, dContact*, unsigned&, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Part::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss04Part::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04Part::_OnDurabilityValueBeforeApplyModifier(Modifier const&, float&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04Part::~Boss04Part()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Part::_OnDurabilityValueAfterChange(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss04Part::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss04Part::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
