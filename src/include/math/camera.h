#pragma once
#include "vector.h"

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }
}

class CMatrix;

class CAffineXForm
{
public:
    CAffineXForm();
    void createRotationMatrix(CMatrix&) const;
    void createViewMatrix(CMatrix&) const;
    virtual void LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    virtual void SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;

public:
    //CAffineXForm_vtbl* __vftable /*VFT*/;
    CVector m_worldOrigin;
    float m_rotYaw = 0.0;
    float m_rotPitch = 0.0;
    float m_rotRoll = 0.0;
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
    void createProjectionMatrix(CMatrix&, float) const;
    void lookAt(CVector const&, CVector const&);
    void lookAt(CVector const&);
    virtual void LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    virtual void SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
    void setFov(float, float, float);

private:
    float m_fovX = 120.0;
    float m_fovY = 90.0;
};
