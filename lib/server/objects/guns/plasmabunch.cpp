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
        m_velocity = 1.0f;
        m_acceleration = 1.0f;
        m_flyTime = 1.0f;
    }

    void PlasmaBunchPrototypeInfo::PostLoad()
    {
        // TODO: implement ::PostLoad
    }

    bool PlasmaBunchPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::ShellPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            m3d::SafeFloatAttrib(m_velocity, xmlNode, "Velocity");
            m3d::SafeFloatAttrib(m_acceleration, xmlNode, "Acceleration");
            m3d::SafeFloatAttrib(m_flyTime, xmlNode, "FlyTime");
            m3d::SafeStrAttrib(m_BlastWavePrototypeName, xmlNode, "BlastWavePrototype");
        }
        return result;
    }

    Obj* PlasmaBunchPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PlasmaBunch::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PlasmaBunch::PlasmaBunch(PlasmaBunchPrototypeInfo const& prototype) : Shell(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PlasmaBunch::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* PlasmaBunch::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Shell);
    }

    void PlasmaBunch::SetVelocity(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PlasmaBunch::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PlasmaBunch::TransferPhysicParamsToSceneGraphNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PlasmaBunch::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* PlasmaBunch::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PlasmaBunchPrototypeInfo const* PlasmaBunch::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PlasmaBunch::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PlasmaBunch::~PlasmaBunch()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* PlasmaBunch::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* PlasmaBunch::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
