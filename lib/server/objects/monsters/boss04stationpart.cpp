#include "boss04stationpart.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss04StationPart)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss04StationPart);

    Boss04StationPartPrototypeInfo::Boss04StationPartPrototypeInfo()
    {
        m_bCollisionTrimeshAllowed = true;
        m_maxHealth = 0.0;
    }

    void Boss04StationPartPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // TODO: implement ::RefreshFromXml
    }

    Obj* Boss04StationPartPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04StationPartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    Boss04StationPart::MeshGroupInfo::MeshGroupInfo(int, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04StationPart::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss04StationPart::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Boss04StationPart::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Boss04StationPart::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Boss04StationPart::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04StationPartPrototypeInfo const* Boss04StationPart::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04StationPart::Boss04StationPart(Boss04StationPartPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Boss04StationPart::GetHealth() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04StationPart::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04StationPart::Registration()
    {
    }

    void Boss04StationPart::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss04StationPart::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    bool Boss04StationPart::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04StationPart::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04StationPart::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04StationPart::~Boss04StationPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04StationPart::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss04StationPart::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss04StationPart::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04StationPart::_UpdateMeshGroupsHealth()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
