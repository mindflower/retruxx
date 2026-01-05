#include "timestatistic.h"

#include <stdexcept>

#include "core/aiparam.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(TimeStatistic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(TimeStatistic);

    CStr TimeStatistic::GetValue() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam TimeStatistic::GetValueAsAIParam() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* TimeStatistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Statistic);
    }

    void TimeStatistic::Zero()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* TimeStatistic::CreateObject()
    {
        return new TimeStatistic;
    }

    m3d::Class* TimeStatistic::GetClass() const
    {
        return RT_CLASS_LOCAL(TimeStatistic);
    }

    void TimeStatistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TimeStatistic::~TimeStatistic() = default;

    void TimeStatistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* TimeStatistic::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TimeStatistic::IncreaseByMilliseconds(long long milliseconds)
    {
        m_milliseconds += milliseconds;
    }

    TimeStatistic::TimeStatistic(TimeStatistic const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TimeStatistic::TimeStatistic()
    {
        this->m_milliseconds = 0;
    }
}
