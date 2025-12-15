#include "mortar.h"

#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Mortar)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Mortar);

    void MortarPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    MortarPrototypeInfo::MortarPrototypeInfo()
    {
        m_damageType = DAMAGE_BLAST;
        m_WithShellsPoolLimit = 1;
        m_initialVelocity = 50.0f;
    }

    bool MortarPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = GunPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_initialVelocity, xmlNode, "InitialVelocity");
        }
        return result;
    }

    Obj* MortarPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Mortar::TMortarAngles::ToVector() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Mortar::TMortarAngles::TMortarAngles(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Mortar::TMortarAngles::TMortarAngles()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Mortar::Mortar(MortarPrototypeInfo const& prototype) : Gun(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Mortar::LookAtPoint(CVector const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Mortar::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Mortar::GetMortarShellPosition(CVector const&, CVector const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Mortar::isLookAtPoint(CVector const&, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    MortarPrototypeInfo const* Mortar::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Mortar::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    Mortar::~Mortar()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Mortar::_LaunchShells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Mortar::GetVelocityByDirection(CVector const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Mortar::setCurrentMortarAngles(TMortarAngles const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Mortar::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Mortar::targetFunction2(CVector const&, CVector const&, float, float, float, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Mortar::targetFunction(CVector const&, CVector const&, float, float, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Mortar::GetMortarDirection(CVector const&, CVector const&, CVector const&, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Mortar::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
