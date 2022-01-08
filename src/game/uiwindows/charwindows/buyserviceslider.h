#pragma once

class BuyServiceSlider :  public m3d::ui::SliderWnd
{
public:
    virtual void SetNotch(int);
    virtual void SetMinMax(int,int);
    virtual ~BuyServiceSlider();
    virtual struct m3d::Class * GetClass() const ;
    int GetMaxPossible() const ;
    void SetMaxPossible(int);
    virtual class m3d::Object * Clone();
    static struct m3d::Class * GetBaseClass();
    static class m3d::Object * CreateObject();
protected:
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    BuyServiceSlider(class BuyServiceSlider const &);
    BuyServiceSlider();
private:
    int m_maxPossible;
};
