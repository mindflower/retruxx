#pragma once

class SpeedometerWnd :  public m3d::ui::Wnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
    static class m3d::Object * __fastcall CreateObject();
    virtual ~SpeedometerWnd();
    static struct m3d::Class * __fastcall GetBaseClass();
protected:
    void UpdateSpeedNumberColor(enum MotionDir);
    SpeedometerWnd(class SpeedometerWnd const &);
    SpeedometerWnd();
    void UpdateSpeedNumber(float,enum MotionDir);
    void OnNewFrame();
    virtual int GameDataUpdate(void *,int);
    void UpdateSpeedNumberBgColor(enum MotionDir);
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    void GetVelocity(float &,enum MotionDir &) const ;
    void UpdateSpeedPointer(float,enum MotionDir);
    virtual int GameDataSetup();
    class ai::Vehicle const * GetVehicle() const ;
private:
    SpeedometerWnd::ArrowPointer m_speedPointer;
    ElectronicDigitalWnd *m_wndSpeedNumber;
    SpeedometerWnd::AuxInfo m_aif;
};
