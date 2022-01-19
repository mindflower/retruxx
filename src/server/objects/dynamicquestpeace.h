#pragma once
#include "dynamicquest.h"

namespace ai
{
    class DynamicQuestPeacePrototypeInfo : public DynamicQuestPrototypeInfo
    {
    public:
        DynamicQuestPeacePrototypeInfo();
        virtual ai::DynamicQuest* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        float m_playerMoneyPart;

    };

    class DynamicQuestPeace :  public DynamicQuest
    {
    public:
        DynamicQuestPeace(DynamicQuestPeacePrototypeInfo const &);
        virtual m3d::Class * GetClass() const ;
        virtual DynamicQuestPeacePrototypeInfo const * GetPrototypeInfo() const ;
        virtual int OnEvent(Event const &);
        virtual DynamicQuestManager::QuestType GetQuestType() const ;
        static m3d::Class * GetBaseClass();
        virtual void PassToAnotherMap();

    protected:
        virtual void _OnCreate();
        virtual int _CalcReward();
        virtual void _OnTake();
        virtual void _OnHirerBecamesEnemyWithPlayer();
        virtual ~DynamicQuestPeace();

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();

    public:
        RT_CLASS_INLINE_DECLARE(DynamicQuestPeace);
    };
}
