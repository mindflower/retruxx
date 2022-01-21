#pragma once

namespace ai
{
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
        NpDistance(class ElectronicDigitalWnd*, enum NavPoint::NavPointType);
        int SetNavPointId(int);
        int SetDistance(float);
        enum NavPoint::NavPointType GetNavPointType() const;
    protected:
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
        class PointBase<float> const& GetCoords(void) const;
        RadarItem(class m3d::rend::TexHandle, class PointBase<float> const&, class PointBase<float> const&, float);
        class PointBase<float> const& GetSize(void) const;
        void Draw(struct m3d::ui::DrawInfo const&) const;
        void SetCoords(class PointBase<float> const&);
        void SetSize(class PointBase<float> const&);
        void SetAngle(float);
        void SetTexture(class m3d::rend::TexHandle);
        float GetAngle(void) const;
        ~RadarItem(void);
        struct BoundsBase<float> GetBounds(void) const;
        class m3d::rend::TexHandle GetTexture(void) const;
    protected:
    private:
        m3d::rend::TexHandle m_texture;
        PointBase<float> m_coords;
        float m_angle;
        PointBase<float> m_size;
    };

    class AuxInfo
    {
    public:
        AuxInfo(void);
    protected:
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

public:
    int UpdateWorldsidesOnNewFrame();
    void ClearTurretItems();
    float GetIcoRotationAngle(class PointBase<float> const &) const ;
    int UpdateNavPointsOnNewFrame();
    int UpdateWorldside(enum Worldside);
    void DrawCameraSight(struct m3d::ui::DrawInfo const &) const ;
    int AddWorldside(enum Worldside);
    void DrawBg(struct m3d::ui::DrawInfo const &);
    void ClearWorldsideItems();
    void DrawNavPoints(struct m3d::ui::DrawInfo const &) const ;
    void DrawPlayerVehicle(struct m3d::ui::DrawInfo const &) const ;
    virtual int GameDataClear(bool);
    void AllowVehicles(bool);
    void CreateWorldSides();
    void SetTest(int);
    int RemoveNavPoint(int);
    struct CVector FixCoord(struct CVector const &) const ;
    void DrawWorldsides(struct m3d::ui::DrawInfo const &) const ;
    virtual int GameDataUpdate(void *,int);
    int UpdateDistance(enum NavPoint::NavPointType);
    void ClearNavPointItems();
    class PointBase<float> WorldToRadarCoords(struct CVector const &,enum ItemType) const ;
    void GetIcoForVehicle(class ai::Vehicle const *,class m3d::rend::TexHandle &,class PointBase<float> &) const ;
    void OnStartLevel();
    int OnDeleteNavPoint(void *);
    virtual int GameDataLoad(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
    int RemoveTurret(int);
    int RemoveVehicle(int);
    void AllowDistances(bool);
    void GetIcoForNavPoint(enum NavPoint::NavPointType,struct CVector const &,class m3d::rend::TexHandle &,class PointBase<float> &) const ;
    int AddTurret(int);
    virtual struct m3d::Class * GetClass() const ;
    int AddVehicle(int);
    int AddNavPoint(int);
    void AllowTurrets(bool);
    void CreateNavPoints();
    int RemoveDistance(int);
    void AllowNavPoints(bool);
    int UpdateNavPoint(int);
    float CalculateDistanceToNavPoint(int) const ;
    class m3d::rend::TexHandle GetIcoForWorldside(enum Worldside) const ;
    virtual class m3d::Object * Clone();
    virtual int GameDataSave(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
    void Reset();
    virtual ~RadarWnd();
    int AddDistance(int);
    float GetDefaultScanRadius() const ;
    bool IsPositionOutsideScanRadius(struct CVector const &) const ;
    virtual void OnPaintOverChildren(struct m3d::ui::DrawInfo const &);
    int RemoveWorldside(enum Worldside);
    int OnAddNavPoint(void *);
    int UpdateDistancesOnNewFrame();
    void GetIcoForObject(class ai::Obj const *,class m3d::rend::TexHandle &,class PointBase<float> &) const ;
    void GetIcoForTurret(class ai::StaticAutoGun const *,class m3d::rend::TexHandle &,class PointBase<float> &) const ;
    int UpdateOnNewFrame();
    static class m3d::Object * CreateObject();
    int UpdateTurretsOnNewFrame();
    void ClearVehicleItems();
    void SetScanRadius(float);
    virtual int GameDataSetup();
    struct CVector GetWorldsideCoords(enum Worldside) const ;
    int UpdateTurret(int);
    int UpdateVehicle(int);
    void DrawHighlight(struct m3d::ui::DrawInfo const &) const ;
    void DrawTurrets(struct m3d::ui::DrawInfo const &) const ;
    float GetScanRadius() const ;
    void DrawVehicles(struct m3d::ui::DrawInfo const &) const ;
    int UpdateVehiclesOnNewFrame();
    static struct m3d::Class * GetBaseClass();
protected:
    RadarWnd();
    RadarWnd(class RadarWnd const &);

public:
    RT_CLASS_INLINE_DECLARE(RadarWnd);

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
