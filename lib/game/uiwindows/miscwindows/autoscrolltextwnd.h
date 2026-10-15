#pragma once
#include <ui/ui.h>

class CinemaPanel;

class AutoScrollTextWnd : public m3d::ui::Wnd
{
    friend class CinemaPanel;

public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    void StartScroll(bool bFromCurrentPosition);
    void StopScroll();
    bool IsScrolling() const;
    void SetScrollSpeed(float speed);
    float GetScrollSpeed() const;
    virtual int SetText(CStr const& caption) override;
    virtual void SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin) override;
    virtual void SetClientEdges(std::vector<float, std::allocator<float>> const& clientEdges) override;
    virtual void SetClientEdges(float left, float top, float right, float bottom) override;
    virtual void SetPane(CStr const& name) override;
    virtual void SetPaneFlags(int flags) override;
    virtual int SetBackground(m3d::rend::TexHandle bgTex) override;
    virtual int SetBackground(CStr const& bgTextureName) override;
    virtual void SetDefaultFont(CStr const& name, float height, m3d::ui::FontType type, m3d::ui::FontParams params)
        override;
    virtual void SetDefaultFont(int uiFont) override;
    void SetWrapMode(m3d::TextWrapFlags wrap);

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    virtual void DrawWndText(m3d::ui::DrawInfo const& di) override;
    void HandleScroll();
    bool NeedScroll() const;
    void SetZeroTextOrigin();
    void ScrollTextOrigin();
    bool TextOrignReachedFinalPosition() const;
    void PrecalcTextBounds();
    void PrecalcScrollPixelSpeed();

    float m_scrollLineSpeed;
    float m_scrollPixelSpeed;
    float m_textOrigin;
    bool m_bScrollingEnabled;
    BoundsBase<float> m_textBounds;

    AutoScrollTextWnd();
    AutoScrollTextWnd(AutoScrollTextWnd const& rhs);

public:
    virtual ~AutoScrollTextWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classAutoScrollTextWnd;
};
