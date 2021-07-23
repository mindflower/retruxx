#pragma once
#include "dynamicquest.h"

namespace ai
{
    class DynamicQuestReachPrototypeInfo : public DynamicQuestPrototypeInfo
    {
    public:
        DynamicQuestReachPrototypeInfo();
        virtual ai::DynamicQuest* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        float m_playerSchwarzPart;

    };

    class DynamicQuestReach : public DynamicQuest
    {
    public:
        virtual m3d::Class * GetClass() const ;
        virtual DynamicQuestReachPrototypeInfo const * GetPrototypeInfo() const ;
        virtual int OnEvent(Event const &);
        virtual DynamicQuestManager::QuestType GetQuestType() const ;
        virtual void Update(float,unsigned int);
        static m3d::Class * __fastcall GetBaseClass();
        DynamicQuestReach(DynamicQuestReachPrototypeInfo const &);

    protected:
        virtual void _OnCreate();
        virtual int _CalcReward();
        virtual ~DynamicQuestReach();

    private:
        static m3d::Object * __fastcall CreateObject();
        virtual m3d::Object * Clone();
        void _OnObjectEntersLocation(Event const &);
    };
}
