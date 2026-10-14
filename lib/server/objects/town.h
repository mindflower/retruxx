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

    class TownPrototypeInfo : public ai::SettlementPrototypeInfo
    {
    public:
        TownPrototypeInfo();
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
        retruxx::vector<ai::Article, retruxx::allocator<ai::Article> > m_Articles;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;
        retruxx::map<int, RandomCoeffWithDispersion, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, RandomCoeffWithDispersion> > > m_resourceIdToRandomCoeffMap;

    private:
        CStr m_gunGeneratorPrototypeName;
        CStr m_gunAffixGeneratorPrototypeName;
        CStr m_cabinsAndBasketsAffixGeneratorPrototypeName;
        void _LoadFromXmlResourceIdToRandomCoeffMap(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    };

    class Town : public ai::Settlement
    {
        friend class TownPrototypeInfo;
    public:
        enum GateState
        {
          GATE_CLOSED = 0,
          GATE_OPENING = 1,
          GATE_OPEN = 2,
          GATE_CLOSING = 3,
          GATE_NUM_STATES = 4,
        };

    protected:
        virtual  ~Town() override;

    private:
        Town(const ai::TownPrototypeInfo& prototypeInfo);
        Town(const ai::Town&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classTown;
        virtual const ai::TownPrototypeInfo* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override;
        virtual int GetPropertyId(const char* PropertyName) const override;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual int OnEvent(const ai::Event& evn) override;
        static void __fastcall Registration();
        virtual void CreateChildren() override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        ai::Team* SpawnCaravanToLocation(ai::Location* location);
        ai::Team* SpawnCaravanToLocation(const char* locationName);
        void GetBuildingTypes(retruxx::set<enum ai::BuildingType, retruxx::less<enum ai::BuildingType>, retruxx::allocator<enum ai::BuildingType> >& buildingTypeSet) const;
        retruxx::vector<ai::Building*, retruxx::allocator<ai::Building*> > GetBuildingByType(int) const;
        retruxx::vector<ai::Building*, retruxx::allocator<ai::Building*> > GetBuildingByType(ai::BuildingType buildingType) const;
        const retruxx::vector<ai::Building*, retruxx::allocator<ai::Building*> >& GetAllBuildings() const;
        retruxx::vector<int, retruxx::allocator<int> > GetDynamicQuestIds() const;
        virtual void RelinkSceneGraphNode() override;
        void SendVehicleOff(ai::Vehicle* pVehicle, bool bQuick);
        void SendVehicleIn(ai::Vehicle* pVehicle);
        void PrepareVehicleForPath(ai::Vehicle* pVehicle, const retruxx::vector<CVector2, retruxx::allocator<CVector2> >& points, bool delayed);
        void SetOpenGateToPlayer(bool bOpen);
        bool GetOpenGateToPlayer() const;
        ai::Workshop* GetShop();
        ai::Workshop* GetWorkshop();
        ai::Workshop* GetWorkshopByObject(const ai::Obj* obj);
        ai::Workshop* GetWorkshopByPrototypeId(int pId);

        enum TownPath
        {
            TP_ENTRY_PATH = 0,
            TP_EXIT_PATH = 1,
            TP_NUM_PATH = 2,
        };

    public:
        void GetPath(ai::Town::TownPath path, retruxx::vector<CVector2, retruxx::allocator<CVector2> >& vehiclePoints, retruxx::vector<CVector, retruxx::allocator<CVector> >& cameraPoints) const;
        void SetPath(ai::Town::TownPath path, const retruxx::vector<CVector2, retruxx::allocator<CVector2> >& vehiclePoints, const retruxx::vector<CVector, retruxx::allocator<CVector> >& cameraPoints);
        const CStr& GetCaravanLocationName() const;
        void GenerateNewDynamicQuestIfNeeded();
        void SetRuined(bool bRuined);
        bool IsRuined() const;
        float GetObjResourceCoeff(const ai::Obj* obj) const;

    protected:
        virtual void _InternalPostLoad() override;
        virtual void _InternalCreateVisualPart() override;

    private:
        bool m_shouldInitializeWorkshops;
        retruxx::map<int, float, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, float> > > m_resourceIdToCoeff;
        retruxx::vector<ai::Building*, retruxx::allocator<ai::Building*> > m_buildings;
        retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> > m_targetClasses;
        ai::Town::GateState m_gateState;
        m3d::SgNode* m_gateNode;
        ai::NumericBoundedBelow<float> m_gateTime;
        int m_maxDefenders;
        ai::CinematicPath m_entryPath;
        ai::CinematicPath m_exitPath;
        CVector m_PointOfViewInInterface;
        int m_playerEnteringTownCount;
        int m_PlayerPathIndex;
        bool m_QuestsGenerated;
        bool m_VehicleShouldBeMoved;
        CVector m_NewPosForVehicle;
        CVector m_NewDirForVehicle;
        ai::Vehicle* m_VehicleToBeMoved;
        float m_timeFromLastEnterTown;
        CameraModes m_OldCameraMode;
        bool m_bOpenGateToPlayer;
        CStr m_caravanLocationName;
        bool m_bRuined;
        ai::DynamicQuest* _GenerateDynamicQuest();
        ai::Team* _GetDefendingTeam();
        void _StartCinematic(ai::Vehicle* pVehicle, const retruxx::vector<CVector, retruxx::allocator<CVector> >& points);
        void _InitializeWorkshops();
        void _SynchronizeGatesState();
        void _CloseGates();
        void _OpenGates();
        void _OnObjectEntersLocation(const ai::Event& evn);
        void _OnObjectLeavesLocation(const ai::Event& evn);
        void _OnTargetReached(const ai::Event& evn);
        void _OnSkipCinematic(const ai::Event& evn);
        void _OnInCinematic(const ai::Event& evn);
        void _OnPlayerVehicleHorn(const ai::Event& evn);
        void _OnPlayerVehicleChanged(const ai::Event&);
    };
}
