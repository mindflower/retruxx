#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <math/vector2.h>

namespace ai
{
    class Team;

    class WanderersManagerPrototypeInfo : public PrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        WanderersManagerPrototypeInfo();
    };

    class WanderersManager : public Obj
    {
    public:
        class WandererState
        {
        public:
            void LoadFromXML(WanderersManager*, m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
            WandererState(WandererState const&);

        private:
            int m_wayPointIndex;
            int m_precisePathIndex;
            int m_caravanInfoIndex;
            int m_precisePathBegin;
            int m_precisePathEnd;
            float m_timeout;
            std::vector<CVector2> m_precisePath;
        };

        class WayPoint
        {
        public:
            WayPoint(WayPoint const&);

        private:
            bool m_transition;
            CVector2 m_position;
            int m_locationId;
            CStr m_name;
        };

        class CaravanInfo
        {
        public:
            void LoadFromXML(WanderersManager const*, m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

        private:
            CStr m_prototypeName;
            std::vector<int> m_wayPointIndices;
        };

    public:
        WanderersManager(ai::WanderersManagerPrototypeInfo const&);
        virtual void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual int OnEvent(ai::Event const&);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual m3d::Class* GetClass() const;
        virtual void Update(float, unsigned int);
        virtual void AddChild(ai::Obj*);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual bool CanChildBeAdded(m3d::Class*) const;
        virtual ai::WanderersManagerPrototypeInfo const* GetPrototypeInfo() const;
        static m3d::Class* __fastcall GetBaseClass();

    protected:
        virtual void _InternalPostLoad();
        virtual ~WanderersManager();

    private:
        void _SaveWayPointsToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        void _SaveCaravansInfoToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        void _OnTargetReached(ai::Event const&);
        void _LoadVagabondsInfoFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void _LoadPrecisePathsFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void _OnObjectDie(ai::Event const&);
        void _OnLostGuardsNeedDirection(ai::Event const&);
        void _SpawnVagabondTeam(int);
        void _LoadCaravansInfoFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void _SavePrecisePathsToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        int _GetWayPointIndex(CStr const&) const;
        std::vector<CVector2, std::allocator<CVector2> > _GetPrecisePath(int, int) const;
        void _SendTeamToPrecisePathPoint(ai::Team*, WandererState const&) const;
        void _SaveVagabondInfoToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual m3d::Object* Clone();
        void _SpawnCaravanTeam(int);
        void _SpawnWanderer();
        void _LoadWayPointsFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void _OnObjectEntersLocation(ai::Event const&);
        unsigned int _GetWanderersCount() const;
        static m3d::Object* __fastcall CreateObject();
        void _SaveWanderersStatesToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        void _LoadWanderersStatesFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void _SetStatePrecisePath(WandererState&, int, int);

    private:
        std::map<CStr, int> m_wayPointIndexMap;
        std::vector<WayPoint> m_wayPoints;
        std::map<std::pair<int, int>, std::vector<CVector2>> m_precisePaths;
        std::vector<CaravanInfo> m_caravanInfos;
        std::vector<int> m_vagabondPrototypeIds;
        int m_maxTeamCount;
        float m_rebornTimeout;
        std::map<int, WandererState> m_wandererStates;
        std::vector<int> m_waitingVagabonds;
        std::vector<int> m_guardVehiclesIds;
        std::vector<int> m_transitionIndices;
        float m_timeBeforeReborn;
    };
}
