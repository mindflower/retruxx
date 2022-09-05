#include "intstatistic.h"

#include <stdexcept>

#include "core/aiparam.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(IntStatistic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(IntStatistic);

    CStr IntStatistic::GetValue() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* IntStatistic::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void IntStatistic::Increase(int)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* IntStatistic::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    IntStatistic::~IntStatistic()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* IntStatistic::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam IntStatistic::GetValueAsAIParam() const
    {
        throw std::logic_error("Not implemented");
    }

    void IntStatistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void IntStatistic::Zero()
    {
        throw std::logic_error("Not implemented");
    }

    void IntStatistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* IntStatistic::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    IntStatistic::IntStatistic()
    {
        throw std::logic_error("Not implemented");
    }

    IntStatistic::IntStatistic(IntStatistic const&)
    {
        throw std::logic_error("Not implemented");
    }
}
