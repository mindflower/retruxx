#pragma once

namespace ai
{
    class Bar;
}

class NpcList;

class BarDlg :  public ChildPanel
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_lstNPCsPatternName;
    };

public:
    virtual m3d::Object * Clone();
    ai::Bar * GetBar() const ;
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    int SetUpForBar(int);
    virtual m3d::Class * GetClass() const ;
    virtual ~BarDlg();

protected:
    BarDlg(BarDlg const &);
    BarDlg();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);
    void FullUpdate();
    virtual int GameDataSetup();
    virtual int GameDataClear(bool);
    virtual int OnAfterRemoveFromWndStation();
    int OnBeginConversation(ai::Npc *);
    virtual int OnAfterAddToWndStation();
    virtual int OnBeforeAddToWndStation();

public:
    RT_CLASS_INLINE_DECLARE(BarDlg);

private:
    AuxInfo m_aif;
    NpcList *m_lstNPCs;
    int m_barId;
};
