#pragma once
#include <ui/ui.h>

class CinemaFadePanel : public m3d::ui::Wnd
{
protected:
    CinemaFadePanel();
    CinemaFadePanel(CinemaFadePanel const& rhs);

public:
    virtual ~CinemaFadePanel() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(CinemaFadePanel);

    enum FadeState
    {
        FS_FADING_UP = 0,
        FS_FADING_DOWN = 1,
        FS_IS_WHITE = 2,
        FS_IS_BLACK = 3,
    };

protected:
    bool m_isCinematicRelated;
    float m_fadePeriod;
    int m_fadeStart;
    float m_curAlpha;
    CinemaFadePanel::FadeState m_state;
    bool m_isFading;
    float GetAlpha();
    void StartFadeCinematicUnrelated(float fadePeriod, int direction);
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;

public:
    void FadeUp(float fadePeriod);
    void FadeDown(float fadePeriod);
    void AttachToScreenCinematicRelated();
    bool IsFading();
};
