#pragma once
#include "vector.h"

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }
}

struct CMatrix;

class CAffineXForm
{
public:
    CVector m_worldOrigin;
    float m_rotYaw;
    float m_rotPitch;
    float m_rotRoll;
    CAffineXForm(const CAffineXForm&);
    CAffineXForm();
    void createViewMatrix(CMatrix& viewMatrix) const;
    void createRotationMatrix(CMatrix& matRot) const;
    void MoveAlong(const CVector&);
    virtual void LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    virtual void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
};

enum CameraModes
{
    CM_FIRST_NOTUSED = 0x0,
    CM_BUMPER = 0x1,
    CM_FOLLOWMODE = 0x2,
    CM_FLYCAMERA = 0x3,
    CM_CONST = 0x4,
    CM_LAST = 0x5,
};

class CCamera : public CAffineXForm
{
public:
    float m_fovX;
    float m_fovY;
    CCamera(const CCamera&);
    CCamera();
    void lookAt(const CVector& Aim, const CVector& Up);
    void lookAt(const CVector& Aim);
    void setFov(float fov, float w, float h);
    void createProjectionMatrix(CMatrix& matProj, float nearOverride) const;
    virtual void LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
    virtual void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
};

