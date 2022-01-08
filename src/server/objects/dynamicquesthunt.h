#pragma once
#include "dynamicquest.h"

namespace ai
{
    class DynamicQuestHuntPrototypeInfo : public DynamicQuestPrototypeInfo
    {
    public:
        virtual ai::DynamicQuest* CreateTargetObject() const;
        DynamicQuestHuntPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        float m_playerSchwarzPart;
        float m_huntSeasonLength;
    };

    class DynamicQuestHunt :  public DynamicQuest
    {
    public:
        DynamicQuestHunt(DynamicQuestHuntPrototypeInfo const &);
        void ConsiderPlayerKill(int);
        virtual DynamicQuestManager::QuestType GetQuestType() const ;
        virtual void UpdateBeforeShowInConversation();
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        float GetTimeBeforeHuntEnd() const ;
        virtual m3d::Class * GetClass() const ;
        virtual void Update(float,unsigned int);
        void ProcessHuntCompletion();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual DynamicQuestHuntPrototypeInfo const * GetPrototypeInfo() const ;
        float GetTimeFromHuntStart() const ;
        static m3d::Class * GetBaseClass();
        virtual void PassToAnotherMap();

    protected:
        virtual void _OnTake();
        virtual int _CalcReward();
        virtual ~DynamicQuestHunt();
        virtual void _OnCreate();

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();

    private:
        int m_fragsAtStart;
        float m_timePassed;
    };
}
