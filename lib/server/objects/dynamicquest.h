#pragma once
#include "base/obj.h"
#include "base/objcontainer.h"
#include "base/prototypeinfo.h"
#include "server/dynamicquestmanager.h"

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

    class DynamicQuestPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        DynamicQuestPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        int m_minReward;
    };

    class DynamicQuest : public ai::Obj
    {
        // DynamicQuestHunt::ConsiderPlayerKill reads the quest state directly.
        friend class DynamicQuestHunt;
        // DynamicQuestManager::CreateQuest fills in the new quest's target, hirer and reward.
        friend class DynamicQuestManager;

    protected:
        virtual  ~DynamicQuest() override = 0;

    protected:
        DynamicQuest(const ai::DynamicQuestPrototypeInfo& prototypeInfo);
        DynamicQuest(const ai::DynamicQuest&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classDynamicQuest;
        virtual const ai::DynamicQuestPrototypeInfo* GetPrototypeInfo() const override;
        virtual int OnEvent(const ai::Event& evn) override;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override;
        virtual int GetPropertyId(const char* PropertyName) const override;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;

        enum QuestStatus
        {
            STATUS_NOT_TAKEN = 0,
            STATUS_PROCESSING = 1,
            STATUS_COMPLETE = 2,
            STATUS_FAILED = 3,
            STATUS_FORGOTTEN = 4,
            STATUS_NUM_STATES = 5,
        };

    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void PassToAnotherMap();
        virtual void UpdateBeforeShowInConversation();
        void Take();
        static void __fastcall Registration();
        virtual ai::DynamicQuestManager::QuestType GetQuestType() const = 0;
        int GetHirerObjId() const;
        int GetTargetObjId() const;
        const CStr& GetHirerName() const;
        const CStr& GetTargetName() const;
        ai::DynamicQuest::QuestStatus GetQuestStatus() const;
        int GetReward() const;
        const ai::GameTime& GetTakeGameTime() const;

    protected:
        virtual void _InternalPostLoad() override;
        virtual void _OnCreate();
        virtual void _OnTake();
        virtual int _CalcReward() = 0;
        virtual void _OnHirerBecamesEnemyWithPlayer();
        void _SetStatus(ai::DynamicQuest::QuestStatus newStatus);
        int m_reward;
        ai::GameTime m_takeGameTime;
        ai::FadingMsgId m_fadingMsgIdOnComplete;
        bool m_bShowMessageForAddMoney;

    private:
        ai::DynamicQuest::QuestStatus m_questStatus;
        int m_hirerObjId;
        int m_targetObjId;
        CStr m_hirerName;
        CStr m_targetName;
        void _OnObjectDie(const ai::Event& evn);
        void _OnObjectEntersLocation(const ai::Event& evn);
        void _OnRelationChanged(const ai::Event& evn);
    };
}
