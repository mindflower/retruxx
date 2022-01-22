#pragma once
#include "localmapwnd.h"
#include <ui/ui.h>

class MapSellList;

namespace m3d
{
    namespace ui
    {
        class TextBoxWnd;
        class ImageWnd;
        class LineWnd;
    }
}

class MapInfoPanel :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_lblObjectNameName;
        CStr m_lblSellName;
        CStr m_lblBuyName;
        CStr m_lnHorz0Name;
        CStr m_lnHorz1Name;
        CStr m_lnVert0Name;
        CStr m_lnVert1Name;
        CStr m_txtObjectDizName;
        CStr m_wndSellListName;
        CStr m_btnModeTradeName;
        CStr m_btnModeClansName;
        CStr m_wndEmbossBtnModeTradeName;
        CStr m_wndEmbossBtnModeClansName;
        CStr m_wndFrameName;
        CStr m_modeBtnPaneNameUnselected;
        CStr m_modeBtnPaneNameSelected;
        CStr m_strIdRuined;
    };

public:
    int SetUpForObject(ObjectInfo *,bool);
    ObjectInfo * GetObjectInfo() const ;
    int SetUpForMode(LocalMapWnd::Mode);
    virtual m3d::Class * GetClass() const ;
    virtual ~MapInfoPanel();
    static m3d::Object * CreateObject();
    int SavePrices() const ;
    int CreateFromPattern(m3d::ui::Wnd const *,bool);
    LocalMapWnd::Mode GetCurrentMode() const ;
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();

protected:
    void UpdateModeButtonsState();
    MapInfoPanel();
    MapInfoPanel(MapInfoPanel const &);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    int Clear();
    virtual int GameDataClear(bool);
    virtual int OnBeforeAddToWndStation();
    void ValidateMode();
    CStr GetObjFullName() const ;
    int SetupForClansMode();
    int ShowClansModeControls(bool);
    int Setup();
    int ShowTradeModeControls(bool);
    int SetupForTradeMode();

public:
    RT_CLASS_DECLARE(MapInfoPanel);

private:
    m3d::ui::Wnd *m_lblObjectName;
    m3d::ui::TextBoxWnd *m_txtObjectDiz;
    MapSellList *m_wndSellList;
    m3d::ui::ImageWnd *m_wndSell;
    m3d::ui::ImageWnd *m_wndBuy;
    m3d::ui::LineWnd *m_lnHorz0;
    m3d::ui::LineWnd *m_lnHorz1;
    m3d::ui::LineWnd *m_lnVert0;
    m3d::ui::LineWnd *m_lnVert1;
    m3d::ui::ButtonWnd *m_btnModeTrade;
    m3d::ui::ButtonWnd *m_btnModeClans;
    m3d::ui::Wnd *m_wndEmbossBtnModeTrade;
    m3d::ui::Wnd *m_wndEmbossBtnModeClans;
    m3d::ui::Wnd *m_wndFrame;
    MapInfoPanel::AuxInfo m_aif;
    ObjectInfo *m_objectInfo;
    LocalMapWnd::Mode m_mode;
    bool m_fromSaveList;
};
