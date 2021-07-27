#pragma once

class JournalWnd :  public ScreenWnd
{
public:
    void ShowAllInEncyclopaedia();
    bool BookExists(CStr const &) const ;
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual class m3d::Object * Clone();
    int AddHistory(CStr const &,class m3d::AIParam const &);
    virtual struct m3d::Class * GetClass() const ;
    int AddClanToEncyclopaedia(int);
    virtual ~JournalWnd();
    int AddPrototypeToEncyclopaedia(CStr const &);
    int AddBook(CStr const &,CStr const &);
protected:
    void PostCurrentTabMessage() const ;
    int UpdateButtonsState();
    enum Tab CtrlId2TabId(int) const ;
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    JournalWnd();
    JournalWnd(class JournalWnd const &);
    virtual int GameDataSetup();
    virtual int GameDataSave(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
    virtual int OnBeforeAddToWndStation();
    virtual int GameDataLoad(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
    int SetCurTab(enum Tab,bool);
    int SelectButton(enum Tab,bool);
private:
    JournalWnd::AuxInfo m_aif;
    m3d::ui::ButtonWnd *m_tabButtons[5];
    ref_ptr<m3d::ui::Wnd> m_tabs[5];
    JournalWnd::Tab m_curTab;
};
