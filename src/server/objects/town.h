#pragma once
#include "building.h"
#include "settlement.h"
#include <math/camera.h>
#include <server/cinematicpath.h>
#include <server/components/numericboundedbelow.h>

class RandomCoeffWithDispersion;

namespace ai
{
    class Article;
    class Workshop;
    class DynamicQuest;
    class Building;

    class TownPrototypeInfo : public SettlementPrototypeInfo
    {
    public:
        TownPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual void PostLoad();
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        void _LoadFromXmlResourceIdToRandomCoeffMap(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        CStr m_musicName;
        CStr m_gateModelName;
        float m_gateOpeningTime;
        float m_gateClosingTime;
        int m_maxDefenders;
        int m_GunGeneratorPrototypeId;
        unsigned int m_DesiredGunsInWorkshop;
        int m_GunAffixGeneratorPrototypeId;
        unsigned int m_GunAffixesCount;
        int m_CabinsAndBasketsAffixGeneratorPrototypeId;
        unsigned int m_CabinsAndBasketsAffixesCount;
        unsigned int m_numCollisionLayersBelowVehicle;
        std::vector<ai::Article> m_Articles;
        std::map<int, RandomCoeffWithDispersion> m_resourceIdToRandomCoeffMap;
        CStr m_gunGeneratorPrototypeName;
        CStr m_gunAffixGeneratorPrototypeName;
        CStr m_cabinsAndBasketsAffixGeneratorPrototypeName;
    };

    class Town :  public Settlement
    {
    public:
        enum GateState
        {
            GATE_CLOSED = 0x0,
            GATE_OPENING = 0x1,
            GATE_OPEN = 0x2,
            GATE_CLOSING = 0x3,
            GATE_NUM_STATES = 0x4,
        };

        enum TownPath
        {
            TP_ENTRY_PATH = 0x0,
            TP_EXIT_PATH = 0x1,
            TP_NUM_PATH = 0x2,
        };

    public:
        void PrepareVehicleForPath(Vehicle *,std::vector<CVector2,std::allocator<CVector2> > const &,bool);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual int GetPropertyId(char const *) const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        virtual void AddChild(Obj *);
        void SetRuined(bool);
        Town(TownPrototypeInfo const &);
        void SendVehicleOff(Vehicle *,bool);
        Team * SpawnCaravanToLocation(char const *);
        Team * SpawnCaravanToLocation(Location *);
        virtual CStr GetPropertyName(int) const ;
        Workshop * GetShop();
        virtual TownPrototypeInfo const * GetPrototypeInfo() const ;
        float GetObjResourceCoeff(Obj const *) const ;
        std::vector<Building *> const & GetAllBuildings() const ;
        bool IsRuined() const ;
        static m3d::Class * GetBaseClass();
        virtual void CreateChildren();
        std::vector<Building *> GetBuildingByType(BuildingType) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        void GetBuildingTypes(std::set<BuildingType> &) const ;
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void SetPath(TownPath,std::vector<CVector2> const &,std::vector<CVector> const &);
        Workshop * GetWorkshopByObject(Obj const *);
        void SendVehicleIn(Vehicle *);
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        void GetPath(TownPath,std::vector<CVector2> &,std::vector<CVector> &) const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void SetOpenGateToPlayer(bool);
        virtual void GetPropertiesIDs(std::set<int> &) const ;
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void RelinkSceneGraphNode();
        virtual int OnEvent(Event const &);
        std::vector<int,std::allocator<int> > GetDynamicQuestIds() const ;
        Workshop * GetWorkshop();
        bool GetOpenGateToPlayer() const ;
        virtual void GetPropertiesNames(std::set<CStr> &) const ;
        virtual void Update(float,unsigned int);
        static void __fastcall Registration();
        virtual m3d::Class * GetClass() const ;
        void GenerateNewDynamicQuestIfNeeded();
        Workshop * GetWorkshopByPrototypeId(int);

    protected:
        virtual void _InternalCreateVisualPart();
        virtual ~Town();
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual void _InternalPostLoad();
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;

    private:
        void _OnPlayerVehicleHorn(Event const &);
        virtual m3d::Object * Clone();
        Team * _GetDefendingTeam();
        static m3d::Object * CreateObject();
        void _OnObjectEntersLocation(Event const &);
        void _InitializeWorkshops();
        void _OnTargetReached(Event const &);
        void _OnInCinematic(Event const &);
        void _OpenGates();
        void _StartCinematic(Vehicle *,std::vector<CVector,std::allocator<CVector> > const &);
        void _OnSkipCinematic(Event const &);
        void _CloseGates();
        void _OnObjectLeavesLocation(Event const &);
        void _SynchronizeGatesState();
        DynamicQuest * _GenerateDynamicQuest();

    public:
        RT_CLASS_DECLARE(Town);

    private:
        bool m_shouldInitializeWorkshops;
        std::map<int,float> m_resourceIdToCoeff;
        std::vector<Building *> m_buildings;
        std::set<m3d::Class *> m_targetClasses;
        GateState m_gateState;
        m3d::SgNode *m_gateNode;
        NumericBoundedBelow<float> m_gateTime;
        int m_maxDefenders;
        CinematicPath m_entryPath;
        CinematicPath m_exitPath;
        CVector m_PointOfViewInInterface;
        int m_playerEnteringTownCount;
        int m_PlayerPathIndex;
        bool m_QuestsGenerated;
        bool m_VehicleShouldBeMoved;
        CVector m_NewPosForVehicle;
        CVector m_NewDirForVehicle;
        Vehicle *m_VehicleToBeMoved;
        float m_timeFromLastEnterTown;
        CameraModes m_OldCameraMode;
        bool m_bOpenGateToPlayer;
        CStr m_caravanLocationName;
        bool m_bRuined;
    };
}
