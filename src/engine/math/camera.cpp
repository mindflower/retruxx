#include <stdexcept>
#include <math/camera.h>

#include "math/matrix.h"

CAffineXForm::CAffineXForm() :
    m_worldOrigin(0.0, 0.0, 0.0)
{
}

void CAffineXForm::createRotationMatrix(CMatrix&) const
{
    throw std::logic_error("Not implemented");
}

void CAffineXForm::createViewMatrix(CMatrix& viewMatrix) const
{
    //TODO: check this and refactor
    float v4; // xmm1_4
    float v5; // xmm0_4
    CMatrix mR; // [esp+8h] [ebp-C0h] BYREF
    CMatrix mT; // [esp+48h] [ebp-80h] BYREF
    CMatrix vv; // [esp+88h] [ebp-40h] BYREF

    mR.rotYPR(m_rotYaw, m_rotPitch, m_rotRoll);
    v4 = 0.0 - m_worldOrigin.x;
    memset(&mT, 0, sizeof(mT));
    mT._41 = v4;
    v5 = 0.0 - m_worldOrigin.z;
    mT._42 = 0.0 - m_worldOrigin.y;
    mT._43 = v5;
    vv._11 = (float)((float)((float)(mR._41 * mT._14) + (float)(mR._31 * mT._13)) + (float)(mR._21 * mT._12)) + mR._11;
    vv._12 = (float)((float)((float)(mR._42 * mT._14) + (float)(mR._32 * mT._13)) + (float)(mR._22 * mT._12)) + mR._12;
    vv._13 = (float)((float)((float)(mR._43 * mT._14) + (float)(mR._33 * mT._13)) + (float)(mR._23 * mT._12)) + mR._13;
    vv._14 = (float)((float)((float)(mR._44 * mT._14) + (float)(mR._34 * mT._13)) + (float)(mR._24 * mT._12)) + mR._14;
    vv._21 = (float)((float)((float)(mT._24 * mR._41) + (float)(mT._23 * mR._31)) + (float)(mT._21 * mR._11)) + mR._21;
    vv._22 = (float)((float)((float)(mT._24 * mR._42) + (float)(mT._23 * mR._32)) + (float)(mT._21 * mR._12)) + mR._22;
    vv._23 = (float)((float)((float)(mT._24 * mR._43) + (float)(mT._23 * mR._33)) + (float)(mT._21 * mR._13)) + mR._23;
    vv._24 = (float)((float)((float)(mT._24 * mR._44) + (float)(mT._23 * mR._34)) + (float)(mT._21 * mR._14)) + mR._24;
    vv._31 = (float)((float)((float)(mT._34 * mR._41) + (float)(mT._32 * mR._21)) + (float)(mT._31 * mR._11)) + mR._31;
    vv._32 = (float)((float)((float)(mT._34 * mR._42) + (float)(mT._32 * mR._22)) + (float)(mT._31 * mR._12)) + mR._32;
    vv._33 = (float)((float)((float)(mT._34 * mR._43) + (float)(mT._32 * mR._23)) + (float)(mT._31 * mR._13)) + mR._33;
    vv._34 = (float)((float)((float)(mT._34 * mR._44) + (float)(mT._32 * mR._24)) + (float)(mT._31 * mR._14)) + mR._34;
    vv._41 = (float)((float)((float)(mR._31 * v5) + (float)(mR._21 * mT._42)) + (float)(v4 * mR._11)) + mR._41;
    vv._42 = (float)((float)((float)(mR._32 * v5) + (float)(mR._22 * mT._42)) + (float)(mR._12 * v4)) + mR._42;
    vv._43 = (float)((float)((float)(mR._33 * v5) + (float)(mR._23 * mT._42)) + (float)(mR._13 * v4)) + mR._43;
    vv._44 = (float)((float)((float)(mR._34 * v5) + (float)(mR._24 * mT._42)) + (float)(mR._14 * v4)) + mR._44;
    mT = vv;
    memcpy(&viewMatrix, &mT, sizeof(CMatrix));
}

void CAffineXForm::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

void CAffineXForm::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

void CCamera::createProjectionMatrix(CMatrix& vv, float f) const
{
    //TODO: check this and refactor
    double v4; // st7
    double v5; // st6
    float v6; // xmm0_4

    v4 = m_fovY * 0.5 * 0.017453292;
    v5 = m_fovX / m_fovY;
    memset(&vv, 0, sizeof(CMatrix));
    v6 = 32000.0 / (float)(32000.0 - f);
    vv._33 = v6;
    vv._43 = 0.0 - (float)(v6 * f);
    vv._34 = 1.0;
    vv._11 = 1.0 / tan(v5 * v4 * 0.5);
    vv._22 = 1.0 / tan(v4 * 0.5);
}

void CCamera::lookAt(CVector const&, CVector const&)
{
    throw std::logic_error("Not implemented");
}

void CCamera::lookAt(CVector const&)
{
    throw std::logic_error("Not implemented");
}

void CCamera::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

void CCamera::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
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
