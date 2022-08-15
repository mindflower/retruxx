#pragma once
#include <deque>
#include <queue>
#include <vector>
#include <core/clazz.h>

namespace ai
{
    class Quest;

    class QuestState
    {
    public:
        enum CompleteStatus
        {
            NOT_COMPLETE = 0x0,
            COMPLETE = 0x1,
            FAILED = 0x2,
        };

    public:
        int GetQuestId() const ;
        __int64 getTakeTime() const ;
        ~QuestState();
        QuestState(int);
        bool PostLoad();
        Quest * GetQuest() const ;
        void RecalcState();
        bool bCanBeGiven() const ;
        void OnPlayerPassToMap(CStr const &);
        bool Fail();
        bool SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        CompleteStatus GetCompleteStatus() const ;
        bool LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void RecalcCanBeGiven();
        bool bIsTaken() const ;
        bool Take();
        static bool __fastcall LessByTakeTime(QuestState const *,QuestState const *);
        void Update();
        bool Complete();

    private:
        bool _SetCompleteStatusUnsafe(CompleteStatus);
        bool _bIsLeaf() const ;
        int m_questId;
        CompleteStatus m_completeStatus;
        bool m_bIsTaken;
        bool m_bCanBeGiven;
        __int64 m_TakeTime;
    };

    class QuestStateManager : public m3d::Object
    {
    public:
        bool LoadFromXmlFile(CStr const&);
        bool SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        static m3d::Object* CreateObject();
        void FailQuestIfTaken(char const*);
        virtual m3d::Class* GetClass() const;
        void CompleteQuestIfTaken(char const*);
        void OnQuestCanBeGiven(int);
        bool LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void OnPlayerPassToMap(CStr const&);
        virtual m3d::Object* Clone();
        void OnQuestCompleteStatusChanged(int);
        void TakeQuest(char const*);
        bool IsQuestComplete(char const*) const;
        bool IsQuestTaken(char const*) const;
        void Update(float);
        void FailQuest(char const*);
        QuestState const* GetQuestStateById(int) const;
        bool CanQuestBeGiven(char const*) const;
        void CompleteQuest(char const*);
        virtual ~QuestStateManager();
        static m3d::Class* GetBaseClass();
        void OnQuestTaken(int);
        bool IsQuestFailed(char const*) const;

    protected:
        QuestStateManager();
        QuestStateManager(QuestStateManager const&);

    private:
        void _ExecuteFunctionCode(CStr const&, CStr const&);
        void _UpdateCanBeGiven(std::queue<int, std::deque<int, std::allocator<int> > >&);
        void _UpdateCanBeGiven(int);
        void _RecalcStates(int);
        QuestState* _GetQuestStateById(int) const;
        QuestState* _GetQuestStateByName(char const*) const;
        void _Clear();

    public:
        RT_CLASS_INLINE_DECLARE(QuestStateManager);

    private:
        std::vector<QuestState*> m_questStates;
        unsigned int m_UpdateCount;
    };

    inline QuestStateManager* theQuestStateManager = nullptr;
}
