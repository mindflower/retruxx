#include "rocket.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Rocket)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Rocket);

    Obj* RocketPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RocketPrototypeInfo::RocketPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RocketPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RocketPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RocketPrototypeInfo const* Rocket::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Rocket::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Rocket::GetVelocity() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Rocket::SetInitialVelocity(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Rocket::Rocket(RocketPrototypeInfo const& prototype) : Shell(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Rocket::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Rocket::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Shell);
    }

    void Rocket::CreateBlastWave() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Rocket::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Rocket::setWithAngleLimit(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Rocket::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Rocket::SetInitialDirection(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Rocket::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Rocket::~Rocket()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Rocket::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Rocket::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
