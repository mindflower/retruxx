#pragma once
#include <ui/button.h>
#include <ui/uilistboxwnd.h>

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}

class CheckButton;

class CheckList :  public m3d::ui::ListBoxWnd<CheckButton *>
{
public:
    int SelectButtonByName(CStr const &);
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual int CreateFromPattern(m3d::ui::Wnd *,bool);
    virtual m3d::Object * Clone();
    virtual int AddButtonByName(CStr const &,CStr const &);
    virtual void SetCurSel(int);
    virtual ~CheckList();
    static m3d::Class * GetBaseClass();

protected:
    virtual int RenderItem(int,PointBase<float> const &,m3d::ui::DrawInfo const &);
    void SelectButton(CheckButton *);
    virtual int MeasureItem(int,BoundsBase<float> &) const ;
    virtual int DeleteItem(int);
    CheckList();
    CheckList(CheckList const &);
    virtual int CompareItem(int,int);

public:
    RT_CLASS_DECLARE(CheckList);
};

class CheckButton :  public m3d::ui::ButtonWnd
{
public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    BoundsBase<float> const & GetVirtualBounds() const ;
    virtual int SetUp(CStr const &,PointBase<float> const &,float,CStr const &);
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    void Select(bool);
    CStr const & GetName() const ;
    CStr const & GetFullName() const ;
    void SetBounds(BoundsBase<float> const &,BoundsBase<float> const &);
    virtual ~CheckButton();

protected:
    CheckButton();
    CheckButton(CheckButton const &);
    virtual CStr CalcFullName() const ;
    virtual void SetBounds(BoundsBase<float> const &,bool);

public:
    RT_CLASS_DECLARE(CheckButton);

private:
    m3d::ui::ImageWnd *m_ico;
    m3d::ui::Wnd *m_lbl;
    CStr m_name;
    CStr m_fullName;
    BoundsBase<float> m_virtualBounds;
};
