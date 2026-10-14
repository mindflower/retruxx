#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>
#include <ui/uilistboxwnd.h>

namespace ai
{
    class Bar;
    class Npc;
}  // namespace ai

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class ImageWnd;
        class ModelWnd;
    }  // namespace ui
}  // namespace m3d

class NpcModelWnd;

class NpcButton : public m3d::ui::Wnd
{
    // NpcList owns the shared button pattern and drives Load/ClearPattern.
    friend class NpcList;

public:
    ai::Npc* GetNpc() const;
    int SetUpForNpc(ai::Npc const* npc);

    struct AuxInfo
    {
        CStr m_wndPatternName;
        CStr m_wndIcoName;
        CStr m_wndIcoBgName;
        CStr m_btnNameName;
        CStr m_wndNameBgName;

        AuxInfo(NpcButton::AuxInfo const&);
        AuxInfo();
    };

    struct Pattern
    {
        m3d::ui::Wnd* m_wndPattern;
        m3d::ui::ModelWnd* m_wndPatternIco;
        m3d::ui::ImageWnd* m_wndPatternIcoBg;
        m3d::ui::ButtonWnd* m_btnPatternName;
        m3d::ui::ImageWnd* m_wndPatternNameBg;
        int m_numRefs;
        Pattern();
    };

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    CStr GetNameForNpc(ai::Npc const* npc) const;
    int CreateFromPattern();
    int CreateChildren();
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    static void __fastcall ClearPattern();
    int m_npcId;
    ref_ptr<NpcModelWnd> m_wndNpcIco;
    m3d::ui::ImageWnd* m_wndNpcIcoBg;
    m3d::ui::ButtonWnd* m_btnNpcName;
    m3d::ui::ImageWnd* m_wndNpcNameBg;
    static NpcButton::AuxInfo m_aif;
    static NpcButton::Pattern m_pattern;
    NpcButton();
    NpcButton(NpcButton const& rhs);

public:
    virtual ~NpcButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classNpcButton;
};

class NpcList : public m3d::ui::ListBoxWnd<NpcButton*>
{
public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    int SetUpForBar(int barId);
    ai::Bar* GetBar() const;

    struct AuxInfo
    {
        float m_space;
        AuxInfo();
    };

protected:
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override;
    virtual int DeleteItem(int itemIdx) override;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    int CreateItems();
    int AddButton(NpcButton* btn);
    void FullUpdate();
    void AddNpcsToEncyclopaedia();

    int m_barId;
    NpcList::AuxInfo m_aif;

    NpcList();
    NpcList(NpcList const& rhs);

public:
    virtual ~NpcList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classNpcList;
};
