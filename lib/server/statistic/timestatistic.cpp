#include "timestatistic.h"

#include <stdexcept>

#include "core/aiparam.h"
#include "core/ini.h"
#include "core/kernel.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(TimeStatistic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(TimeStatistic);

    namespace
    {
        // Splits the stored millisecond count the way both GetValue and
        // GetValueAsAIParam need it.
        void SplitTime(__int64 milliseconds, int& days, int& hours, int& minutes, int& seconds)
        {
            __int64 total = milliseconds / 1000;
            seconds = static_cast<int>(total % 60);
            total /= 60;
            minutes = static_cast<int>(total % 60);
            total /= 60;
            hours = static_cast<int>(total % 24);
            days = static_cast<int>(total / 24);
        }
    }  // namespace

    CStr TimeStatistic::GetValue() const
    {
        // RVA 0x72E990 - only the non-zero parts are named, so a short session
        // reads "12 seconds " rather than "0 days 00 hours ...".
        int days = 0;
        int hours = 0;
        int minutes = 0;
        int seconds = 0;
        SplitTime(m_milliseconds, days, hours, minutes, seconds);

        CStr res;
        if (days)
        {
            res += CStr::format_("%d days ", days);
        }
        if (hours)
        {
            res += CStr::format_("%.2d hours ", hours);
        }
        if (minutes)
        {
            res += CStr::format_("%.2d minutes ", minutes);
        }
        if (seconds)
        {
            res += CStr::format_("%.2d seconds ", seconds);
        }
        return res;
    }

    m3d::AIParam TimeStatistic::GetValueAsAIParam() const
    {
        // RVA 0x72EB50 - scripts get the parts as a list rather than a string.
        int days = 0;
        int hours = 0;
        int minutes = 0;
        int seconds = 0;
        SplitTime(m_milliseconds, days, hours, minutes, seconds);

        retruxx::vector<int> timeParts;
        timeParts.push_back(days);
        timeParts.push_back(hours);
        timeParts.push_back(minutes);
        timeParts.push_back(seconds);
        return m3d::AIParam(timeParts);
    }

    m3d::Class* TimeStatistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Statistic);
    }

    void TimeStatistic::Zero()
    {
        // RVA 0x72E630
        m_milliseconds = 0;
    }

    m3d::Object* TimeStatistic::CreateObject()
    {
        // RVA 0x72E670
        return new TimeStatistic();
    }

    m3d::Class* TimeStatistic::GetClass() const
    {
        return RT_CLASS_LOCAL(TimeStatistic);
    }

    void TimeStatistic::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x72E930
        Statistic::SaveToXml(xmlFile, xmlNode);
        xmlNode->SetAttribute("Milliseconds", CStr(m_milliseconds).c_str());
    }

    TimeStatistic::~TimeStatistic() = default;

    void TimeStatistic::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x72E6E0
        Statistic::LoadFromXml(xmlFile, xmlNode);
        m3d::SafeInt64Attrib(m_milliseconds, xmlNode, "Milliseconds");
    }

    m3d::Object* TimeStatistic::Clone()
    {
        // RVA 0x72EB20 - see IntStatistic::Clone; the copy constructor asserts.
        return new TimeStatistic(*this);
    }

    void TimeStatistic::IncreaseByMilliseconds(__int64 milliseconds)
    {
        // RVA 0x72E650
        m_milliseconds += milliseconds;
    }

    TimeStatistic::TimeStatistic()
    {
        // RVA 0x72E610
        m_milliseconds = 0;
    }

    TimeStatistic::TimeStatistic(TimeStatistic const&)
    {
        // RVA 0x72E760
        M3D_ASSERT(0);
    }
}
