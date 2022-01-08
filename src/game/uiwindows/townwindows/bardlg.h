#pragma once

class BarDlg :  public ChildPanel
{
public:
    virtual class m3d::Object * Clone();
    class ai::Bar * GetBar() const ;
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    int SetUpForBar(int);
    virtual struct m3d::Class * GetClass() const ;
    virtual ~BarDlg();
protected:
    BarDlg(class BarDlg const &);
    BarDlg();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    void FullUpdate();
    virtual int GameDataSetup();
    virtual int GameDataClear(bool);
    virtual int OnAfterRemoveFromWndStation();
    int OnBeginConversation(class ai::Npc *);
    virtual int OnAfterAddToWndStation();
    virtual int OnBeforeAddToWndStation();
private:
    BarDlg::AuxInfo m_aif;
    NpcList *m_lstNPCs;
    int m_barId;
};
