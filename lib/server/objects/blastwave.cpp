#include "blastwave.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(BlastWave)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(BlastWave);

    bool BlastWavePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_SPHERE);
            m3d::SafeFloatAttrib(m_WaveForceIntensity, xmlNode, "WaveForceIntensity");
            m3d::SafeFloatAttrib(m_WaveDamageIntensity, xmlNode, "WaveDamageIntensity");
            m3d::SafeStrAttrib(m_EffectName, xmlNode, "Effect");
        }
        return result;
    }

    BlastWavePrototypeInfo::BlastWavePrototypeInfo()
    {
        m_WaveForceIntensity = 0.0;
        m_WaveDamageIntensity = 0.0;
    }

    Obj* BlastWavePrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BlastWavePrototypeInfo::_InternalCopyFrom(ai::PrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BlastWave::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BlastWave::GetPropertiesNames(retruxx::set<CStr>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* BlastWave::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    bool BlastWave::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BlastWave::GetBlastRadius() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* BlastWave::GetClass() const
    {
        return RT_CLASS_LOCAL(BlastWave);
    }

    int BlastWave::GetEmitterId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    short BlastWave::GetRocketExplosionType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int BlastWave::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BlastWavePrototypeInfo const* BlastWave::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr BlastWave::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus BlastWave::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BlastWave::GetDampedDamageIntensity(float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BlastWave::GetWaveDamageIntensity() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BlastWave::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BlastWave::GetWaveForceIntensity() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BlastWave::GetDampedForceIntensity(float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BlastWave::SetEmitterId(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BlastWave::SetRocketExplosionType(short)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BlastWave::SetRocketId(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BlastWave::GetDampingCoefficeient(float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BlastWave::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int BlastWave::CollideBlastWaveAndPhysicObj(BlastWave*, Obj*, dContact*, unsigned&, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BlastWave::BlastWave(BlastWavePrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool BlastWave::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool BlastWave::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BlastWave::~BlastWave()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* BlastWave::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* BlastWave::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
