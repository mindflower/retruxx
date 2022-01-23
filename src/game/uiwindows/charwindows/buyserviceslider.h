#pragma once
#include <ui/slider.h>

class BuyServiceSlider :  public m3d::ui::SliderWnd
{
public:
    virtual void SetNotch(int);
    virtual void SetMinMax(int,int);
    virtual ~BuyServiceSlider();
    virtual m3d::Class * GetClass() const ;
    int GetMaxPossible() const ;
    void SetMaxPossible(int);
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    BuyServiceSlider(BuyServiceSlider const &);
    BuyServiceSlider();

public:
    RT_CLASS_DECLARE(BuyServiceSlider);

private:
    int m_maxPossible;
};
