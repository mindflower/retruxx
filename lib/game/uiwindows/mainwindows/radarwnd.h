#pragma once
#include <core/console/cvar.h>
#include <game/uimisc/navpoint.h>
#include <map>
#include <ui/ui.h>
#include <vector>

class ElectronicDigitalWnd;

namespace ai
{
    class Obj;
    class Vehicle;
    class StaticAutoGun;
}  // namespace ai

class RadarWnd : public m3d::ui::Wnd
{
public:
    m3d::CVar m_cvDefaultRadarScanRadius;
    void AllowNavPoints(bool bAllow);
    void AllowVehicles(bool bAllow);
    void AllowTurrets(bool bAllow);
    void AllowDistances(bool bAllow);
    void SetScanRadius(float scanRadius);
    void SetTest(int test);

    struct RadarItem
    {
        RadarItem(RadarItem const&);
        RadarItem(m3d::rend::TexHandle tex, PointBase<float> const& size, PointBase<float> const& coords, float angle);
        ~RadarItem();
        void Draw(m3d::ui::DrawInfo const& di) const;
        void SetTexture(m3d::rend::TexHandle texture);
        void SetCoords(PointBase<float> const& coords);
        void SetAngle(float angle);
        void SetSize(PointBase<float> const& size);
        m3d::rend::TexHandle GetTexture() const;
        PointBase<float> const& GetCoords() const;
        float GetAngle() const;
        PointBase<float> const& GetSize() const;
        BoundsBase<float> GetBounds() const;
        m3d::rend::TexHandle m_texture;
        PointBase<float> m_coords;
        float m_angle;
        PointBase<float> m_size;
    };

    struct NpDistance
    {
        NpDistance(ElectronicDigitalWnd* wndDigital, NavPoint::NavPointType npType);
        ~NpDistance();
        NavPoint::NavPointType GetNavPointType() const;
        int SetNavPointId(int npId);
        int GetNavPointId() const;
        int SetDistance(float distance);
        void Show(bool bShow);
        bool IsValid() const;
        NavPoint::NavPointType m_navPointType;
        ElectronicDigitalWnd* m_wndDigital;
        int m_navPointId;
    };

    enum RadarIco
    {
        RADARICO_VEHICLE_OWN_SMALL = 0,
        RADARICO_VEHICLE_OWN_LARGE = 1,
        RADARICO_VEHICLE_ENEMY_SMALL = 2,
        RADARICO_VEHICLE_ENEMY_LARGE = 3,
        RADARICO_VEHICLE_NEUTRAL_SMALL = 4,
        RADARICO_VEHICLE_NEUTRAL_LARGE = 5,
        RADARICO_VEHICLE_ALLY_SMALL = 6,
        RADARICO_VEHICLE_ALLY_LARGE = 7,
        RADARICO_TURRET_OWN_SMALL = 8,
        RADARICO_TURRET_OWN_LARGE = 9,
        RADARICO_TURRET_ENEMY_SMALL = 10,
        RADARICO_TURRET_ENEMY_LARGE = 11,
        RADARICO_TURRET_NEUTRAL_SMALL = 12,
        RADARICO_TURRET_NEUTRAL_LARGE = 13,
        RADARICO_TURRET_ALLY_SMALL = 14,
        RADARICO_TURRET_ALLY_LARGE = 15,
        RADARICO_QUEST_MAIN_FAR = 16,
        RADARICO_QUEST_MAIN_NEAR = 17,
        RADARICO_QUEST_USER_FAR = 18,
        RADARICO_QUEST_USER_NEAR = 19,
        RADARICO_LOCATION_USER_FAR = 20,
        RADARICO_LOCATION_USER_NEAR = 21,
        RADARICO_WORLDSIDE_NORD = 22,
        RADARICO_WORLDSIDE_SOUTH = 23,
        RADARICO_NUM_RADARICOS = 24,
    };

    enum Worldside
    {
        WORLDSIDE_NORD = 0,
        WORLDSIDE_SOUTH = 1,
        WORLDSIDE_NUM_WORLDSIDES = 2,
    };

    enum ItemType
    {
        ITEMTYPE_OBJECT = 0,
        ITEMTYPE_NAVPOINT = 1,
        ITEMTYPE_WORLDSIDE = 2,
    };

    struct AuxInfo
    {
        CStr m_icoNames[24];
        PointBase<float> m_icoSizeVehicleSmall;
        PointBase<float> m_icoSizeVehicleLarge;
        PointBase<float> m_icoSizeTurretSmall;
        PointBase<float> m_icoSizeTurretLarge;
        PointBase<float> m_icoSizeMainQuestFar;
        PointBase<float> m_icoSizeMainQuestNear;
        PointBase<float> m_icoSizeUserQuestFar;
        PointBase<float> m_icoSizeUserQuestNear;
        PointBase<float> m_icoSizeUserLocationFar;
        PointBase<float> m_icoSizeUserLocationNear;
        PointBase<float> m_icoSizeWorldside;
        PointBase<float> m_rotationCenter;
        float m_innerRadius;
        float m_outerRadius;
        CStr m_cameraSightTexName;
        PointBase<float> m_cameraSightSz;
        CStr m_playerVehicleTexName;
        PointBase<float> m_playerVehicleSz;
        CStr m_highlightTexName;
        BoundsBase<float> m_highlightB;
        CStr m_wndsDistancesNames[3];
        AuxInfo(RadarWnd::AuxInfo const&);
        AuxInfo();
    };

