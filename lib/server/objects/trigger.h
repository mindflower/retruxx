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
        Trigger(const ai::TriggerPrototypeInfo& prototypeInfo);
        Trigger(const ai::Trigger&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classTrigger;

        virtual const ai::TriggerPrototypeInfo* GetPrototypeInfo() const override;
        virtual int OnEvent(const ai::Event& evn) override;

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

        enum eTriggerState
        {
            TS_EVENTWAIT = 0,
            TS_ACTION = 1,
            TS_OFF = 2,
        };

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual bool NeedCinematicUpdate() override;
        void SetVar(const char* VarName, m3d::AIParam& rValue);
        m3d::AIParam Var(const char* varName);
        void AddEvent(const char* eventName, const char* param);
        void DelEvent(const char* eventName);
        void DelEventObj(const char* eventName, const char* objName);
        int GetTriggeredObjectAmount() const;
        int GetTriggeredObjectID(int num) const;
        void AddTriggeredObjectID(int objId);
        int GetCount() const;
        int IncCount();
        int IsActivated() const;
        void Activate();
        void Deactivate();
        void ActivateIfNeeded();
        const char* GetCallEvent() const;
        const char* GetCallObjName() const;
        int GetCallObjId() const;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        void LoadFromMapXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        static void __fastcall Registration();

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

        static CStr __fastcall _EmbedTriggerBody(const CStr& scriptCode, const CStr& triggerName);
        void _LoadEventsFromMapXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        void _LoadScriptFromMapXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        void _LoadTriggerRuntimesFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        void _SaveTriggerRuntimesToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void _StoreCallEvent(const ai::Event& evn);
        void _OnTargetReachedOrObjectDie(const ai::Event& evn);
        void _OnObjectChangesLocation(const ai::Event& evn);
        void _OnTimePeriod(const ai::Event& evn);
        void _OnFramesPassed(const ai::Event& evn);
        void _OnCinematicFly(const ai::Event& evn);
        void _OnCinemaMessage(const ai::Event& evn);
        void _OnDefaultEvent(const ai::Event& evn);
    };
}
