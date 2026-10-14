#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <math/vector2.h>
#include "team.h"

namespace ai
{
    class WanderersManagerPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        WanderersManagerPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class WanderersManager : public ai::Obj
    {
        friend class WanderersManagerPrototypeInfo;
    protected:
        virtual  ~WanderersManager() override;

    private:
        WanderersManager(const ai::WanderersManagerPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classWanderersManager;
        virtual const ai::WanderersManagerPrototypeInfo* GetPrototypeInfo() const override;
        virtual int OnEvent(const ai::Event& evn) override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;

        struct WayPoint
        {
            WayPoint(const ai::WanderersManager::WayPoint& __that);
            WayPoint(bool transition, const CVector2& position, const CStr& wayPointName, int locationId);
            bool m_transition;
            char Padding_72[3];
            CVector2 m_position;
            int m_locationId;
            CStr m_name;
        };
        
        struct CaravanInfo
        {
            CStr m_prototypeName;
            retruxx::vector<int, retruxx::allocator<int> > m_wayPointIndices;
            void LoadFromXML(const ai::WanderersManager* manager, m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        };

    public:
        unsigned int GetCaravansNum() const;
        const ai::WanderersManager::CaravanInfo& GetCaravanInfo(unsigned int) const;
        unsigned int GetWayPointsNum() const;
        const ai::WanderersManager::WayPoint& GetWayPoint(unsigned int) const;

    protected:
        virtual void _InternalPostLoad() override;

        using StrToWayPointIndexMap = retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > >;
        using WayPoints = retruxx::vector<ai::WanderersManager::WayPoint, retruxx::allocator<ai::WanderersManager::WayPoint> >;
        using WayPointsPair = retruxx::pair<int, int>;
        using PrecisePath = retruxx::vector<CVector2, retruxx::allocator<CVector2> >;
        using PrecisePaths = retruxx::map<retruxx::pair<int, int>, retruxx::vector<CVector2, retruxx::allocator<CVector2> >, retruxx::less<retruxx::pair<int, int> >, retruxx::allocator<retruxx::pair<retruxx::pair<int, int> const, retruxx::vector<CVector2, retruxx::allocator<CVector2> > > > >;
        using CaravanInfos = retruxx::vector<ai::WanderersManager::CaravanInfo, retruxx::allocator<ai::WanderersManager::CaravanInfo> >;

        struct WandererState
        {
            int m_wayPointIndex;
            int m_precisePathIndex;
            int m_caravanInfoIndex;
            int m_precisePathBegin;
            int m_precisePathEnd;
            float m_timeout;
            retruxx::vector<CVector2, retruxx::allocator<CVector2> > m_precisePath;
            WandererState(const ai::WanderersManager::WandererState& __that);
            WandererState();
            void LoadFromXML(ai::WanderersManager* manager, m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        };

        using WandererStates = retruxx::map<int, ai::WanderersManager::WandererState, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, ai::WanderersManager::WandererState> > >;

    private:
        retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > > m_wayPointIndexMap;
        retruxx::vector<ai::WanderersManager::WayPoint, retruxx::allocator<ai::WanderersManager::WayPoint> > m_wayPoints;
        retruxx::map<retruxx::pair<int, int>, retruxx::vector<CVector2, retruxx::allocator<CVector2> >, retruxx::less<retruxx::pair<int, int> >, retruxx::allocator<retruxx::pair<retruxx::pair<int, int> const, retruxx::vector<CVector2, retruxx::allocator<CVector2> > > > > m_precisePaths;
        retruxx::vector<ai::WanderersManager::CaravanInfo, retruxx::allocator<ai::WanderersManager::CaravanInfo> > m_caravanInfos;
        retruxx::vector<int, retruxx::allocator<int> > m_vagabondPrototypeIds;
        int m_maxTeamCount;
        float m_rebornTimeout;
        retruxx::map<int, ai::WanderersManager::WandererState, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, ai::WanderersManager::WandererState> > > m_wandererStates;
        retruxx::vector<int, retruxx::allocator<int> > m_waitingVagabonds;
        retruxx::vector<int, retruxx::allocator<int> > m_guardVehiclesIds;
        retruxx::vector<int, retruxx::allocator<int> > m_transitionIndices;
        float m_timeBeforeReborn;
        void _LoadWayPointsFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        void _LoadPrecisePathsFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        void _LoadCaravansInfoFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        void _LoadVagabondsInfoFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        void _LoadWanderersStatesFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        void _SaveWayPointsToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void _SavePrecisePathsToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void _SaveCaravansInfoToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void _SaveVagabondInfoToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void _SaveWanderersStatesToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void _SpawnWanderer();
        void _SpawnCaravanTeam(int index);
        void _SpawnVagabondTeam(int index);
        int _GetWayPointIndex(const CStr& wayPointName) const;
        void _SetStatePrecisePath(ai::WanderersManager::WandererState& state, int beginIndex, int endIndex);
        retruxx::vector<CVector2, retruxx::allocator<CVector2> > _GetPrecisePath(int beginIndex, int endIndex) const;
        void _SendTeamToPrecisePathPoint(ai::Team* team, const ai::WanderersManager::WandererState& state) const;
        unsigned int _GetWanderersCount() const;
        void _OnTargetReached(const ai::Event& evn);
        void _OnObjectDie(const ai::Event& evn);
        void _OnObjectEntersLocation(const ai::Event& evn);
        void _OnLostGuardsNeedDirection(const ai::Event& evn);
    };
}
