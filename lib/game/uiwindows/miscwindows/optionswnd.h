#pragma once
#include <core/ref_ptr.h>
#include <ui/button.h>
#include <ui/wnd.h>

class OptionTabButton;

class OptionsWnd : public m3d::ui::ModalWnd
{
public:
    enum Tab
    {
        TAB_VIDEO = 0,
        TAB_SOUND = 1,
        TAB_CONTROL = 2,
        TAB_GAME = 3,
        TAB_NUM_TABS = 4,
        TAB_INVALID = TAB_NUM_TABS,
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        CStr m_tabButtonNames[4] = { "tabBtnVideo" , "tabBtnSound", "tabBtnControl", "tabBtnGame"};
    };

public:
    virtual m3d::Class* GetClass() const;
    virtual int GameDataSetup();
    virtual m3d::Object* Clone();

    static m3d::Class* GetBaseClass();
    static m3d::Object* CreateObject();
    
protected:
    virtual int OnBeforeAddToWndStation();
    OptionsWnd();
    OptionsWnd(OptionsWnd const&);
    void UpdateTabButtonsStates();
    int SetCurTab(Tab);
    void SelectTabButton(Tab);
    int ApplyTabChanges(Tab);
    virtual int OnAfterRemoveFromWndStation(void);
    int ShowOptionWindowForTab(Tab);
    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    int GetOptionWindowGuiIdByTabId(Tab) const;
    virtual int CanClose();

public:
    RT_CLASS_DECLARE(OptionsWnd);

private:
    OptionsWnd::Tab m_curTabId = TAB_NUM_TABS;
    OptionsWnd::Tab m_lastTabId = TAB_NUM_TABS;
    std::vector<OptionTabButton*> m_tabButtons;
    std::vector<ref_ptr<m3d::ui::Wnd>> m_optionWindows;
    OptionsWnd::AuxInfo m_aif;
};

class OptionTabButton :  public m3d::ui::ButtonWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

        CStr m_paneNameSel = "PaneBtnOptionsSel";
        CStr m_paneNameUnsel = "PaneBtnOptionsUnsel";
    };
public:
    static inline AuxInfo m_aif;

public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    OptionsWnd::Tab GetTabId() const ;
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    void Select(bool);
    int SetupForTab(OptionsWnd::Tab);
    virtual m3d::Object * Clone();
    virtual ~OptionTabButton();
    bool IsSelected() const ;
    static m3d::Class * GetBaseClass();

protected:
    OptionTabButton(OptionTabButton const &);
    OptionTabButton();
    void UpdatePane();

public:
    RT_CLASS_DECLARE(OptionTabButton);

private:
    bool m_bSelected = false;
    OptionsWnd::Tab m_tabId = OptionsWnd::TAB_NUM_TABS;
};
