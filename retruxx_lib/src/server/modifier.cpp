#include "modifier.h"
#include <stdexcept>
#include <math/vector.h>
#include <math/vector2.h>

namespace ai
{
    Modifier::Modifier()
    {
        throw std::logic_error("Not implemented");
    }

    Modifier::Modifier(Modifier const&)
    {
        throw std::logic_error("Not implemented");
    }

    Modifier::~Modifier()
    {
        throw std::logic_error("Not implemented");
    }

    void Modifier::Create(CStr const&, eModifierOperation, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Modifier::Apply(float*, float) const
    {
        throw std::logic_error("Not implemented");
    }

    void Modifier::Apply(CVector2*, CVector2) const
    {
        throw std::logic_error("Not implemented");
    }

    void Modifier::Apply(CVector*, CVector) const
    {
        throw std::logic_error("Not implemented");
    }

    Modifier& Modifier::operator=(Modifier const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Modifier::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Modifier::ApplyModelNum(int*, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Modifier::ApplyModelNumList(std::vector<int, std::allocator<int>>*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Modifier::ReadFromStr(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Modifier::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Modifier::ApplyEngineID(int*, int) const
    {
        throw std::logic_error("Not implemented");
    }
}
