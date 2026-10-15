#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"

#include "retruxx/common.h"

namespace ai
{
    class TriggerPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Trigger : public ai::Obj
    {
        friend class TriggerPrototypeInfo;

    protected:
        virtual ~Trigger() override;

    private:
        Trigger(ai::TriggerPrototypeInfo const& prototypeInfo);
        Trigger(ai::Trigger const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classTrigger;

        virtual ai::TriggerPrototypeInfo const* GetPrototypeInfo() const override;
        virtual int OnEvent(ai::Event const& evn) override;

    protected:
        static void RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(retruxx::set<CStr>& Props) const override;
        virtual void GetPropertiesIDs(retruxx::set<int>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr> m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus> m_propertiesSaveStatesMap;

        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

        enum eTriggerState
        {
            TS_EVENTWAIT = 0,
            TS_ACTION = 1,
            TS_OFF = 2,
        };

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual bool NeedCinematicUpdate() override;
        void SetVar(char const* VarName, m3d::AIParam& rValue);
        m3d::AIParam Var(char const* varName);
        void AddEvent(char const* eventName, char const* param);
        void DelEvent(char const* eventName);
        void DelEventObj(char const* eventName, char const* objName);
        int GetTriggeredObjectAmount() const;
        int GetTriggeredObjectID(int num) const;
        void AddTriggeredObjectID(int objId);
        int GetCount() const;
        int IncCount();
        int IsActivated() const;
        void Activate();
        void Deactivate();
        void ActivateIfNeeded();
        char const* GetCallEvent() const;
        char const* GetCallObjName() const;
        int GetCallObjId() const;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        void LoadFromMapXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        static void Registration();

    private:
        retruxx::map<CStr, m3d::AIParam> m_variables;
        ai::Trigger::eTriggerState m_state;
        bool m_StateKeep;
        int m_Count;
        float m_timeOutForTimePeriod;
        unsigned int m_framesForFramesPassed;
        CStr m_flyPathForCinematicFly;
        int m_idForCinemaMsg;
        bool m_bScriptPresent;
        CStr m_triggerScriptFuncName;
        retruxx::vector<int> m_ObjIDs;

        struct auxEventInfo
        {
            ai::eGameEvent m_eventId = GE_UNKNOWN;
            CStr m_objName;
            //auxEventInfo(const ai::Trigger::auxEventInfo& __that);
            //auxEventInfo();
        };

        struct CallEventInfo : public auxEventInfo
        {
            int m_callObjId;
            CallEventInfo();
        };

        using auxEventInfoVector = retruxx::vector<ai::Trigger::auxEventInfo>;

    private:
        retruxx::vector<ai::Trigger::auxEventInfo> m_eventInfos;
        ai::Trigger::CallEventInfo m_callEvent;
        bool m_bCanUpdate;

        static CStr _EmbedTriggerBody(CStr const& scriptCode, CStr const& triggerName);
        void _LoadEventsFromMapXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        void _LoadScriptFromMapXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        void _LoadTriggerRuntimesFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        void _SaveTriggerRuntimesToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void _StoreCallEvent(ai::Event const& evn);
        void _OnTargetReachedOrObjectDie(ai::Event const& evn);
        void _OnObjectChangesLocation(ai::Event const& evn);
        void _OnTimePeriod(ai::Event const& evn);
        void _OnFramesPassed(ai::Event const& evn);
        void _OnCinematicFly(ai::Event const& evn);
        void _OnCinemaMessage(ai::Event const& evn);
        void _OnDefaultEvent(ai::Event const& evn);
    };
}  // namespace ai
