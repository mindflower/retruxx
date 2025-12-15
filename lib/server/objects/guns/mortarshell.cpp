#include "mortarshell.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(MortarShell)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(MortarShell);

    MortarShellPrototypeInfo::MortarShellPrototypeInfo()
    {
        m_velocity = 1.0f;
        m_acceleration = 1.0f;
        m_flyTime = 1.0f;
        m_blastWavePrototypeId = -1;
    }

    bool MortarShellPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ShellPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            ai::SimplePhysicObjPrototypeInfo::_SetGeomType(GEOM_TYPE_BOX);
            m3d::SafeFloatAttrib(m_flyTime, xmlNode, "FlyTime");
            m3d::SafeStrAttrib(m_blastWavePrototypeName, xmlNode, "BlastWavePrototype");
        }
        return result;
    }

    void MortarShellPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* MortarShellPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MortarShell::SetInitialPosition(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector MortarShell::GetInitialVelocity() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector MortarShell::GetInitialPosition() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MortarShell::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MortarShell::SetInitialVelocity(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MortarShell::CreateBlastWave() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* MortarShell::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Shell);
    }

    MortarShellPrototypeInfo const* MortarShell::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* MortarShell::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MortarShell::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    MortarShell::MortarShell(MortarShellPrototypeInfo const& prototype) : Shell(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MortarShell::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    MortarShell::~MortarShell()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* MortarShell::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* MortarShell::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
