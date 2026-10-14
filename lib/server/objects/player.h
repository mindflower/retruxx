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
        virtual ai::Obj* CreateTargetObject() const override;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        const CStr& GetModelName() const;
        unsigned int GetSkinNumber() const;
        unsigned int GetCfgNumber() const;

    private:
        CStr m_modelName;
        unsigned int m_skinNumber;
        unsigned int m_cfgNumber;
    };

    class Player : public Obj
    {
        friend class PlayerPrototypeInfo;
    protected:
        virtual ~Player() override;

    private:
        Player(const ai::PlayerPrototypeInfo& prototypeInfo);
        Player(const ai::Player&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classPlayer;
        virtual const ai::PlayerPrototypeInfo* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(retruxx::set<CStr>& Props) const override;
        virtual void GetPropertiesIDs(retruxx::set<int>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override;
        virtual int GetPropertyId(const char* PropertyName) const override;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr> m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus> m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual int OnEvent(const ai::Event& evn) override;

        enum PlayerFightState
        {
            FIGHT_CLEAR = 0,
            FIGHT_ALARM = 1,
            FIGHT_BATTLE = 2,
            FIGHT_BATTLE_JUST_FINISHED = 3,
            NUM_FIGHT_STATES = 4,
        };

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        virtual void Remove() override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void SetPassedToAnotherMapStatus() override;
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
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override;
        virtual unsigned int GetSchwarz() const override;
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
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        const ai::InfoCone& GetInfoCone() const;
        int GetInfoObjId() const;
        int GetRadioManagerId() const;
        const CStr& GetModelName() const;
        unsigned int GetSkinNumber() const;
        unsigned int GetCfgNumber() const;
        bool HuntQuestIsTaken() const;
        virtual void RenderDebugInfo() const override;

    protected:
        virtual void _InternalPostLoad() override;

        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Player, int, void>;

    private:
        void _OnMoneyValueAfterChange(int oldMoneyValue);
        ai::NumericBoundedBelow<int> m_money;
        //char Padding_24[120];
        int m_vehicleObjId;
        ai::RadioManager* m_radioManager;
        retruxx::vector<CStr> m_questItemPrototypeNames;
        int m_infoObjId;
        ai::InfoCone* m_infoCone;
        float m_timeInfoObjTimeout;
        ai::Player::PlayerFightState m_playerFightState;
        ai::Player::PlayerFightState m_prevPlayerFightState;
        ai::NumericInRangeRegenerating<float> m_timeOfNoBattle;
        CStr m_lastSaveDir;
        CStr m_modelName;
        unsigned int m_skinNumber;
        unsigned int m_cfgNumber;
        bool m_huntQuestIsTaken;
        char Padding_25[3];
        int m_curNumForVehicleWithoutName;
        void _OnObjectDie(const ai::Event& evn);
        void _OnNoticeEnemy(const ai::Event& evn);
        void _OnUnderAttack(const ai::Event& evn);
        void _OnDynamicQuestTaken(const ai::Event& evn);
        void _OnDynamicQuestComplete(const ai::Event& evn);
        void _OnDynamicQuestForgotten(const ai::Event& evn);
        void _OnDynamicQuestFailed(const ai::Event& evn);
    };

    inline Player* thePlayer = nullptr;
}
