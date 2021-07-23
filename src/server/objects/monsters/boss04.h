#pragma once
#include <math/vector2.h>
#include <server/objects/base/complexphysicobj.h>

namespace ai
{
    class Boss04Drone;
    class Boss04Station;

    class Boss04PrototypeInfo : public ComplexPhysicObjPrototypeInfo
    {
    public:
        virtual void PostLoad();
        Boss04PrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        int m_stationPrototypeId;
        int m_dronePrototypeId;
        CVector2 m_timeBetweenDrones;
        unsigned int m_maxDrones;
        std::vector<std::vector<CStr>> m_stationToPartBindings;
        std::vector<int> m_droneSpawningLpIds;
        CStr m_stationPrototypeName;
        CStr m_dronePrototypeName;
        std::vector<CStr> m_droneSpawningLpNames;

    };

    class Boss04 :  public ComplexPhysicObj
    {
    public:
        enum Boss04State
        {
            STATE_WAITING = 0x0,
            STATE_ACTION = 0x1,
            STATE_DEAD = 0x2,
        };

    public:
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        void NextState();
        Boss04(Boss04PrototypeInfo const &);
        virtual CStr GetPropertyName(int) const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        virtual m3d::Class * GetClass() const ;
        static void __fastcall Registration();
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void CreateChildren();
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        float GetMaxHealth() const ;
        virtual int GetPropertyId(char const *) const ;
        virtual Boss04PrototypeInfo const * GetPrototypeInfo() const ;
        virtual bool RemoveChild(Obj *);
        virtual void SetRotation(Quaternion const &);
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        float GetHealth() const ;
        virtual void AddChild(Obj *);
        virtual void Update(float,unsigned int);
        static m3d::Class * __fastcall GetBaseClass();
        virtual int OnEvent(Event const &);
        virtual void SetPosition(CVector const &);
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;

    protected:
        virtual void _InternalPostLoad();
        virtual ~Boss04();
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual void _InternalCreateVisualPart();
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;

    private:
        void _OnBoss04StationDestroyed(Event const &);
        void _SpawnDrone();
        void _SetState(Boss04State);
        virtual m3d::Object * Clone();
        static m3d::Object * __fastcall CreateObject();
        void _CreateStations(bool);

    private:
        std::vector<Boss04Station *> m_stations;
        std::vector<Boss04Drone *> m_drones;
        std::vector<CStr> m_pathNamesForDrones;
        Boss04State m_state;
    };
}
