#include "quest.h"

#include <stdexcept>

namespace ai
{
	Quest::ConditionToGive::ConditionToGive(ConditionOperator, ConditionType)
	{
		throw std::logic_error("Not implemented");
	}

	std::vector<int> const& Quest::GetPrecedingQuestIds() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& Quest::GetFuncOnFail() const
	{
		throw std::logic_error("Not implemented");
	}

	int Quest::GetId() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& Quest::GetName() const
	{
		throw std::logic_error("Not implemented");
	}

	std::vector<CStr, std::allocator<CStr>> const& Quest::getActionLevels() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Quest::bSubscribeAutomatic() const
	{
		throw std::logic_error("Not implemented");
	}

	Quest::Quest() :
		m_conditionToGive(ConditionToGive::PRECEDERS_AND, ConditionToGive::PRECEDERS_COMPLETE)
	{
		throw std::logic_error("Not implemented");
	}

	std::vector<int, std::allocator<int>> const& Quest::GetSubQuestIds() const
	{
		throw std::logic_error("Not implemented");
	}

	void Quest::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	int Quest::GetParentId() const
	{
		throw std::logic_error("Not implemented");
	}

	long long Quest::getTimeForComplete() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& Quest::GetFuncOnComplete() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& Quest::GetFuncOnCanBeGiven() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& Quest::GetFuncOnTake() const
	{
		throw std::logic_error("Not implemented");
	}

	Quest::ConditionToGive const& Quest::GetConditionToGive() const
	{
		throw std::logic_error("Not implemented");
	}

	Quest::SubQuestCondition Quest::GetSubQuestCondition() const
	{
		throw std::logic_error("Not implemented");
	}

	Quest::~Quest()
	{
		throw std::logic_error("Not implemented");
	}

	bool Quest::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	bool Quest::bCheckAllSubQuests() const
	{
		throw std::logic_error("Not implemented");
	}

	std::vector<int> const& Quest::GetSubsequentQuestIds() const
	{
		throw std::logic_error("Not implemented");
	}

	unsigned QuestManager::GetNumQuests() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr QuestManager::QuestIdVectorToStr(std::vector<int, std::allocator<int>> const&)
	{
		throw std::logic_error("Not implemented");
	}

	QuestManager::QuestManager()
	{
		throw std::logic_error("Not implemented");
	}

	void QuestManager::StrToQuestIdVector(CStr const&, std::vector<int, std::allocator<int>>&)
	{
		throw std::logic_error("Not implemented");
	}

	QuestManager::~QuestManager()
	{
		throw std::logic_error("Not implemented");
	}

	int QuestManager::GetQuestIdByName(CStr const&) const
	{
		throw std::logic_error("Not implemented");
	}

	std::set<int> const* QuestManager::GetMutexByQuestId(int) const
	{
		throw std::logic_error("Not implemented");
	}

	bool QuestManager::LoadFromXmlFile(char const*)
	{
		throw std::logic_error("Not implemented");
	}

	Quest* QuestManager::GetQuestById(int) const
	{
		throw std::logic_error("Not implemented");
	}

	int QuestManager::AddQuest(Quest*)
	{
		throw std::logic_error("Not implemented");
	}

	void QuestManager::_Clear()
	{
		throw std::logic_error("Not implemented");
	}
}
