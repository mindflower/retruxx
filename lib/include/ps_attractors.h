#pragma once

#include <psprops.h>
#include <core/ref_ptr.h>
#include <core/stringm3d.h>
#include <math/vector.h>

struct CMatrix;

namespace m3d
{
    class ParticlesList;
    struct Particle;
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }
    class Attr
    {
    public:
        Attr(const m3d::Attr&);
        Attr();
        virtual  ~Attr();
        static m3d::Attr* __fastcall New(const CStr& ClassName);
        static m3d::Attr* __fastcall Factory(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& attr);
        void SetName(CStr name);
        CStr GetName();
        bool IsOn();
        void On(bool);
        bool IsWork(float time);
        void SetState(float Time);
        virtual void ReadFromXmlNode(m3d::cmn::XmlFile*, ref_ptr<m3d::cmn::XmlNode>&) = 0;
        virtual void WriteToXmlNode(m3d::cmn::XmlFile*, ref_ptr<m3d::cmn::XmlNode>&) = 0;
        virtual void ReadFromProto(const AttrProps& props);
        virtual void WriteToProto(AttrProps& props);
        virtual void InitParticle(m3d::Particle*, float, CMatrix&, bool, float) = 0;
        virtual void InitParticlesList(m3d::ParticlesList*, CMatrix&, bool, float) = 0;
        virtual void AffectParticle(m3d::Particle*, float, CMatrix&, bool, float) = 0;
        virtual void AffectParticlesList(m3d::ParticlesList*, CMatrix&, bool, float) = 0;
        CStr m_Name;
        CStr m_ClassName;
        bool m_On;
        char Padding_185[3];
        WorkTime m_wtime;
        TimeMode m_timemode;
        ForceMode m_mode;
        WorkMode m_State;
        bool m_emitterOn;
        char Padding_186[3];
        Force m_force[3];
        CVector m_org;
        CoordinatesSystemType m_csType;
        GameInteraction m_interactionType;
    };

    class Attractor : public Force, public m3d::Attr
    {
    public:
        virtual void ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& pattr) override;
        virtual void WriteToXmlNode(m3d::cmn::XmlFile* xmlFile, ref_ptr<m3d::cmn::XmlNode>& psroot) override;
        virtual void ReadFromProto(const AttrProps& props) override;
        virtual void WriteToProto(AttrProps& props) override;
        virtual void InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override;

    private:
        void SetEmitter(TimeMode mode, float emitSt, float emitFin, float emitRpt);
        void SetForce(float posx, float posy, float posz);
        void GetEmitter(TimeMode& mode, float& emitSt, float& emitFin, float& emitRpt) const;
        void GetForce(float& posx, float& posy, float& posz) const;
    };

    class Emitter
    {
    public:
        float m_emitAtPeriod;
        WorkTime m_wtime;
        unsigned int m_maxParticles;
        float m_ttlMin;
        float m_ttlMax;
        float m_resettime;
        bool m_localStop;
        char Padding_67[3];
        float m_stopTime;
        float m_start;
        void Set(float ttlmin, float ttlmax, float max, float emit, float emitSt, float emitFin, float emitRpt);
        void ReadFromXmlNode(ref_ptr<m3d::cmn::XmlNode>& emitter);
        void WriteToXmlNode(ref_ptr<m3d::cmn::XmlNode>& emitter);
        bool IsWork(float time);
        void LocalStop(m3d::Particle* pParticle, float Time);
        unsigned int Emit(double time, float lastFrameSecs);
    };

    class RotAttractor : public m3d::Attr
    {
    public:
        virtual void ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& prattr) override;
        virtual void WriteToXmlNode(m3d::cmn::XmlFile* xmlFile, ref_ptr<m3d::cmn::XmlNode>& psroot) override;
        virtual void InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override;

    private:
        void SetEmitter(TimeMode mode, float emitSt, float emitFin, float emitRpt);
        void SetForce(bool target);
        void SetForce(float posx, float posy, float posz);
        void SetForce(ForceMode mode);
        void GetEmitter(TimeMode& mode, float& emitSt, float& emitFin, float& emitRpt) const;
        void GetForce(bool& target) const;
        void GetForce(float& posx, float& posy, float& posz) const;
        void GetForce(ForceMode& mode) const;
    };

    class GameAttractor : public m3d::Attr
    {
    public:
        virtual void ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& gattr) override;
        virtual void WriteToXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& psroot) override;
        virtual void InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override;
    };

    class SAttractor : public m3d::Attr
    {
    public:
        virtual void ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& sattr) override;
        virtual void WriteToXmlNode(m3d::cmn::XmlFile* xmlFile, ref_ptr<m3d::cmn::XmlNode>& psroot) override;
        virtual void InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override;
        virtual void AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override;

    private:
        void SetEmitter(TimeMode mode, float emitSt, float emitFin, float emitRpt);
        void SetForce(bool target);
        void SetForce(ForceMode mode, CoordinatesSystemType system);
        void GetEmitter(TimeMode& mode, float& emitSt, float& emitFin, float& emitRpt) const;
        void GetForce(bool& target) const;
        void GetForce(ForceMode& mode, CoordinatesSystemType& system) const;
    };


    unsigned int rndGet();
    float rnd(float, float);

    void CalcForcesCarthesian(CVector&, Force const (&)[3], float);
    void CalcForcesPolar(CVector&, Force const (&)[3], float);
    void CalcForcesPolarOrg(CVector&, Force const (&)[3], CVector const&, float);
    CVector CalcForces(CoordinatesSystemType, Force const (&)[3], float, CVector const&);
}

