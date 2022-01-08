#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"

namespace ai
{
    class TriggerPrototypeInfo : public PrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
    };

    class Trigger :  public Obj
    {
    public:
        class auxEventInfo
        {
        public:
            auxEventInfo(auxEventInfo const&);

        private:
            eGameEvent m_eventId;
            CStr m_objName;
        };

        enum eTriggerState
        {
            TS_EVENTWAIT = 0x0,
            TS_ACTION = 0x1,
            TS_OFF = 0x2,
        };

        class CallEventInfo : public auxEventInfo
        {
        public:
            CallEventInfo();

        private:
            int m_callObjId;
        };

    public:
        void Deactivate();
        static m3d::Class * GetBaseClass();
        int GetTriggeredObjectAmount() const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void DelEvent(char const *);
        int GetTriggeredObjectID(int) const ;
        int GetCount() const ;
        void ActivateIfNeeded();
        int IsActivated() const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual bool NeedCinematicUpdate();
        void AddTriggeredObjectID(int);
        char const * GetCallObjName() const ;
        void Activate();
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        static void __fastcall Registration();
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual int GetPropertyId(char const *) const ;
        Trigger(TriggerPrototypeInfo const &);
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual int OnEvent(Event const &);
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        int IncCount();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void SetVar(char const *,m3d::AIParam &);
        char const * GetCallEvent() const ;
        void LoadFromMapXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        virtual void Update(float,unsigned int);
        virtual m3d::Class * GetClass() const ;
        virtual CStr GetPropertyName(int) const ;
        m3d::AIParam Var(char const *);
        virtual TriggerPrototypeInfo const * GetPrototypeInfo() const ;
        void DelEventObj(char const *,char const *);
        void AddEvent(char const *,char const *);
        int GetCallObjId() const ;

    protected:
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual ~Trigger();

    private:
        void _LoadTriggerRuntimesFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void _OnTargetReachedOrObjectDie(Event const &);
        virtual m3d::Object * Clone();
        void _StoreCallEvent(Event const &);
        void _OnObjectChangesLocation(Event const &);
        void _LoadScriptFromMapXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void _OnFramesPassed(Event const &);
        static CStr __fastcall _EmbedTriggerBody(CStr const &,CStr const &);
        void _SaveTriggerRuntimesToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void _OnCinematicFly(Event const &);
        static m3d::Object * CreateObject();
        void _OnTimePeriod(Event const &);
        void _OnDefaultEvent(Event const &);
        void _OnCinemaMessage(Event const &);
        void _LoadEventsFromMapXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);

    private:
        std::map<CStr,m3d::AIParam> m_variables;
        eTriggerState m_state;
        bool m_StateKeep;
        int m_Count;
        float m_timeOutForTimePeriod;
        unsigned int m_framesForFramesPassed;
        CStr m_flyPathForCinematicFly;
        int m_idForCinemaMsg;
        bool m_bScriptPresent;
        CStr m_triggerScriptFuncName;
        std::vector<int> m_ObjIDs;
        std::vector<auxEventInfo> m_eventInfos;
        CallEventInfo m_callEvent;
        bool m_bCanUpdate;
    };
}
