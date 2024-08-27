#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>
#include <ui/uilistboxwnd.h>

namespace ai
{
    class Bar;
    class Npc;
}

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class ImageWnd;
    }
}

class NpcModelWnd;

class NpcButton : public m3d::ui::Wnd
{
public:
    static m3d::Class* GetBaseClass();
    ai::Npc* GetNpc() const;
    virtual m3d::Object* Clone();
    virtual ~NpcButton();
    int SetUpForNpc(ai::Npc const*);
    static m3d::Object* CreateObject();
    virtual m3d::Class* GetClass() const;

protected:
    CStr GetNameForNpc(ai::Npc const*) const;
    static void __fastcall ClearPattern();
    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    static int __fastcall LoadPattern(m3d::ui::Wnd*);
    NpcButton();
    NpcButton(NpcButton const&);
    int CreateChildren();
    int CreateFromPattern();

public:
    RT_CLASS_DECLARE(NpcButton);

private:
    int m_npcId;
    ref_ptr<NpcModelWnd> m_wndNpcIco;
    m3d::ui::ImageWnd* m_wndNpcIcoBg;
    m3d::ui::ButtonWnd* m_btnNpcName;
    m3d::ui::ImageWnd* m_wndNpcNameBg;
};

class NpcList :  public m3d::ui::ListBoxWnd<NpcButton *>
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        float m_space;
    };

public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    ai::Bar * GetBar() const ;
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    virtual int GameDataClear(bool);
    int SetUpForBar(int);
    virtual m3d::Object * Clone();
    virtual ~NpcList();
    static m3d::Class * GetBaseClass();

protected:
    virtual int OnAfterAddToWndStation();
    virtual int MeasureItem(int, BoundsBase<float> &) const ;
    void FullUpdate();
    int AddButton(NpcButton *);
    virtual int CompareItem(int,int);
    virtual int OnBeforeAddToWndStation();
    virtual int OnAfterRemoveFromWndStation();
    int CreateItems();
    void AddNpcsToEncyclopaedia();
    virtual int RenderItem(int, PointBase<float> const &, m3d::ui::DrawInfo const &);
    NpcList();
    NpcList(NpcList const &);
    virtual int DeleteItem(int);

public:
    RT_CLASS_DECLARE(NpcList);

private:
    int m_barId;
    NpcList::AuxInfo m_aif;
};
