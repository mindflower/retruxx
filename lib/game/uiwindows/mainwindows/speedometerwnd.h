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
        /* 0x0000 */ CStr m_wndSpeedNumberName;
        /* 0x000c */ CStr m_speedPointerTextureName;
        /* 0x0018 */ PointBase<float> m_speedPointerSize;
        /* 0x0020 */ PointBase<float> m_speedPointerCenter;
        /* 0x0028 */ float m_speedPointerMinAngle;
        /* 0x002c */ float m_speedPointerMaxAngle;
        /* 0x0030 */ float m_speedPointerMaxAngleBackward;
        /* 0x0034 */ float m_speedPointerRadius;
        /* 0x0038 */ unsigned int m_colorForvard;
        /* 0x003c */ unsigned int m_colorBackward;
        AuxInfo(const SpeedometerWnd::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0040 */

    struct ArrowPointer
    {
        ArrowPointer(const SpeedometerWnd::ArrowPointer&);
        ArrowPointer();
        ~ArrowPointer();
        void Draw(const m3d::ui::DrawInfo& di);
        void SetTexture(m3d::rend::TexHandle tex);
        m3d::rend::TexHandle GetTexture() const;
        /* 0x0000 */ PointBase<float> m_coords;
        /* 0x0008 */ float m_angle;
        /* 0x000c */ PointBase<float> m_size;
        /* 0x0014 */ m3d::rend::TexHandle m_texture;
    }; /* size: 0x0018 */

    enum MotionDir
    {
        DIR_FORWARD = 0,
        DIR_BACKWARD = 1,
        DIR_DEAD = 2,
    };

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    void OnNewFrame();
    virtual int OnPaint(const m3d::ui::DrawInfo& di) override /* 0x88 */;
    const ai::Vehicle* GetVehicle() const;
    void GetVelocity(float& velocity, SpeedometerWnd::MotionDir& dir) const;
    void UpdateSpeedPointer(float velocity, SpeedometerWnd::MotionDir dir);
    void UpdateSpeedNumber(float velocity, SpeedometerWnd::MotionDir dir);
    void UpdateSpeedNumberColor(SpeedometerWnd::MotionDir dir);
    void UpdateSpeedNumberBgColor(SpeedometerWnd::MotionDir dir);
    /* 0x0220 */ SpeedometerWnd::ArrowPointer m_speedPointer;
    /* 0x0238 */ ElectronicDigitalWnd* m_wndSpeedNumber;
    /* 0x023c */ SpeedometerWnd::AuxInfo m_aif;
    SpeedometerWnd();
    SpeedometerWnd(const SpeedometerWnd& rhs);

public:
    virtual ~SpeedometerWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classSpeedometerWnd;
}; /* size: 0x027c */
