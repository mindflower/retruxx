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
        return RT_CLASS_LOCAL(Statistic);
    }

    m3d::Object* IntIntRatioStatistic::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void IntIntRatioStatistic::IncreaseDenominator(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    IntIntRatioStatistic::~IntIntRatioStatistic()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* IntIntRatioStatistic::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr IntIntRatioStatistic::GetValue() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void IntIntRatioStatistic::IncreaseNumerator(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* IntIntRatioStatistic::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void IntIntRatioStatistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void IntIntRatioStatistic::Zero()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam IntIntRatioStatistic::GetValueAsAIParam() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void IntIntRatioStatistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    IntIntRatioStatistic::IntIntRatioStatistic()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    IntIntRatioStatistic::IntIntRatioStatistic(IntIntRatioStatistic const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
