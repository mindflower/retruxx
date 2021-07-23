#pragma once

class CBButton :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    int GetCBId() const ;
    int SetupForCB(int);
    static class m3d::Object * __fastcall CreateObject();
    virtual ~CBButton();
    int GetPrice() const ;
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
    void Select(bool);
protected:
    virtual int CreateChildren();
    virtual int CreateFromPattern();
    CBButton();
    CBButton(class CBButton const &);
    static void __fastcall ClearPattern(enum Type);
    virtual int FullUpdate();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    static int __fastcall LoadPattern(class ref_ptr<class m3d::ui::Wnd>,enum Type);
private:
    int m_cbId;
    m3d::ui::Wnd *m_wndName;
    m3d::ui::ButtonWnd *m_btnBuy;
    ref_ptr<RepositoryItemWnd> m_wndPicture;
    m3d::ui::Wnd *m_wndDurability;
    m3d::ui::Wnd *m_wndResistPiercing;
    m3d::ui::Wnd *m_wndResistBlast;
    m3d::ui::Wnd *m_wndResistEnergy;
    m3d::ui::Wnd *m_wndWeight;
    m3d::ui::Wnd *m_wndPrice;
    CBButton::Type m_type;
    bool m_bSelected;
};
