#include "favoritestringstatistic.h"

#include <stdexcept>

#include "core/aiparam.h"
#include "core/ini.h"
#include "core/kernel.h"
#include "core/ref_ptr.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(FavoriteStringStatistic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(FavoriteStringStatistic);

    m3d::Class* FavoriteStringStatistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Statistic);
    }

    m3d::Object* FavoriteStringStatistic::Clone()
    {
        // RVA 0x72F490 - see IntStatistic::Clone; the copy constructor asserts.
        return new FavoriteStringStatistic(*this);
    }

    void FavoriteStringStatistic::Zero()
    {
        // RVA 0x72F1C0
        m_counters.clear();
    }

    CStr FavoriteStringStatistic::GetValue() const
    {
        // RVA 0x72EE30 - the favourite is simply whichever key has been counted
        // most. NOTE: ties go to whichever comes first in the map's ordering.
        CStr res;
        unsigned int best = 0;
        for (auto const& entry : m_counters)
        {
            if (entry.second > best)
            {
                res = entry.first;
                best = entry.second;
            }
        }
        return res;
    }

    void FavoriteStringStatistic::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x72ED20 - one "Counter" child per distinct key.
        Statistic::SaveToXml(xmlFile, xmlNode);
        for (auto const& entry : m_counters)
        {
            ref_ptr<m3d::cmn::XmlNode> counterNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Counter");
            xmlNode->AddChild(counterNode);

            counterNode->SetAttribute("Name", entry.first.c_str());
            counterNode->SetAttribute("Value", CStr(entry.second).c_str());
        }
    }

    void FavoriteStringStatistic::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x72EF50
        Statistic::LoadFromXml(xmlFile, xmlNode);
        if (xmlNode->IsEmpty())
        {
            return;
        }

        // NOTE: the shipped code keeps this across iterations and only
        // overwrites it when the attribute is present and not negative, so a
        // "Counter" missing its Value inherits the previous one's count. On the
        // very first such entry it inherits an unrelated stack value; starting
        // from zero here is the one deviation.
        int value = 0;

        ref_ptr<m3d::cmn::XmlNode> counterNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(counterNode, "Counter");
        while (!counterNode->IsEmpty())
        {
            CStr name;
            m3d::SafeStrAttrib(name, counterNode, "Name");

            if (!counterNode->IsEmpty())
            {
                if (char const* raw = counterNode->GetAttribute("Value"))
                {
                    int const parsed = atoi(raw);
                    if (parsed >= 0)
                    {
                        value = parsed;
                    }
                }
            }

            m_counters.insert(std::make_pair(name, static_cast<unsigned int>(value)));

            counterNode->GetNextSibling(counterNode, "Counter");
        }
    }

    m3d::AIParam FavoriteStringStatistic::GetValueAsAIParam() const
    {
        // RVA 0x72F1F0
        return m3d::AIParam(GetValue());
    }

    m3d::Object* FavoriteStringStatistic::CreateObject()
    {
        // RVA 0x72F4C0
        return new FavoriteStringStatistic();
    }

    FavoriteStringStatistic::~FavoriteStringStatistic() = default;

    m3d::Class* FavoriteStringStatistic::GetClass() const
    {
        return RT_CLASS_LOCAL(FavoriteStringStatistic);
    }

    void FavoriteStringStatistic::Increase(CStr const& value)
    {
        // RVA 0x72F0D0 - a key that has not been seen starts at zero and is then
        // counted, so the first Increase leaves it at one.
        auto it = m_counters.find(value);
        if (it == m_counters.end())
        {
            m_counters.insert(std::make_pair(value, 0u));
            it = m_counters.find(value);
        }
        ++it->second;
    }

    FavoriteStringStatistic::FavoriteStringStatistic()
    {
        // RVA 0x72F230
    }

    FavoriteStringStatistic::FavoriteStringStatistic(FavoriteStringStatistic const&)
    {
        // RVA 0x72F2A0
        M3D_ASSERT(0);
    }
}
