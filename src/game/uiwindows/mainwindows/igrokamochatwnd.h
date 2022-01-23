#pragma once
#include <ui/ui.h>

class IgrokaMochatWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_boderNames[4];
    };

public:
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    virtual ~IgrokaMochatWnd();

protected:
    virtual int GameDataClear(bool);
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    void HideBorders();
    void UpdateAlpha();
    void UpdateStartTimes();
    void OnPlayerVehicleDamaged(void *);
    IgrokaMochatWnd(IgrokaMochatWnd const &);
    IgrokaMochatWnd();
    void OnNewFrame();
    void ShowBorder(int,bool);
    std::vector<int, std::allocator<int> > GetBordersByAttackerId(int) const ;
    virtual int OnBeforeAddToWndStation();

public:
    RT_CLASS_DECLARE(IgrokaMochatWnd);

private:
    m3d::rend::TexHandle m_borderTextures[4];
    BoundsBase<float> m_borderBounds[4];
    unsigned __int8 m_borderAlpha[4];
    unsigned int m_borderStartTime[4];
    IgrokaMochatWnd::AuxInfo m_aif;
    float m_integratedDamage;
};
