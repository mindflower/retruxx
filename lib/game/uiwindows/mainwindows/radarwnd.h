#pragma once
#include <core/console/cvar.h>
#include <game/uimisc/navpoint.h>
#include <ui/ui.h>

class ElectronicDigitalWnd;

namespace ai
{
    class Obj;
    class Vehicle;
    class StaticAutoGun;
}

class RadarWnd :  public m3d::ui::Wnd
{
public:
    class NpDistance
    {
    public:
        ~NpDistance();
        int GetNavPointId() const;
        NpDistance(ElectronicDigitalWnd*, NavPoint::NavPointType);
        int SetNavPointId(int);
        int SetDistance(float);
        NavPoint::NavPointType GetNavPointType() const;

    private:
        void Show(bool);
        bool IsValid() const;
        NavPoint::NavPointType m_navPointType;
        ElectronicDigitalWnd* m_wndDigital;
        int m_navPointId;
    };

    class RadarItem
    {
    public:
        PointBase<float> const& GetCoords() const;
        RadarItem(m3d::rend::TexHandle, PointBase<float> const&, PointBase<float> const&, float);
        PointBase<float> const& GetSize() const;
        void Draw(m3d::ui::DrawInfo const&) const;
        void SetCoords(PointBase<float> const&);
        void SetSize(PointBase<float> const&);
        void SetAngle(float);
        void SetTexture(m3d::rend::TexHandle);
        float GetAngle() const;
        ~RadarItem();
        BoundsBase<float> GetBounds() const;
        m3d::rend::TexHandle GetTexture() const;

    private:
        m3d::rend::TexHandle m_texture;
        PointBase<float> m_coords;
        float m_angle;
        PointBase<float> m_size;
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
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
    };

    enum Worldside
    {
        WORLDSIDE_NORD = 0x0,
        WORLDSIDE_SOUTH = 0x1,
        WORLDSIDE_NUM_WORLDSIDES = 0x2,
    };

    enum ItemType
    {
        ITEMTYPE_OBJECT = 0,
        ITEMTYPE_NAVPOINT = 1,
        ITEMTYPE_WORLDSIDE = 2,
    };

public:
    int UpdateWorldsidesOnNewFrame();
    void ClearTurretItems();
    float GetIcoRotationAngle(PointBase<float> const &) const ;
    int UpdateNavPointsOnNewFrame();
    int UpdateWorldside(Worldside);
    void DrawCameraSight(m3d::ui::DrawInfo const &) const ;
    int AddWorldside(Worldside);
    void DrawBg(m3d::ui::DrawInfo const &);
    void ClearWorldsideItems();
    void DrawNavPoints(m3d::ui::DrawInfo const &) const ;
    void DrawPlayerVehicle(m3d::ui::DrawInfo const &) const ;
    virtual int GameDataClear(bool);
    void AllowVehicles(bool);
    void CreateWorldSides();
    void SetTest(int);
    int RemoveNavPoint(int);
    CVector FixCoord(CVector const &) const ;
    void DrawWorldsides(m3d::ui::DrawInfo const &) const ;
    virtual int GameDataUpdate(void *,int);
    int UpdateDistance(NavPoint::NavPointType);
    void ClearNavPointItems();
    PointBase<float> WorldToRadarCoords(CVector const &,ItemType) const ;
    void GetIcoForVehicle(ai::Vehicle const *,m3d::rend::TexHandle &,PointBase<float> &) const ;
    void OnStartLevel();
    int OnDeleteNavPoint(void *);
    virtual int GameDataLoad(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
    int RemoveTurret(int);
    int RemoveVehicle(int);
    void AllowDistances(bool);
    void GetIcoForNavPoint(NavPoint::NavPointType,CVector const &,m3d::rend::TexHandle &,PointBase<float> &) const ;
    int AddTurret(int);
    virtual m3d::Class * GetClass() const ;
    int AddVehicle(int);
    int AddNavPoint(int);
    void AllowTurrets(bool);
    void CreateNavPoints();
    int RemoveDistance(int);
    void AllowNavPoints(bool);
    int UpdateNavPoint(int);
    float CalculateDistanceToNavPoint(int) const ;
    m3d::rend::TexHandle GetIcoForWorldside(enum Worldside) const ;
    virtual m3d::Object * Clone();
    virtual int GameDataSave(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
    void Reset();
    virtual ~RadarWnd();
    int AddDistance(int);
    float GetDefaultScanRadius() const ;
    bool IsPositionOutsideScanRadius(CVector const &) const ;
    virtual void OnPaintOverChildren(m3d::ui::DrawInfo const &);
    int RemoveWorldside(Worldside);
    int OnAddNavPoint(void *);
    int UpdateDistancesOnNewFrame();
    void GetIcoForObject(ai::Obj const *,m3d::rend::TexHandle &,PointBase<float> &) const ;
    void GetIcoForTurret(ai::StaticAutoGun const *,m3d::rend::TexHandle &,PointBase<float> &) const ;
    int UpdateOnNewFrame();
    static m3d::Object * CreateObject();
    int UpdateTurretsOnNewFrame();
    void ClearVehicleItems();
    void SetScanRadius(float);
    virtual int GameDataSetup();
    CVector GetWorldsideCoords(Worldside) const ;
    int UpdateTurret(int);
    int UpdateVehicle(int);
    void DrawHighlight(m3d::ui::DrawInfo const &) const ;
    void DrawTurrets(m3d::ui::DrawInfo const &) const ;
    float GetScanRadius() const ;
    void DrawVehicles(m3d::ui::DrawInfo const &) const ;
    int UpdateVehiclesOnNewFrame();
    static m3d::Class * GetBaseClass();

protected:
    RadarWnd();
    RadarWnd(RadarWnd const &);

public:
    RT_CLASS_DECLARE(RadarWnd);

private:
    m3d::CVar m_cvDefaultRadarScanRadius;
    std::map<int,std::vector<RadarWnd::RadarItem *>> m_vehicleItems;
    std::map<int,std::vector<RadarWnd::RadarItem *>> m_turretItems;
    std::map<int,std::vector<RadarWnd::RadarItem *>> m_navPointItems;
    std::map<int,std::vector<RadarWnd::RadarItem *>> m_worldsideItems;
    std::vector<RadarWnd::NpDistance> m_distances;
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
