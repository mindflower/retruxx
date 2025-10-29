#include "quest.h"

#include <stdexcept>

#include "utils.h"
#include "core/ini.h"
#include "core/log.h"
#include "core/ref_ptr.h"

namespace ai
{
	namespace
	{
		Quest::SubQuestCondition Str2SubQuestCondition(const CStr& str)
		{
			if (str == "and")
			{
				return Quest::SUBQUEST_AND;
			}
			if (str == "or")
			{
				return Quest::SUBQUEST_OR;
			}
			if (str == "xor")
			{
				return Quest::SUBQUEST_XOR;
			}
			M3D_LOG_INFO("Error: undefined quest type: " + str);
			return Quest::SUBQUEST_AND;
		}

		void Str2ConditionToGive(const CStr& str, Quest::ConditionToGive& condition)
		{
			if (str.empty())
			{
				condition.m_operator = Quest::ConditionToGive::PRECEDERS_AND;
				condition.m_type = Quest::ConditionToGive::PRECEDERS_COMPLETE;
				return;
			}
			const CStr op = str.substr(0, 3);
			if (op == "all")
			{
				condition.m_operator = Quest::ConditionToGive::PRECEDERS_AND;
			}
			else if (op == "any")
			{
				condition.m_operator = Quest::ConditionToGive::PRECEDERS_OR;
			}
			else
			{
				M3D_LOG_INFO("Error loading quests: incorrect condition: " + str);
				condition.m_operator = Quest::ConditionToGive::PRECEDERS_AND;
			}
			const CStr typ = str.substr(4);
			if (typ == "complete")
			{
				condition.m_type = Quest::ConditionToGive::PRECEDERS_COMPLETE;
			}
			else if (typ == "failed")
			{
				condition.m_type = Quest::ConditionToGive::PRECEDERS_FAILED;
			}
			else if (typ == "taken")
			{
				condition.m_type = Quest::ConditionToGive::PRECEDERS_TAKEN;
			}
			else
			{
				M3D_LOG_INFO("Error loading quests: incorrect condition: " + str);
				condition.m_type = Quest::ConditionToGive::PRECEDERS_COMPLETE;
			}
		}
	}

	Quest::ConditionToGive::ConditionToGive(ConditionOperator op, ConditionType typ) :
	    m_operator(op),
	    m_type(typ)
	{
	}

	retruxx::vector<int> const& Quest::GetPrecedingQuestIds() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr const& Quest::GetFuncOnFail() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int Quest::GetId() const
	{
		return m_questId;
	}

	CStr const& Quest::GetName() const
	{
		return m_name;
	}

	retruxx::vector<CStr, retruxx::allocator<CStr>> const& Quest::getActionLevels() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Quest::bSubscribeAutomatic() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Quest::Quest() :
		m_conditionToGive(ConditionToGive::PRECEDERS_AND, ConditionToGive::PRECEDERS_COMPLETE)
	{
	}

	retruxx::vector<int, retruxx::allocator<int>> const& Quest::GetSubQuestIds() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void Quest::PostLoad()
	{
		//TODO: check this
		m_parentQuestId = theQuestManager->GetQuestIdByName(m_parentQuestName);
		for (auto const& subQuestName : m_subQuestNames)
		{
			auto questId = theQuestManager->GetQuestIdByName(subQuestName);
		    //for (auto const quest : theQuestManager->m_quests)
		    //{
		    //    if (quest->GetName() == subQuestName)
		    //    {
			//		questId = quest->GetId();
			//		break;
		    //    }
		    //}
			m_subQuestIds.push_back(questId);
		}
		for (auto const& precName : m_precedingQuestNames)
		{
			auto questId = theQuestManager->GetQuestIdByName(precName);
			//for (auto const quest : theQuestManager->m_quests)
			//{
			//	if (quest->GetName() == precName)
			//	{
			//		questId = quest->GetId();
			//		break;
			//	}
			//}
			if (questId == -1)
			{
				M3D_LOG_ERR("Error: quest '" + precName + "' does not exist");
			}
			else
			{
				m_precedingQuestIds.push_back(questId);
				theQuestManager->GetQuestById(questId)->m_subsequentQuestIds.push_back(m_questId);
			}
		}
	}

	int Quest::GetParentId() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	long long Quest::getTimeForComplete() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr const& Quest::GetFuncOnComplete() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr const& Quest::GetFuncOnCanBeGiven() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr const& Quest::GetFuncOnTake() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Quest::ConditionToGive const& Quest::GetConditionToGive() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Quest::SubQuestCondition Quest::GetSubQuestCondition() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	Quest::~Quest()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool Quest::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
		m3d::SafeStrAttrib(m_name, xmlNode, "Name");
		m3d::SafeBoolAttrib(m_bSubscribeAutomatic, xmlNode, "Automatic");
		CStr subCond;
		m3d::SafeStrAttrib(subCond, xmlNode, "SubQuestsCondition");
		if (!subCond.empty())
		{
			m_subQuestCondition = Str2SubQuestCondition(subCond);
		}
		m3d::SafeBoolAttrib(m_bCheckAllSubQuests, xmlNode, "CheckAll");

		m_subQuestIds.clear();
		m_subsequentQuestIds.clear();
		m_precedingQuestIds.clear();
		m_subQuestNames.clear();
		m_precedingQuestNames.clear();

		CStr cond;
		m3d::SafeStrAttrib(cond, xmlNode, "ConditionToGive");
		Str2ConditionToGive(cond, m_conditionToGive);

		CStr precQuests;
		m3d::SafeStrAttrib(precQuests, xmlNode, "PrecedingQuests");
		ai::StrToStringVector(precQuests, m_precedingQuestNames);

