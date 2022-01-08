#pragma once

class MotherPanelTabButton :  public m3d::ui::ButtonWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    enum MotherPanel::Tab GetTabId() const ;
    void Select(bool);
    bool IsSelected() const ;
    virtual class m3d::Object * Clone();
    enum Mode GetMode() const ;
    static struct m3d::Class * GetBaseClass();
    virtual ~MotherPanelTabButton();
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    int SetupForTab(enum MotherPanel::Tab);
    void SetMode(enum Mode);
    static class m3d::Object * CreateObject();
protected:
    void UpdateTex();
    void ClearInfo();
    static CStr __fastcall Mode2Str(enum Mode);
    MotherPanelTabButton(class MotherPanelTabButton const &);
    MotherPanelTabButton();
    void UpdateTooltip();
    bool CanApplyMode(enum Mode) const ;
    bool HasMode(enum Mode) const ;
    void InitInfo();
    class ai::Building const * GetBuilding(enum Mode) const ;
private:
    bool m_bSelected;
    MotherPanel::Tab m_tabId;
    MotherPanelTabButton::Mode m_mode;
    MotherPanelTabButton::PerModeInfo *m_info[2];
};
