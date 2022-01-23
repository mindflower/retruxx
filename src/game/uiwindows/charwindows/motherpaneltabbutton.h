#pragma once
#include "motherpanel.h"
#include <ui/button.h>

class MotherPanelTabButton :  public m3d::ui::ButtonWnd
{
public:
    enum Mode
    {
        MODE_IN_FIELD = 0x0,
        MODE_IN_TOWN = 0x1,
        MODE_NUM_MODES = 0x2,
    };

    class PerModeInfo
    {
    public:
        m3d::rend::TexHandle GetUnselTex(void) const;
        PerModeInfo(m3d::rend::TexHandle, m3d::rend::TexHandle);
        m3d::rend::TexHandle GetSelTex(void) const;
        ~PerModeInfo(void);

    private:
        m3d::rend::TexHandle m_selTex;
        m3d::rend::TexHandle m_unselTex;
    };

public:
    virtual m3d::Class * GetClass() const ;
    MotherPanel::Tab GetTabId() const ;
    void Select(bool);
    bool IsSelected() const ;
    virtual m3d::Object * Clone();
    Mode GetMode() const ;
    static m3d::Class * GetBaseClass();
    virtual ~MotherPanelTabButton();
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    int SetupForTab(MotherPanel::Tab);
    void SetMode(Mode);
    static m3d::Object * CreateObject();

protected:
    void UpdateTex();
    void ClearInfo();
    static CStr __fastcall Mode2Str(Mode);
    MotherPanelTabButton(MotherPanelTabButton const &);
    MotherPanelTabButton();
    void UpdateTooltip();
    bool CanApplyMode(Mode) const ;
    bool HasMode(Mode) const ;
    void InitInfo();
    ai::Building const * GetBuilding(Mode) const ;

public:
    RT_CLASS_DECLARE(MotherPanelTabButton);

private:
    bool m_bSelected;
    MotherPanel::Tab m_tabId;
    MotherPanelTabButton::Mode m_mode;
    MotherPanelTabButton::PerModeInfo *m_info[2];
};