		m3d::SafeStrAttrib(m_funcOnComplete, xmlNode, "OnComplete");
		m3d::SafeStrAttrib(m_funcOnFail, xmlNode, "OnFail");
		m3d::SafeStrAttrib(m_funcOnTake, xmlNode, "OnTake");
		m3d::SafeStrAttrib(m_funcOnCanBeGiven, xmlNode, "OnCanBeGiven");
		m3d::SafeInt64Attrib(m_TimeForComplete, xmlNode, "TimeForComplete");

		CStr levels;
		m3d::SafeStrAttrib(levels, xmlNode, "Levels");
		m3d::Tokenize(levels, m_ActionLevels, "(), ;\t");
		ref_ptr questNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
		xmlNode->GetFirstChild(questNode, "quest");
		while(!questNode->IsEmpty())
		{
		    if (subCond.empty())
		    {
				M3D_LOG_ERR("Error loading quest: the quest '" + m_name + "' has children but there is no quest condition");
		    }
			auto quest = new Quest;
			if (!quest->LoadFromXml(xmlFile, questNode))
			{
				M3D_LOG_ERR("Error: couldn't load quest from XML File. The quest is child of quest '" + m_name + "'");
				return false;
			}
			m_subQuestNames.push_back(quest->GetName());
			quest->m_parentQuestName = m_name;
			theQuestManager->AddQuest(quest);
			questNode->GetNextSibling(questNode, "quest");
		}
		return true;
	}

	bool Quest::bCheckAllSubQuests() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	retruxx::vector<int> const& Quest::GetSubsequentQuestIds() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	unsigned QuestManager::GetNumQuests() const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	CStr QuestManager::QuestIdVectorToStr(retruxx::vector<int, retruxx::allocator<int>> const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

    QuestItemPrototypeInfo::QuestItemPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestItemPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const CStr& QuestItemPrototypeInfo::GetModelName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* QuestItemPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

	QuestManager::QuestManager()
	{
	}

	void QuestManager::StrToQuestIdVector(CStr const&, retruxx::vector<int, retruxx::allocator<int>>&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	QuestManager::~QuestManager()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int QuestManager::GetQuestIdByName(CStr const& questName) const
	{
		for (auto const quest : m_quests)
		{
		    if (quest->GetName() == questName)
		    {
				return quest->GetId();
		    }
		}
		return -1;
	}

	retruxx::set<int> const* QuestManager::GetMutexByQuestId(int) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool QuestManager::LoadFromXmlFile(char const* filename)
	{
		//TODO: check this
		_Clear();
		CStr err;
		ref_ptr xmlFile = m3d::ReadXmlFile(filename, &err);
		if (xmlFile)
		{
			ref_ptr questsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
			xmlFile->GetFirstChild(questsNode, "quests");
			if (questsNode->IsEmpty())
			{
				M3D_LOG_INFO("QuestManager::LoadFromXmlFile error - cannot find root node \"quests\"");
				return 0;
			}
			ref_ptr questNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
			for (questsNode->GetFirstChild(questNode, "quest"); !questNode->IsEmpty(); questNode->GetNextSibling(questNode, "quest"))
			{
				auto quest = new Quest;
				if (!quest->LoadFromXml(xmlFile, questNode))
				{
					M3D_LOG_INFO("Error: couldn't load quest from XML file. The quest is root quest.");
					return false;
				}
				AddQuest(quest);
			}  
			for (auto* quest : m_quests)
			{
				quest->PostLoad();
			}

			ref_ptr mutuallyExclusivesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
			questsNode->GetFirstChild(mutuallyExclusivesNode, "MutuallyExclusives");
			if (!mutuallyExclusivesNode->IsEmpty())
			{
				ref_ptr mutexNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
				for (mutuallyExclusivesNode->GetFirstChild(mutexNode, "mutex"); !mutexNode->IsEmpty(); mutexNode->GetNextSibling(mutexNode, "mutex"))
				{
					CStr questsNameAttr;
					m3d::SafeStrAttrib(questsNameAttr, mutexNode, "quests");
					retruxx::vector<CStr> questsNameVec;
					StrToStringVector(questsNameAttr, questsNameVec);
					retruxx::set<int> newMtx;
					for (auto const& name : questsNameVec)
					{
						int questNameId = -1;
					    for (auto const quest : m_quests)
					    {
					        if (quest->GetName() == name)
					        {
								questNameId = quest->GetId();
								break;
					        }
					    }
						if (questNameId == -1)
						{
							M3D_LOG_INFO("Warning: Loading QuestManager: invalid quest name '" + name + CStr("'"));
						}
						else
						{
							for (auto const& mtx : m_mutexes)
							{
							    if (mtx.find(questNameId) != mtx.end())
							    {
							        M3D_LOG_INFO("Warning: Loading QuestManager: quest name " + name + " exists in multiple mutexes");
							    }
							}
							newMtx.insert(questNameId);
						}
					}
					m_mutexes.push_back(newMtx);
				}
				return true;
			}
			return false;
		}
	    M3D_LOG_INFO("No file: " + CStr(filename));
	    return false;
	}

	Quest* QuestManager::GetQuestById(int questId) const
	{
		if (questId < 0 || m_quests.size() < questId)
		{
			return nullptr;
		}
		return m_quests[questId];
	}

	int QuestManager::AddQuest(Quest* quest)
	{
		m_quests.push_back(quest);
		quest->m_questId = m_quests.size() - 1;
		return quest->m_questId;
	}

	void QuestManager::_Clear()
	{
		for (auto quest : m_quests)
		{
			delete quest;
		}
		m_quests.clear();
		m_mutexes.clear();
	}
}
