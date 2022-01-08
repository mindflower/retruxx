#pragma once

class MenuItem
{
public:
    MenuItem(CStr const &,unsigned int,enum ControlType);
    void AddChild(class MenuItem *);
    ~MenuItem();
    class MenuItem * GetMenuItemViaName(CStr const &);
    class MenuItem const * GetMenuItemViaName(CStr const &) const ;
protected:
private:
    CStr m_name;
    unsigned int m_id;
    MenuItem *m_parent;
    std::vector<MenuItem *> m_children;
    ControlType m_ctrlType;
    bool m_disabled;
    bool m_visible;
};

class GameMenuWnd :  public m3d::ui::ModalWnd, MenuItem
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
    GameMenuWnd(class GameMenuWnd const &);
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
private:
    MenuItem *m_curItem;
    MenuItem *m_minItem;
};
