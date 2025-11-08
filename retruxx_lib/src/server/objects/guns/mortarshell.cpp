#include "mortarshell.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(MortarShell)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(MortarShell);

    MortarShellPrototypeInfo::MortarShellPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool MortarShellPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
}
