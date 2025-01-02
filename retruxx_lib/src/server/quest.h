#pragma once
#include <core/stringm3d.h>
#include "thirdparty/containers.h"

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }
}

namespace ai
{

    class Quest
    {
        friend class QuestManager;

    public:
        enum SubQuestCondition
        {
            SUBQUEST_AND = 0,
            SUBQUEST_OR = 1,
            SUBQUEST_XOR = 2,
        };

        struct ConditionToGive
        {
            enum ConditionOperator
            {
                PRECEDERS_AND = 0,
                PRECEDERS_OR = 1,
            };

            enum ConditionType
            {
                PRECEDERS_COMPLETE = 0,
                PRECEDERS_FAILED = 1,
                PRECEDERS_TAKEN = 2,
            };

            /* 0x0000 */ ai::Quest::ConditionToGive::ConditionOperator m_operator;
            /* 0x0004 */ ai::Quest::ConditionToGive::ConditionType m_type;
            ConditionToGive(ai::Quest::ConditionToGive::ConditionOperator op, ai::Quest::ConditionToGive::ConditionType type);
        }; /* size: 0x0008 */

    public:
        Quest(const ai::Quest&);
        Quest();
        virtual  ~Quest() /* 0x00 */;
        virtual bool LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) /* 0x04 */;
        virtual void PostLoad() /* 0x08 */;
        int GetId() const;
        int GetParentId() const;
        const CStr& GetName() const;
        bool bSubscribeAutomatic() const;
        const ai::Quest::ConditionToGive& GetConditionToGive() const;
        ai::Quest::SubQuestCondition GetSubQuestCondition() const;
        bool bCheckAllSubQuests() const;
        const retruxx::vector<int>& GetSubQuestIds() const;
        const retruxx::vector<int>& GetSubsequentQuestIds() const;
        const retruxx::vector<int>& GetPrecedingQuestIds() const;
        const CStr& GetFuncOnComplete() const;
        const CStr& GetFuncOnFail() const;
        const CStr& GetFuncOnTake() const;
        const CStr& GetFuncOnCanBeGiven() const;
        int64_t getTimeForComplete() const;
        const retruxx::vector<CStr>& getActionLevels() const;

    protected:
        /* 0x0008 */ int m_questId;
        /* 0x000c */ int m_parentQuestId;
        /* 0x0010 */ bool m_bSubscribeAutomatic;
        /* 0x0011 */ char Padding_160[3];
        /* 0x0014 */ retruxx::vector<int> m_subQuestIds;
        /* 0x0024 */ ai::Quest::SubQuestCondition m_subQuestCondition;
        /* 0x0028 */ bool m_bCheckAllSubQuests;
        /* 0x0029 */ char Padding_161[3];
        /* 0x002c */ retruxx::vector<int> m_subsequentQuestIds;
        /* 0x003c */ retruxx::vector<int> m_precedingQuestIds;
        /* 0x004c */ ai::Quest::ConditionToGive m_conditionToGive;
        /* 0x0054 */ CStr m_name;
        /* 0x0060 */ CStr m_parentQuestName;
        /* 0x006c */ retruxx::vector<CStr> m_subQuestNames;
        /* 0x007c */ retruxx::vector<CStr> m_precedingQuestNames;
        /* 0x008c */ CStr m_funcOnComplete;
        /* 0x0098 */ CStr m_funcOnFail;
        /* 0x00a4 */ CStr m_funcOnTake;
        /* 0x00b0 */ CStr m_funcOnCanBeGiven;
        /* 0x00bc */ char Padding_162[4];
        /* 0x00c0 */ int64_t m_TimeForComplete;
        /* 0x00c8 */ retruxx::vector<CStr> m_ActionLevels;
    }; /* size: 0x00d8 */

    static_assert(sizeof(Quest) == 0xd8);

    class QuestManager
    {
    public:
        QuestManager(const ai::QuestManager&);
        QuestManager();
        virtual  ~QuestManager() /* 0x00 */;
        virtual bool LoadFromXmlFile(const char* fileName) /* 0x04 */;
        virtual int AddQuest(ai::Quest* quest) /* 0x08 */;
        int GetQuestIdByName(const CStr& questName) const;
        virtual ai::Quest* GetQuestById(int questId) const /* 0x0c */;
        unsigned int GetNumQuests() const;
        const retruxx::set<int>* GetMutexByQuestId(int questId) const;
        void StrToQuestIdVector(const CStr& str, retruxx::vector<int>& questIds);
        CStr QuestIdVectorToStr(const retruxx::vector<int>& questIds);

    protected:
        void _Clear();
        /* 0x0004 */ retruxx::vector<ai::Quest*> m_quests;
        /* 0x0014 */ retruxx::vector<retruxx::set<int>> m_mutexes;
    }; /* size: 0x0024 */

    static_assert(sizeof(QuestManager) == 0x24);

    inline QuestManager* theQuestManager = nullptr;
}
