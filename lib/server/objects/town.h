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
        /* 0x00a0 */ CStr m_musicName;
        /* 0x00ac */ CStr m_gateModelName;
        /* 0x00b8 */ float m_gateOpeningTime;
        /* 0x00bc */ float m_gateClosingTime;
        /* 0x00c0 */ int m_maxDefenders;
        /* 0x00c4 */ int m_GunGeneratorPrototypeId;
        /* 0x00c8 */ unsigned int m_DesiredGunsInWorkshop;
        /* 0x00cc */ int m_GunAffixGeneratorPrototypeId;
        /* 0x00d0 */ unsigned int m_GunAffixesCount;
        /* 0x00d4 */ int m_CabinsAndBasketsAffixGeneratorPrototypeId;
        /* 0x00d8 */ unsigned int m_CabinsAndBasketsAffixesCount;
        /* 0x00dc */ unsigned int m_numCollisionLayersBelowVehicle;
        /* 0x00e0 */ retruxx::vector<ai::Article, retruxx::allocator<ai::Article> > m_Articles;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        /* 0x00f0 */ retruxx::map<int, RandomCoeffWithDispersion, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, RandomCoeffWithDispersion> > > m_resourceIdToRandomCoeffMap;

    private:
        /* 0x00fc */ CStr m_gunGeneratorPrototypeName;
        /* 0x0108 */ CStr m_gunAffixGeneratorPrototypeName;
        /* 0x0114 */ CStr m_cabinsAndBasketsAffixGeneratorPrototypeName;
        void _LoadFromXmlResourceIdToRandomCoeffMap(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    }; /* size: 0x0120 */

    static_assert(sizeof(TownPrototypeInfo) == 0x0120);

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
        virtual  ~Town() override /* 0x00 */;

    private:
        Town(const ai::TownPrototypeInfo& prototypeInfo);
        Town(const ai::Town&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classTown;
        virtual const ai::TownPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

    public:
        virtual int OnEvent(const ai::Event& evn) override /* 0x00 */;
        static void __fastcall Registration();
        virtual void CreateChildren() override /* 0x00 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        ai::Team* SpawnCaravanToLocation(ai::Location* location);
        ai::Team* SpawnCaravanToLocation(const char* locationName);
        void GetBuildingTypes(retruxx::set<enum ai::BuildingType, retruxx::less<enum ai::BuildingType>, retruxx::allocator<enum ai::BuildingType> >& buildingTypeSet) const;
        retruxx::vector<ai::Building*, retruxx::allocator<ai::Building*> > GetBuildingByType(int) const;
        retruxx::vector<ai::Building*, retruxx::allocator<ai::Building*> > GetBuildingByType(ai::BuildingType buildingType) const;
        const retruxx::vector<ai::Building*, retruxx::allocator<ai::Building*> >& GetAllBuildings() const;
        retruxx::vector<int, retruxx::allocator<int> > GetDynamicQuestIds() const;
        virtual void RelinkSceneGraphNode() override /* 0x00 */;
        void SendVehicleOff(ai::Vehicle* pVehicle, bool bQuick);
        void SendVehicleIn(ai::Vehicle* pVehicle);
        void PrepareVehicleForPath(ai::Vehicle* pVehicle, const retruxx::vector<CVector2, retruxx::allocator<CVector2> >& points, bool delayed);
        void SetOpenGateToPlayer(bool bOpen);
        bool GetOpenGateToPlayer() const;
        ai::Workshop* GetShop();
        ai::Workshop* GetWorkshop();
        ai::Workshop* GetWorkshopByObject(const ai::Obj* obj);
        ai::Workshop* GetWorkshopByPrototypeId(int pId);

        enum TownPath;

    public:
        void GetPath(ai::Town::TownPath path, retruxx::vector<CVector2, retruxx::allocator<CVector2> >& vehiclePoints, retruxx::vector<CVector, retruxx::allocator<CVector> >& cameraPoints) const;
        void SetPath(ai::Town::TownPath path, const retruxx::vector<CVector2, retruxx::allocator<CVector2> >& vehiclePoints, const retruxx::vector<CVector, retruxx::allocator<CVector> >& cameraPoints);
        const CStr& GetCaravanLocationName() const;
        void GenerateNewDynamicQuestIfNeeded();
        void SetRuined(bool bRuined);
        bool IsRuined() const;
        float GetObjResourceCoeff(const ai::Obj* obj) const;

    protected:
        virtual void _InternalPostLoad() override /* 0x00 */;
        virtual void _InternalCreateVisualPart() override /* 0x00 */;

    private:
        /* 0x0224 */ bool m_shouldInitializeWorkshops;
        /* 0x0225 */ char Padding_45[3];
        /* 0x0228 */ retruxx::map<int, float, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, float> > > m_resourceIdToCoeff;
        /* 0x0234 */ retruxx::vector<ai::Building*, retruxx::allocator<ai::Building*> > m_buildings;
        /* 0x0244 */ retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> > m_targetClasses;
        /* 0x0250 */ ai::Town::GateState m_gateState;
        /* 0x0254 */ m3d::SgNode* m_gateNode;
        /* 0x0258 */ ai::NumericBoundedBelow<float> m_gateTime;
        /* 0x02d0 */ int m_maxDefenders;
        /* 0x02d4 */ ai::CinematicPath m_entryPath;
        /* 0x02f4 */ ai::CinematicPath m_exitPath;
        /* 0x0314 */ CVector m_PointOfViewInInterface;
        /* 0x0320 */ int m_playerEnteringTownCount;
        /* 0x0324 */ int m_PlayerPathIndex;
        /* 0x0328 */ bool m_QuestsGenerated;
        /* 0x0329 */ bool m_VehicleShouldBeMoved;
        /* 0x032a */ char Padding_46[2];
        /* 0x032c */ CVector m_NewPosForVehicle;
        /* 0x0338 */ CVector m_NewDirForVehicle;
        /* 0x0344 */ ai::Vehicle* m_VehicleToBeMoved;
        /* 0x0348 */ float m_timeFromLastEnterTown;
        /* 0x034c */ CameraModes m_OldCameraMode;
        /* 0x0350 */ bool m_bOpenGateToPlayer;
        /* 0x0351 */ char Padding_47[3];
        /* 0x0354 */ CStr m_caravanLocationName;
        /* 0x0360 */ bool m_bRuined;
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
    }; /* size: 0x0364 */

    static_assert(sizeof(Town) == 0x0364);
}
