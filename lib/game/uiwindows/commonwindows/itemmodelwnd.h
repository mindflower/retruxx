#pragma once
#include <ui/modelwnd.h>

class ItemModelWnd : public m3d::ui::ModelWnd
{
public:
    virtual int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc) /* 0x12c */;
    int SetModelByName(CStr const& modelName, unsigned int skinNumber, unsigned int cfgNumber);
    void AllowRotate(bool bAllow);
    bool IsAllowedRotate() const;
    void SetRotationVelocity(float velocity);
    float GetRotationVelocity() const;
    void AllowRotateByHandX(bool bAllow);
    void AllowRotateByHandY(bool bAllow);
    bool IsAllowedRotateByHandX() const;
    bool IsAllowedRotateByHandY() const;
    void SetRotationByHandVelocity(float velocity);
    float GetRotationByHandVelocity() const;
    void SetDefaultRotationAngleX(float angle);
    float GetDefaultRotationAngleX() const;
    void SetAutosized(bool bAutosized);
    bool IsAutosized() const;
    void SetDefaultTranslation(CVector const& translation);
    CVector const& GetDefaultTranslation() const;

protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override /* 0x00 */;
    virtual int OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas) override /* 0x00 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
    virtual int OnNewFrame() /* 0x130 */;
    virtual void UpdateCamera() /* 0x134 */;
    virtual void UpdateRotationAngle() /* 0x138 */;
    virtual bool IsValid() const /* 0x13c */;
    bool IsDisabled() const;
    void SetRotationByHandMode(bool bState);
    void CalcAutosizeTranslation(CVector& translation) const;

    /* 0x0294 */ PointBase<float> m_rotationAngle;
    /* 0x029c */ float m_rotationVelocity;
    /* 0x02a0 */ float m_rotationByHandVelocity;
    /* 0x02a4 */ bool m_bAllowRotate;
    /* 0x02a5 */ bool m_bAllowRotateByHandX;
    /* 0x02a6 */ bool m_bAllowRotateByHandY;
    /* 0x02a7 */ bool m_bInRotationByHandMode;
    /* 0x02a8 */ float m_defaultRotationAngleX;
    /* 0x02ac */ bool m_bAutosized;
    /* 0x02ad */ char Padding_227[3];
    /* 0x02b0 */ CVector m_defaultTranslation;
    ItemModelWnd();
    ItemModelWnd(ItemModelWnd const& rhs);

public:
    virtual ~ItemModelWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classItemModelWnd;
}; /* size: 0x02bc */
