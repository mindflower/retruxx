#pragma once
#include "screenwnd.h"
#include <core/ref_ptr.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}

class JournalWnd :  public ScreenWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_tabButtonNames[5];
        CStr m_tabBtnPaneNameUnselected;
        CStr m_tabBtnPaneNameSelected;
    };

    enum Tab
    {
        TAB_HISTORY = 0x0,
        TAB_BOOKS = 0x1,
        TAB_RELATIONS = 0x2,
        TAB_STATS = 0x3,
        TAB_ENCYCLOPAEDIA = 0x4,
        NUM_TABS = 0x5,
    };

public:
    void ShowAllInEncyclopaedia();
    bool BookExists(CStr const &) const ;
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    int AddHistory(CStr const &,m3d::AIParam const &);
    virtual m3d::Class * GetClass() const ;
    int AddClanToEncyclopaedia(int);
    virtual ~JournalWnd();
    int AddPrototypeToEncyclopaedia(CStr const &);
    int AddBook(CStr const &,CStr const &);

protected:
    void PostCurrentTabMessage() const ;
    int UpdateButtonsState();
    enum Tab CtrlId2TabId(int) const ;
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);
    JournalWnd();
    JournalWnd(JournalWnd const &);
    virtual int GameDataSetup();
    virtual int GameDataSave(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
    virtual int OnBeforeAddToWndStation();
    virtual int GameDataLoad(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
    int SetCurTab(Tab,bool);
    int SelectButton(Tab,bool);

public:
    RT_CLASS_DECLARE(JournalWnd);

private:
    JournalWnd::AuxInfo m_aif;
    m3d::ui::ButtonWnd *m_tabButtons[5];
    ref_ptr<m3d::ui::Wnd> m_tabs[5];
    JournalWnd::Tab m_curTab;
};
