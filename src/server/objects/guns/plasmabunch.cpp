#include "plasmabunch.h"

#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(PlasmaBunch)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(PlasmaBunch);

    PlasmaBunchPrototypeInfo::PlasmaBunchPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    void PlasmaBunchPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    bool PlasmaBunchPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* PlasmaBunchPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    void PlasmaBunch::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    PlasmaBunch::PlasmaBunch(PlasmaBunchPrototypeInfo const& prototype) : Shell(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void PlasmaBunch::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* PlasmaBunch::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void PlasmaBunch::SetVelocity(float)
    {
        throw std::logic_error("Not implemented");
    }

    void PlasmaBunch::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void PlasmaBunch::TransferPhysicParamsToSceneGraphNode()
    {
        throw std::logic_error("Not implemented");
    }

    void PlasmaBunch::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* PlasmaBunch::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    PlasmaBunchPrototypeInfo const* PlasmaBunch::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void PlasmaBunch::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    PlasmaBunch::~PlasmaBunch()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* PlasmaBunch::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* PlasmaBunch::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}
