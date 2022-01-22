#pragma once
#include "screenwnd.h"
#include <core/ref_ptr.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}

class SaveSellList;
class MapInfoPanel;
class LocalChartWnd;
class MapComboBox;

class LocalMapWnd :  public ScreenWnd
{
public:
    enum Mode
    {
        MODE_TRADE = 0x0,
        MODE_CLANS = 0x1,
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_cbMapsName;
        CStr m_wndChartName;
        CStr m_wndInfoName;
        CStr m_wndSaveSellListName;
        CStr m_wndChartBgName;
        CStr m_addNpDlgStr;
        CStr m_deleteNpDlgStr;
        CStr m_btnNavPointName;
        CStr m_btnEnableObjectNamesName;
        CStr m_btnAddTownToSavedListName;
        CStr m_wndEmbossBtnNavPointName;
        CStr m_wndEmbossBtnEnableObjectNamesName;
        CStr m_wndEmbossBtnAddTownToSavedListName;
        CStr m_strBtnAddTownToSavedList;
        CStr m_strActive;
        CStr m_strInactive;
        CStr m_strOut;
        CStr m_strIn;
        CStr m_strDown;
        CStr m_strIdTooltipSavePrices;
        CStr m_strIdTooltipRemovePrices;
        CStr m_strIdTooltipAddNavPoint;
        CStr m_strIdTooltipDeleteNavPoint;
        CStr m_strBtnNavPoint;
        CStr m_strEnableObjectNames;
        CStr m_strIdTooltipShowObjectNames;
        CStr m_strIdTooltipHideObjectNames;
    };

public:
    int SetUpForLevel(CStr const &);
    virtual m3d::Object * Clone();
    CStr GetCurrentLevelName() const ;
    static m3d::Class * GetBaseClass();
    virtual m3d::Class * GetClass() const ;
    static bool IsObjectInfoValidForPrices(class ObjectInfo *);
    static m3d::Object * CreateObject();
    virtual ~LocalMapWnd();
    ObjectInfo * GetCurrentObjectInfo() const ;
    Mode GetCurrentMode() const ;

protected:
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataSetup();
    void OnBtnEnableObjectNamesClick(m3d::ui::Wnd *,unsigned int, m3d::AIParam const &);
    void UpdateBtnAddTownToSavedListState();
    bool IsObjectInfoSavedList(ObjectInfo *) const ;
    void OnNavPointAdded(void *);
    int SavePricesForCurrentObjectInfo();
    int GetUserLocationNavPointId() const ;
    void OnBtnNavPointClick(m3d::ui::Wnd *,unsigned int, m3d::AIParam const &);
    int AddNavPoint(CVector const &,bool);
    int AddNavPoint(ObjectInfo *,bool);
    void UpdateSavedListSelection();
    bool CanUserLocationNavPointBeAddedFromBtn() const ;
    void UpdateBtnEnableObjectNamesState();
    ObjectInfo * GetInfoForNearestVisibleObject() const ;
    void ShowInfo(ObjectInfo *,bool);
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
    LocalMapWnd(LocalMapWnd const &);
    LocalMapWnd();
    void OnBtnAddTownToSavedListClick(m3d::ui::Wnd *,unsigned int, m3d::AIParam const &);
    virtual int OnBeforeAddToWndStation();
    int ChangeMap();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);

public:
    RT_CLASS_DECLARE(LocalMapWnd);

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
