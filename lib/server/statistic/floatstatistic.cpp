#include "floatstatistic.h"

#include <stdexcept>

#include "core/aiparam.h"
#include "core/ini.h"
#include "core/kernel.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(FloatStatistic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(FloatStatistic);

    CStr FloatStatistic::GetValue() const
    {
        // RVA 0x72E550
        return CStr::format_("%.2f", m_value);
    }

    void FloatStatistic::Increase(float delta)
    {
        // RVA 0x72E250
        m_value = delta + m_value;
    }

    m3d::Object* FloatStatistic::Clone()
    {
        // RVA 0x72E580 - see IntStatistic::Clone; the copy constructor asserts.
        return new FloatStatistic(*this);
    }

    void FloatStatistic::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x72E2E0
        Statistic::LoadFromXml(xmlFile, xmlNode);
        m3d::SafeFloatAttrib(m_value, xmlNode, "Value");
    }

    m3d::Object* FloatStatistic::CreateObject()
    {
        // RVA 0x72E270
        return new FloatStatistic();
    }

    FloatStatistic::~FloatStatistic() = default;

    m3d::Class* FloatStatistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Statistic);
    }

    m3d::AIParam FloatStatistic::GetValueAsAIParam() const
    {
        // RVA 0x72E5B0
        return m3d::AIParam(m_value);
    }

    void FloatStatistic::Zero()
    {
        // RVA 0x72E230
        m_value = 0.0f;
    }

    void FloatStatistic::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x72E4F0
        Statistic::SaveToXml(xmlFile, xmlNode);
        xmlNode->SetAttribute("Value", CStr(m_value).c_str());
    }

    m3d::Class* FloatStatistic::GetClass() const
    {
        return RT_CLASS_LOCAL(FloatStatistic);
    }

    FloatStatistic::FloatStatistic()
    {
        // RVA 0x72E210
        m_value = 0.0f;
    }

    FloatStatistic::FloatStatistic(FloatStatistic const&)
    {
        // RVA 0x72E320
        M3D_ASSERT(0);
    }
}
