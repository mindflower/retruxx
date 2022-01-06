#include <stdexcept>
#include <math/camera.h>

CAffineXForm::CAffineXForm()
{
    throw std::logic_error("Not implemented");
}

void CAffineXForm::createRotationMatrix(CMatrix&) const
{
    throw std::logic_error("Not implemented");
}

void CAffineXForm::createViewMatrix(CMatrix&) const
{
    throw std::logic_error("Not implemented");
}

void CAffineXForm::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

void CAffineXForm::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

void CCamera::createProjectionMatrix(CMatrix&, float) const
{
    throw std::logic_error("Not implemented");
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

CCamera::CCamera()
{
    throw std::logic_error("Not implemented");
}

void CCamera::setFov(float, float, float)
{
    throw std::logic_error("Not implemented");
}
