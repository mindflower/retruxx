#pragma once
#include <ui/ui.h>

class AutoScrollTextWnd :  public m3d::ui::Wnd
{
public:
    virtual int SetBackground(m3d::rend::TexHandle);
    virtual int SetBackground(CStr const &);
    void StartScroll(bool);
    virtual void SetBounds(BoundsBase<float> const &,bool);
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    virtual void SetClientEdges(std::vector<float, std::allocator<float> > const &);
    virtual void SetClientEdges(float,float,float,float);
    void StopScroll();
    virtual m3d::Object * Clone();
    float GetScrollSpeed() const ;
    void SetWrapMode(m3d::TextWrapFlags);
    virtual int SetText(CStr const &);
    virtual void SetDefaultFont(CStr const &,float, m3d::ui::FontType, m3d::ui::FontParams);
    virtual void SetDefaultFont(int);
    virtual ~AutoScrollTextWnd();
    bool IsScrolling() const ;
    virtual void SetPaneFlags(int);
    virtual void SetPane(CStr const &);
    void SetScrollSpeed(float);
    static m3d::Class * GetBaseClass();

protected:
    virtual void DrawWndText(m3d::ui::DrawInfo const &);
    bool NeedScroll() const ;
    void HandleScroll();
    AutoScrollTextWnd();
    AutoScrollTextWnd(AutoScrollTextWnd const &);
    void SetZeroTextOrigin();
    void PrecalcTextBounds();
    void ScrollTextOrigin();
    void PrecalcScrollPixelSpeed();
    bool TextOrignReachedFinalPosition() const ;
    virtual int OnPaint(m3d::ui::DrawInfo const &);

public:
    RT_CLASS_DECLARE(AutoScrollTextWnd);

private:
    float m_scrollLineSpeed;
    float m_scrollPixelSpeed;
    float m_textOrigin;
    bool m_bScrollingEnabled;
    BoundsBase<float> m_textBounds;
};
