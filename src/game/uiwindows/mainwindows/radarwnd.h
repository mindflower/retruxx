#pragma once

namespace RadarWnd
{
    class NpDistance
    {
    public:
        ~NpDistance();
        int GetNavPointId() const ;
        NpDistance(class ElectronicDigitalWnd *,enum NavPoint::NavPointType);
        int SetNavPointId(int);
        int SetDistance(float);
        enum NavPoint::NavPointType GetNavPointType() const ;
    protected:
    private:
        void Show(bool);
        bool IsValid() const ;
        NavPoint::NavPointType m_navPointType;
        ElectronicDigitalWnd *m_wndDigital;
        int m_navPointId;
    };
}

class RadarWnd :  public m3d::ui::Wnd
{
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
    static class m3d::Object * __fastcall CreateObject();
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
    static struct m3d::Class * __fastcall GetBaseClass();
protected:
    RadarWnd();
    RadarWnd(class RadarWnd const &);
private:
    std::_Allocate<RadarItem *>(uint,RadarItem * *);
    ClearRadarItemMap(std::map<int,std::vector<RadarItem *>> &);
    std::fill<RadarItem * *,RadarItem *>(RadarItem * *,RadarItem * *,RadarItem * const &);
    std::fill<NpDistance *,NpDistance>(NpDistance *,NpDistance *,NpDistance const &);
    AddObject(int,std::map<int,std::vector<RadarItem *>> &);
    std::_Construct<RadarItem *,RadarItem *>(RadarItem * *,RadarItem * const &);
    std::_Ptr_cat<NpDistance *,NpDistance *>(NpDistance * &,NpDistance * &);
    RemoveObject(int,std::map<int,std::vector<RadarItem *>> &);
    std::allocator<RadarItem *>::allocator<RadarItem *>();
    std::allocator<RadarItem *>::allocator<RadarItem *>();
    std::copy_backward<RadarItem * *,RadarItem * *>(RadarItem * *,RadarItem * *,RadarItem * *);
    std::_Copy_backward_opt<NpDistance *,NpDistance *>(NpDistance *,NpDistance *,NpDistance *,std::_Nonscalar_ptr_iterator_tag);
    std::_Copy_backward_opt<RadarItem * *,RadarItem * *>(RadarItem * *,RadarItem * *,RadarItem * *,std::_Scalar_ptr_iterator_tag);
    std::_Destroy<RadarItem *>(RadarItem * *);
    std::_Uninit_copy<RadarItem *,RadarItem *>(RadarItem * *,RadarItem * *,RadarItem * *,std::allocator<RadarItem *> &,std::_Scalar_ptr_iterator_tag);
    std::_Construct<NpDistance,NpDistance>(NpDistance *,NpDistance const &);
    std::_Allocate<NpDistance>(uint,NpDistance *);
    std::_Ptr_cat<RadarItem>(RadarItem * *,RadarItem * *);
    RemoveItem(std::map<int,std::vector<RadarItem *>> &,int);
    UpdateObject(int,std::map<int,std::vector<RadarItem *>> const &);
    std::allocator<NpDistance>::allocator<NpDistance>();
    std::allocator<NpDistance>::allocator<NpDistance>();
    std::_Destroy<NpDistance>(NpDistance *);
    UpdateObjectsOnNewFrame(std::map<int,std::vector<RadarItem *>> &,m3d::Class const *);
    DrawItems(m3d::ui::DrawInfo const &,std::map<int,std::vector<RadarItem *>> const &);
    std::copy_backward<NpDistance *,NpDistance *>(NpDistance *,NpDistance *,NpDistance *);
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
