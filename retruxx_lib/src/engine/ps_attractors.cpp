#define NOMINMAX
#define _USE_MATH_DEFINES
#include "ps_attractors.h"

#include "particles.h"
#include "retruxx/common.h"

#include <cmath>
#include <core/log.h>

namespace m3d
{
    Attr::Attr(const Attr&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Attr::Attr()
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

    Attr::~Attr()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Attr* __fastcall Attr::New(const CStr& ClassName)
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
    Attr* Attr::Factory(cmn::XmlFile* m_file, ref_ptr<cmn::XmlNode>& attr)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Attr::SetName(CStr name)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    CStr Attr::GetName()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    bool Attr::IsOn()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Attr::On(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    bool Attr::IsWork(float time)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Attr::SetState(float Time)
    {
        // TODO: check and refactor this
        if (!this->m_On)
        {
            this->m_State = OFF;
            return;
        }

        auto v2 = Time - this->m_wtime.m_start;
        if (v2 < 0.0 || this->m_wtime.m_length <= (float)(v2 - (float)((float)(int)(float)(v2 / this->m_wtime.m_repeat) * this->m_wtime.m_repeat)))
        {
            this->m_State = OFF;
            return;
        }
        switch (m_mode)
        {
        case PS_FORCE_ACCEL: this->m_State = ACCELERATION; return;
        case PS_FORCE_VEL: this->m_State = SPEED; return;
        case PS_FORCE_POS: this->m_State = WMPOSITION; break;
        default:
            this->m_State = OFF;
            break;
        }
    }

    void Attr::ReadFromXmlNode(cmn::XmlFile*, ref_ptr<cmn::XmlNode>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Attr::WriteToXmlNode(cmn::XmlFile*, ref_ptr<cmn::XmlNode>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Attr::InitParticle(Particle*, float, CMatrix&, bool, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Attr::InitParticlesList(ParticlesList*, CMatrix&, bool, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Attr::AffectParticle(Particle*, float, CMatrix&, bool, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Attr::AffectParticlesList(ParticlesList*, CMatrix&, bool, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Attr::ReadFromProto(const AttrProps& props)
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

    void Attr::WriteToProto(AttrProps& props)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Attractor::ReadFromXmlNode(cmn::XmlFile* m_file, ref_ptr<cmn::XmlNode>& pattr)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Attractor::WriteToXmlNode(cmn::XmlFile* xmlFile, ref_ptr<cmn::XmlNode>& psroot)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Attractor::ReadFromProto(const AttrProps& props)
    {
        m_min = props.m_min;
        m_max = props.m_max;
        m_freq = props.m_freq;
        m_type = props.m_type;
    }

    void Attractor::WriteToProto(AttrProps& props)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Attractor::InitParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Attractor::InitParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Attractor::AffectParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Attractor::AffectParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Attractor::SetEmitter(TimeMode mode, float emitSt, float emitFin, float emitRpt)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Attractor::SetForce(float posx, float posy, float posz)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Attractor::GetEmitter(TimeMode& mode, float& emitSt, float& emitFin, float& emitRpt) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Attractor::GetForce(float& posx, float& posy, float& posz) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Emitter::Set(float ttlmin, float ttlmax, float max, float emit, float emitSt, float emitFin, float emitRpt)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Emitter::ReadFromXmlNode(ref_ptr<cmn::XmlNode>& emitter)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void Emitter::WriteToXmlNode(ref_ptr<cmn::XmlNode>& emitter)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    bool Emitter::IsWork(float time)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Emitter::LocalStop(m3d::Particle* pParticle, float Time)
    {
        if (!this->m_localStop && Time >= this->m_stopTime && this->m_stopTime != 0.0)
        {
            pParticle->m_vel.x = 0.0;
            pParticle->m_vel.y = 0.0;
            pParticle->m_vel.z = 0.0;
            pParticle->m_rotvel.x = 0.0;
            pParticle->m_rotvel.y = 0.0;
            pParticle->m_rotvel.z = 0.0;
        }
        if (this->m_localStop && (float)(Time - pParticle->m_time0) >= this->m_stopTime && this->m_stopTime != 0.0)
        {
            pParticle->m_vel.x = 0.0;
            pParticle->m_vel.y = 0.0;
            pParticle->m_vel.z = 0.0;
            pParticle->m_rotvel.x = 0.0;
            pParticle->m_rotvel.y = 0.0;
            pParticle->m_rotvel.z = 0.0;
        }
    }

    unsigned int Emitter::Emit(double time, float lastFrameSecs)
    {
        // TODO: generated code Emitter::Emit
        int particlesToEmit = 0;

        // Handle special case for time = 0 with specific wavelength
        if (time == 0.0 && m_wtime.m_length == 0.1f)
        {
            return static_cast<int>(m_emitAtPeriod);
        }

        // Main emission logic for positive time values
        if (time >= 0.0 && m_wtime.m_length != 0.1f)
        {
            // Calculate position within current wave period
            double cycles = time / m_wtime.m_length;
            double frameStart = (cycles - floor(cycles)) * m_wtime.m_length;

            if (frameStart < m_wtime.m_length)
            {
                // Calculate fraction of period covered this frame
                double frameFraction = lastFrameSecs / m_wtime.m_length;
                frameFraction = std::min(frameFraction, 1.0);  // Clamp to 1.0

                // Adjust if we're crossing period boundary
                if (frameStart + frameFraction > m_wtime.m_length)
                {
                    frameFraction = m_wtime.m_length - frameStart;
                }

                // Calculate emission rate (particles per period)
                double particlesPerPeriod = m_wtime.m_length / m_emitAtPeriod;

                if (frameFraction <= particlesPerPeriod)
                {
                    // Check if we crossed an emission boundary
                    unsigned int startInterval = static_cast<unsigned int>(frameStart / particlesPerPeriod);
                    unsigned int endInterval = static_cast<unsigned int>((frameStart + frameFraction) / particlesPerPeriod);

                    if (startInterval != endInterval)
                    {
                        particlesToEmit = 1;
                    }
                }
                else
                {
                    // Emit multiple particles for larger time steps
                    particlesToEmit = static_cast<int>(frameFraction / particlesPerPeriod);
                }
            }
        }

        // Always emit at least one particle at time = 0
        if (time == 0.0 && particlesToEmit == 0)
        {
            particlesToEmit = 1;
        }

        // Check if we're outside the active emission time window
        float timeInRepeatCycle = time - (floor(time / m_wtime.m_repeat) * m_wtime.m_repeat);
        if (time < 0.0 || m_wtime.m_length <= timeInRepeatCycle)
        {
            return 0;
        }

        return particlesToEmit;
    }
    void RotAttractor::ReadFromXmlNode(cmn::XmlFile* m_file, ref_ptr<cmn::XmlNode>& prattr)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RotAttractor::WriteToXmlNode(cmn::XmlFile* xmlFile, ref_ptr<cmn::XmlNode>& psroot)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RotAttractor::InitParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RotAttractor::InitParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RotAttractor::AffectParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RotAttractor::AffectParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void RotAttractor::SetEmitter(TimeMode mode, float emitSt, float emitFin, float emitRpt)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void RotAttractor::SetForce(bool target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void RotAttractor::SetForce(float posx, float posy, float posz)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void RotAttractor::SetForce(ForceMode mode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void RotAttractor::GetEmitter(TimeMode& mode, float& emitSt, float& emitFin, float& emitRpt) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void RotAttractor::GetForce(bool& target) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void RotAttractor::GetForce(float& posx, float& posy, float& posz) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void RotAttractor::GetForce(ForceMode& mode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameAttractor::ReadFromXmlNode(cmn::XmlFile* m_file, ref_ptr<cmn::XmlNode>& gattr)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameAttractor::WriteToXmlNode(cmn::XmlFile* m_file, ref_ptr<cmn::XmlNode>& psroot)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameAttractor::InitParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameAttractor::InitParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameAttractor::AffectParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GameAttractor::AffectParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SAttractor::ReadFromXmlNode(cmn::XmlFile* m_file, ref_ptr<cmn::XmlNode>& sattr)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SAttractor::WriteToXmlNode(cmn::XmlFile* xmlFile, ref_ptr<cmn::XmlNode>& psroot)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SAttractor::InitParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        m_State = this->m_State;
        if (m_State != SPEED || this->m_emitterOn)
        {
            if (m_State == WMPOSITION && !this->m_emitterOn)
            {
                auto forces = m3d::CalcForces(m_csType, m_force, Time, pParticle->m_dir);
                if (Orient)
                {
                    auto v15 = (float)((float)(Local._22 * forces.y) + (float)(Local._32 * forces.z)) + (float)(Local._12 * forces.x);
                    auto v11 = (float)((float)(Local._23 * forces.y) + (float)(Local._33 * forces.z)) + (float)(Local._13 * forces.x);
                    forces.x = (float)((float)(Local._21 * forces.y) + (float)(Local._31 * forces.z)) + (float)(Local._11 * forces.x);
                    forces.y = v15;
                    forces.z = v11;
                }
                pParticle->m_locorigin.x = pParticle->m_locorigin.x + (float)(forces.x * ForceCoeff);
                pParticle->m_locorigin.y = (float)(forces.y * ForceCoeff) + pParticle->m_locorigin.y;
                pParticle->m_locorigin.z = (float)(forces.z * ForceCoeff) + pParticle->m_locorigin.z;
            }
        }
        else
        {
            auto forces = m3d::CalcForces(m_csType, m_force, Time, pParticle->m_dir);
            if (Orient)
            {
                auto v7 = (float)((float)(Local._22 * forces.y) + (float)(Local._12 * forces.x)) + (float)(Local._32 * forces.z);
                auto v8 = (float)((float)(Local._23 * forces.y) + (float)(Local._13 * forces.x)) + (float)(Local._33 * forces.z);
                forces.x = (float)((float)(Local._21 * forces.y) + (float)(Local._31 * forces.z)) + (float)(Local._11 * forces.x);
                forces.y = v7;
                forces.z = v8;
            }
            pParticle->m_vel.y = pParticle->m_vel.y + (float)(forces.y * ForceCoeff);
            pParticle->m_vel.x = pParticle->m_vel.x + (float)(forces.x * ForceCoeff);
            pParticle->m_vel.z = pParticle->m_vel.z + (float)(forces.z * ForceCoeff);
        }
    }

    void SAttractor::InitParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        // TODO: check and refactor this
        if (m_State == SPEED && m_emitterOn)
        {
            auto time = parts->m_time;

            CVector dir = ZeroVector;

            auto forces = CalcForces(m_csType, m_force, time, dir);
            if (Orient)
            {
                auto v6 = (float)((float)(Local._22 * forces.y) + (float)(Local._32 * forces.z)) + (float)(Local._12 * forces.x);
                auto v7 = (float)((float)(Local._23 * forces.y) + (float)(Local._33 * forces.z)) + (float)(Local._13 * forces.x);
                dir.x = (float)((float)(Local._21 * forces.y) + (float)(Local._31 * forces.z)) + (float)(Local._11 * forces.x);
                dir.y = v6;
                dir.z = v7;
                forces.x = dir.x;
                forces.y = v6;
                forces.z = v7;
            }
            parts->m_vel.x = parts->m_vel.x + (float)(forces.x * ForceCoeff);
            parts->m_vel.y = (float)(forces.y * ForceCoeff) + parts->m_vel.y;
            parts->m_vel.z = (float)(forces.z * ForceCoeff) + parts->m_vel.z;
        }
    }

    void SAttractor::AffectParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        if (this->m_State == ACCELERATION && !this->m_emitterOn)
        {
            auto forces = m3d::CalcForces(this->m_csType, m_force, Time, pParticle->m_dir);
            if (Orient)
            {
                auto v6 = (float)((float)(Local._22 * forces.y) + (float)(Local._32 * forces.z)) + (float)(Local._12 * forces.x);
                auto v7 = (float)((float)(Local._23 * forces.y) + (float)(Local._33 * forces.z)) + (float)(Local._13 * forces.x);
                forces.x = (float)((float)(Local._21 * forces.y) + (float)(Local._31 * forces.z)) + (float)(Local._11 * forces.x);
                forces.y = v6;
                forces.z = v7;
            }
            pParticle->m_accel.y = pParticle->m_accel.y + (float)(forces.y * ForceCoeff);
            pParticle->m_accel.x = pParticle->m_accel.x + (float)(forces.x * ForceCoeff);
            pParticle->m_accel.z = pParticle->m_accel.z + (float)(forces.z * ForceCoeff);
        }
    }

    void SAttractor::AffectParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        if (this->m_State == ACCELERATION && this->m_emitterOn)
        {
            auto time = parts->m_time;

            CVector dir = ZeroVector;
            auto forces = m3d::CalcForces(m_csType, m_force, time, dir);
            if (Orient)
            {
                auto v6 = (float)((float)(Local._22 * forces.y) + (float)(Local._32 * forces.z)) + (float)(Local._12 * forces.x);
                auto v7 = (float)((float)(Local._23 * forces.y) + (float)(Local._33 * forces.z)) + (float)(Local._13 * forces.x);
                dir.x = (float)((float)(Local._21 * forces.y) + (float)(Local._31 * forces.z)) + (float)(Local._11 * forces.x);
                dir.y = v6;
                dir.z = v7;
                forces.x = dir.x;
                forces.y = v6;
                forces.z = v7;
            }
            parts->m_accel.x = parts->m_accel.x + (float)(forces.x * ForceCoeff);
            parts->m_accel.y = (float)(forces.y * ForceCoeff) + parts->m_accel.y;
            parts->m_accel.z = (float)(forces.z * ForceCoeff) + parts->m_accel.z;
        }
    }

    void SAttractor::SetEmitter(TimeMode mode, float emitSt, float emitFin, float emitRpt)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void SAttractor::SetForce(bool target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void SAttractor::SetForce(ForceMode mode, CoordinatesSystemType system)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void SAttractor::GetEmitter(TimeMode& mode, float& emitSt, float& emitFin, float& emitRpt) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void SAttractor::GetForce(bool& target) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void SAttractor::GetForce(ForceMode& mode, CoordinatesSystemType& system) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned g_rndSeed = 0;

    unsigned int rndGet()
    {
        g_rndSeed = 214013 * g_rndSeed + 2531011;
        return g_rndSeed;
    }

    float rnd(float a, float b)
    {
        auto v4 = b - a;
        auto ba = (214013 * g_rndSeed + 2531011) >> 16;
        g_rndSeed = 214013 * g_rndSeed + 2531011;
        return v4 * (double)ba * 0.000015259022 + a;
    }

    void CalcForcesCarthesian(CVector& dest, Force const (&forces)[3], float time)
    {
        // TODO: generated code CalcForcesCarthesian
        for (int i = 0; i < 3; ++i)
        {
            const Force& force = (forces)[i];
            float minValue = force.m_min;    // *((float *)p_m_type - 3)
            float maxValue = force.m_max;    // *((float *)p_m_type - 2)
            float frequency = force.m_freq;  // *((float *)p_m_type - 1)
            ForceType type = force.m_type;

            if (type == PS_FORCE_RANDOM)
            {
                // Random force between min and max values
                float randomValue = static_cast<float>(rndGet() >> 16) * 0.000015259022f;  // 1.0f / 65536.0f
                (&dest.x)[i] = (maxValue - minValue) * randomValue + minValue;
            }
            else if (type == PS_FORCE_SINE)
            {
                // Sine wave force
                (&dest.x)[i] = sin(time * frequency) * minValue;
            }
            // Note: Original code didn't handle other force types
        }
    }

    void CalcForcesPolar(CVector& dest, Force const (&forces)[3], float time)
    {
        // TODO: generated code CalcForcesPolar
        CVector force;

        // Calculate force components for each spherical coordinate (radius, theta, phi)
        for (int i = 0; i < 3; ++i)
        {
            const Force& currentForce = (forces)[i];
            float minValue = currentForce.m_min;    // *((float *)p_m_type - 3)
            float maxValue = currentForce.m_max;    // *((float *)p_m_type - 2)
            float frequency = currentForce.m_freq;  // *((float *)p_m_type - 1)
            ForceType type = currentForce.m_type;

            float& outputComponent = (&force.x)[i];

            switch (type)
            {
            case PS_FORCE_RANDOM:
            {
                // Random value between min and max
                float randomFactor = static_cast<float>(rndGet() >> 16) / 65536.0f;
                outputComponent = (maxValue - minValue) * randomFactor + minValue;
                break;
            }

            case PS_FORCE_SINE:
            {
                // Sine wave oscillating between min and max values
                float sineValue = fabs(sin(time * frequency));
                outputComponent = (maxValue - minValue) * sineValue + minValue;
                break;
            }

            default:
                // Handle other force types if needed
                break;
            }
        }

        // Convert spherical coordinates (radius, theta, phi) to Cartesian coordinates (x, y, z)
        // force.x = radius, force.y = theta (azimuth), force.z = phi (inclination)

        if (force.y == 0.0f && force.z == 0.0f)
        {
            // Zero vector case
            dest = CVector(0.0f, 0.0f, 0.0f);
        }
        else
        {
            // Convert degrees to radians
            float thetaRad = force.y * (M_PI / 180.0f);  // azimuth angle
            float phiRad = force.z * (M_PI / 180.0f);    // inclination angle

            // Spherical to Cartesian conversion:
            // x = r * sin(theta) * cos(phi)
            // z = r * cos(theta) * cos(phi)
            // y = r * sin(phi)
            float cosPhi = cos(phiRad);

            dest.x = sin(thetaRad) * cosPhi * force.x;  // radius * sin(azimuth) * cos(inclination)
            dest.z = cos(thetaRad) * cosPhi * force.x;  // radius * cos(azimuth) * cos(inclination)
            dest.y = sin(phiRad) * force.x;             // radius * sin(inclination)
        }
    }

    void CalcForcesPolarOrg(CVector&, Force const (&)[3], CVector const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    CVector CalcForces(CoordinatesSystemType cst, Force const (&forces)[3], float time, CVector const& dir)
    {
        CVector v;
        if (cst)
        {
            if (cst == PS_CST_POLAR)
                m3d::CalcForcesPolar(v, forces, time);
            else
                m3d::CalcForcesPolarOrg(v, forces, dir, time);
        }
        else
        {
            m3d::CalcForcesCarthesian(v, forces, time);
        }
        return v;
    }
}  // namespace m3d