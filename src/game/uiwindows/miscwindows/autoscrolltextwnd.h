#pragma once

class AutoScrollTextWnd :  public m3d::ui::Wnd
{
public:
    virtual int SetBackground(class m3d::rend::TexHandle);
    virtual int SetBackground(CStr const &);
    void StartScroll(bool);
    virtual void SetBounds(struct BoundsBase<float> const &,bool);
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    virtual void SetClientEdges(class std::vector<float,class std::allocator<float> > const &);
    virtual void SetClientEdges(float,float,float,float);
    void StopScroll();
    virtual class m3d::Object * Clone();
    float GetScrollSpeed() const ;
    void SetWrapMode(enum m3d::TextWrapFlags);
    virtual int SetText(CStr const &);
    virtual void SetDefaultFont(CStr const &,float,enum m3d::ui::FontType,union m3d::ui::FontParams);
    virtual void SetDefaultFont(int);
    virtual ~AutoScrollTextWnd();
    bool IsScrolling() const ;
    virtual void SetPaneFlags(int);
    virtual void SetPane(CStr const &);
    void SetScrollSpeed(float);
    static struct m3d::Class * __fastcall GetBaseClass();
protected:
    virtual void DrawWndText(struct m3d::ui::DrawInfo const &);
    bool NeedScroll() const ;
    void HandleScroll();
    AutoScrollTextWnd();
    AutoScrollTextWnd(class AutoScrollTextWnd const &);
    void SetZeroTextOrigin();
    void PrecalcTextBounds();
    void ScrollTextOrigin();
    void PrecalcScrollPixelSpeed();
    bool TextOrignReachedFinalPosition() const ;
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
private:
    float m_scrollLineSpeed;
    float m_scrollPixelSpeed;
    float m_textOrigin;
    bool m_bScrollingEnabled;
    BoundsBase<float> m_textBounds;
};
