#include "statisticmanager.h"

#include <stdexcept>

#include "core/aiparam.h"
#include "core/ini.h"
#include "core/kernel.h"
#include "core/ref_ptr.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Statistic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Statistic);

    m3d::Class* Statistic::GetRtClass() const
    {
        // RVA 0x694300
        return RT_CLASS_LOCAL(Statistic);
    }

    m3d::Class* Statistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    bool Statistic::GetGlobalFlag() const
    {
        // RVA 0x506510
        return m_bGlobalFlag;
    }

    void Statistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x694670
        m3d::SafeStrAttrib(m_name, xmlNode, "Name");
        m3d::SafeBoolAttrib(m_bGlobalFlag, xmlNode, "Global");
    }

    void Statistic::SetGlobalFlag(bool bGlobal)
    {
        // RVA 0x5CB760
        m_bGlobalFlag = bGlobal;
    }

    Statistic::~Statistic() = default;

    void Statistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x694A50
        xmlNode->SetAttribute("Name", m_name.c_str());
        xmlNode->SetAttribute("Global", CStr(static_cast<int>(m_bGlobalFlag)).c_str());
    }

    Statistic::Statistic()
    {
        // RVA 0x694610 - NOTE: m_bGlobalFlag is left uninitialised; every path
        // that creates a statistic sets it before use.
    }

    Statistic::Statistic(Statistic const&)
    {
        // A statistic is owned by the manager and never copied.
        M3D_ASSERT(0);
    }

    // ------------------------------------------------------------------------

    StatisticManager::StatisticManager()
    {
        // RVA 0x697340
    }

    StatisticManager::~StatisticManager()
    {
        // RVA 0x6972D0
        Clear();
        m_statistics.clear();
    }

    void StatisticManager::Clear()
    {
        // RVA 0x696000
        for (auto& entry : m_statistics)
        {
            delete entry.second;
            entry.second = nullptr;
        }
        m_statistics.clear();
    }

    Statistic* StatisticManager::_CreateStatisticByClassName(CStr const& statisticClassName)
    {
        // RVA 0x694AB0 - statistics are built through the engine's class
        // registry, so the XML only has to name the class.
        auto* res = M3D_KERNEL->New(statisticClassName.c_str());
        M3D_ASSERT(IS_KIND_OF(res, Statistic));
        return static_cast<Statistic*>(res);
    }

    Statistic* StatisticManager::GetStatistic(CStr const& name, CStr const& statisticClassName)
    {
        // RVA 0x6960A0 - looking a statistic up creates it on first use, which
        // is why callers pass the class name alongside.
        auto const it = m_statistics.find(name);
        if (it != m_statistics.end())
        {
            return it->second;
        }

        auto* created = _CreateStatisticByClassName(statisticClassName);
        m_statistics[name] = created;
        m_statistics.find(name)->second->SetName(name);
        return m_statistics.find(name)->second;
    }

    Statistic const* StatisticManager::GetStatistic(CStr const& name) const
    {
        // RVA 0x695690 - the const lookup never creates anything.
        auto const it = m_statistics.find(name);
        if (it == m_statistics.end())
        {
            return nullptr;
        }
        return it->second;
    }

    void StatisticManager::GetAllStatistics(std::vector<Statistic const*>& allStats) const
    {
        // RVA 0x695D80
        allStats.clear();
        for (auto const& entry : m_statistics)
        {
            allStats.push_back(entry.second);
        }
    }

    void StatisticManager::ZeroStatisticsForLevel(CStr const& levelName)
    {
        // RVA 0x696160 - each statistic is also kept per level under the name
        // "<statistic><level>"; those copies are reset when the level restarts.
        // NOTE: the class name passed below is empty, which is safe only because
        // the entry is known to exist already.
        static CStr const perLevelStatistics[] = {
            STATISTIC_WARES,
            STATISTIC_GAME_TIME,
            STATISTIC_REAL_TIME,
            STATISTIC_PEOPLES_KILLED,
            STATISTIC_PATH_ELAPSED,
            STATISTIC_GDP,
            STATISTIC_DEATH_COUNTER,
            STATISTIC_VEHICLE_KILLED,
            STATISTIC_SHELLS_EMITTED,
            STATISTIC_FAVORITE_GUN,
            STATISTIC_HIT_RATIO,
        };

        for (auto const& statisticName : perLevelStatistics)
        {
            CStr const key = statisticName + levelName;
            if (theStatisticManager->GetStatistic(key))
            {
                theStatisticManager->GetStatistic(key, CStr())->Zero();
            }
        }
    }

    void StatisticManager::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x694EF0
        for (auto const& entry : m_statistics)
        {
            ref_ptr<m3d::cmn::XmlNode> statisticNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Statistic");
            xmlNode->AddChild(statisticNode);

            statisticNode->SetAttribute("ClassName", entry.second->GetClassNameA());
            entry.second->SaveToXml(xmlFile, statisticNode);
        }
    }

    void StatisticManager::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x6970C0 - each statistic names its own class, and files itself
        // under the name it read back.
        Clear();
        if (xmlNode->IsEmpty())
        {
            return;
        }

        ref_ptr<m3d::cmn::XmlNode> statisticNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(statisticNode, "Statistic");
        while (!statisticNode->IsEmpty())
        {
            CStr strStatisticClassName;
            m3d::SafeStrAttrib(strStatisticClassName, statisticNode, "ClassName");

            auto* statistic = _CreateStatisticByClassName(strStatisticClassName);
            statistic->LoadFromXml(xmlFile, statisticNode);
            m_statistics[CStr(statistic->GetName())] = statistic;

            statisticNode->GetNextSibling(statisticNode, "Statistic");
        }
    }

    CStr StatisticManager::GetAllStatisticsDescription() const
    {
        // RVA 0x695000 - one "<name>\t<value>" line per statistic.
        CStr res;
        for (auto const& entry : m_statistics)
        {
            CStr const statDescription = CStr(entry.second->GetName()) + CStr("\t") + entry.second->GetValue();
            res += statDescription;
            res += CStr("\n");
        }
        return res;
    }
}
