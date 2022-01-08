#pragma once

class NpcList :  public m3d::ui::ListBoxWnd<NpcButton *>
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    class ai::Bar * GetBar() const ;
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    virtual int GameDataClear(bool);
    int SetUpForBar(int);
    virtual class m3d::Object * Clone();
    virtual ~NpcList();
    static struct m3d::Class * GetBaseClass();
protected:
    virtual int OnAfterAddToWndStation();
    virtual int MeasureItem(int,struct BoundsBase<float> &) const ;
    void FullUpdate();
    int AddButton(class NpcButton *);
    virtual int CompareItem(int,int);
    virtual int OnBeforeAddToWndStation();
    virtual int OnAfterRemoveFromWndStation();
    int CreateItems();
    void AddNpcsToEncyclopaedia();
    virtual int RenderItem(int,class PointBase<float> const &,struct m3d::ui::DrawInfo const &);
    NpcList();
    NpcList(class NpcList const &);
    virtual int DeleteItem(int);
private:
    int m_barId;
    NpcList::AuxInfo m_aif;
};

class NpcButton :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * GetBaseClass();
    class ai::Npc * GetNpc() const ;
    virtual class m3d::Object * Clone();
    virtual ~NpcButton();
    int SetUpForNpc(class ai::Npc const *);
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
protected:
    CStr GetNameForNpc(class ai::Npc const *) const ;
    static void __fastcall ClearPattern();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    static int __fastcall LoadPattern(class m3d::ui::Wnd *);
    NpcButton();
    NpcButton(class NpcButton const &);
    int CreateChildren();
    int CreateFromPattern();
private:
    int m_npcId;
    ref_ptr<NpcModelWnd> m_wndNpcIco;
    m3d::ui::ImageWnd *m_wndNpcIcoBg;
    m3d::ui::ButtonWnd *m_btnNpcName;
    m3d::ui::ImageWnd *m_wndNpcNameBg;
};