    using RadarItemVector = std::vector<RadarWnd::RadarItem*, std::allocator<RadarWnd::RadarItem*>>;
    using RadarItemMap = std::map<
        int,
        std::vector<RadarWnd::RadarItem*, std::allocator<RadarWnd::RadarItem*>>,
        std::less<int>,
        std::allocator<std::pair<int const, std::vector<RadarWnd::RadarItem*, std::allocator<RadarWnd::RadarItem*>>>>>;
    using RadarItemPair = std::pair<int, std::vector<RadarWnd::RadarItem*, std::allocator<RadarWnd::RadarItem*>>>;
    using DistanceVector = std::vector<RadarWnd::NpDistance, std::allocator<RadarWnd::NpDistance>>;

protected:
    RadarWnd();
    RadarWnd(RadarWnd const& rhs);

public:
    virtual ~RadarWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(RadarWnd);
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual void OnPaintOverChildren(m3d::ui::DrawInfo const& di) override;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    int UpdateOnNewFrame();
    int UpdateObjectsOnNewFrame(RadarItemMap& items, m3d::Class const* cls);
    int UpdateVehiclesOnNewFrame();
    int UpdateTurretsOnNewFrame();
    int UpdateNavPointsOnNewFrame();
    int UpdateWorldsidesOnNewFrame();
    int UpdateDistancesOnNewFrame();
    int AddObject(int objId, RadarItemMap& items);
    int RemoveObject(int objId, RadarItemMap& items);
    // NOTE: declared in the PDB but no code was emitted for it in the shipped
    // binary and nothing references it, so it is left without a definition.
    int RemoveObject(int);
    int AddVehicle(int vehicleId);
    int RemoveVehicle(int vehicleId);
    int AddTurret(int turretId);
    int RemoveTurret(int turretId);
    int AddNavPoint(int npId);
    int RemoveNavPoint(int npId);
    int RemoveItem(RadarItemMap& itemMap, int itemId);
    int AddWorldside(RadarWnd::Worldside side);
    int RemoveWorldside(RadarWnd::Worldside side);
    int AddDistance(int npId);
    int RemoveDistance(int npId);
    int UpdateObject(int objId, RadarItemMap const& items);
    int UpdateVehicle(int vehicleId);
    int UpdateTurret(int turretId);
    int UpdateNavPoint(int npId);
    int UpdateWorldside(RadarWnd::Worldside side);
    int UpdateDistance(NavPoint::NavPointType npType);
    void GetIcoForObject(ai::Obj const* o, m3d::rend::TexHandle& tex, PointBase<float>& icoSize) const;
    void GetIcoForVehicle(ai::Vehicle const* v, m3d::rend::TexHandle& tex, PointBase<float>& icoSize) const;
    void GetIcoForTurret(ai::StaticAutoGun const* t, m3d::rend::TexHandle& tex, PointBase<float>& icoSize) const;
    void GetIcoForNavPoint(
        NavPoint::NavPointType npType,
        CVector const& worldCoord,
        m3d::rend::TexHandle& tex,
        PointBase<float>& icoSize) const;
    m3d::rend::TexHandle GetIcoForWorldside(RadarWnd::Worldside side) const;
    void DrawVehicles(m3d::ui::DrawInfo const& di) const;
    void DrawTurrets(m3d::ui::DrawInfo const& di) const;
    void DrawNavPoints(m3d::ui::DrawInfo const& di) const;
    void DrawWorldsides(m3d::ui::DrawInfo const& di) const;
    void DrawCameraSight(m3d::ui::DrawInfo const& di) const;
    void DrawPlayerVehicle(m3d::ui::DrawInfo const& di) const;
    void DrawHighlight(m3d::ui::DrawInfo const& di) const;
    void DrawItems(m3d::ui::DrawInfo const& di, RadarItemMap const& items) const;
    void DrawBg(m3d::ui::DrawInfo const& di);
    bool IsPositionOutsideScanRadius(CVector const& worldPos) const;
    float GetScanRadius() const;
    float GetDefaultScanRadius() const;
    PointBase<float> WorldToRadarCoords(CVector const& wrldCoords, RadarWnd::ItemType type) const;
    float GetIcoRotationAngle(PointBase<float> const& wndCoords) const;
    CVector GetWorldsideCoords(RadarWnd::Worldside side) const;
    void OnStartLevel();
    void ClearVehicleItems();
    void ClearTurretItems();
    void ClearNavPointItems();
    void ClearWorldsideItems();
    void ClearRadarItemMap(RadarItemMap& itemMap);
    int OnAddNavPoint(void* data);
    int OnDeleteNavPoint(void* data);
    void CreateWorldSides();
    void CreateNavPoints();
    float CalculateDistanceToNavPoint(int npId) const;
    void Reset();
    CVector FixCoord(CVector const& originalNpCoord) const;
    RadarItemMap m_vehicleItems;
    RadarItemMap m_turretItems;
    RadarItemMap m_navPointItems;
    RadarItemMap m_worldsideItems;
    DistanceVector m_distances;
    RadarWnd::AuxInfo m_aif;
    float m_scanRadius;
    m3d::rend::TexHandle m_icoTextures[24];
    bool m_bNavPointsAllowed;
    bool m_bVehiclesAllowed;
    bool m_bTurretsAllowed;
    bool m_bDistancesAllowed;
    bool m_bNavPointsEnabled;
    bool m_bVehiclesEnabled;
    bool m_bTurretsEnabled;
    bool m_bDistancesEnabled;
    m3d::rend::TexHandle m_cameraSightTex;
    m3d::rend::TexHandle m_playerVehicleTex;
    m3d::rend::TexHandle m_highlightTex;
    int m_test;
};
