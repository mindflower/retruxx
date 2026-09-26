#define NOMINMAX
#define _USE_MATH_DEFINES
#include "ps_attractors.h"

#include "particles.h"
#include "retruxx/common.h"

#include <cmath>
#include <core/log.h>
#include <core/ini.h>
#include <client.h>
#include <world.h>
#include <cstdio>
#include <cstring>

namespace m3d
{
    Attr::Attr(const Attr& other) :
        m_Name(other.m_Name),
        m_ClassName(other.m_ClassName),
        m_On(other.m_On),
        m_wtime(other.m_wtime),
        m_timemode(other.m_timemode),
        m_mode(other.m_mode),
        m_State(other.m_State),
        m_emitterOn(other.m_emitterOn),
        m_org(other.m_org),
        m_csType(other.m_csType),
        m_interactionType(other.m_interactionType)
    {
        // NOTE: declared in the PDB but never emitted; taken to be the member-wise copy.
        for (int i = 0; i < 3; ++i)
        {
            m_force[i] = other.m_force[i];
        }
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
        // RVA 0x950740
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
        // RVA 0x954660 - an attractor of the node's class, read from it. NOTE: an unknown class is not checked for,
        // so the null returned by New is used.
        CStr const ClassName(attr->GetAttribute("Class"));
        Attr* const result = New(ClassName);
        result->ReadFromXmlNode(m_file, attr);
        result->m_ClassName = ClassName;
        return result;
    }
    void Attr::SetName(CStr name)
    {
        // RVA 0x950790
        m_Name = name;
    }
    CStr Attr::GetName()
    {
        // NOTE: declared in the PDB but never emitted.
        return m_Name;
    }
    bool Attr::IsOn()
    {
        // NOTE: declared in the PDB but never emitted.
        return m_On;
    }
    void Attr::On(bool on)
    {
        // NOTE: declared in the PDB but never emitted.
        m_On = on;
    }
    bool Attr::IsWork(float time)
    {
        // RVA 0x950100 - inside the working part of the repeat period.
        float const t = time - m_wtime.m_start;
        return t >= 0.0f && m_wtime.m_length > t - static_cast<float>(static_cast<int>(t / m_wtime.m_repeat)) * m_wtime.m_repeat;
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
        // RVA 0x950A10 - NOTE: the names are copied without a bound on the 50-character fields.
        if (m_Name.c_str() && strlen(m_Name.c_str()))
        {
            strcpy(props.m_Name, m_Name.c_str());
        }
        else
        {
            props.m_Name[0] = 0;
        }
        if (m_ClassName.c_str() && strlen(m_ClassName.c_str()))
        {
            strcpy(props.m_ClassName, m_ClassName.c_str());
        }
        else
        {
            props.m_ClassName[0] = 0;
        }
        props.m_On = m_On;
        props.m_wtime = m_wtime;
        props.m_timemode = m_timemode;
        props.m_mode = m_mode;
        props.m_State = m_State;
        props.m_emitterOn = m_emitterOn;
        props.m_force[0] = m_force[0];
        props.m_force[1] = m_force[1];
        props.m_force[2] = m_force[2];
        props.m_org = m_org;
        props.m_csType = m_csType;
        props.m_interactionType = m_interactionType;
    }

    namespace
    {
        // Reads a float attribute with sscanf. NOTE: in the shipped build a missing attribute leaves the target
        // unset; the callers here start their targets at 0.
        void ScanFloat(cmn::XmlNode const* node, char const* name, float& value)
        {
            CStr const str(node->GetAttribute(name));
            sscanf(str.c_str(), "%f", &value);
        }

        void ScanVector(cmn::XmlNode const* node, char const* name, float& x, float& y, float& z)
        {
            CStr const str(node->GetAttribute(name));
            sscanf(str.c_str(), "%f %f %f", &x, &y, &z);
        }

        // The emitter window from its node: Phase, EmitLength and RepeatPeriod, clamped as SetEmitter does.
        void ReadWorkTime(cmn::XmlNode const* emitter, WorkTime& wtime)
        {
            float emitstart = 0.0f;
            float emitlength = 0.0f;
            float emitrepeat = 0.0f;
            ScanFloat(emitter, "Phase", emitstart);
            ScanFloat(emitter, "EmitLength", emitlength);
            ScanFloat(emitter, "RepeatPeriod", emitrepeat);
            wtime.m_start = emitstart < 0.0f ? 0.0f : emitstart;
            wtime.m_length = emitlength < 0.0f ? 0.0f : emitlength;
            wtime.m_repeat = wtime.m_length > emitrepeat ? wtime.m_length : emitrepeat;
        }

