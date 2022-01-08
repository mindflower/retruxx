#pragma once

class DurabilityIndicatorInMainInterfaceWnd :  public m3d::ui::ImageWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    virtual int CreateFromPattern(class m3d::ui::Wnd *,bool);
    void SetType(enum Type,CStr const &);
    virtual class m3d::Object * Clone();
    void SetVehicleId(int);
    virtual ~DurabilityIndicatorInMainInterfaceWnd();
    static struct m3d::Class * GetBaseClass();
protected:
    unsigned int GetColorByValue(float,float) const ;
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    class ai::Vehicle const * GetVehicle() const ;
    DurabilityIndicatorInMainInterfaceWnd();
    DurabilityIndicatorInMainInterfaceWnd(class DurabilityIndicatorInMainInterfaceWnd const &);
    virtual void UpdateTooltip(float,float);
    void OnVehiclePartChanged(void *);
    void GetValue(float &,float &) const ;
    void FullUpdate(bool);
    void OnNewFrame();
    void UpdateColor(float,float);
    class ai::VehiclePart const * GetVehiclePart() const ;
private:
    int m_vehiclePartId;
    int m_vehicleId;
    CStr m_partName;
    DurabilityIndicatorInMainInterfaceWnd::Type m_type;
    CStr m_strCabinDurability;
    CStr m_strBasketDurability;
    float m_prevCurVal;
    float m_prevMaxVal;
};
