#include "floatstatistic.h"

#include <stdexcept>

#include "core/aiparam.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(FloatStatistic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(FloatStatistic);

    CStr FloatStatistic::GetValue() const
    {
        throw std::logic_error("Not implemented");
    }

    void FloatStatistic::Increase(float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* FloatStatistic::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void FloatStatistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* FloatStatistic::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    FloatStatistic::~FloatStatistic()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* FloatStatistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Statistic);
    }

    m3d::AIParam FloatStatistic::GetValueAsAIParam() const
    {
        throw std::logic_error("Not implemented");
    }

    void FloatStatistic::Zero()
    {
        throw std::logic_error("Not implemented");
    }

    void FloatStatistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* FloatStatistic::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    FloatStatistic::FloatStatistic(FloatStatistic const&)
    {
        throw std::logic_error("Not implemented");
    }

    FloatStatistic::FloatStatistic()
    {
        throw std::logic_error("Not implemented");
    }
}
