#pragma once
#include <ui/image.h>

class TwinklingLampWnd :  public m3d::ui::ImageWnd
{
public:
    void SetValue(float,float);
    virtual m3d::Object * Clone();
    void SetTwinklePeriod(unsigned int);
    static m3d::Class * GetBaseClass();
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    static m3d::Object * CreateObject();
    void SetThreshold(float);
    virtual ~TwinklingLampWnd();
    virtual m3d::Class * GetClass() const ;

protected:
    TwinklingLampWnd(TwinklingLampWnd const &);
    TwinklingLampWnd();
    virtual int OnTick(int,int);
    void UpdateLamp();
    bool Visible() const ;
    void Show(bool);
    void Twinkle();

public:
    RT_CLASS_DECLARE(TwinklingLampWnd);

private:
    float m_threshold;
    float m_value;
    float m_maxValue;
    unsigned int m_twinklePeriod;
};
