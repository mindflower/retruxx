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
    /* 0x0220 */ m3d::CVar m_cvDefaultRadarScanRadius;
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
        /* 0x0000 */ m3d::rend::TexHandle m_texture;
        /* 0x0004 */ PointBase<float> m_coords;
        /* 0x000c */ float m_angle;
        /* 0x0010 */ PointBase<float> m_size;
    }; /* size: 0x0018 */

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
        /* 0x0000 */ NavPoint::NavPointType m_navPointType;
        /* 0x0004 */ ElectronicDigitalWnd* m_wndDigital;
        /* 0x0008 */ int m_navPointId;
    }; /* size: 0x000c */

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
        /* 0x0000 */ CStr m_icoNames[24];
        /* 0x0120 */ PointBase<float> m_icoSizeVehicleSmall;
        /* 0x0128 */ PointBase<float> m_icoSizeVehicleLarge;
        /* 0x0130 */ PointBase<float> m_icoSizeTurretSmall;
        /* 0x0138 */ PointBase<float> m_icoSizeTurretLarge;
        /* 0x0140 */ PointBase<float> m_icoSizeMainQuestFar;
        /* 0x0148 */ PointBase<float> m_icoSizeMainQuestNear;
        /* 0x0150 */ PointBase<float> m_icoSizeUserQuestFar;
        /* 0x0158 */ PointBase<float> m_icoSizeUserQuestNear;
        /* 0x0160 */ PointBase<float> m_icoSizeUserLocationFar;
        /* 0x0168 */ PointBase<float> m_icoSizeUserLocationNear;
        /* 0x0170 */ PointBase<float> m_icoSizeWorldside;
        /* 0x0178 */ PointBase<float> m_rotationCenter;
        /* 0x0180 */ float m_innerRadius;
        /* 0x0184 */ float m_outerRadius;
        /* 0x0188 */ CStr m_cameraSightTexName;
        /* 0x0194 */ PointBase<float> m_cameraSightSz;
        /* 0x019c */ CStr m_playerVehicleTexName;
        /* 0x01a8 */ PointBase<float> m_playerVehicleSz;
        /* 0x01b0 */ CStr m_highlightTexName;
        /* 0x01bc */ BoundsBase<float> m_highlightB;
        /* 0x01cc */ CStr m_wndsDistancesNames[3];
        AuxInfo(RadarWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x01f0 */

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
    virtual ~RadarWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(RadarWnd);
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual void OnPaintOverChildren(m3d::ui::DrawInfo const& di) override /* 0x8c */;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x110 */;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x114 */;
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
    /* 0x024c */ RadarItemMap m_vehicleItems;
    /* 0x0258 */ RadarItemMap m_turretItems;
    /* 0x0264 */ RadarItemMap m_navPointItems;
    /* 0x0270 */ RadarItemMap m_worldsideItems;
    /* 0x027c */ DistanceVector m_distances;
    /* 0x028c */ RadarWnd::AuxInfo m_aif;
    /* 0x047c */ float m_scanRadius;
    /* 0x0480 */ m3d::rend::TexHandle m_icoTextures[24];
    /* 0x04e0 */ bool m_bNavPointsAllowed;
    /* 0x04e1 */ bool m_bVehiclesAllowed;
    /* 0x04e2 */ bool m_bTurretsAllowed;
    /* 0x04e3 */ bool m_bDistancesAllowed;
    /* 0x04e4 */ bool m_bNavPointsEnabled;
    /* 0x04e5 */ bool m_bVehiclesEnabled;
    /* 0x04e6 */ bool m_bTurretsEnabled;
    /* 0x04e7 */ bool m_bDistancesEnabled;
    /* 0x04e8 */ m3d::rend::TexHandle m_cameraSightTex;
    /* 0x04ec */ m3d::rend::TexHandle m_playerVehicleTex;
    /* 0x04f0 */ m3d::rend::TexHandle m_highlightTex;
    /* 0x04f4 */ int m_test;
}; /* size: 0x04f8 */
