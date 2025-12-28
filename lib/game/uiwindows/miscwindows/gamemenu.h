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
  /* 0x0000 */ CStr m_name;
  /* 0x000c */ unsigned int m_id;
  /* 0x0010 */ MenuItem* m_parent;
  /* 0x0014 */ std::vector<MenuItem *,std::allocator<MenuItem *> > m_children;
  /* 0x0024 */ ControlType m_ctrlType;
  /* 0x0028 */ bool m_disabled;
  /* 0x0029 */ bool m_visible;
  MenuItem(const MenuItem&);
  MenuItem(const CStr& name, unsigned int id, ControlType ct);
  ~MenuItem();
  void AddChild(MenuItem* child);
  const MenuItem* GetMenuItemViaName(const CStr& name) const;
  MenuItem* GetMenuItemViaName(const CStr& name);
}; /* size: 0x002c */

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
    static inline CStr SAVE_GAME_MENUITEM_NAME = "SaveGame";
    static inline CStr DEATH_LEVEL_NAME = "DeathMenu";

private:
    MenuItem *m_curItem = nullptr;
    MenuItem *m_minItem = nullptr;
};
