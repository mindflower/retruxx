#pragma once
#include "dynamicquest.h"
#include <server/dynamicquestmanager.h>

namespace ai
{
    class DynamicQuestConvoyPrototypeInfo : public DynamicQuestPrototypeInfo
    {
    public:
        DynamicQuestConvoyPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual DynamicQuest* CreateTargetObject() const;

    private:
        float m_playerSchwarzPart;
        float m_criticalDistFromPlayer;
        float m_criticalTime;

    };

    class DynamicQuestConvoy :  public DynamicQuest
    {
    public:
        virtual DynamicQuestManager::QuestType GetQuestType() const ;
        int GetCaravanId() const ;
        DynamicQuestConvoy(DynamicQuestConvoyPrototypeInfo const &);
        virtual m3d::Class * GetClass() const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual DynamicQuestConvoyPrototypeInfo const * GetPrototypeInfo() const ;
        static m3d::Class * GetBaseClass();
        virtual void Update(float,unsigned int);
        virtual int OnEvent(Event const &);

    protected:
        void _Unsubscribe();
        virtual ~DynamicQuestConvoy();
        virtual void _OnTake();
        virtual int _CalcReward();
        virtual void _OnCreate();
        void _Subscribe(int,int);

    private:
        void _OnObjectDie(Event const &);
        void _OnTargetReached(Event const &);
        void _OnObjectEntersLocation(Event const &);
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();

    public:
        RT_CLASS_DECLARE(DynamicQuestConvoy);

    private:
        int m_caravanId;
        int m_targetLocationId;
        float m_timePlayerIsTooFar;
    };
}
