#include "ps_attractors.h"
#include "retruxx/common.h"
#include <core/log.h>

m3d::Attr::Attr(const m3d::Attr&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Attr::Attr()
{
    this->m_org.x = 0.0;
    this->m_org.y = 0.0;
    this->m_org.z = 0.0;
    this->m_wtime.m_start = 0.0;
    this->m_wtime.m_repeat = 4.0;
    this->m_On = 0;
    this->m_timemode = PS_TIME_GLOBAL;
    this->m_mode = PS_FORCE_ACCEL;
    this->m_csType = PS_CST_CARTHESIAN;
    this->m_wtime.m_length = 0.5;
    this->m_emitterOn = 0;
    this->m_interactionType = GI_WIND;
}

m3d::Attr::~Attr()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Attr* __fastcall m3d::Attr::New(const CStr& ClassName)
{
    if (ClassName == "Space")
    {
        return new SAttractor;
    }
    if (ClassName == "Point")
    {
        return new Attractor;
    }
    if (ClassName == "Rotate")
    {
        return new RotAttractor;
    }
    if (ClassName == "Game")
    {
        return new GameAttractor;
    }
    M3D_LOG_INFO("Wrong class name: " + ClassName);
    return nullptr;
}

void m3d::Attr::ReadFromProto(const AttrProps& props)
{
    m_Name = props.m_Name;
    m_ClassName = props.m_ClassName;
    this->m_On = props.m_On;
    this->m_wtime = props.m_wtime;
    this->m_timemode = props.m_timemode;
    this->m_mode = props.m_mode;
    this->m_State = props.m_State;
    this->m_emitterOn = props.m_emitterOn;
    this->m_force[0].m_min = props.m_force[0].m_min;
    this->m_force[0].m_max = props.m_force[0].m_max;
    this->m_force[0].m_freq = props.m_force[0].m_freq;
    this->m_force[0].m_type = props.m_force[0].m_type;
    this->m_force[1] = props.m_force[1];
    this->m_force[2] = props.m_force[2];
    this->m_org = props.m_org;
    this->m_csType = props.m_csType;
    this->m_interactionType = props.m_interactionType;
}

void m3d::Attr::WriteToProto(AttrProps& props)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::Attractor::ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& pattr)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::Attractor::WriteToXmlNode(m3d::cmn::XmlFile* xmlFile, ref_ptr<m3d::cmn::XmlNode>& psroot)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::Attractor::ReadFromProto(const AttrProps& props)
{
    m_min = props.m_min;
    m_max = props.m_max;
    m_freq = props.m_freq;
    m_type = props.m_type;
}

void m3d::Attractor::WriteToProto(AttrProps& props)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::Attractor::InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::Attractor::InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::Attractor::AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::Attractor::AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::RotAttractor::ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& prattr)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::RotAttractor::WriteToXmlNode(m3d::cmn::XmlFile* xmlFile, ref_ptr<m3d::cmn::XmlNode>& psroot)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::RotAttractor::InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::RotAttractor::InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::RotAttractor::AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::RotAttractor::AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::GameAttractor::ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& gattr)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::GameAttractor::WriteToXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& psroot)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::GameAttractor::InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::GameAttractor::InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::GameAttractor::AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::GameAttractor::AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::SAttractor::ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& sattr)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::SAttractor::WriteToXmlNode(m3d::cmn::XmlFile* xmlFile, ref_ptr<m3d::cmn::XmlNode>& psroot)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::SAttractor::InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::SAttractor::InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::SAttractor::AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void m3d::SAttractor::AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
{
    RETRUXX_NOT_IMPLEMENTED;
}
