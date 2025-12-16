#include "boss04station.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss04Station)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss04Station);

    bool Boss04StationPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    Boss04StationPrototypeInfo::Boss04StationPrototypeInfo() = default;

    void Boss04StationPrototypeInfo::PostLoad()
    {
        // TODO: implement ::PostLoad
    }

    Obj* Boss04StationPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::EvaluateToDead()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04Station::Boss04Station(Boss04StationPrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::InflictDamage(DamageInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::Registration()
    {
    }

    bool Boss04Station::bDestroyed() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss04Station::GetBaseClass()
    {
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    bool Boss04Station::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss04Station::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Boss04Station::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Boss04Station::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Boss04Station::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04StationPrototypeInfo const* Boss04Station::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04Station::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04Station::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Station::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04Station::~Boss04Station()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss04Station::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss04Station::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
