#include "staticautogun.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(StaticAutoGun)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(StaticAutoGun);

    StaticAutoGunPrototypeInfo::StaticAutoGunPrototypeInfo()
    {
        m_maxHealth = 1.0f;
    }

    Obj* StaticAutoGunPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool StaticAutoGunPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            if (m_parentPrototypeName.empty())
            {
                m3d::SafeFloatAttrib(m_maxHealth, xmlNode, "MaxHealth");
                m3d::SafeStrAttrib(m_destroyedModelName, xmlNode, "DestroyedModel");
            }
        }
        return result;
    }

    void StaticAutoGunPrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::WeaponLookAtPoint(CVector const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool StaticAutoGun::RemoveChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* StaticAutoGun::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    NumericInRange<float> const& StaticAutoGun::Health() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    NumericInRange<float>& StaticAutoGun::Health()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus StaticAutoGun::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr StaticAutoGun::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    StaticAutoGunPrototypeInfo const* StaticAutoGun::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int StaticAutoGun::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::Registration()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool StaticAutoGun::PointIsReachableForGun(CVector const&, retruxx::vector<int, retruxx::allocator<int>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gun* StaticAutoGun::getGun()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::InflictDamage(DamageInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::FireFromWeapon(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::SetPositionSelf(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool StaticAutoGun::ApplyModifier(Modifier const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    StaticAutoGun::StaticAutoGun(StaticAutoGunPrototypeInfo const& prototype) :
        ComplexPhysicObj(prototype),
        m_health(prototype.m_maxHealth, 0.0, prototype.m_maxHealth),
        m_timeForNextCheck(0.1, 0.0, 10.0, -1.0)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* StaticAutoGun::GetBaseClass()
    {
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    float StaticAutoGun::GetMaxFiringRangeAI() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::AddChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool StaticAutoGun::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::_Construct(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool StaticAutoGun::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    StaticAutoGun::~StaticAutoGun()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool StaticAutoGun::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* StaticAutoGun::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool StaticAutoGun::_OnHealthValueBeforeApplyModifier(Modifier const&, float&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticAutoGun::_OnHealthValueAfterChange(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* StaticAutoGun::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
