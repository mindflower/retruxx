#pragma once

class MapInfoPanel :  public m3d::ui::Wnd
{
public:
    int SetUpForObject(class ObjectInfo *,bool);
    class ObjectInfo * GetObjectInfo() const ;
    int SetUpForMode(enum LocalMapWnd::Mode);
    virtual struct m3d::Class * GetClass() const ;
    virtual ~MapInfoPanel();
    static class m3d::Object * CreateObject();
    int SavePrices() const ;
    int CreateFromPattern(class m3d::ui::Wnd const *,bool);
    enum LocalMapWnd::Mode GetCurrentMode() const ;
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
protected:
    void UpdateModeButtonsState();
    MapInfoPanel();
    MapInfoPanel(class MapInfoPanel const &);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
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
