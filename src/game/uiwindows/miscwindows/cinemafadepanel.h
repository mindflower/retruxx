#pragma once

class CinemaFadePanel :  public m3d::ui::Wnd
{
public:
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    void AttachToScreenCinematicRelated();
    static struct m3d::Class * GetBaseClass();
    virtual ~CinemaFadePanel();
    virtual class m3d::Object * Clone();
protected:
    void StartFadeCinematicUnrelated(float,int);
    float GetAlpha();
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    CinemaFadePanel();
    CinemaFadePanel(class CinemaFadePanel const &);
private:
    bool m_isCinematicRelated;
    float m_fadePeriod;
    int m_fadeStart;
    float m_curAlpha;
    CinemaFadePanel::FadeState m_state;
    bool m_isFading;
};
