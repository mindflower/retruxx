#include "intstatistic.h"

#include <stdexcept>

#include "core/aiparam.h"
#include "core/ini.h"
#include "core/kernel.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(IntStatistic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(IntStatistic);

    CStr IntStatistic::GetValue() const
    {
        // RVA 0x72E160
        return CStr::format_("%d", m_value);
    }

    m3d::Object* IntStatistic::Clone()
    {
        // RVA 0x72E180 - NOTE: this goes through the copy constructor, which
        // asserts, so a statistic can never actually be cloned.
        return new IntStatistic(*this);
    }

    void IntStatistic::Increase(int delta)
    {
        // RVA 0x72DE70
        m_value += delta;
    }

    m3d::Object* IntStatistic::CreateObject()
    {
        // RVA 0x72DE80
        return new IntStatistic();
    }

    IntStatistic::~IntStatistic() = default;

    m3d::Class* IntStatistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Statistic);
    }

    m3d::AIParam IntStatistic::GetValueAsAIParam() const
    {
        // RVA 0x72E1B0
        return m3d::AIParam(m_value);
    }

    void IntStatistic::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x72DEF0
        Statistic::LoadFromXml(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_value, xmlNode, "Value");
    }

    void IntStatistic::Zero()
    {
        // RVA 0x72DE50
        m_value = 0;
    }

    void IntStatistic::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x72E100
        Statistic::SaveToXml(xmlFile, xmlNode);
        xmlNode->SetAttribute("Value", CStr(m_value).c_str());
    }

    m3d::Class* IntStatistic::GetClass() const
    {
        return RT_CLASS_LOCAL(IntStatistic);
    }

    IntStatistic::IntStatistic()
    {
        // RVA 0x72DE30
        m_value = 0;
    }

    IntStatistic::IntStatistic(IntStatistic const&)
    {
        // RVA 0x72DF30
        M3D_ASSERT(0);
    }
}
