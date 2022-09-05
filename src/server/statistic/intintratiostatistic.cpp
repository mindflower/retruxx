#include "intintratiostatistic.h"

#include <stdexcept>

#include "core/aiparam.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(IntIntRatioStatistic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(IntIntRatioStatistic);

    m3d::Class* IntIntRatioStatistic::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* IntIntRatioStatistic::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void IntIntRatioStatistic::IncreaseDenominator(int)
    {
        throw std::logic_error("Not implemented");
    }

    IntIntRatioStatistic::~IntIntRatioStatistic()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* IntIntRatioStatistic::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    CStr IntIntRatioStatistic::GetValue() const
    {
        throw std::logic_error("Not implemented");
    }

    void IntIntRatioStatistic::IncreaseNumerator(int)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* IntIntRatioStatistic::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void IntIntRatioStatistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void IntIntRatioStatistic::Zero()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam IntIntRatioStatistic::GetValueAsAIParam() const
    {
        throw std::logic_error("Not implemented");
    }

    void IntIntRatioStatistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    IntIntRatioStatistic::IntIntRatioStatistic()
    {
        throw std::logic_error("Not implemented");
    }

    IntIntRatioStatistic::IntIntRatioStatistic(IntIntRatioStatistic const&)
    {
        throw std::logic_error("Not implemented");
    }
}
