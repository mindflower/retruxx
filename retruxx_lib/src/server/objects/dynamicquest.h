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
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        /* 0x0040 */ int m_minReward;
    }; /* size: 0x0044 */

    static_assert(sizeof(DynamicQuestPrototypeInfo) == 0x0044);

    class DynamicQuest : public ai::Obj
    {
    protected:
        virtual  ~DynamicQuest() override = 0 /* 0x00 */;

    private:
        DynamicQuest(const ai::DynamicQuestPrototypeInfo& prototypeInfo);
        DynamicQuest(const ai::DynamicQuest&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classDynamicQuest;
        virtual const ai::DynamicQuestPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual int OnEvent(const ai::Event& evn) override /* 0x40 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x58 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x5c */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x60 */;
        virtual CStr GetPropertyName(int id) const override /* 0x78 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x7c */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x74 */;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x10c */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x108 */;

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x98 */;

        enum QuestStatus;

    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xac */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb4 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        virtual void PassToAnotherMap() /* 0x110 */;
        virtual void UpdateBeforeShowInConversation() /* 0x114 */;
        void Take();
        static void __fastcall Registration();
        virtual ai::DynamicQuestManager::QuestType GetQuestType() const = 0 /* 0x118 */;
        int GetHirerObjId() const;
        int GetTargetObjId() const;
        const CStr& GetHirerName() const;
        const CStr& GetTargetName() const;
        ai::DynamicQuest::QuestStatus GetQuestStatus() const;
        int GetReward() const;
        const ai::GameTime& GetTakeGameTime() const;

    protected:
        virtual void _InternalPostLoad() override /* 0xfc */;
        virtual void _OnCreate() /* 0x11c */;
        virtual void _OnTake() /* 0x120 */;
        virtual int _CalcReward() = 0 /* 0x124 */;
        virtual void _OnHirerBecamesEnemyWithPlayer() /* 0x128 */;
        void _SetStatus(ai::DynamicQuest::QuestStatus newStatus);
        /* 0x00c0 */ int m_reward;
        /* 0x00c4 */ char Padding_43[4];
        /* 0x00c8 */ ai::GameTime m_takeGameTime;
        /* 0x00e0 */ ai::FadingMsgId m_fadingMsgIdOnComplete;
        /* 0x00e4 */ bool m_bShowMessageForAddMoney;
        /* 0x00e5 */ char Padding_44[3];

    private:
        /* 0x00e8 */ ai::DynamicQuest::QuestStatus m_questStatus;
        /* 0x00ec */ int m_hirerObjId;
        /* 0x00f0 */ int m_targetObjId;
        /* 0x00f4 */ CStr m_hirerName;
        /* 0x0100 */ CStr m_targetName;
        void _OnObjectDie(const ai::Event& evn);
        void _OnObjectEntersLocation(const ai::Event& evn);
        void _OnRelationChanged(const ai::Event& evn);
    }; /* size: 0x0110 */

    static_assert(sizeof(DynamicQuest) == 0x0110);
}
