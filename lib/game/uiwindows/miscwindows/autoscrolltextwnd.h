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
    virtual int SetText(CStr const& caption) override /* 0x48 */;
    virtual void SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin) override /* 0x58 */;
    virtual void SetClientEdges(std::vector<float, std::allocator<float>> const& clientEdges) override /* 0x60 */;
    virtual void SetClientEdges(float left, float top, float right, float bottom) override /* 0x60 */;
    virtual void SetPane(CStr const& name) override /* 0x3c */;
    virtual void SetPaneFlags(int flags) override /* 0x40 */;
    virtual int SetBackground(m3d::rend::TexHandle bgTex) override /* 0x7c */;
    virtual int SetBackground(CStr const& bgTextureName) override /* 0x7c */;
    virtual void SetDefaultFont(CStr const& name, float height, m3d::ui::FontType type, m3d::ui::FontParams params)
        override /* 0xec */;
    virtual void SetDefaultFont(int uiFont) override /* 0xec */;
    void SetWrapMode(m3d::TextWrapFlags wrap);

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x88 */;
    virtual void DrawWndText(m3d::ui::DrawInfo const& di) override /* 0x84 */;
    void HandleScroll();
    bool NeedScroll() const;
    void SetZeroTextOrigin();
    void ScrollTextOrigin();
    bool TextOrignReachedFinalPosition() const;
    void PrecalcTextBounds();
    void PrecalcScrollPixelSpeed();

    /* 0x0220 */ float m_scrollLineSpeed;
    /* 0x0224 */ float m_scrollPixelSpeed;
    /* 0x0228 */ float m_textOrigin;
    /* 0x022c */ bool m_bScrollingEnabled;
    /* 0x022d */ char Padding_294[3];
    /* 0x0230 */ BoundsBase<float> m_textBounds;

    AutoScrollTextWnd();
    AutoScrollTextWnd(AutoScrollTextWnd const& rhs);

public:
    virtual ~AutoScrollTextWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classAutoScrollTextWnd;
}; /* size: 0x0240 */
