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
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam TimeStatistic::GetValueAsAIParam() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* TimeStatistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Statistic);
    }

    void TimeStatistic::Zero()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* TimeStatistic::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* TimeStatistic::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void TimeStatistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    TimeStatistic::~TimeStatistic()
    {
        throw std::logic_error("Not implemented");
    }

    void TimeStatistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* TimeStatistic::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void TimeStatistic::IncreaseByMilliseconds(long long)
    {
        throw std::logic_error("Not implemented");
    }

    TimeStatistic::TimeStatistic(TimeStatistic const&)
    {
        throw std::logic_error("Not implemented");
    }

    TimeStatistic::TimeStatistic()
    {
        throw std::logic_error("Not implemented");
    }
}
