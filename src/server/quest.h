#pragma once
#include <set>
#include <vector>
#include <core/stringm3d.h>

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }
}

namespace ai
{
    class Quest
    {
    public:
        enum SubQuestCondition
        {
            SUBQUEST_AND = 0x0,
            SUBQUEST_OR = 0x1,
            SUBQUEST_XOR = 0x2,
        };

        class ConditionToGive
        {
        public:
            enum ConditionOperator
            {
                PRECEDERS_AND = 0x0,
                PRECEDERS_OR = 0x1,
            };

            enum ConditionType
            {
                PRECEDERS_COMPLETE = 0x0,
                PRECEDERS_FAILED = 0x1,
                PRECEDERS_TAKEN = 0x2,
            };

        public:
            ConditionToGive(ConditionOperator, ConditionType);

        private:
            ConditionOperator m_operator;
            ConditionType m_type;
        };

    public:
        std::vector<int> const & GetPrecedingQuestIds() const ;
        CStr const & GetFuncOnFail() const ;
        int GetId() const ;
        CStr const & GetName() const ;
        std::vector<CStr,std::allocator<CStr> > const & getActionLevels() const ;
        bool bSubscribeAutomatic() const ;
        Quest();
        std::vector<int,std::allocator<int> > const & GetSubQuestIds() const ;
        virtual void PostLoad();
        int GetParentId() const ;
        __int64 getTimeForComplete() const ;
        CStr const & GetFuncOnComplete() const ;
        CStr const & GetFuncOnCanBeGiven() const ;
        CStr const & GetFuncOnTake() const ;
        ConditionToGive const & GetConditionToGive() const ;
        SubQuestCondition GetSubQuestCondition() const ;
        virtual ~Quest();
        virtual bool LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        bool bCheckAllSubQuests() const ;
        std::vector<int> const & GetSubsequentQuestIds() const ;

    private:
        //Quest_vtbl *__vftable /*VFT*/;
        int m_questId;
        int m_parentQuestId;
        bool m_bSubscribeAutomatic;
        std::vector<int> m_subQuestIds;
        SubQuestCondition m_subQuestCondition;
        bool m_bCheckAllSubQuests;
        std::vector<int> m_subsequentQuestIds;
        std::vector<int> m_precedingQuestIds;
        ConditionToGive m_conditionToGive;
        CStr m_name;
        CStr m_parentQuestName;
        std::vector<CStr> m_subQuestNames;
        std::vector<CStr> m_precedingQuestNames;
        CStr m_funcOnComplete;
        CStr m_funcOnFail;
        CStr m_funcOnTake;
        CStr m_funcOnCanBeGiven;
        __int64 m_TimeForComplete;
        std::vector<CStr> m_ActionLevels;
    };

    class QuestManager
    {
    public:
        unsigned int GetNumQuests() const;
        CStr QuestIdVectorToStr(std::vector<int, std::allocator<int> > const&);
        QuestManager();
        void StrToQuestIdVector(CStr const&, std::vector<int, std::allocator<int> >&);
        virtual ~QuestManager();
        int GetQuestIdByName(CStr const&) const;
        std::set<int> const* GetMutexByQuestId(int) const;
        virtual bool LoadFromXmlFile(char const*);
        virtual Quest* GetQuestById(int) const;
        virtual int AddQuest(Quest*);

    protected:
        void _Clear();

    private:
        //QuestManager_vtbl* __vftable /*VFT*/;
        std::vector<Quest*> m_quests;
        std::vector<std::set<int>> m_mutexes;
    };
}
