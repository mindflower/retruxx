#pragma once
#include <ui/button.h>
#include <ui/uilistboxwnd.h>

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}  // namespace m3d

class CheckButton;

class CheckList : public m3d::ui::ListBoxWnd<CheckButton*>
{
public:
    virtual int CreateFromPattern(m3d::ui::Wnd* pattern, bool deleteSrc) /* 0x144 */;
    virtual void SetCurSel(int i) override /* 0x11c */;
    virtual int AddButtonByName(CStr const& name, CStr const& fullName) /* 0x148 */;
    int SelectButtonByName(CStr const& name);

protected:
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override /* 0x134 */;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override /* 0x138 */;
    virtual int DeleteItem(int itemIdx) override /* 0x13c */;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override /* 0x140 */;
    void SelectButton(CheckButton* btn);
    CheckList();
    CheckList(CheckList const& rhs);

public:
    virtual ~CheckList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classCheckList;
}; /* size: 0x0238 */

class CheckButton : public m3d::ui::ButtonWnd
{
public:
    virtual int SetUp(CStr const& name, PointBase<float> const& origin, float width, CStr const& fullName) /* 0x11c */;
    void Select(bool bSelect);
    CStr const& GetName() const;
    CStr const& GetFullName() const;
    BoundsBase<float> const& GetVirtualBounds() const;
    void SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin);
    void SetBounds(BoundsBase<float> const& rect, BoundsBase<float> const& virtualRect);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_icoTexName;
        /* 0x000c */ PointBase<float> m_icoSz;
        /* 0x0014 */ float m_spaceX;
        /* 0x0018 */ float m_spaceY;
        /* 0x001c */ unsigned int m_textColorSel;
        /* 0x0020 */ unsigned int m_textColorUnsel;
        AuxInfo(CheckButton::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0024 */

protected:
    virtual CStr CalcFullName() const /* 0x120 */;
    /* 0x023c */ m3d::ui::ImageWnd* m_ico;
    /* 0x0240 */ m3d::ui::Wnd* m_lbl;
    /* 0x0244 */ CStr m_name;
    /* 0x0250 */ CStr m_fullName;
    static inline CheckButton::AuxInfo m_aif;
    /* 0x025c */ BoundsBase<float> m_virtualBounds;
    CheckButton();
    CheckButton(CheckButton const& rhs);

public:
    virtual ~CheckButton() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classCheckButton;
}; /* size: 0x026c */
