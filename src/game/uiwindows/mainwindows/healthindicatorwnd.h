#pragma once

class HealthIndicatorWnd :  public m3d::ui::ProgressBarWnd
{
public:
    virtual ~HealthIndicatorWnd();
    void SetType(enum Type);
    virtual class m3d::Object * Clone();
    virtual int CreateFromPattern(class m3d::ui::Wnd *,bool);
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    void SetVehicleId(int);
    virtual struct m3d::Class * GetClass() const ;
protected:
    virtual int GameDataUpdate(void *,int);
    virtual void UpdateBarTexture();
    HealthIndicatorWnd();
    HealthIndicatorWnd(class HealthIndicatorWnd const &);
    class ai::Vehicle * GetVehicle() const ;
    void SetBarTextures(CStr const &,CStr const &,CStr const &);
    virtual int UpdateOnNewFrame();
    virtual void UpdateValue();
    virtual void UpdateTooltip();
private:
    int m_vehicleId;
    m3d::rend::TexHandle m_barTextures[3];
    HealthIndicatorWnd::Type m_type;
    CStr m_strHealth;
};