        void WriteWorkTime(cmn::XmlNode* emitter, WorkTime const& wtime, TimeMode timemode)
        {
            emitter->SetAttribute("Phase", CStr(wtime.m_start).c_str());
            emitter->SetAttribute("EmitLength", CStr(wtime.m_length).c_str());
            emitter->SetAttribute("RepeatPeriod", CStr(wtime.m_repeat).c_str());
            emitter->SetAttribute("TimeMode", timemode ? "local" : "global");
        }

        // "vel", "accel" or "pos"; false for anything else.
        bool ParseWorkMode(CStr const& s, ForceMode& mode)
        {
            if (s == "vel")
            {
                mode = PS_FORCE_VEL;
                return true;
            }
            if (s == "accel")
            {
                mode = PS_FORCE_ACCEL;
                return true;
            }
            if (s == "pos")
            {
                mode = PS_FORCE_POS;
                return true;
            }
            return false;
        }

        // Nothing is written for a mode that is none of the three.
        void WriteWorkMode(cmn::XmlNode* emitter, ForceMode mode)
        {
            if (mode == PS_FORCE_ACCEL)
            {
                emitter->SetAttribute("WorkMode", "accel");
            }
            else if (mode == PS_FORCE_POS)
            {
                emitter->SetAttribute("WorkMode", "pos");
            }
            else if (mode == PS_FORCE_VEL)
            {
                emitter->SetAttribute("WorkMode", "vel");
            }
        }
    }  // namespace

    void Attractor::ReadFromXmlNode(cmn::XmlFile* m_file, ref_ptr<cmn::XmlNode>& pattr)
    {
        // RVA 0x951E00 - a point attractor: Force (Min, Max, Pos) and Emitter (WorkMode and the window). The force
        // is random, with frequency pi/2. NOTE: TimeMode is read but not used.
        ref_ptr<cmn::XmlNode> force = m_file->CreateNode();
        ref_ptr<cmn::XmlNode> emitter = m_file->CreateNode();
        m_On = true;
        CStr const str1(pattr->GetAttribute("Name"));
        pattr->GetFirstChild(force, "Force");
        pattr->GetFirstChild(emitter, "Emitter");
        float fxmin = 0.0f;
        float fxmax = 0.0f;
        ScanFloat(force, "Min", fxmin);
        ScanFloat(force, "Max", fxmax);
        ForceMode mode;
        if (!ParseWorkMode(CStr(emitter->GetAttribute("WorkMode")), mode))
        {
            // NOTE: an unknown mode takes the leftover value of a reused stack slot, the Force node's address, so
            // the attractor never works.
            mode = static_cast<ForceMode>(reinterpret_cast<intptr_t>(static_cast<cmn::XmlNode*>(force)));
        }
        m_max = fxmax;
        m_min = fxmin;
        m_freq = 1.5707964f;
        m_type = PS_FORCE_RANDOM;
        m_mode = mode;
        float fx = 0.0f;
        float fy = 0.0f;
        float fz = 0.0f;
        ScanVector(force, "Pos", fx, fy, fz);
        m_org.x = fx;
        m_org.y = fy;
        m_org.z = fz;
        CStr const timeMode(emitter->GetAttribute("TimeMode"));
        ReadWorkTime(emitter, m_wtime);
        m_Name = CStr(str1);
    }

    void Attractor::WriteToXmlNode(cmn::XmlFile* xmlFile, ref_ptr<cmn::XmlNode>& psroot)
    {
        // RVA 0x952380 - only an attractor that is on is written.
        if (!m_On)
        {
            return;
        }
        CStr const str(m_Name);
        ref_ptr<cmn::XmlNode> attr = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Attr");
        ref_ptr<cmn::XmlNode> force = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Force");
        ref_ptr<cmn::XmlNode> emitter = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Emitter");
        attr->SetAttribute("Name", str.c_str());
        attr->SetAttribute("Class", m_ClassName.c_str());
        force->SetAttribute("Min", CStr(m_min).c_str());
        force->SetAttribute("Max", CStr(m_max).c_str());
        force->SetAttribute("Pos", CStr(m_org).c_str());
        WriteWorkMode(emitter, m_mode);
        WriteWorkTime(emitter, m_wtime, m_timemode);
        attr->AddChild(emitter);
        attr->AddChild(force);
        psroot->AddChild(attr);
    }

    void Attractor::ReadFromProto(const AttrProps& props)
    {
        // RVA 0x9504A0. The base copy has to run first - without it none of
        // m_On, m_wtime, m_mode, m_State, m_org or m_force[] is ever set, so an
        // attractor loaded from effects.bps is inert or reads stale memory.
        Attr::ReadFromProto(props);

        // The binary copies the whole Force base in one go from props.m_min
        // (`this->Force = *(Force *)&props->m_min`); these four fields are it.
        m_min = props.m_min;
        m_max = props.m_max;
        m_freq = props.m_freq;
        m_type = props.m_type;
    }

