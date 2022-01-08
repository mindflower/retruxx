#pragma once

class ItemModelWnd :  public m3d::ui::ModelWnd
{
public:
    bool IsAllowedRotateByHandX() const ;
    bool IsAllowedRotateByHandY() const ;
    virtual class m3d::Object * Clone();
    void AllowRotate(bool);
    static struct m3d::Class * GetBaseClass();
    float GetRotationVelocity() const ;
    virtual ~ItemModelWnd();
    bool IsAutosized() const ;
    void SetRotationVelocity(float);
    static class m3d::Object * CreateObject();
    int SetModelByName(CStr const &,unsigned int,unsigned int);
    virtual int CreateFromPattern(class m3d::ui::Wnd *,bool);
    void SetRotationByHandVelocity(float);
    float GetRotationByHandVelocity() const ;
    void AllowRotateByHandX(bool);
    struct CVector const & GetDefaultTranslation() const ;
    void SetAutosized(bool);
    void AllowRotateByHandY(bool);
    virtual struct m3d::Class * GetClass() const ;
    void SetDefaultRotationAngleX(float);
    void SetDefaultTranslation(struct CVector const &);
    bool IsAllowedRotate() const ;
    float GetDefaultRotationAngleX() const ;
protected:
    virtual int OnAfterRemoveFromWndStation();
    virtual void UpdateCamera();
    bool IsDisabled() const ;
    ItemModelWnd(class ItemModelWnd const &);
    ItemModelWnd();
    virtual int OnMouseButton0(unsigned int,class PointBase<float> const &);
    virtual bool IsValid() const ;
    void SetRotationByHandMode(bool);
    void CalcAutosizeTranslation(struct CVector &) const ;
    virtual int OnBeforeAddToWndStation();
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    virtual int OnMouseMove(class PointBase<float> const &,class PointBase<float> const &);
    virtual int OnNewFrame();
    virtual void UpdateRotationAngle();
private:
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
};
