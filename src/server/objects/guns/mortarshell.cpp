#include "mortarshell.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(MortarShell)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(MortarShell);

    MortarShellPrototypeInfo::MortarShellPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool MortarShellPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void MortarShellPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* MortarShellPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    void MortarShell::SetInitialPosition(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    CVector MortarShell::GetInitialVelocity() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector MortarShell::GetInitialPosition() const
    {
        throw std::logic_error("Not implemented");
    }

    void MortarShell::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void MortarShell::SetInitialVelocity(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void MortarShell::CreateBlastWave() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* MortarShell::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    MortarShellPrototypeInfo const* MortarShell::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* MortarShell::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void MortarShell::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    MortarShell::MortarShell(MortarShellPrototypeInfo const& prototype) : Shell(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void MortarShell::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    MortarShell::~MortarShell()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* MortarShell::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* MortarShell::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}
