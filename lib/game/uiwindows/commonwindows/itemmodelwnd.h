#pragma once
#include <ui/modelwnd.h>

class ItemModelWnd :  public m3d::ui::ModelWnd
{
public:
    bool IsAllowedRotateByHandX() const ;
    bool IsAllowedRotateByHandY() const ;
    virtual m3d::Object * Clone();
    void AllowRotate(bool);
    static m3d::Class * GetBaseClass();
    float GetRotationVelocity() const ;
    virtual ~ItemModelWnd();
    bool IsAutosized() const ;
    void SetRotationVelocity(float);
    static m3d::Object * CreateObject();
    int SetModelByName(CStr const &,unsigned int,unsigned int);
    virtual int CreateFromPattern(m3d::ui::Wnd *,bool);
    void SetRotationByHandVelocity(float);
    float GetRotationByHandVelocity() const ;
    void AllowRotateByHandX(bool);
    CVector const & GetDefaultTranslation() const ;
    void SetAutosized(bool);
    void AllowRotateByHandY(bool);
    virtual m3d::Class * GetClass() const ;
    void SetDefaultRotationAngleX(float);
    void SetDefaultTranslation(CVector const &);
    bool IsAllowedRotate() const ;
    float GetDefaultRotationAngleX() const ;

protected:
    virtual int OnAfterRemoveFromWndStation();
    virtual void UpdateCamera();
    bool IsDisabled() const ;
    ItemModelWnd(ItemModelWnd const &);
    ItemModelWnd();
    virtual int OnMouseButton0(unsigned int, PointBase<float> const &);
    virtual bool IsValid() const ;
    void SetRotationByHandMode(bool);
    void CalcAutosizeTranslation(CVector &) const ;
    virtual int OnBeforeAddToWndStation();
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    virtual int OnMouseMove(PointBase<float> const &, PointBase<float> const &);
    virtual int OnNewFrame();
    virtual void UpdateRotationAngle();

public:
    RT_CLASS_DECLARE(ItemModelWnd);

private:
    PointBase<float> m_rotationAngle{-0.52359879, 0.0};
    float m_rotationVelocity = 0.17453294;
    float m_rotationByHandVelocity = 0.017453292;
    bool m_bAllowRotate = true;
    bool m_bAllowRotateByHandX = true;
    bool m_bAllowRotateByHandY = true;
    bool m_bInRotationByHandMode;
    float m_defaultRotationAngleX = -0.52359879;
    bool m_bAutosized = true;
    CVector m_defaultTranslation{0.0, 0.0, 0.0};
};
