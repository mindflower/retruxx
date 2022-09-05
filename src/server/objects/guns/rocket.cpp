#include "rocket.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Rocket)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Rocket);

    Obj* RocketPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    RocketPrototypeInfo::RocketPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    void RocketPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    bool RocketPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    RocketPrototypeInfo const* Rocket::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Rocket::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    float Rocket::GetVelocity() const
    {
        throw std::logic_error("Not implemented");
    }

    void Rocket::SetInitialVelocity(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    Rocket::Rocket(RocketPrototypeInfo const& prototype) : Shell(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void Rocket::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Rocket::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void Rocket::CreateBlastWave() const
    {
        throw std::logic_error("Not implemented");
    }

    void Rocket::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    void Rocket::setWithAngleLimit(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Rocket::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Rocket::SetInitialDirection(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Rocket::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Rocket::~Rocket()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Rocket::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Rocket::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}
