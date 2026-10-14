#pragma once
#include <core/stringm3d.h>

#include "objects/base/prototypeinfo.h"
#include "retruxx/common.h"

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

            ConditionOperator m_operator;
            ConditionType m_type;
            ConditionToGive(ConditionOperator op, ConditionType type);
        };

    public:
        Quest(const Quest&);
        Quest();
        virtual  ~Quest();
        virtual bool LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        virtual void PostLoad();
        int GetId() const;
        int GetParentId() const;
        const CStr& GetName() const;
        bool bSubscribeAutomatic() const;
        const ConditionToGive& GetConditionToGive() const;
        SubQuestCondition GetSubQuestCondition() const;
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
        int m_questId;
        int m_parentQuestId;
        bool m_bSubscribeAutomatic;
        char Padding_160[3];
        retruxx::vector<int> m_subQuestIds;
        SubQuestCondition m_subQuestCondition;
        bool m_bCheckAllSubQuests;
        char Padding_161[3];
        retruxx::vector<int> m_subsequentQuestIds;
        retruxx::vector<int> m_precedingQuestIds;
        ConditionToGive m_conditionToGive;
        CStr m_name;
        CStr m_parentQuestName;
        retruxx::vector<CStr> m_subQuestNames;
        retruxx::vector<CStr> m_precedingQuestNames;
        CStr m_funcOnComplete;
        CStr m_funcOnFail;
        CStr m_funcOnTake;
        CStr m_funcOnCanBeGiven;
        char Padding_162[4];
        int64_t m_TimeForComplete;
        retruxx::vector<CStr> m_ActionLevels;
    };

    class QuestItemPrototypeInfo : public PrototypeInfo
    {
    public:
        QuestItemPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        const CStr& GetModelName() const;
        virtual Obj* CreateTargetObject() const override;

    private:
        CStr m_modelName;
    };

    class QuestManager
    {
    public:
        QuestManager(const QuestManager&);
        QuestManager();
        virtual  ~QuestManager();
        virtual bool LoadFromXmlFile(const char* fileName);
        virtual int AddQuest(Quest* quest);
        int GetQuestIdByName(const CStr& questName) const;
        virtual Quest* GetQuestById(int questId) const;
        unsigned int GetNumQuests() const;
        const retruxx::set<int>* GetMutexByQuestId(int questId) const;
        void StrToQuestIdVector(const CStr& str, retruxx::vector<int>& questIds);
        CStr QuestIdVectorToStr(const retruxx::vector<int>& questIds);

    protected:
        void _Clear();
        retruxx::vector<Quest*> m_quests;
        retruxx::vector<retruxx::set<int>> m_mutexes;
    };

    inline QuestManager* theQuestManager = nullptr;
}
