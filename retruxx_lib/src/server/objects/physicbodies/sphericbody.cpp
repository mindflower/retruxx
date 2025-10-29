#include "sphericbody.h"
#include "physichelpers.h"
#
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(SphericBody)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SphericBody);

    SphericBodyPrototypeInfo::SphericBodyPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool SphericBodyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* SphericBody::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* SphericBody::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicBody);
    }

    void SphericBody::SetMass(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SphericBody::SphericBody(std::vector<CollisionInfo, std::allocator<CollisionInfo>> const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SphericBody::SphericBody(SphericBodyPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float SphericBody::GetRadius() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SphericBody::~SphericBody()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* SphericBody::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* SphericBody::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SphericBody::SphericBody()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SphericBody::SphericBody(SphericBody const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Sphere const* SphericBody::_Sphere() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
