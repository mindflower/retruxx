#pragma once
#include "dynamicquest.h"

namespace ai
{
    class DynamicQuestDestroyPrototypeInfo : public DynamicQuestPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        DynamicQuestDestroyPrototypeInfo();
        virtual DynamicQuest* CreateTargetObject() const;

    private:
        float m_targetSchwarzPart;
    };

    class DynamicQuestDestroy : public DynamicQuest
    {
    public:
        DynamicQuestDestroy(DynamicQuestDestroyPrototypeInfo const &);
        static void __fastcall LoadNamesForTargetsFromXml(m3d::cmn::XmlNode const *);
        static m3d::Class * __fastcall GetBaseClass();
        virtual m3d::Class * GetClass() const ;
        static bool __fastcall FreeNameForTargetExists();
        virtual DynamicQuestDestroyPrototypeInfo const * GetPrototypeInfo() const ;
        virtual void Update(float,unsigned int);
        static void __fastcall SaveNamesForTargetsToXml(m3d::cmn::XmlNode *);
        virtual DynamicQuestManager::QuestType GetQuestType() const ;
        virtual int OnEvent(Event const &);
        static CStr __fastcall GetRandomNameForTarget();

    protected:
        virtual int _CalcReward();
        virtual void _OnCreate();
        virtual ~DynamicQuestDestroy();

    private:
        virtual m3d::Object * Clone();
        void _OnObjectDie(Event const &);
        static m3d::Object * __fastcall CreateObject();
    };
}
