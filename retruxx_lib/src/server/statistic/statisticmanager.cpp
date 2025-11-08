#include "statisticmanager.h"

#include <stdexcept>

#include "core/kernel.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Statistic)
    RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Statistic);

    m3d::Class* Statistic::GetRtClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Statistic::GetBaseClass()
    {
		return RT_CLASS_LOCAL(Object);
    }

    bool Statistic::GetGlobalFlag() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Statistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Statistic::SetGlobalFlag(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

	Statistic::~Statistic() = default;

    void Statistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

	Statistic::Statistic() = default;

	StatisticManager::StatisticManager() = default;

	CStr StatisticManager::GetAllStatisticsDescription() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void StatisticManager::ZeroStatisticsForLevel(CStr const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	StatisticManager::~StatisticManager()
	{
		Clear();
	}

	void StatisticManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void StatisticManager::Clear()
	{
		for (auto& stat : m_statistics)
		{
			delete stat.second;
		}
		m_statistics.clear();
	}

	void StatisticManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void StatisticManager::GetAllStatistics(std::vector<Statistic const*>&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Statistic* StatisticManager::GetStatistic(CStr const& name, CStr const& statisticClassName)
	{
        auto it = m_statistics.find(name);
		if (it != m_statistics.end())
		{
			return it->second;
		}

        auto statistic = _CreateStatisticByClassName(statisticClassName);
        m_statistics[name] = statistic;
		return statistic;
	}

	Statistic const* StatisticManager::GetStatistic(CStr const&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Statistic* StatisticManager::_CreateStatisticByClassName(CStr const& className)
	{
		auto res = M3D_KERNEL->New(className.c_str());
		M3D_ASSERT(IS_KIND_OF(res, Statistic));
		return RT_DYNCAST(res, Statistic);
	}
}
