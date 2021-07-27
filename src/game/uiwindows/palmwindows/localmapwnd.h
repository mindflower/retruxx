#pragma once

class LocalMapWnd :  public ScreenWnd
{
public:
    int SetUpForLevel(CStr const &);
    virtual class m3d::Object * Clone();
    CStr GetCurrentLevelName() const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual struct m3d::Class * GetClass() const ;
    static bool __fastcall IsObjectInfoValidForPrices(class ObjectInfo *);
    static class m3d::Object * __fastcall CreateObject();
    virtual ~LocalMapWnd();
    class ObjectInfo * GetCurrentObjectInfo() const ;
    enum Mode GetCurrentMode() const ;
protected:
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataSetup();
    void OnBtnEnableObjectNamesClick(class m3d::ui::Wnd *,unsigned int,class m3d::AIParam const &);
    void UpdateBtnAddTownToSavedListState();
    bool IsObjectInfoSavedList(class ObjectInfo *) const ;
    void OnNavPointAdded(void *);
    int SavePricesForCurrentObjectInfo();
    int GetUserLocationNavPointId() const ;
    void OnBtnNavPointClick(class m3d::ui::Wnd *,unsigned int,class m3d::AIParam const &);
    int AddNavPoint(struct CVector const &,bool);
    int AddNavPoint(class ObjectInfo *,bool);
    void UpdateSavedListSelection();
    bool CanUserLocationNavPointBeAddedFromBtn() const ;
    void UpdateBtnEnableObjectNamesState();
    class ObjectInfo * GetInfoForNearestVisibleObject() const ;
    void ShowInfo(class ObjectInfo *,bool);
    virtual int OnAfterAddToWndStation();
    void GetBtnEnableObjectNamesTexNames(bool,CStr &,CStr &,CStr &) const ;
    void GetBtnAddTownToSavedListTexNames(bool,CStr &,CStr &,CStr &) const ;
    bool CanUserLocationNavPointBeDeletedFromBtn() const ;
    void EnsureShowInfo();
    int RemoveSavedPricesForCurrentObjectInfo();
    void UpdateBtnNavPointState();
    bool IsObjectNamesEnabled() const ;
    void GetBtnNavPointTexNames(bool,CStr &,CStr &,CStr &) const ;
    void OnObjectInfoChanged();
    void LaunchGlobalMap();
    int DeleteNavPoint(int,bool);
    void EnableObjectNames(bool);
    void OnNavPointDeleted(void *);
    void ValidateSavedObjectInfos();
    LocalMapWnd(class LocalMapWnd const &);
    LocalMapWnd();
    void OnBtnAddTownToSavedListClick(class m3d::ui::Wnd *,unsigned int,class m3d::AIParam const &);
    virtual int OnBeforeAddToWndStation();
    int ChangeMap();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
private:
    ref_ptr<MapComboBox> m_cbMaps;
    LocalMapWnd::AuxInfo m_aif;
    ref_ptr<LocalChartWnd> m_wndChart;
    ref_ptr<MapInfoPanel> m_wndInfo;
    ref_ptr<SaveSellList> m_wndSaveSellList;
    m3d::ui::ButtonWnd *m_btnNavPoint;
    m3d::ui::ButtonWnd *m_btnAddTownToSavedList;
    m3d::ui::ButtonWnd *m_btnEnableObjectNames;
    m3d::ui::Wnd *m_wndEmbossBtnNavPoint;
    m3d::ui::Wnd *m_wndEmbossBtnAddTownToSavedList;
    m3d::ui::Wnd *m_wndEmbossBtnEnableObjectNames;
};
