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
    }  // namespace cmn
}  // namespace m3d

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
        Quest(Quest const&);
        Quest();
        virtual ~Quest();
        virtual bool LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        virtual void PostLoad();
        int GetId() const;
        int GetParentId() const;
        CStr const& GetName() const;
        bool bSubscribeAutomatic() const;
        ConditionToGive const& GetConditionToGive() const;
        SubQuestCondition GetSubQuestCondition() const;
        bool bCheckAllSubQuests() const;
        retruxx::vector<int> const& GetSubQuestIds() const;
        retruxx::vector<int> const& GetSubsequentQuestIds() const;
        retruxx::vector<int> const& GetPrecedingQuestIds() const;
        CStr const& GetFuncOnComplete() const;
        CStr const& GetFuncOnFail() const;
        CStr const& GetFuncOnTake() const;
        CStr const& GetFuncOnCanBeGiven() const;
        int64_t getTimeForComplete() const;
        retruxx::vector<CStr> const& getActionLevels() const;

    protected:
        int m_questId;
        int m_parentQuestId;
        bool m_bSubscribeAutomatic;
        retruxx::vector<int> m_subQuestIds;
        SubQuestCondition m_subQuestCondition;
        bool m_bCheckAllSubQuests;
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
        int64_t m_TimeForComplete;
        retruxx::vector<CStr> m_ActionLevels;
    };

    class QuestItemPrototypeInfo : public PrototypeInfo
    {
    public:
        QuestItemPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        CStr const& GetModelName() const;
        virtual Obj* CreateTargetObject() const override;

    private:
        CStr m_modelName;
    };

    class QuestManager
    {
    public:
        QuestManager(QuestManager const&);
        QuestManager();
        virtual ~QuestManager();
        virtual bool LoadFromXmlFile(char const* fileName);
        virtual int AddQuest(Quest* quest);
        int GetQuestIdByName(CStr const& questName) const;
        virtual Quest* GetQuestById(int questId) const;
        unsigned int GetNumQuests() const;
        retruxx::set<int> const* GetMutexByQuestId(int questId) const;
        void StrToQuestIdVector(CStr const& str, retruxx::vector<int>& questIds);
        CStr QuestIdVectorToStr(retruxx::vector<int> const& questIds);

    protected:
        void _Clear();
        retruxx::vector<Quest*> m_quests;
        retruxx::vector<retruxx::set<int>> m_mutexes;
    };

    inline QuestManager* theQuestManager = nullptr;
}  // namespace ai
