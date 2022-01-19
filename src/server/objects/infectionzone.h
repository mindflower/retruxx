#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <math/geom2d.h>

namespace ai
{
    class InfectionLair;
    class InfectionTeam;
    class Vehicle;

    class InfectionZonePrototypeInfo : public PrototypeInfo
    {
    public:
        InfectionZonePrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;

    private:
        float m_minDistToPlayer;
        float m_criticalTeamDist;
        float m_criticalTeamTime;
        float m_blindTeamDist;
        float m_blindTeamTime;
        int m_dropOutSegmentAngle;
        float m_dropOutTimeOut;

    };
    class InfectionZone :  public Obj
    {
    public:
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        static void __fastcall Registration();
        void DeleteDropOutPoint(unsigned int);
        virtual void Update(float,unsigned int);
        void ResetTimeOut();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void AddPolygonPoint(float,float,unsigned int);
        virtual void SetBelong(int);
        void SetPolygonPoint(float,float,unsigned int);
        static m3d::Class * GetBaseClass();
        void DeletePolygonPoint(unsigned int);
        bool IsPlayerInside();
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        InfectionZone(InfectionZonePrototypeInfo const &);
        virtual void AddChild(Obj *);
        virtual void _InternalPostLoad();
        void AddDropOutPoint(float,float);
        virtual bool RemoveChild(Obj *);
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual m3d::Class * GetClass() const ;
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual int GetPropertyId(char const *) const ;
        virtual InfectionZonePrototypeInfo const * GetPrototypeInfo() const ;
        virtual CStr GetPropertyName(int) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        virtual void Remove();
        virtual void RenderDebugInfo() const ;
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);

    protected:
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual ~InfectionZone();

    private:
        void _TryDropOut(unsigned int);
        unsigned int _GetPlayerSchwarz() const ;
        InfectionTeam * _GetInfectionTeam();
        void _WatchRespawn(float);
        void _WatchRespawnTimeoutFinished(float);
        Vehicle const * _GetPlayerVehicle() const ;
        unsigned int _GetEnemiesSchwarz() const ;
        void _WatchPlayerInside();
        static m3d::Object * CreateObject();
        InfectionLair * _GetInfectionLair();
        virtual m3d::Object * Clone();
        void _GetGoodRandomDropOutPoints(std::vector<CVector,std::allocator<CVector> > &) const ;

    public:
        RT_CLASS_INLINE_DECLARE(InfectionZone);

    private:
        geom2d::Polygon2<float> m_infectionPolygon;
        std::vector<geom2d::Point2<float>> m_dropOutPoints;
        float m_dropOutTimeOut;
        float m_lastFramePlayerInsideWithoutEnemies;
        CStr m_infectionTeamPrototypeName;
        int m_infectionTeamId;
        CStr m_infectionLairName;
        int m_infectionLairId;
        float m_minDistToPlayer;
        float m_criticalTeamDist;
        float m_criticalTeamTime;
        float m_blindTeamDist;
        float m_blindTeamTime;
        bool m_hadPlayerInside;
        float m_baseTimeoutForRespawn;
        float m_timeoutForRespawn;
        float m_timeForRespawn;
        float m_dropOutCos;
    };
}
