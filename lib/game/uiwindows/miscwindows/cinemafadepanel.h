#pragma once
#include <ui/ui.h>

class CinemaFadePanel : public m3d::ui::Wnd
{
protected:
    CinemaFadePanel();
    CinemaFadePanel(CinemaFadePanel const& rhs);

public:
    virtual ~CinemaFadePanel() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(CinemaFadePanel);

    enum FadeState
    {
        FS_FADING_UP = 0,
        FS_FADING_DOWN = 1,
        FS_IS_WHITE = 2,
        FS_IS_BLACK = 3,
    };

protected:
    /* 0x0220 */ bool m_isCinematicRelated;
    /* 0x0224 */ float m_fadePeriod;
    /* 0x0228 */ int m_fadeStart;
    /* 0x022c */ float m_curAlpha;
    /* 0x0230 */ CinemaFadePanel::FadeState m_state;
    /* 0x0234 */ bool m_isFading;
    float GetAlpha();
    void StartFadeCinematicUnrelated(float fadePeriod, int direction);
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x88 */;

public:
    void FadeUp(float fadePeriod);
    void FadeDown(float fadePeriod);
    void AttachToScreenCinematicRelated();
    bool IsFading();
}; /* size: 0x0238 */
