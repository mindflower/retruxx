#pragma once

class DurabilityIndicatorWnd :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    virtual int CreateFromPattern(class m3d::ui::Wnd *,bool);
    virtual class m3d::Object * Clone();
    void SetType(enum Type,class CStr const &);
    virtual struct m3d::Class * GetClass() const ;
    void SetVehicleId(int);
    virtual ~DurabilityIndicatorWnd();
protected:
    class m3d::rend::TexHandle GetImageByDurabilityRelValue(float) const ;
    virtual int UpdateOnNewFrame();
    DurabilityIndicatorWnd();
    DurabilityIndicatorWnd(class DurabilityIndicatorWnd const &);
    virtual struct BoundsBase<float> GetRect() const ;
    class ai::Vehicle * GetVehicle() const ;
    void SetDurTextures(class CStr const &,class CStr const &,class CStr const &,class CStr const &);
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    virtual int GameDataUpdate(void *,int);
    virtual int UpdateOnVehiclePartChanged();
    virtual void UpdateTooltip(float,float);
    class ai::VehiclePart * GetVehiclePart() const ;
    void UpdateVehiclePartImage();
    virtual void UpdateValue(bool);
private:
    int m_vehiclePartId;
    int m_vehicleId;
    CStr m_partName;
    m3d::rend::TexHandle m_durTextures[4];
    DurabilityIndicatorWnd::Type m_type;
    CStr m_strCabinDurability;
    CStr m_strBasketDurability;
    float m_prevCurVal;
    float m_prevMaxVal;
};
