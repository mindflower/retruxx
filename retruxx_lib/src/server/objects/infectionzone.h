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
        /* 0x0040 */ float m_minDistToPlayer;
        /* 0x0044 */ float m_criticalTeamDist;
        /* 0x0048 */ float m_criticalTeamTime;
        /* 0x004c */ float m_blindTeamDist;
        /* 0x0050 */ float m_blindTeamTime;
        /* 0x0054 */ int m_dropOutSegmentAngle;
        /* 0x0058 */ float m_dropOutTimeOut;
        InfectionZonePrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
    }; /* size: 0x005c */

    static_assert(sizeof(InfectionZonePrototypeInfo) == 0x005c);

    class InfectionZone : public ai::Obj
    {
    protected:
        virtual  ~InfectionZone() override /* 0x00 */;

    private:
        InfectionZone(const ai::InfectionZonePrototypeInfo& prototypeInfo);
        InfectionZone(const ai::InfectionZone&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classInfectionZone;
        virtual const ai::InfectionZonePrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

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
        virtual void AddChild(ai::Obj* pObj) override /* 0x94 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x98 */;
        virtual bool RemoveChild(ai::Obj* pChild) override /* 0x9c */;
        virtual void Remove() override /* 0x54 */;
        virtual void SetBelong(int newBelong) override /* 0xc0 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x80 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xac */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb4 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        virtual void _InternalPostLoad() override /* 0xfc */;
        virtual void RenderDebugInfo() const override /* 0xe4 */;
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
        /* 0x00c0 */ geom2d::Polygon2<float> m_infectionPolygon;

        using FPoint2Vector = retruxx::vector<geom2d::Point2<float>, retruxx::allocator<geom2d::Point2<float> > >;

    private:
        /* 0x00d0 */ retruxx::vector<geom2d::Point2<float>, retruxx::allocator<geom2d::Point2<float> > > m_dropOutPoints;
        /* 0x00e0 */ float m_dropOutTimeOut;
        /* 0x00e4 */ float m_lastFramePlayerInsideWithoutEnemies;
        /* 0x00e8 */ CStr m_infectionTeamPrototypeName;
        /* 0x00f4 */ int m_infectionTeamId;
        /* 0x00f8 */ CStr m_infectionLairName;
        /* 0x0104 */ int m_infectionLairId;
        /* 0x0108 */ float m_minDistToPlayer;
        /* 0x010c */ float m_criticalTeamDist;
        /* 0x0110 */ float m_criticalTeamTime;
        /* 0x0114 */ float m_blindTeamDist;
        /* 0x0118 */ float m_blindTeamTime;
        void _GetGoodRandomDropOutPoints(retruxx::vector<CVector, retruxx::allocator<CVector> >& points) const;
        void _TryDropOut(unsigned int restSchwarz);
        ai::InfectionTeam* _GetInfectionTeam();
        ai::InfectionLair* _GetInfectionLair();
        /* 0x011c */ bool m_hadPlayerInside;
        /* 0x011d */ char Padding_276[3];
        /* 0x0120 */ float m_baseTimeoutForRespawn;
        /* 0x0124 */ float m_timeoutForRespawn;
        /* 0x0128 */ float m_timeForRespawn;
        /* 0x012c */ float m_dropOutCos;
        const ai::Vehicle* _GetPlayerVehicle() const;
        void _WatchPlayerInside();
        unsigned int _GetPlayerSchwarz() const;
        unsigned int _GetEnemiesSchwarz() const;
        void _WatchRespawn(float elapsedTime);
        void _WatchRespawnTimeoutFinished(float elapsedTime);
        void _DoSpawnVehicles(unsigned int);
    }; /* size: 0x0130 */

    static_assert(sizeof(InfectionZone) == 0x0130);
}
