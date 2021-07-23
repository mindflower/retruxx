#pragma once

class BindKeysWnd :  public m3d::ui::Wnd
{
public:
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual class m3d::Object * Clone();
    virtual ~BindKeysWnd();
    int ApplyChanges(bool);
protected:
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    virtual int OnBeforeAddToWndStation();
    void LoadDefaultBindings();
    virtual int GameDataSetup();
    BindKeysWnd(class BindKeysWnd const &);
    BindKeysWnd();
    virtual int GameDataUpdate(void *,int);
    void ApplyBindings();
    void CancelBindings();
private:
    m3d::ui::ListBoxWnd<BindKeysItem *>::~ListBoxWnd<BindKeysItem *>();
    m3d::ui::ListBoxWnd<BindKeysItem *>::ListBoxWnd<BindKeysItem *>();
    BindKeysWnd::AuxInfo m_aif;
    bool m_isInited;
    bool m_isDirty;
    BindKeysWnd::BindKeysList *m_lstBindings;
    bool m_bChanged;
};
