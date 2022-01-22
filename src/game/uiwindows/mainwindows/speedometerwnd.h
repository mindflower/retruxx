#pragma once
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
}

class ElectronicDigitalWnd;

class SpeedometerWnd :  public m3d::ui::Wnd
{
public:
    class ArrowPointer
    {
    public:
        m3d::rend::TexHandle GetTexture() const;
        void SetTexture(m3d::rend::TexHandle);
        ~ArrowPointer();
        ArrowPointer();
        void Draw(m3d::ui::DrawInfo const&);

    private:
        PointBase<float> m_coords;
        float m_angle;
        PointBase<float> m_size;
        m3d::rend::TexHandle m_texture;
    };

    enum MotionDir
    {
        DIR_FORWARD = 0x0,
        DIR_BACKWARD = 0x1,
        DIR_DEAD = 0x2,
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndSpeedNumberName;
        CStr m_speedPointerTextureName;
        PointBase<float> m_speedPointerSize;
        PointBase<float> m_speedPointerCenter;
        float m_speedPointerMinAngle;
        float m_speedPointerMaxAngle;
        float m_speedPointerMaxAngleBackward;
        float m_speedPointerRadius;
        unsigned int m_colorForvard;
        unsigned int m_colorBackward;
    };

public:
    virtual m3d::Class * GetClass() const ;
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    virtual ~SpeedometerWnd();
    static m3d::Class * GetBaseClass();

protected:
    void UpdateSpeedNumberColor(MotionDir);
    SpeedometerWnd(SpeedometerWnd const &);
    SpeedometerWnd();
    void UpdateSpeedNumber(float,MotionDir);
    void OnNewFrame();
    virtual int GameDataUpdate(void *,int);
    void UpdateSpeedNumberBgColor(MotionDir);
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    void GetVelocity(float &,MotionDir &) const ;
    void UpdateSpeedPointer(float,MotionDir);
    virtual int GameDataSetup();
    ai::Vehicle const * GetVehicle() const ;

public:
    RT_CLASS_DECLARE(SpeedometerWnd);

private:
    SpeedometerWnd::ArrowPointer m_speedPointer;
    ElectronicDigitalWnd *m_wndSpeedNumber;
    SpeedometerWnd::AuxInfo m_aif;
};
