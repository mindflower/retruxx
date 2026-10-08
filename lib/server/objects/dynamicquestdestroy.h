#pragma once
#include "dynamicquest.h"
#include <server/dynamicquestmanager.h>

namespace ai
{
    class DynamicQuestDestroyPrototypeInfo : public DynamicQuestPrototypeInfo
    {
        // DynamicQuestDestroy::_CalcReward reads the share the reward is worked out from.
        friend class DynamicQuestDestroy;

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
        static m3d::Class * GetBaseClass();
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
        static m3d::Object * CreateObject();

    public:
        RT_CLASS_DECLARE(DynamicQuestDestroy);

    private:
        // The pool of names still free for a bandit leader; saved with the level.
        static inline retruxx::vector<CStr, retruxx::allocator<CStr> > m_namesForTargets;
    };
}
