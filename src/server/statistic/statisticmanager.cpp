#include "statisticmanager.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Statistic)
    RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Statistic);

    m3d::Class* Statistic::GetRtClass() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Statistic::GetBaseClass()
    {
		return RT_CLASS_LOCAL(Object);
    }

    bool Statistic::GetGlobalFlag() const
    {
        throw std::logic_error("Not implemented");
    }

    void Statistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Statistic::SetGlobalFlag(bool)
    {
        throw std::logic_error("Not implemented");
    }

    Statistic::~Statistic()
    {
        throw std::logic_error("Not implemented");
    }

    void Statistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    Statistic::Statistic()
    {
        throw std::logic_error("Not implemented");
    }

	StatisticManager::StatisticManager()
	{
		throw std::logic_error("Not implemented");
	}

	CStr StatisticManager::GetAllStatisticsDescription() const
	{
		throw std::logic_error("Not implemented");
	}

	void StatisticManager::ZeroStatisticsForLevel(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	StatisticManager::~StatisticManager()
	{
		throw std::logic_error("Not implemented");
	}

	void StatisticManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	void StatisticManager::Clear()
	{
		throw std::logic_error("Not implemented");
	}

	void StatisticManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void StatisticManager::GetAllStatistics(std::vector<Statistic const*>&) const
	{
		throw std::logic_error("Not implemented");
	}

	Statistic* StatisticManager::GetStatistic(CStr const&, CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	Statistic const* StatisticManager::GetStatistic(CStr const&) const
	{
		throw std::logic_error("Not implemented");
	}

	Statistic* StatisticManager::_CreateStatisticByClassName(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}
}
