#include "turboaccelerationpusher.h"

#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(TurboAccelerationPusher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(TurboAccelerationPusher);

    TurboAccelerationPusherPrototypeInfo::TurboAccelerationPusherPrototypeInfo()
    {
        m_AccelerationValue = 1.0f;
        m_AccelerationTime = 0.0f;
    }

    bool TurboAccelerationPusherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = GunPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_AccelerationValue, xmlNode, "AccelerationValue");
            m3d::SafeFloatAttrib(m_AccelerationTime, xmlNode, "AccelerationTime");
        }
        return result;
    }

    Obj* TurboAccelerationPusherPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TurboAccelerationPusher::TurboAccelerationPusher(TurboAccelerationPusherPrototypeInfo const& prototype) : Gun(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* TurboAccelerationPusher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    m3d::Class* TurboAccelerationPusher::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TurboAccelerationPusherPrototypeInfo const* TurboAccelerationPusher::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TurboAccelerationPusher::_LaunchShells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TurboAccelerationPusher::~TurboAccelerationPusher()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* TurboAccelerationPusher::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool TurboAccelerationPusher::isLookAtPoint(CVector const&, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* TurboAccelerationPusher::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
