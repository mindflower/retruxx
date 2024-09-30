#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"

#include "thirdparty/containers.h"

namespace ai
{
    class TriggerPrototypeInfo : public PrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
    };

    class Trigger : public ai::Obj
    {
        RETRUXX_DLL_FRIEND_CLASS(Trigger);

    protected:
        virtual ~Trigger() override /* 0x00 */;

    private:
        Trigger(const ai::TriggerPrototypeInfo& prototypeInfo);
        Trigger(const ai::Trigger&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classTrigger;

        virtual const ai::TriggerPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual int OnEvent(const ai::Event& evn) override /* 0x40 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x58 */;
        virtual void GetPropertiesNames(oldstd::set<CStr>& Props) const override /* 0x5c */;
        virtual void GetPropertiesIDs(oldstd::set<int>& Props) const override /* 0x60 */;
        virtual CStr GetPropertyName(int id) const override /* 0x78 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x7c */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x74 */;

    protected:
        static inline oldstd::map<CStr, int, ai::Obj::LessNoCaseCStr> m_propertiesMap;
        static inline oldstd::map<int, enum ai::eGObjPropertySaveStatus> m_propertiesSaveStatesMap;

        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x10c */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x108 */;

        enum eTriggerState
        {
            TS_EVENTWAIT = 0,
            TS_ACTION = 1,
            TS_OFF = 2,
        };

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x98 */;
        virtual bool NeedCinematicUpdate() override /* 0x48 */;
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
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x80 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xac */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        void LoadFromMapXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb4 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        static void __fastcall Registration();

    private:
        /* 0x00c0 */ oldstd::map<CStr, m3d::AIParam> m_variables;
        /* 0x00cc */ ai::Trigger::eTriggerState m_state;
        /* 0x00d0 */ bool m_StateKeep;
        /* 0x00d1 */ char Padding_295[3];
        /* 0x00d4 */ int m_Count;
        /* 0x00d8 */ float m_timeOutForTimePeriod;
        /* 0x00dc */ unsigned int m_framesForFramesPassed;
        /* 0x00e0 */ CStr m_flyPathForCinematicFly;
        /* 0x00ec */ int m_idForCinemaMsg;
        /* 0x00f0 */ bool m_bScriptPresent;
        /* 0x00f1 */ char Padding_296[3];
        /* 0x00f4 */ CStr m_triggerScriptFuncName;
        /* 0x0100 */ retruxx::vector<int> m_ObjIDs;

        struct auxEventInfo
        {
            /* 0x0000 */ ai::eGameEvent m_eventId;
            /* 0x0004 */ CStr m_objName;
            //auxEventInfo(const ai::Trigger::auxEventInfo& __that);
            //auxEventInfo();

        }; /* size: 0x0010 */

        struct CallEventInfo : public auxEventInfo
        {
            /* 0x0010 */ int m_callObjId;
           // CallEventInfo(const ai::Trigger::CallEventInfo&);
           CallEventInfo();
        }; /* size: 0x0014 */

        using auxEventInfoVector = oldstd::vector<ai::Trigger::auxEventInfo>;

    private:
        /* 0x0110 */ oldstd::vector<ai::Trigger::auxEventInfo, oldstd::allocator<ai::Trigger::auxEventInfo> > m_eventInfos;
        /* 0x0120 */ ai::Trigger::CallEventInfo m_callEvent;
        /* 0x0134 */ bool m_bCanUpdate;

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
    }; /* size: 0x0138 */

    static_assert(sizeof(Trigger) == 0x0138);
}
