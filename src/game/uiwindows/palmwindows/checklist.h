#pragma once

class CheckList :  public m3d::ui::ListBoxWnd<CheckButton *>
{
public:
    int SelectButtonByName(class CStr const &);
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    virtual int CreateFromPattern(class m3d::ui::Wnd *,bool);
    virtual class m3d::Object * Clone();
    virtual int AddButtonByName(class CStr const &,class CStr const &);
    virtual void SetCurSel(int);
    virtual ~CheckList();
    static struct m3d::Class * __fastcall GetBaseClass();
protected:
    virtual int RenderItem(int,class PointBase<float> const &,struct m3d::ui::DrawInfo const &);
    void SelectButton(class CheckButton *);
    virtual int MeasureItem(int,struct BoundsBase<float> &) const ;
    virtual int DeleteItem(int);
    CheckList();
    CheckList(class CheckList const &);
    virtual int CompareItem(int,int);
private:
};

class CheckButton :  public m3d::ui::ButtonWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    struct BoundsBase<float> const & GetVirtualBounds() const ;
    virtual int SetUp(class CStr const &,class PointBase<float> const &,float,class CStr const &);
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    void Select(bool);
    class CStr const & GetName() const ;
    class CStr const & GetFullName() const ;
    void SetBounds(struct BoundsBase<float> const &,struct BoundsBase<float> const &);
    virtual ~CheckButton();
protected:
    CheckButton();
    CheckButton(class CheckButton const &);
    virtual class CStr CalcFullName() const ;
    virtual void SetBounds(struct BoundsBase<float> const &,bool);
private:
    m3d::ui::ImageWnd *m_ico;
    m3d::ui::Wnd *m_lbl;
    CStr m_name;
    CStr m_fullName;
    BoundsBase<float> m_virtualBounds;
};
