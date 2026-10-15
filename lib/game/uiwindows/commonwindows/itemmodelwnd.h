#pragma once
#include <ui/modelwnd.h>

class ItemModelWnd : public m3d::ui::ModelWnd
{
public:
    virtual int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
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
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override;
    virtual int OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    virtual int OnNewFrame();
    virtual void UpdateCamera();
    virtual void UpdateRotationAngle();
    virtual bool IsValid() const;
    bool IsDisabled() const;
    void SetRotationByHandMode(bool bState);
    void CalcAutosizeTranslation(CVector& translation) const;

    PointBase<float> m_rotationAngle;
    float m_rotationVelocity;
    float m_rotationByHandVelocity;
    bool m_bAllowRotate;
    bool m_bAllowRotateByHandX;
    bool m_bAllowRotateByHandY;
    bool m_bInRotationByHandMode;
    float m_defaultRotationAngleX;
    bool m_bAutosized;
    CVector m_defaultTranslation;
    ItemModelWnd();
    ItemModelWnd(ItemModelWnd const& rhs);

public:
    virtual ~ItemModelWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classItemModelWnd;
};
