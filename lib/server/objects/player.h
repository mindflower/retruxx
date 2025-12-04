#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <server/components/numericboundedbelow.h>
#include <server/components/numericinrangeregenerating.h>

namespace ai
{
    class InfoCone;
    class IzvratRepository;
    class Vehicle;
    class RadioManager;

    class PlayerPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        PlayerPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        const CStr& GetModelName() const;
        unsigned int GetSkinNumber() const;
        unsigned int GetCfgNumber() const;

    private:
        /* 0x0040 */ CStr m_modelName;
        /* 0x004c */ unsigned int m_skinNumber;
        /* 0x0050 */ unsigned int m_cfgNumber;
    }; /* size: 0x0054 */

    static_assert(sizeof(PlayerPrototypeInfo) == 0x0054);

    class Player : public Obj
    {
        friend class PlayerPrototypeInfo;
    protected:
        virtual ~Player() override /* 0x00 */;

    private:
        Player(const ai::PlayerPrototypeInfo& prototypeInfo);
        Player(const ai::Player&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classPlayer;
        virtual const ai::PlayerPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x58 */;
        virtual void GetPropertiesNames(retruxx::set<CStr>& Props) const override /* 0x5c */;
        virtual void GetPropertiesIDs(retruxx::set<int>& Props) const override /* 0x60 */;
        virtual CStr GetPropertyName(int id) const override /* 0x78 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x7c */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x74 */;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr> m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus> m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x10c */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x108 */;

    public:
        virtual int OnEvent(const ai::Event& evn) override /* 0x40 */;

        enum PlayerFightState
        {
            FIGHT_CLEAR = 0,
            FIGHT_ALARM = 1,
            FIGHT_BATTLE = 2,
            FIGHT_BATTLE_JUST_FINISHED = 3,
            NUM_FIGHT_STATES = 4,
        };

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x98 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x94 */;
        virtual bool RemoveChild(ai::Obj* pChild) override /* 0x9c */;
        virtual void Remove() override /* 0x54 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x80 */;
        virtual void SetPassedToAnotherMapStatus() override /* 0x50 */;
        ai::Vehicle* GetVehicle() const;
        void ChangeVehicleByNew(int newVehiclePrototypeId, bool bDeleteOldVehicle);
        void ChangeVehicleByExisting(int newVehicleObjId, bool bDeleteOldVehicle);
        const ai::NumericBoundedBelow<int>& Money() const;
        ai::NumericBoundedBelow<int>& Money();
        int GetMoney() const;
        void AddMoney(int amount);
        float GetHealth() const;
        float GetMaxHealth() const;
        float GetFuel() const;
        float GetMaxFuel() const;
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override /* 0xec */;
        virtual unsigned int GetSchwarz() const override /* 0xf0 */;
        bool AddItemsToRepository(const char* prototypeName, int amount);
        bool RemoveItemsFromRepository(const char* prototypeName, int amount);
        bool HasAmountOfItemsInRepository(const char* prototypeName, int amount) const;
        bool CanPlaceItemsToRepository(const char* prototypeName, int amount);
        ai::IzvratRepository* GetRepository() const;
        const CStr& GetLastSaveDir() const;
        void SetLastSaveDir(const CStr& saveDir);
        static void __fastcall Registration();
        bool IsQuestItemPresent(const CStr& itemPrototypeName) const;
        int AddQuestItem(const CStr& itemPrototypeName);
        int RemoveQuestItem(const CStr& itemPrototypeName);
        const retruxx::vector<CStr>& GetQuestItemPrototypeNames() const;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xac */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb4 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        const ai::InfoCone& GetInfoCone() const;
        int GetInfoObjId() const;
        int GetRadioManagerId() const;
        const CStr& GetModelName() const;
        unsigned int GetSkinNumber() const;
        unsigned int GetCfgNumber() const;
        bool HuntQuestIsTaken() const;
        virtual void RenderDebugInfo() const override /* 0xe4 */;

    protected:
        virtual void _InternalPostLoad() override /* 0xfc */;

        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Player, int, void>;

    private:
        void _OnMoneyValueAfterChange(int oldMoneyValue);
        /* 0x00c0 */ ai::NumericBoundedBelow<int> m_money;
        ///* 0x00c0 */ char Padding_24[120];
        /* 0x0138 */ int m_vehicleObjId;
        /* 0x013c */ ai::RadioManager* m_radioManager;
        /* 0x0140 */ retruxx::vector<CStr> m_questItemPrototypeNames;
        /* 0x0150 */ int m_infoObjId;
        /* 0x0154 */ ai::InfoCone* m_infoCone;
        /* 0x0158 */ float m_timeInfoObjTimeout;
        /* 0x015c */ ai::Player::PlayerFightState m_playerFightState;
        /* 0x0160 */ ai::Player::PlayerFightState m_prevPlayerFightState;
        /* 0x0164 */ ai::NumericInRangeRegenerating<float> m_timeOfNoBattle;
        /* 0x023c */ CStr m_lastSaveDir;
        /* 0x0248 */ CStr m_modelName;
        /* 0x0254 */ unsigned int m_skinNumber;
        /* 0x0258 */ unsigned int m_cfgNumber;
        /* 0x025c */ bool m_huntQuestIsTaken;
        /* 0x025d */ char Padding_25[3];
        /* 0x0260 */ int m_curNumForVehicleWithoutName;
        void _OnObjectDie(const ai::Event& evn);
        void _OnNoticeEnemy(const ai::Event& evn);
        void _OnUnderAttack(const ai::Event& evn);
        void _OnDynamicQuestTaken(const ai::Event& evn);
        void _OnDynamicQuestComplete(const ai::Event& evn);
        void _OnDynamicQuestForgotten(const ai::Event& evn);
        void _OnDynamicQuestFailed(const ai::Event& evn);
    }; /* size: 0x0264 */

    static_assert(sizeof(Player) == 0x264);

    inline Player* thePlayer = nullptr;
}
