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
        /* 0x0000 */ CStr m_cbMapsName;
        /* 0x000c */ CStr m_wndChartName;
        /* 0x0018 */ CStr m_wndInfoName;
        /* 0x0024 */ CStr m_wndSaveSellListName;
        /* 0x0030 */ CStr m_wndChartBgName;
        /* 0x003c */ CStr m_addNpDlgStr;
        /* 0x0048 */ CStr m_deleteNpDlgStr;
        /* 0x0054 */ CStr m_btnNavPointName;
        /* 0x0060 */ CStr m_btnEnableObjectNamesName;
        /* 0x006c */ CStr m_btnAddTownToSavedListName;
        /* 0x0078 */ CStr m_wndEmbossBtnNavPointName;
        /* 0x0084 */ CStr m_wndEmbossBtnEnableObjectNamesName;
        /* 0x0090 */ CStr m_wndEmbossBtnAddTownToSavedListName;
        /* 0x009c */ CStr m_strBtnAddTownToSavedList;
        /* 0x00a8 */ CStr m_strActive;
        /* 0x00b4 */ CStr m_strInactive;
        /* 0x00c0 */ CStr m_strOut;
        /* 0x00cc */ CStr m_strIn;
        /* 0x00d8 */ CStr m_strDown;
        /* 0x00e4 */ CStr m_strIdTooltipSavePrices;
        /* 0x00f0 */ CStr m_strIdTooltipRemovePrices;
        /* 0x00fc */ CStr m_strIdTooltipAddNavPoint;
        /* 0x0108 */ CStr m_strIdTooltipDeleteNavPoint;
        /* 0x0114 */ CStr m_strBtnNavPoint;
        /* 0x0120 */ CStr m_strEnableObjectNames;
        /* 0x012c */ CStr m_strIdTooltipShowObjectNames;
        /* 0x0138 */ CStr m_strIdTooltipHideObjectNames;
        AuxInfo(LocalMapWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0144 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
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

    /* 0x0224 */ ref_ptr<MapComboBox> m_cbMaps;
    /* 0x0228 */ LocalMapWnd::AuxInfo m_aif;
    /* 0x036c */ ref_ptr<LocalChartWnd> m_wndChart;
    /* 0x0370 */ ref_ptr<MapInfoPanel> m_wndInfo;
    /* 0x0374 */ ref_ptr<SaveSellList> m_wndSaveSellList;
    /* 0x0378 */ m3d::ui::ButtonWnd* m_btnNavPoint;
    /* 0x037c */ m3d::ui::ButtonWnd* m_btnAddTownToSavedList;
    /* 0x0380 */ m3d::ui::ButtonWnd* m_btnEnableObjectNames;
    /* 0x0384 */ m3d::ui::Wnd* m_wndEmbossBtnNavPoint;
    /* 0x0388 */ m3d::ui::Wnd* m_wndEmbossBtnAddTownToSavedList;
    /* 0x038c */ m3d::ui::Wnd* m_wndEmbossBtnEnableObjectNames;
    
    LocalMapWnd();
    LocalMapWnd(LocalMapWnd const& rhs);

public:
    virtual ~LocalMapWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classLocalMapWnd;
}; /* size: 0x0390 */
