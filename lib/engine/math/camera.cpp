#include <stdexcept>
#include <math/camera.h>

#include "math/matrix.h"
#include "retruxx/common.h"
#include <core/ini.h>

#include <cmath>
#include <cstring>

CAffineXForm::CAffineXForm() :
    m_worldOrigin(0.0, 0.0, 0.0),
    m_rotRoll(0.0),
    m_rotPitch(0.0),
    m_rotYaw(0.0)
{
}

void CAffineXForm::createRotationMatrix(CMatrix& matRot) const
{
    // RVA 0x406090
    matRot.rotYPR(m_rotYaw, m_rotPitch, m_rotRoll);
}

void CAffineXForm::createViewMatrix(CMatrix& viewMatrix) const
{
    // RVA 0x41D880 - the rotation preceded by the translation to the origin, i.e. T(-origin) * R.
    // The original multiplies by a zero-filled translation matrix, so only the fourth row picks
    // anything up; it is computed here in the same order.
    CMatrix mR;
    mR.rotYPR(m_rotYaw, m_rotPitch, m_rotRoll);

    float const tx = 0.0f - m_worldOrigin.x;
    float const ty = 0.0f - m_worldOrigin.y;
    float const tz = 0.0f - m_worldOrigin.z;

    CMatrix vv = mR;
    vv._41 = mR._31 * tz + mR._21 * ty + tx * mR._11 + mR._41;
    vv._42 = mR._32 * tz + mR._22 * ty + mR._12 * tx + mR._42;
    vv._43 = mR._33 * tz + mR._23 * ty + mR._13 * tx + mR._43;
    vv._44 = mR._34 * tz + mR._24 * ty + mR._14 * tx + mR._44;
    viewMatrix = vv;
}

void CAffineXForm::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const* xmlNode)
{
    // RVA 0x59C030
    m3d::SafeVectorAttrib(m_worldOrigin, xmlNode, "WorldOrigin");
    m3d::SafeFloatAttrib(m_rotYaw, xmlNode, "RotYaw");
    m3d::SafeFloatAttrib(m_rotPitch, xmlNode, "RotPitch");
    m3d::SafeFloatAttrib(m_rotRoll, xmlNode, "RotRoll");
}

void CAffineXForm::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode* xmlNode) const
{
    // RVA 0x59C0D0
    xmlNode->SetAttribute("WorldOrigin", CStr(m_worldOrigin).c_str());
    xmlNode->SetAttribute("RotYaw", CStr(m_rotYaw).c_str());
    xmlNode->SetAttribute("RotPitch", CStr(m_rotPitch).c_str());
    xmlNode->SetAttribute("RotRoll", CStr(m_rotRoll).c_str());
}

void CCamera::createProjectionMatrix(CMatrix& vv, float f) const
{
    // RVA 0x41D770 - a left-handed perspective projection with the near plane at f and the far
    // plane fixed at 32000. NOTE: the half angle is halved again inside tan, so the projection
    // actually spans half of m_fovX by half of m_fovY.
    double const halfFovY = m_fovY * 0.5 * 0.017453292;
    double const aspect = m_fovX / m_fovY;
    std::memset(&vv, 0, sizeof(CMatrix));
    float const q = 32000.0f / (32000.0f - f);
    vv._33 = q;
    vv._43 = 0.0f - q * f;
    vv._34 = 1.0f;
    vv._11 = static_cast<float>(1.0 / std::tan(aspect * halfFovY * 0.5));
    vv._22 = static_cast<float>(1.0 / std::tan(halfFovY * 0.5));
}

CCamera::CCamera() :
    m_fovX(120.0),
    m_fovY(90.0)
{
}

void CCamera::lookAt(CVector const& aim, CVector const& up)
{
    // RVA 0x405B50
    CMatrix m;
    m.lookAtLH(m_worldOrigin, aim, up);
    m.getYPR(m_rotYaw, m_rotPitch, m_rotRoll);
}

void CCamera::lookAt(CVector const& aim)
{
    CVector up; // [esp+0h] [ebp-4Ch] BYREF
    CMatrix v4;

    up.x = 0.0;
    up.y = 1.0;
    up.z = 0.0;
    v4.lookAtLH(this->m_worldOrigin, aim, up);
    v4.getYPR(this->m_rotYaw, this->m_rotPitch, this->m_rotRoll);
}

void CCamera::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    // RVA 0x59ED90
    CAffineXForm::LoadFromXml(xmlFile, xmlNode);
    m3d::SafeFloatAttrib(m_fovX, xmlNode, "FovX");
    m3d::SafeFloatAttrib(m_fovY, xmlNode, "FovY");
}

void CCamera::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
{
    // RVA 0x59EE00
    CAffineXForm::SaveToXml(xmlFile, xmlNode);
    xmlNode->SetAttribute("FovX", CStr(m_fovX).c_str());
    xmlNode->SetAttribute("FovY", CStr(m_fovY).c_str());
}

void CCamera::setFov(float fov, float w, float h)
{
    auto v4 = fov;
    if (w <= h)
    {
        this->m_fovX = fov;
        v4 = (fov / h) * w;
    }
    else
    {
        this->m_fovX = (fov / h) * w;
    }
    this->m_fovY = v4;
}
