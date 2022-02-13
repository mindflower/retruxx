#pragma once
#include <ui/ui.h>

class CinemaFadePanel : public m3d::ui::Wnd
{
public:
    enum FadeState
    {
        FS_FADING_UP = 0x0,
        FS_FADING_DOWN = 0x1,
        FS_IS_WHITE = 0x2,
        FS_IS_BLACK = 0x3,
    };

public:
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    void AttachToScreenCinematicRelated();
    static m3d::Class * GetBaseClass();
    virtual ~CinemaFadePanel();
    virtual m3d::Object * Clone();

protected:
    void StartFadeCinematicUnrelated(float,int);
    float GetAlpha();
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    CinemaFadePanel();
    CinemaFadePanel(CinemaFadePanel const &);

public:
    RT_CLASS_DECLARE(CinemaFadePanel);

private:
    bool m_isCinematicRelated = true;
    float m_fadePeriod = 0;
    int m_fadeStart = -1;
    float m_curAlpha = 255.0;
    FadeState m_state = FS_IS_WHITE;
    bool m_isFading = false;
};
