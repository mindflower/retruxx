#include "staticautogun.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(StaticAutoGun)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(StaticAutoGun);

    StaticAutoGunPrototypeInfo::StaticAutoGunPrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj* StaticAutoGunPrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool StaticAutoGunPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGunPrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::WeaponLookAtPoint(CVector const&, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool StaticAutoGun::RemoveChild(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* StaticAutoGun::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    NumericInRange<float> const& StaticAutoGun::Health() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    NumericInRange<float>& StaticAutoGun::Health()
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus StaticAutoGun::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr StaticAutoGun::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    StaticAutoGunPrototypeInfo const* StaticAutoGun::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int StaticAutoGun::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::RenderDebugInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::Registration()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool StaticAutoGun::PointIsReachableForGun(CVector const&, retruxx::vector<int, retruxx::allocator<int>> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Gun* StaticAutoGun::getGun()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::InflictDamage(DamageInfo const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::FireFromWeapon(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::SetPositionSelf(CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool StaticAutoGun::ApplyModifier(Modifier const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::Update(float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    StaticAutoGun::StaticAutoGun(StaticAutoGunPrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* StaticAutoGun::GetBaseClass()
    {
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    float StaticAutoGun::GetMaxFiringRangeAI() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::AddChild(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool StaticAutoGun::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::_InternalPostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::_Construct(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool StaticAutoGun::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    StaticAutoGun::~StaticAutoGun()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool StaticAutoGun::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* StaticAutoGun::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool StaticAutoGun::_OnHealthValueBeforeApplyModifier(Modifier const&, float&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticAutoGun::_OnHealthValueAfterChange(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* StaticAutoGun::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }
}
