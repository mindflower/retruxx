#pragma once
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
}

class ElectronicDigitalWnd;

class SpeedometerWnd : public m3d::ui::Wnd
{
public:
    struct AuxInfo
    {
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
        AuxInfo(const SpeedometerWnd::AuxInfo&);
        AuxInfo();
    };

    struct ArrowPointer
    {
        ArrowPointer(const SpeedometerWnd::ArrowPointer&);
        ArrowPointer();
        ~ArrowPointer();
        void Draw(const m3d::ui::DrawInfo& di);
        void SetTexture(m3d::rend::TexHandle tex);
        m3d::rend::TexHandle GetTexture() const;
        PointBase<float> m_coords;
        float m_angle;
        PointBase<float> m_size;
        m3d::rend::TexHandle m_texture;
    };

    enum MotionDir
    {
        DIR_FORWARD = 0,
        DIR_BACKWARD = 1,
        DIR_DEAD = 2,
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    void OnNewFrame();
    virtual int OnPaint(const m3d::ui::DrawInfo& di) override;
    const ai::Vehicle* GetVehicle() const;
    void GetVelocity(float& velocity, SpeedometerWnd::MotionDir& dir) const;
    void UpdateSpeedPointer(float velocity, SpeedometerWnd::MotionDir dir);
    void UpdateSpeedNumber(float velocity, SpeedometerWnd::MotionDir dir);
    void UpdateSpeedNumberColor(SpeedometerWnd::MotionDir dir);
    void UpdateSpeedNumberBgColor(SpeedometerWnd::MotionDir dir);
    SpeedometerWnd::ArrowPointer m_speedPointer;
    ElectronicDigitalWnd* m_wndSpeedNumber;
    SpeedometerWnd::AuxInfo m_aif;
    SpeedometerWnd();
    SpeedometerWnd(const SpeedometerWnd& rhs);

public:
    virtual ~SpeedometerWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classSpeedometerWnd;
};
