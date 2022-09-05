#include "staticautogun.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(StaticAutoGun)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(StaticAutoGun);

    StaticAutoGunPrototypeInfo::StaticAutoGunPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* StaticAutoGunPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool StaticAutoGunPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGunPrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::WeaponLookAtPoint(CVector const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool StaticAutoGun::RemoveChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* StaticAutoGun::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    NumericInRange<float> const& StaticAutoGun::Health() const
    {
        throw std::logic_error("Not implemented");
    }

    NumericInRange<float>& StaticAutoGun::Health()
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus StaticAutoGun::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr StaticAutoGun::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    StaticAutoGunPrototypeInfo const* StaticAutoGun::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    int StaticAutoGun::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    bool StaticAutoGun::PointIsReachableForGun(CVector const&, std::vector<int, std::allocator<int>> const&)
    {
        throw std::logic_error("Not implemented");
    }

    Gun* StaticAutoGun::getGun()
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::InflictDamage(DamageInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::FireFromWeapon(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::SetPositionSelf(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool StaticAutoGun::ApplyModifier(Modifier const&)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    StaticAutoGun::StaticAutoGun(StaticAutoGunPrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* StaticAutoGun::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    float StaticAutoGun::GetMaxFiringRangeAI() const
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::AddChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    bool StaticAutoGun::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::_Construct(bool)
    {
        throw std::logic_error("Not implemented");
    }

    bool StaticAutoGun::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    StaticAutoGun::~StaticAutoGun()
    {
        throw std::logic_error("Not implemented");
    }

    bool StaticAutoGun::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* StaticAutoGun::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    bool StaticAutoGun::_OnHealthValueBeforeApplyModifier(Modifier const&, float&)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticAutoGun::_OnHealthValueAfterChange(float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* StaticAutoGun::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}
