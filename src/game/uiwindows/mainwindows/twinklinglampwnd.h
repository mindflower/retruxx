#pragma once

class TwinklingLampWnd :  public m3d::ui::ImageWnd
{
public:
    void SetValue(float,float);
    virtual class m3d::Object * Clone();
    void SetTwinklePeriod(unsigned int);
    static struct m3d::Class * __fastcall GetBaseClass();
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    static class m3d::Object * __fastcall CreateObject();
    void SetThreshold(float);
    virtual ~TwinklingLampWnd();
    virtual struct m3d::Class * GetClass() const ;
protected:
    TwinklingLampWnd(class TwinklingLampWnd const &);
    TwinklingLampWnd();
    virtual int OnTick(int,int);
    void UpdateLamp();
    bool Visible() const ;
    void Show(bool);
    void Twinkle();
private:
    float m_threshold;
    float m_value;
    float m_maxValue;
    unsigned int m_twinklePeriod;
};
