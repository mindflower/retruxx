#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <math/geom2d.h>

namespace ai
{
    class InfectionLair;
    class InfectionTeam;
    class Vehicle;

    class InfectionZonePrototypeInfo : public ai::PrototypeInfo
    {
    public:
        float m_minDistToPlayer;
        float m_criticalTeamDist;
        float m_criticalTeamTime;
        float m_blindTeamDist;
        float m_blindTeamTime;
        int m_dropOutSegmentAngle;
        float m_dropOutTimeOut;
        InfectionZonePrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class InfectionZone : public ai::Obj
    {
        friend class InfectionZonePrototypeInfo;

    protected:
        virtual ~InfectionZone() override;

    private:
        InfectionZone(ai::InfectionZonePrototypeInfo const& prototypeInfo);
        InfectionZone(ai::InfectionZone const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classInfectionZone;
        virtual ai::InfectionZonePrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(
            retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::
            map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>>
                m_propertiesMap;
        static inline retruxx::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            retruxx::less<int>,
            retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        virtual void Remove() override;
        virtual void SetBelong(int newBelong) override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void _InternalPostLoad() override;
        virtual void RenderDebugInfo() const override;
        void AddPolygonPoint(float x, float y, unsigned int j);
        void DeletePolygonPoint(unsigned int j);
        void SetPolygonPoint(float x, float y, unsigned int j);
        unsigned int GetNumPolygonPoints();
        CVector GetPolygonPoint(unsigned int);
        void AddDropOutPoint(float x, float y);
        void DeleteDropOutPoint(unsigned int j);
        unsigned int GetNumDropOutPoints();
        CVector GetDropOutPoint(unsigned int);
        bool IsPlayerInside();
        void ResetTimeOut();
        static void __fastcall Registration();

    private:
        geom2d::Polygon2<float> m_infectionPolygon;

        using FPoint2Vector = retruxx::vector<geom2d::Point2<float>, retruxx::allocator<geom2d::Point2<float>>>;

    private:
        retruxx::vector<geom2d::Point2<float>, retruxx::allocator<geom2d::Point2<float>>> m_dropOutPoints;
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
        void _GetGoodRandomDropOutPoints(retruxx::vector<CVector, retruxx::allocator<CVector>>& points) const;
        void _TryDropOut(unsigned int restSchwarz);
        ai::InfectionTeam* _GetInfectionTeam();
        ai::InfectionLair* _GetInfectionLair();
        bool m_hadPlayerInside;
        float m_baseTimeoutForRespawn;
        float m_timeoutForRespawn;
        float m_timeForRespawn;
        float m_dropOutCos;
        ai::Vehicle const* _GetPlayerVehicle() const;
        void _WatchPlayerInside();
        unsigned int _GetPlayerSchwarz() const;
        unsigned int _GetEnemiesSchwarz() const;
        void _WatchRespawn(float elapsedTime);
        void _WatchRespawnTimeoutFinished(float elapsedTime);
        void _DoSpawnVehicles(unsigned int);
    };
}  // namespace ai
