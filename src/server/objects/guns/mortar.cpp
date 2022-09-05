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
        throw std::logic_error("Not implemented");
    }

    MortarPrototypeInfo::MortarPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool MortarPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* MortarPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector Mortar::TMortarAngles::ToVector() const
    {
        throw std::logic_error("Not implemented");
    }

    Mortar::TMortarAngles::TMortarAngles(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    Mortar::TMortarAngles::TMortarAngles()
    {
        throw std::logic_error("Not implemented");
    }

    Mortar::Mortar(MortarPrototypeInfo const& prototype) : Gun(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void Mortar::LookAtPoint(CVector const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Mortar::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector Mortar::GetMortarShellPosition(CVector const&, CVector const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    bool Mortar::isLookAtPoint(CVector const&, float) const
    {
        throw std::logic_error("Not implemented");
    }

    MortarPrototypeInfo const* Mortar::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Mortar::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    Mortar::~Mortar()
    {
        throw std::logic_error("Not implemented");
    }

    void Mortar::_LaunchShells()
    {
        throw std::logic_error("Not implemented");
    }

    CVector Mortar::GetVelocityByDirection(CVector const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Mortar::setCurrentMortarAngles(TMortarAngles const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Mortar::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    float Mortar::targetFunction2(CVector const&, CVector const&, float, float, float, float) const
    {
        throw std::logic_error("Not implemented");
    }

    float Mortar::targetFunction(CVector const&, CVector const&, float, float, float) const
    {
        throw std::logic_error("Not implemented");
    }

    CVector Mortar::GetMortarDirection(CVector const&, CVector const&, CVector const&, float) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Mortar::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}
