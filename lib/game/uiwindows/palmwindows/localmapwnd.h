#pragma once
#include "screenwnd.h"
#include <core/ref_ptr.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}  // namespace m3d

class SaveSellList;
class MapInfoPanel;
class LocalChartWnd;
class MapComboBox;
class ObjectInfo;

class LocalMapWnd : public ScreenWnd
{
public:
    enum Mode
    {
        MODE_TRADE = 0,
        MODE_CLANS = 1,
    };

public:
    LocalMapWnd::Mode GetCurrentMode() const;
    CStr GetCurrentLevelName() const;
    int SetUpForLevel(CStr const& levelName);
    static bool __fastcall IsObjectInfoValidForPrices(ObjectInfo* objectInfo);
    ObjectInfo* GetCurrentObjectInfo() const;

    struct AuxInfo
    {
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
        AuxInfo(LocalMapWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterAddToWndStation() override;
    int ChangeMap();
    int SavePricesForCurrentObjectInfo();
    int ShowInfoForSavedPriceList(ObjectInfo*);
    int RemoveSavedPricesForCurrentObjectInfo();
    void LaunchGlobalMap();
    int AddNavPoint(ObjectInfo* objectInfo, bool bWithUserWarning);
    int AddNavPoint(CVector const& worldOrigin, bool bWithUserWarning);
    int DeleteNavPoint(int npId, bool bWithUserWarning);
    void UpdateBtnNavPointState();
    void OnNavPointAdded(void* data);
    void OnNavPointDeleted(void* data);
    int GetUserLocationNavPointId() const;
    void OnBtnAddTownToSavedListClick(m3d::ui::Wnd* wndFrom, unsigned int idFrom, m3d::AIParam const& data);
    void UpdateBtnAddTownToSavedListState();
    bool IsObjectInfoSavedList(ObjectInfo* objectInfo) const;
    void GetBtnAddTownToSavedListTexNames(bool bActive, CStr& icoIdOut, CStr& icoIdIn, CStr& icoIdDown) const;
    void ShowInfo(ObjectInfo* objectInfo, bool bFromSaveList);
    void OnObjectInfoChanged();
    void UpdateSavedListSelection();
    void OnBtnNavPointClick(m3d::ui::Wnd* wndFrom, unsigned int idFrom, m3d::AIParam const& data);
    bool CanUserLocationNavPointBeAddedFromBtn() const;
    bool CanUserLocationNavPointBeDeletedFromBtn() const;
    void GetBtnNavPointTexNames(bool bActive, CStr& icoIdOut, CStr& icoIdIn, CStr& icoIdDown) const;
    void OnBtnEnableObjectNamesClick(m3d::ui::Wnd* wndFrom, unsigned int idFrom, m3d::AIParam const& data);
    void EnableObjectNames(bool bEnable);
    bool IsObjectNamesEnabled() const;
    void UpdateBtnEnableObjectNamesState();
    void GetBtnEnableObjectNamesTexNames(bool bActive, CStr& icoIdOut, CStr& icoIdIn, CStr& icoIdDown) const;
    void EnsureShowInfo();
    ObjectInfo* GetInfoForNearestVisibleObject() const;
    void ValidateSavedObjectInfos();

    ref_ptr<MapComboBox> m_cbMaps;
    LocalMapWnd::AuxInfo m_aif;
    ref_ptr<LocalChartWnd> m_wndChart;
    ref_ptr<MapInfoPanel> m_wndInfo;
    ref_ptr<SaveSellList> m_wndSaveSellList;
    m3d::ui::ButtonWnd* m_btnNavPoint;
    m3d::ui::ButtonWnd* m_btnAddTownToSavedList;
    m3d::ui::ButtonWnd* m_btnEnableObjectNames;
    m3d::ui::Wnd* m_wndEmbossBtnNavPoint;
    m3d::ui::Wnd* m_wndEmbossBtnAddTownToSavedList;
    m3d::ui::Wnd* m_wndEmbossBtnEnableObjectNames;
    
    LocalMapWnd();
    LocalMapWnd(LocalMapWnd const& rhs);

public:
    virtual ~LocalMapWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classLocalMapWnd;
};
