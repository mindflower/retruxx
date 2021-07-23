#pragma once

class IgrokaMochatWnd :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual class m3d::Object * Clone();
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~IgrokaMochatWnd();
protected:
    virtual int GameDataClear(bool);
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    void HideBorders();
    void UpdateAlpha();
    void UpdateStartTimes();
    void OnPlayerVehicleDamaged(void *);
    IgrokaMochatWnd(class IgrokaMochatWnd const &);
    IgrokaMochatWnd();
    void OnNewFrame();
    void ShowBorder(int,bool);
    class std::vector<int,class std::allocator<int> > GetBordersByAttackerId(int) const ;
    virtual int OnBeforeAddToWndStation();
private:
    m3d::rend::TexHandle m_borderTextures[4];
    BoundsBase<float> m_borderBounds[4];
    unsigned __int8 m_borderAlpha[4];
    unsigned int m_borderStartTime[4];
    IgrokaMochatWnd::AuxInfo m_aif;
    float m_integratedDamage;
};
