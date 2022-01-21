#pragma once
#include <ui/wnd.h>

enum ControlType
{
    CT_BUTTON = 0x0,
    CT_CHECK = 0x1,
    CT_YES_NO = 0x1,
    CT_SLIDER = 0x2,
    CT_CENTERED_LABEL = 0x3,
    CT_BLANK = 0x4,
    CT_LINE = 0x5,
};

class MenuItem
{
public:
    MenuItem(CStr const &,unsigned int,ControlType);
    void AddChild(class MenuItem *);
    ~MenuItem();
    MenuItem * GetMenuItemViaName(CStr const &);
    MenuItem const * GetMenuItemViaName(CStr const &) const ;

private:
    CStr m_name;
    unsigned int m_id;
    MenuItem *m_parent;
    std::vector<MenuItem *> m_children;
    ControlType m_ctrlType;
    bool m_disabled;
    bool m_visible;
};

class GameMenuWnd :  public m3d::ui::ModalWnd, public MenuItem
{
public:
    static struct m3d::Class * GetBaseClass();
    CStr GetCurrentLevelName() const ;
    virtual class m3d::Object * Clone();
    CStr GetCurrentRootLevelName() const ;
    virtual ~GameMenuWnd();
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    bool SetMenuLevel(CStr const &,CStr const &);

protected:
    GameMenuWnd();
    GameMenuWnd(GameMenuWnd const &);
    virtual int OnInitModal();
    class MenuItem * CreateBlankSpaceMenuItem() const ;
    void RecalcLayout();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    class MenuItem * CreateTextLabelMenuItem(CStr const &) const ;
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    virtual int OnBeforeAddToWndStation();
    virtual int OnActivate(bool);
    int CreateGameMenuWnd();
    class MenuItem * CreateLineMenuItem() const ;
    void CloseCurrentMenuLevel(bool,bool,bool);

public:
    RT_CLASS_DECLARE(GameMenuWnd);
    static inline CStr ROOT_LEVEL_NAME = "main";

private:
    MenuItem *m_curItem;
    MenuItem *m_minItem;
};
