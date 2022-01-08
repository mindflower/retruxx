#pragma once
#include "base/obj.h"
#include "base/objcontainer.h"
#include "base/prototypeinfo.h"

namespace ai
{
    enum FadingMsgId
    {
        FM_CONVOY_FAILED = 0x0,
        FM_HUNT_COMPLETED = 0x1,
        FM_DYNAMIC_QUEST_COMPLETED = 0x2,
        FM_RELATION_CHANGED = 0x3,
        FM_TOWN_CANT_ENTER_ENEMY = 0x4,
        FM_TOWN_CANT_ENTER_LOCKED = 0x5,
        FM_TOWN_ENTER = 0x6,
        FM_ITEM_PICKUP = 0x7,
        FM_ALL_CHESTS_TAKEN = 0x8,
        FM_TAKE_CHEST_HINT = 0x9,
        FM_SUNRISE = 0xA,
        FM_DAY = 0xB,
        FM_SUNSET = 0xC,
        FM_NIGHT = 0xD,
        FM_ITEMS_REMAINED_ON_GROUND = 0xE,
        FM_PLAYER_ADD_MONEY = 0xF,
        FM_PLAYER_GIVE_MONEY = 0x10,
        FM_DYNAMIC_QUEST_FAILED_BECAUSE_HIRER_BECAMES_ENEMY = 0x11,
    };

    class DynamicQuestPrototypeInfo : public PrototypeInfo
    {
    public:
        DynamicQuestPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        int m_minReward;

    };
    class DynamicQuest : public Obj
    {
    public:
        enum QuestStatus
        {
            STATUS_NOT_TAKEN = 0x0,
            STATUS_PROCESSING = 0x1,
            STATUS_COMPLETE = 0x2,
            STATUS_FAILED = 0x3,
            STATUS_FORGOTTEN = 0x4,
            STATUS_NUM_STATES = 0x5,
        };

    public:
        virtual int GetPropertyId(char const *) const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        DynamicQuest(DynamicQuestPrototypeInfo const &);
        virtual DynamicQuestPrototypeInfo const * GetPrototypeInfo() const ;
        virtual CStr GetPropertyName(int) const ;
        GameTime const & GetTakeGameTime() const ;
        CStr const & GetTargetName() const ;
        int GetHirerObjId() const ;
        void Take();
        virtual void PassToAnotherMap();
        static m3d::Class * GetBaseClass();
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        int GetReward() const ;
        CStr const & GetHirerName() const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual int OnEvent(Event const &);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void UpdateBeforeShowInConversation();
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual m3d::Class * GetClass() const ;
        int GetTargetObjId() const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        static void __fastcall Registration();
        QuestStatus GetQuestStatus() const ;

    protected:
        virtual ~DynamicQuest();
        virtual void _OnHirerBecamesEnemyWithPlayer();
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        void _SetStatus(QuestStatus);
        virtual void _OnCreate();
        virtual void _OnTake();
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual void _InternalPostLoad();
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;

    private:
        virtual m3d::Object * Clone();
        void _OnObjectEntersLocation(Event const &);
        static m3d::Object * CreateObject();
        void _OnObjectDie(Event const &);
        void _OnRelationChanged(Event const &);
        int m_reward;
        GameTime m_takeGameTime;
        FadingMsgId m_fadingMsgIdOnComplete;
        bool m_bShowMessageForAddMoney;
        QuestStatus m_questStatus;
        int m_hirerObjId;
        int m_targetObjId;
        CStr m_hirerName;
        CStr m_targetName;
    };
}
