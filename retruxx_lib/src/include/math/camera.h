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

class CMatrix;

class CAffineXForm
{
public:
    /* 0x0004 */ CVector m_worldOrigin;
    /* 0x0010 */ float m_rotYaw;
    /* 0x0014 */ float m_rotPitch;
    /* 0x0018 */ float m_rotRoll;
    CAffineXForm(const CAffineXForm&);
    CAffineXForm();
    void createViewMatrix(CMatrix& viewMatrix) const;
    void createRotationMatrix(CMatrix& matRot) const;
    void MoveAlong(const CVector&);
    virtual void LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) /* 0x00 */;
    virtual void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const /* 0x04 */;
}; /* size: 0x001c */

static_assert(sizeof(CAffineXForm) == 0x001c);

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
    /* 0x001c */ float m_fovX;
    /* 0x0020 */ float m_fovY;
    CCamera(const CCamera&);
    CCamera();
    void lookAt(const CVector& Aim, const CVector& Up);
    void lookAt(const CVector& Aim);
    void setFov(float fov, float w, float h);
    void createProjectionMatrix(CMatrix& matProj, float nearOverride) const;
    virtual void LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
    virtual void SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x04 */;
}; /* size: 0x0024 */

static_assert(sizeof(CCamera) == 0x0024);
