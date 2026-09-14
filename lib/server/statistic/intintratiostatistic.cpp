#include "intintratiostatistic.h"

#include <stdexcept>

#include "core/aiparam.h"
#include "core/ini.h"
#include "core/kernel.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(IntIntRatioStatistic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(IntIntRatioStatistic);

    namespace
    {
        // NOTE: an untouched ratio reads as 100%, not 0%, so a statistic that
        // has never been counted looks perfect rather than hopeless.
        float RatioPercent(int numerator, int denominator)
        {
            if (denominator)
            {
                return static_cast<float>(numerator) * 100.0f / static_cast<float>(denominator);
            }
            return 100.0f;
        }
    }  // namespace

    m3d::Class* IntIntRatioStatistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Statistic);
    }

    m3d::Object* IntIntRatioStatistic::CreateObject()
    {
        // RVA 0x72F5A0
        return new IntIntRatioStatistic();
    }

    void IntIntRatioStatistic::IncreaseDenominator(int delta)
    {
        // RVA 0x72F590
        m_denominator += delta;
    }

    IntIntRatioStatistic::~IntIntRatioStatistic() = default;

    m3d::Object* IntIntRatioStatistic::Clone()
    {
        // RVA 0x72F940 - see IntStatistic::Clone; the copy constructor asserts.
        return new IntIntRatioStatistic(*this);
    }

    CStr IntIntRatioStatistic::GetValue() const
    {
        // RVA 0x72F8E0
        return CStr::format_("%.2f", RatioPercent(m_numerator, m_denominator));
    }

    void IntIntRatioStatistic::IncreaseNumerator(int delta)
    {
        // RVA 0x72F580
        m_numerator += delta;
    }

    m3d::Class* IntIntRatioStatistic::GetClass() const
    {
        return RT_CLASS_LOCAL(IntIntRatioStatistic);
    }

    void IntIntRatioStatistic::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x72F850
        Statistic::SaveToXml(xmlFile, xmlNode);
        xmlNode->SetAttribute("Numerator", CStr(m_numerator).c_str());
        xmlNode->SetAttribute("Denominator", CStr(m_denominator).c_str());
    }

    void IntIntRatioStatistic::Zero()
    {
        // RVA 0x72F560
        m_numerator = 0;
        m_denominator = 0;
    }

    m3d::AIParam IntIntRatioStatistic::GetValueAsAIParam() const
    {
        // RVA 0x72F970
        return m3d::AIParam(RatioPercent(m_numerator, m_denominator));
    }

    void IntIntRatioStatistic::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x72F610
        Statistic::LoadFromXml(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_numerator, xmlNode, "Numerator");
        m3d::SafeIntAttrib(m_denominator, xmlNode, "Denominator");
    }

    IntIntRatioStatistic::IntIntRatioStatistic()
    {
        // RVA 0x72F540
        m_numerator = 0;
        m_denominator = 0;
    }

    IntIntRatioStatistic::IntIntRatioStatistic(IntIntRatioStatistic const&)
    {
        // RVA 0x72F680
        M3D_ASSERT(0);
    }
}
