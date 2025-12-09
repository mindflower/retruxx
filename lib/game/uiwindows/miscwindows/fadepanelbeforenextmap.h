#pragma once
#include <ui/ui.h>

class FadePanelBeforeNextMap :  public m3d::ui::Wnd
{
public:
    virtual m3d::Class * GetClass() const ;
    virtual ~FadePanelBeforeNextMap();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();

protected:
    FadePanelBeforeNextMap(FadePanelBeforeNextMap const &);
    FadePanelBeforeNextMap();
    unsigned int CalcCurColor() const ;
    void OnPlayerPassToMapFading();
    void UpdateAlpha();
    void StartFading();
    void OnNewFrame();
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    virtual int GameDataUpdate(void *,int);

public:
    RT_CLASS_DECLARE(FadePanelBeforeNextMap);

private:
    float m_alpha;
    unsigned int m_fadeStartTime;
    float m_fadePeriod;
    unsigned int m_blackColor;
};
