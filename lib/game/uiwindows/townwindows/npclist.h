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
        /* 0x0000 */ CStr m_wndPatternName;
        /* 0x000c */ CStr m_wndIcoName;
        /* 0x0018 */ CStr m_wndIcoBgName;
        /* 0x0024 */ CStr m_btnNameName;
        /* 0x0030 */ CStr m_wndNameBgName;

        AuxInfo(NpcButton::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x003c */

    struct Pattern
    {
        /* 0x0000 */ m3d::ui::Wnd* m_wndPattern;
        /* 0x0004 */ m3d::ui::ModelWnd* m_wndPatternIco;
        /* 0x0008 */ m3d::ui::ImageWnd* m_wndPatternIcoBg;
        /* 0x000c */ m3d::ui::ButtonWnd* m_btnPatternName;
        /* 0x0010 */ m3d::ui::ImageWnd* m_wndPatternNameBg;
        /* 0x0014 */ int m_numRefs;
        Pattern();
    }; /* size: 0x0018 */

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    CStr GetNameForNpc(ai::Npc const* npc) const;
    int CreateFromPattern();
    int CreateChildren();
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    static void __fastcall ClearPattern();
    /* 0x0220 */ int m_npcId;
    /* 0x0224 */ ref_ptr<NpcModelWnd> m_wndNpcIco;
    /* 0x0228 */ m3d::ui::ImageWnd* m_wndNpcIcoBg;
    /* 0x022c */ m3d::ui::ButtonWnd* m_btnNpcName;
    /* 0x0230 */ m3d::ui::ImageWnd* m_wndNpcNameBg;
    static NpcButton::AuxInfo m_aif;
    static NpcButton::Pattern m_pattern;
    NpcButton();
    NpcButton(NpcButton const& rhs);

public:
    virtual ~NpcButton() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classNpcButton;
}; /* size: 0x0234 */

class NpcList : public m3d::ui::ListBoxWnd<NpcButton*>
{
public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    int SetUpForBar(int barId);
    ai::Bar* GetBar() const;

    struct AuxInfo
    {
        /* 0x0000 */ float m_space;
        AuxInfo();
    }; /* size: 0x0004 */

protected:
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override /* 0x134 */;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override /* 0x138 */;
    virtual int DeleteItem(int itemIdx) override /* 0x13c */;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override /* 0x140 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
    int CreateItems();
    int AddButton(NpcButton* btn);
    void FullUpdate();
    void AddNpcsToEncyclopaedia();

    /* 0x0238 */ int m_barId;
    /* 0x023c */ NpcList::AuxInfo m_aif;

    NpcList();
    NpcList(NpcList const& rhs);

public:
    virtual ~NpcList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classNpcList;
}; /* size: 0x0240 */
