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
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* IntStatistic::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void IntStatistic::Increase(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* IntStatistic::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    IntStatistic::~IntStatistic()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* IntStatistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Statistic);
    }

    m3d::AIParam IntStatistic::GetValueAsAIParam() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void IntStatistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void IntStatistic::Zero()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void IntStatistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* IntStatistic::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    IntStatistic::IntStatistic()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    IntStatistic::IntStatistic(IntStatistic const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
