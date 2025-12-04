#include "modifier.h"
#include <stdexcept>
#include <math/vector.h>
#include <math/vector2.h>

namespace ai
{
    Modifier::Modifier()
    {
        this->m_Operation = MO_ADD;
        this->m_magicPrototypeId = -1;
        this->m_SenderID = -1;
    }

    Modifier::Modifier(Modifier const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Modifier::~Modifier()
    {
    }

    void Modifier::Create(CStr const&, eModifierOperation, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Modifier::Apply(float*, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Modifier::Apply(CVector2*, CVector2) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Modifier::Apply(CVector*, CVector) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Modifier::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Modifier::ApplyModelNum(int*, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Modifier::ApplyModelNumList(retruxx::vector<int>*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Modifier::ReadFromStr(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Modifier::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Modifier::ApplyEngineID(int*, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
