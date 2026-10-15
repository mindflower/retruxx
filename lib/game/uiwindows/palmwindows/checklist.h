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
    virtual int CreateFromPattern(m3d::ui::Wnd* pattern, bool deleteSrc);
    virtual void SetCurSel(int i) override;
    virtual int AddButtonByName(CStr const& name, CStr const& fullName);
    int SelectButtonByName(CStr const& name);

protected:
    virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override;
    virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override;
    virtual int DeleteItem(int itemIdx) override;
    virtual int CompareItem(int itemIdx0, int itemIdx1) override;
    void SelectButton(CheckButton* btn);
    CheckList();
    CheckList(CheckList const& rhs);

public:
    virtual ~CheckList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classCheckList;
};

class CheckButton : public m3d::ui::ButtonWnd
{
public:
    virtual int SetUp(CStr const& name, PointBase<float> const& origin, float width, CStr const& fullName);
    void Select(bool bSelect);
    CStr const& GetName() const;
    CStr const& GetFullName() const;
    BoundsBase<float> const& GetVirtualBounds() const;
    void SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin);
    void SetBounds(BoundsBase<float> const& rect, BoundsBase<float> const& virtualRect);

    struct AuxInfo
    {
        CStr m_icoTexName;
        PointBase<float> m_icoSz;
        float m_spaceX;
        float m_spaceY;
        unsigned int m_textColorSel;
        unsigned int m_textColorUnsel;
        AuxInfo(CheckButton::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual CStr CalcFullName() const;
    m3d::ui::ImageWnd* m_ico;
    m3d::ui::Wnd* m_lbl;
    CStr m_name;
    CStr m_fullName;
    static inline CheckButton::AuxInfo m_aif;
    BoundsBase<float> m_virtualBounds;
    CheckButton();
    CheckButton(CheckButton const& rhs);

public:
    virtual ~CheckButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classCheckButton;
};
