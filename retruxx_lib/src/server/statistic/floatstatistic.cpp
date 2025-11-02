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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void FloatStatistic::Increase(float delta)
    {
        this->m_value = delta + this->m_value;
    }

    m3d::Object* FloatStatistic::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void FloatStatistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* FloatStatistic::CreateObject()
    {
        return new FloatStatistic;
    }

    FloatStatistic::~FloatStatistic()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* FloatStatistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Statistic);
    }

    m3d::AIParam FloatStatistic::GetValueAsAIParam() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void FloatStatistic::Zero()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void FloatStatistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* FloatStatistic::GetClass() const
    {
        return RT_CLASS_LOCAL(FloatStatistic);
    }

    FloatStatistic::FloatStatistic(FloatStatistic const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    FloatStatistic::FloatStatistic()
    {
        m_value = 0.0;
    }
}