    void Attractor::WriteToProto(AttrProps& props)
    {
        // RVA 0x9526E0
        Attr::WriteToProto(props);
        props.m_min = m_min;
        props.m_max = m_max;
        props.m_freq = m_freq;
        props.m_type = m_type;
    }

    void Attractor::InitParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        // RVA 0x952730
        WorkMode state = m_State;

        if (state == SPEED && m_max != 0.0f && m_min != 0.0f)
        {
            // Calculate vector from particle to attractor
            CVector toAttractor;
            toAttractor.x = pParticle->m_locorigin.x - m_org.x;
            toAttractor.y = pParticle->m_locorigin.y - m_org.y;
            toAttractor.z = pParticle->m_locorigin.z - m_org.z;

            // Calculate distance
            float distance = sqrt(toAttractor.x * toAttractor.x + toAttractor.y * toAttractor.y + toAttractor.z * toAttractor.z);

            // If too close, fade out particle
            if (distance < 0.1f)
            {
                pParticle->m_fade = 0.0f;
                return;
            }

            // Calculate force magnitude based on force type
            float forceMagnitude;
            if (m_type == PS_FORCE_RANDOM)
            {
                forceMagnitude = m3d::rnd(m_min, m_max);
            }
            else if (m_type == PS_FORCE_SINE)
            {
                forceMagnitude = fabs(sin(Time * m_freq)) * (m_max - m_min) + m_min;
            }
            else
            {
                forceMagnitude = 1.0f;  // Default force
            }

            // Normalize direction vector and apply force
            CVector normalizedDir = toAttractor.getNormalized();

            pParticle->m_vel.x += normalizedDir.x * forceMagnitude * ForceCoeff;
            pParticle->m_vel.y += normalizedDir.y * forceMagnitude * ForceCoeff;
            pParticle->m_vel.z += normalizedDir.z * forceMagnitude * ForceCoeff;
        }
        else if (state == WMPOSITION)
        {
            // Calculate vector from particle to attractor
            CVector toAttractor;
            toAttractor.x = pParticle->m_locorigin.x - m_org.x;
            toAttractor.y = pParticle->m_locorigin.y - m_org.y;
            toAttractor.z = pParticle->m_locorigin.z - m_org.z;

            // Calculate squared distance
            float distanceSq = toAttractor.x * toAttractor.x + toAttractor.y * toAttractor.y + toAttractor.z * toAttractor.z;

            // Calculate actual distance
            float distance = sqrt(distanceSq);

            // If too close, fade out particle
            if (distance < 0.1f)
            {
                pParticle->m_fade = 0.0f;
                return;
            }

            // Calculate force magnitude based on force type
            float forceMagnitude;
            if (m_type == PS_FORCE_RANDOM)
            {
                forceMagnitude = m3d::rnd(m_min, m_max);
            }
            else if (m_type == PS_FORCE_SINE)
            {
                forceMagnitude = fabs(sin(Time * m_freq)) * (m_max - m_min) + m_min;
            }
            else
            {
                forceMagnitude = 1.0f;  // Default force
            }

            // Normalize direction vector (with epsilon to avoid division by zero)
            float invDistance = 1.0f / sqrt(distanceSq + 1.1920929e-7f);
            CVector normalizedDir;
            normalizedDir.x = toAttractor.x * invDistance;
            normalizedDir.y = toAttractor.y * invDistance;
            normalizedDir.z = toAttractor.z * invDistance;

            // Apply position-based force. Only three factors: the normalized
            // direction, the force magnitude and ForceCoeff (0x95295C..0x952991).
            // MSVC reuses the Time parameter slot to hold forceMagnitude, which
            // makes the decompilation read as though Time were still a factor.
            pParticle->m_locorigin.x += normalizedDir.x * forceMagnitude * ForceCoeff;
            pParticle->m_locorigin.y += normalizedDir.y * forceMagnitude * ForceCoeff;
            pParticle->m_locorigin.z += normalizedDir.z * forceMagnitude * ForceCoeff;
        }
    }

    void Attractor::InitParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
    }

    void Attractor::AffectParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        // RVA 0x9529D0
        // Only process if in ACCELERATION mode with valid force range
        if (m_State != ACCELERATION || m_max == 0.0f || m_min == 0.0f)
        {
            return;
        }

        // Calculate vector from particle to attractor
        CVector toAttractor;
        toAttractor.x = pParticle->m_locorigin.x - m_org.x;
        toAttractor.y = pParticle->m_locorigin.y - m_org.y;
        toAttractor.z = pParticle->m_locorigin.z - m_org.z;

        // Calculate distance to attractor
        float distance = sqrt(toAttractor.x * toAttractor.x + toAttractor.y * toAttractor.y + toAttractor.z * toAttractor.z);

        // If particle is too close to attractor, kill it
        if (distance < 0.1f)
        {
            pParticle->m_fade = 0.0f;
            return;
        }

        // Calculate force magnitude based on force type
        float forceMagnitude;
        if (m_type == PS_FORCE_RANDOM)
        {
            forceMagnitude = m3d::rnd(m_min, m_max);
        }
        else if (m_type == PS_FORCE_SINE)
        {
            forceMagnitude = fabs(sin(Time * m_freq)) * (m_max - m_min) + m_min;
        }
        else
        {
            forceMagnitude = 1.0f;  // Default force magnitude
        }

        // Calculate inverse square law force (gravity-like)
        float inverseSquareForce = forceMagnitude / (distance * distance);

        // Normalize direction vector
        CVector normalizedDir = toAttractor.getNormalized();

        // Apply force to particle acceleration (inverse square law)
        pParticle->m_accel.x += normalizedDir.x * inverseSquareForce * ForceCoeff;
        pParticle->m_accel.y += normalizedDir.y * inverseSquareForce * ForceCoeff;
        pParticle->m_accel.z += normalizedDir.z * inverseSquareForce * ForceCoeff;
    }

    void Attractor::AffectParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
    }
    void Attractor::SetEmitter(TimeMode, float emitSt, float emitFin, float emitRpt)
    {
        // RVA 0x9503F0 - NOTE: the time mode is ignored.
        m_wtime.m_start = emitSt < 0.0f ? 0.0f : emitSt;
        m_wtime.m_length = emitFin < 0.0f ? 0.0f : emitFin;
        m_wtime.m_repeat = m_wtime.m_length > emitRpt ? m_wtime.m_length : emitRpt;
    }
    void Attractor::SetForce(float posx, float posy, float posz)
    {
        // RVA 0x950450
        m_org.x = posx;
        m_org.y = posy;
        m_org.z = posz;
    }
    void Attractor::GetEmitter(TimeMode&, float& emitSt, float& emitFin, float& emitRpt) const
    {
        // RVA 0x950430 - NOTE: the time mode is not reported.
        emitSt = m_wtime.m_start;
        emitFin = m_wtime.m_length;
        emitRpt = m_wtime.m_repeat;
    }
    void Attractor::GetForce(float& posx, float& posy, float& posz) const
    {
        // RVA 0x950480
        posx = m_org.x;
        posy = m_org.y;
        posz = m_org.z;
    }

    void Emitter::Set(float ttlmin, float ttlmax, float max, float emit, float emitSt, float emitFin, float emitRpt)
    {
        // RVA 0x953A00 - at most 200 particles (a negative count wraps and is clamped too).
        m_ttlMin = ttlmin;
        m_ttlMax = ttlmax;
        m_maxParticles = static_cast<unsigned int>(static_cast<int>(max));
        if (m_maxParticles > 200)
        {
            m_maxParticles = 200;
        }
        m_emitAtPeriod = emit;
        m_wtime.m_start = emitSt < 0.0f ? 0.0f : emitSt;
        m_wtime.m_length = emitFin < 0.0f ? 0.0f : emitFin;
        m_wtime.m_repeat = m_wtime.m_length > emitRpt ? m_wtime.m_length : emitRpt;
    }
    void Emitter::ReadFromXmlNode(ref_ptr<cmn::XmlNode>& emitter)
    {
        // RVA 0x953AA0 - the particle lifetime and count, the emission rate and window; PhaseMax (the latest
        // start, else Phase), ResetTime, LocalStop and StopTime are optional.
        float ttlmin = 0.0f;
        float ttlmax = 0.0f;
        float maxpart = 0.0f;
        float emitpart = 0.0f;
        float emitstart = 0.0f;
        float emitlength = 0.0f;
        float emitrepeat = 0.0f;
        ScanFloat(emitter, "ParticleAgeMin", ttlmin);
        ScanFloat(emitter, "ParticleAgeMax", ttlmax);
        ScanFloat(emitter, "MaxParticles", maxpart);
        ScanFloat(emitter, "EmitAtPeriod", emitpart);
        ScanFloat(emitter, "Phase", emitstart);
        CStr str(emitter->GetAttribute("PhaseMax"));
        if (str.c_str() && strlen(str.c_str()))
        {
            float value = 0.0f;
            sscanf(str.c_str(), "%f", &value);
            m_start = value;
        }
        else
        {
            m_start = emitstart;
        }
        ScanFloat(emitter, "EmitLength", emitlength);
        ScanFloat(emitter, "RepeatPeriod", emitrepeat);
        Set(ttlmin, ttlmax, maxpart, emitpart, emitstart, emitlength, emitrepeat);
        str = CStr(emitter->GetAttribute("ResetTime"));
        if (str.c_str() && strlen(str.c_str()))
        {
            sscanf(str.c_str(), "%f", &m_resettime);
        }
        str = CStr(emitter->GetAttribute("LocalStop"));
        if (str.c_str() && strlen(str.c_str()))
        {
            float value = 0.0f;
            sscanf(str.c_str(), "%f", &value);
            m_localStop = value != 0.0f;
        }
        else
        {
            m_localStop = false;
        }
        str = CStr(emitter->GetAttribute("StopTime"));
        if (str.c_str() && strlen(str.c_str()))
        {
            float value = 0.0f;
            sscanf(str.c_str(), "%f", &value);
            m_stopTime = value;
        }
        else
        {
            m_stopTime = 0.0f;
        }
    }
    void Emitter::WriteToXmlNode(ref_ptr<cmn::XmlNode>& emitter)
    {
        // RVA 0x9540D0
        emitter->SetAttribute("ParticleAgeMin", CStr(m_ttlMin).c_str());
        emitter->SetAttribute("ParticleAgeMax", CStr(m_ttlMax).c_str());
        emitter->SetAttribute("MaxParticles", CStr(m_maxParticles).c_str());
        emitter->SetAttribute("EmitAtPeriod", CStr(m_emitAtPeriod).c_str());
        emitter->SetAttribute("Phase", CStr(m_wtime.m_start).c_str());
        emitter->SetAttribute("EmitLength", CStr(m_wtime.m_length).c_str());
        emitter->SetAttribute("RepeatPeriod", CStr(m_wtime.m_repeat).c_str());
        emitter->SetAttribute("ResetTime", CStr(m_resettime).c_str());
        emitter->SetAttribute("LocalStop", CStr(static_cast<int>(m_localStop)).c_str());
        emitter->SetAttribute("StopTime", CStr(m_stopTime).c_str());
        emitter->SetAttribute("PhaseMax", CStr(m_start).c_str());
    }
    bool Emitter::IsWork(float time)
    {
        // RVA 0x950600 - NOTE: unlike Attr::IsWork, the phase is not taken off the time.
        return time >= 0.0f &&
            m_wtime.m_length > time - static_cast<float>(static_cast<int>(time / m_wtime.m_repeat)) * m_wtime.m_repeat;
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
        // RVA 0x952B40 - a rotation attractor: Force (Min, Max as vectors, random with frequency pi/2 per axis) and
        // Emitter (WorkMode, EmitterOn and the window). The origin is reset. NOTE: TimeMode is read but not used.
        ref_ptr<cmn::XmlNode> force = m_file->CreateNode();
        ref_ptr<cmn::XmlNode> emitter = m_file->CreateNode();
        m_On = true;
        CStr const str1(prattr->GetAttribute("Name"));
        m_Name = CStr(str1);
        prattr->GetFirstChild(force, "Force");
        prattr->GetFirstChild(emitter, "Emitter");
        float mins[3] = {0.0f, 0.0f, 0.0f};
        float maxs[3] = {0.0f, 0.0f, 0.0f};
        ScanVector(force, "Min", mins[0], mins[1], mins[2]);
        ScanVector(force, "Max", maxs[0], maxs[1], maxs[2]);
        for (int i = 0; i < 3; ++i)
        {
            m_force[i].m_max = maxs[i];
            m_force[i].m_min = mins[i];
            m_force[i].m_freq = 1.5707964f;
            m_force[i].m_type = PS_FORCE_RANDOM;
        }
        ForceMode mode;
        if (!ParseWorkMode(CStr(emitter->GetAttribute("WorkMode")), mode))
        {
            // NOTE: an unknown mode takes the leftover value of a reused stack slot, the Force node's address, so
            // the attractor never works.
            mode = static_cast<ForceMode>(reinterpret_cast<intptr_t>(static_cast<cmn::XmlNode*>(force)));
        }
        m_org.x = 0.0f;
        m_org.y = 0.0f;
        m_org.z = 0.0f;
        m_mode = mode;
        CStr const emitterOn(emitter->GetAttribute("EmitterOn"));
        if (emitterOn.c_str() && strlen(emitterOn.c_str()))
        {
            m_emitterOn = emitterOn == "1";
        }
        CStr const timeMode(emitter->GetAttribute("TimeMode"));
        ReadWorkTime(emitter, m_wtime);
    }

    void RotAttractor::WriteToXmlNode(cmn::XmlFile* xmlFile, ref_ptr<cmn::XmlNode>& psroot)
    {
        // RVA 0x953100 - only an attractor that is on is written.
        if (!m_On)
        {
            return;
        }
        CStr const str(m_Name);
        ref_ptr<cmn::XmlNode> attr = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Attr");
        ref_ptr<cmn::XmlNode> force = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Force");
        ref_ptr<cmn::XmlNode> emitter = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Emitter");
        attr->SetAttribute("Name", str.c_str());
        attr->SetAttribute("Class", m_ClassName.c_str());
        force->SetAttribute("Min", CStr(CVector(m_force[0].m_min, m_force[1].m_min, m_force[2].m_min)).c_str());
        force->SetAttribute("Max", CStr(CVector(m_force[0].m_max, m_force[1].m_max, m_force[2].m_max)).c_str());
        WriteWorkMode(emitter, m_mode);
        WriteWorkTime(emitter, m_wtime, m_timemode);
        emitter->SetAttribute("EmitterOn", m_emitterOn ? "1" : "0");
        attr->AddChild(emitter);
        attr->AddChild(force);
        psroot->AddChild(attr);
    }

    void RotAttractor::InitParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        if (m_State == SPEED && !m_emitterOn)
        {
            CVector dest;
            m3d::CalcForcesCarthesian(dest, m_force, Time);
            pParticle->m_rotvel.x = pParticle->m_rotvel.x + dest.x;
            pParticle->m_rotvel.y = pParticle->m_rotvel.y + dest.y;
            pParticle->m_rotvel.z = pParticle->m_rotvel.z + dest.z;
        }
    }

    void RotAttractor::InitParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        if (m_State == SPEED && m_emitterOn)
        {
            CVector dest;
            m3d::CalcForcesCarthesian(dest, m_force, parts->m_time);
            parts->m_rotvel.x = parts->m_rotvel.x + dest.x;
            parts->m_rotvel.y = dest.y + parts->m_rotvel.y;
            parts->m_rotvel.z = dest.z + parts->m_rotvel.z;
        }
    }

    void RotAttractor::AffectParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        if (this->m_State == ACCELERATION && !this->m_emitterOn)
        {
            CVector dest;
            m3d::CalcForcesCarthesian(dest, m_force, Time);
            pParticle->m_rotaccel.x = pParticle->m_rotaccel.x + dest.x;
            pParticle->m_rotaccel.y = pParticle->m_rotaccel.y + dest.y;
            pParticle->m_rotaccel.z = pParticle->m_rotaccel.z + dest.z;
        }
    }

    void RotAttractor::AffectParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        if (m_State == ACCELERATION && m_emitterOn)
        {
            CVector dest;
            m3d::CalcForcesCarthesian(dest, m_force, parts->m_time);
            parts->m_rotaccel.x = parts->m_rotaccel.x + dest.x;
            parts->m_rotaccel.y = dest.y + parts->m_rotaccel.y;
            parts->m_rotaccel.z = dest.z + parts->m_rotaccel.z;
        }
    }
    void RotAttractor::SetEmitter(TimeMode, float emitSt, float emitFin, float emitRpt)
    {
        // RVA 0x9504F0 - NOTE: the time mode is ignored.
        m_wtime.m_start = emitSt < 0.0f ? 0.0f : emitSt;
        m_wtime.m_length = emitFin < 0.0f ? 0.0f : emitFin;
        m_wtime.m_repeat = m_wtime.m_length > emitRpt ? m_wtime.m_length : emitRpt;
    }
    void RotAttractor::SetForce(bool target)
    {
        // RVA 0x9505C0
        m_emitterOn = target;
    }
    void RotAttractor::SetForce(float posx, float posy, float posz)
    {
        // RVA 0x950560
        m_org.x = posx;
        m_org.y = posy;
        m_org.z = posz;
    }
    void RotAttractor::SetForce(ForceMode mode)
    {
        // RVA 0x950550
        m_mode = mode;
    }
    void RotAttractor::GetEmitter(TimeMode&, float& emitSt, float& emitFin, float& emitRpt) const
    {
        // RVA 0x950530 - NOTE: the time mode is not reported.
        emitSt = m_wtime.m_start;
        emitFin = m_wtime.m_length;
        emitRpt = m_wtime.m_repeat;
    }
    void RotAttractor::GetForce(bool& target) const
    {
        // RVA 0x9505D0
        target = m_emitterOn;
    }
    void RotAttractor::GetForce(float& posx, float& posy, float& posz) const
    {
        // RVA 0x9505A0
        posx = m_org.x;
        posy = m_org.y;
        posz = m_org.z;
    }
    void RotAttractor::GetForce(ForceMode& mode) const
    {
        // RVA 0x950590
        mode = m_mode;
    }

    void GameAttractor::ReadFromXmlNode(cmn::XmlFile*, ref_ptr<cmn::XmlNode>& gattr)
    {
        // RVA 0x953610 - the game interaction to apply; the wind when not given.
        m_On = true;
        SafeStrAttrib(m_Name, gattr, "Name");
        cmn::XmlNode const* node = gattr;
        char const* interaction = node->IsEmpty() ? nullptr : node->GetAttribute("Interaction");
        m_interactionType = interaction ? static_cast<GameInteraction>(atoi(interaction)) : GI_WIND;
    }

    void GameAttractor::WriteToXmlNode(cmn::XmlFile* m_file, ref_ptr<cmn::XmlNode>&)
    {
        // RVA 0x9536A0 - NOTE: the node is filled in but never added to the parent, so game attractors are not
        // saved.
        if (!m_On)
        {
            return;
        }
        ref_ptr<cmn::XmlNode> attr = m_file->CreateNode(cmn::XML_NODE_ELEMENT, "Attr");
        attr->SetAttribute("Class", m_ClassName.c_str());
        attr->SetAttribute("Name", m_Name.c_str());
        attr->SetAttribute("Interaction", CStr(static_cast<int>(m_interactionType)).c_str());
    }

    void GameAttractor::InitParticle(Particle* pParticle, float, CMatrix&, bool, float)
    {
        // RVA 0x953750 - the wind blows new particles along.
        if (m_interactionType == GI_WIND)
        {
            CVector const& wind = pClient->GetWorld().m_weatherManager.GetActiveWeather()->GetWindInfo().GetCurWind();
            pParticle->m_vel.x = wind.x + pParticle->m_vel.x;
            pParticle->m_vel.y = wind.y + pParticle->m_vel.y;
            pParticle->m_vel.z = wind.z + pParticle->m_vel.z;
        }
    }

    void GameAttractor::InitParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        // RVA 0x9505E0 - empty in the shipped build, like Attractor's.
    }

    void GameAttractor::AffectParticle(Particle* pParticle, float, CMatrix&, bool, float)
    {
        // RVA 0x9537A0 - then follows the changes of the wind.
        if (m_interactionType == GI_WIND)
        {
            CVector const& delta = pClient->GetWorld().m_weatherManager.GetActiveWeather()->GetWindInfo().GetDeltaVel();
            pParticle->m_vel.x = delta.x + pParticle->m_vel.x;
            pParticle->m_vel.y = delta.y + pParticle->m_vel.y;
            pParticle->m_vel.z = delta.z + pParticle->m_vel.z;
        }
    }

    void GameAttractor::AffectParticlesList(ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        // RVA 0x9505F0 - empty in the shipped build, like Attractor's.
    }

    void SAttractor::ReadFromXmlNode(cmn::XmlFile* m_file, ref_ptr<cmn::XmlNode>& sattr)
    {
        // RVA 0x950B50 - a space attractor: Force (Min, Max as vectors) and Emitter (FuncMode "rnd" or sine, the
        // periods of the three axes, WorkMode, SystemMode, EmitterOn, TimeMode and the window).
        ref_ptr<cmn::XmlNode> saforce = m_file->CreateNode();
        ref_ptr<cmn::XmlNode> emitter = m_file->CreateNode();
        m_On = true;
        CStr const str1(sattr->GetAttribute("Name"));
        sattr->GetFirstChild(saforce, "Force");
        sattr->GetFirstChild(emitter, "Emitter");
        float mins[3] = {0.0f, 0.0f, 0.0f};
        float maxs[3] = {0.0f, 0.0f, 0.0f};
        ScanVector(saforce, "Min", mins[0], mins[1], mins[2]);
        ScanVector(saforce, "Max", maxs[0], maxs[1], maxs[2]);
        CStr const funcMode(emitter->GetAttribute("FuncMode"));
        bool const hasFuncMode = funcMode.c_str() && strlen(funcMode.c_str());
        ForceType const type = hasFuncMode && !(funcMode == "rnd") ? PS_FORCE_SINE : PS_FORCE_RANDOM;
        // NOTE: the periods are only read when FuncMode is given, and an axis without one keeps the previous
        // axis's period (2 to begin with).
        float period = 2.0f;
        char const* const periods[3] = {"PeriodX", "PeriodY", "PeriodZ"};
        for (int i = 0; i < 3; ++i)
        {
            CStr const str(emitter->GetAttribute(periods[i]));
            if (hasFuncMode)
            {
                sscanf(str.c_str(), "%f", &period);
            }
            m_force[i].m_max = maxs[i];
            m_force[i].m_min = mins[i];
            m_force[i].m_freq = 3.1415927f / period;
            m_force[i].m_type = type;
        }
        ForceMode mode;
        if (!ParseWorkMode(CStr(emitter->GetAttribute("WorkMode")), mode))
        {
            // NOTE: an unknown mode takes the bits of the last period read, from a reused stack slot.
            int bits;
            memcpy(&bits, &period, sizeof(bits));
            mode = static_cast<ForceMode>(bits);
        }
        CStr const systemMode(emitter->GetAttribute("SystemMode"));
        m_mode = mode;
        m_csType = systemMode == "cart" ? PS_CST_CARTHESIAN : PS_CST_POLAR;
        CStr const emitterOn(emitter->GetAttribute("EmitterOn"));
        if (emitterOn.c_str() && strlen(emitterOn.c_str()))
        {
            m_emitterOn = emitterOn == "1";
        }
        CStr const timeMode(emitter->GetAttribute("TimeMode"));
        TimeMode const timemode = timeMode == "global" ? PS_TIME_GLOBAL : PS_TIME_LOCAL;
        ReadWorkTime(emitter, m_wtime);
        m_timemode = timemode;
        m_Name = CStr(str1);
    }

    void SAttractor::WriteToXmlNode(cmn::XmlFile* xmlFile, ref_ptr<cmn::XmlNode>& psroot)
    {
        // RVA 0x951320 - only an attractor that is on is written; FuncMode follows the first axis.
        if (!m_On)
        {
            return;
        }
        CStr const str(m_Name);
        ref_ptr<cmn::XmlNode> attr = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Attr");
        ref_ptr<cmn::XmlNode> force = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Force");
        ref_ptr<cmn::XmlNode> emitter = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Emitter");
        attr->SetAttribute("Name", str.c_str());
        attr->SetAttribute("Class", m_ClassName.c_str());
        force->SetAttribute("Min", CStr(CVector(m_force[0].m_min, m_force[1].m_min, m_force[2].m_min)).c_str());
        force->SetAttribute("Max", CStr(CVector(m_force[0].m_max, m_force[1].m_max, m_force[2].m_max)).c_str());
        emitter->SetAttribute("SystemMode", m_csType ? "polar" : "cart");
        emitter->SetAttribute("FuncMode", m_force[0].m_type == PS_FORCE_SINE ? "sin" : "rnd");
        WriteWorkMode(emitter, m_mode);
        emitter->SetAttribute("PeriodX", CStr(3.1415927f / m_force[0].m_freq).c_str());
        emitter->SetAttribute("PeriodY", CStr(3.1415927f / m_force[1].m_freq).c_str());
        emitter->SetAttribute("PeriodZ", CStr(3.1415927f / m_force[2].m_freq).c_str());
        WriteWorkTime(emitter, m_wtime, m_timemode);
        emitter->SetAttribute("EmitterOn", m_emitterOn ? "1" : "0");
        attr->AddChild(emitter);
        attr->AddChild(force);
        psroot->AddChild(attr);
    }

    void SAttractor::InitParticle(Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff)
    {
        WorkMode const state = this->m_State;
        if (state != SPEED || this->m_emitterOn)
        {
            if (state == WMPOSITION && !this->m_emitterOn)
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
        // RVA 0x950310
        m_wtime.m_start = emitSt < 0.0f ? 0.0f : emitSt;
        m_wtime.m_length = emitFin < 0.0f ? 0.0f : emitFin;
        m_wtime.m_repeat = m_wtime.m_length > emitRpt ? m_wtime.m_length : emitRpt;
        m_timemode = mode;
    }
    void SAttractor::SetForce(bool target)
    {
        // RVA 0x9503B0
        m_emitterOn = target;
    }
    void SAttractor::SetForce(ForceMode mode, CoordinatesSystemType system)
    {
        // RVA 0x950390
        m_csType = system;
        m_mode = mode;
    }
    void SAttractor::GetEmitter(TimeMode& mode, float& emitSt, float& emitFin, float& emitRpt) const
    {
        // RVA 0x950360
        emitSt = m_wtime.m_start;
        emitFin = m_wtime.m_length;
        emitRpt = m_wtime.m_repeat;
        mode = m_timemode;
    }
    void SAttractor::GetForce(bool& target) const
    {
        // RVA 0x9503E0
        target = m_emitterOn;
    }
    void SAttractor::GetForce(ForceMode& mode, CoordinatesSystemType& system) const
    {
        // RVA 0x9503C0
        mode = m_mode;
        system = m_csType;
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
        // RVA 0x8E1C50
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
        // RVA 0x8E1CE0
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
                float randomFactor = static_cast<float>(rndGet() >> 16) * 0.000015259022f;
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

    void CalcForcesPolarOrg(CVector& dest, Force const (&forces)[3], CVector const& dir, float time)
    {
        // RVA 0x8E1E00. Unlike the other two this uses only forces[0], as a
        // scalar magnitude along the supplied direction.
        Force const& force = forces[0];

        float magnitude;
        if (force.m_type == PS_FORCE_RANDOM)
        {
            float const randomFactor = static_cast<float>(rndGet() >> 16) * 0.000015259022f;
            magnitude = (force.m_max - force.m_min) * randomFactor + force.m_min;
        }
        else
        {
            // PS_FORCE_SINE. For any other value the binary leaves the reused
            // `time` parameter slot alone and multiplies by the raw time, which
            // ForceType cannot actually reach.
            magnitude = fabs(sin(time * force.m_freq)) * (force.m_max - force.m_min) + force.m_min;
        }

        dest.x = dir.x * magnitude;
        dest.y = dir.y * magnitude;
        dest.z = dir.z * magnitude;
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