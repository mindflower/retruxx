#pragma once
#include <psprops.h>
#include <core/ref_ptr.h>
#include <core/stringm3d.h>
#include <math/vector.h>

class CMatrix;

namespace m3d
{
    class ParticlesList;
    class Particle;

    namespace cmn
    {
        class XmlFile;
        struct XmlNode;
    }

    class Attr
    {
    public:
        Attr(const m3d::Attr&);
        Attr();
        virtual  ~Attr() /* 0x00 */;
        static m3d::Attr* __fastcall New(const CStr& ClassName);
        static m3d::Attr* __fastcall Factory(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& attr);
        void SetName(CStr name);
        CStr GetName();
        bool IsOn();
        void On(bool);
        bool IsWork(float time);
        void SetState(float Time);
        virtual void ReadFromXmlNode(m3d::cmn::XmlFile*, ref_ptr<m3d::cmn::XmlNode>&) = 0 /* 0x04 */;
        virtual void WriteToXmlNode(m3d::cmn::XmlFile*, ref_ptr<m3d::cmn::XmlNode>&) = 0 /* 0x08 */;
        virtual void ReadFromProto(const AttrProps& props) /* 0x0c */;
        virtual void WriteToProto(AttrProps& props) /* 0x10 */;
        virtual void InitParticle(m3d::Particle*, float, CMatrix&, bool, float) = 0 /* 0x14 */;
        virtual void InitParticlesList(m3d::ParticlesList*, CMatrix&, bool, float) = 0 /* 0x18 */;
        virtual void AffectParticle(m3d::Particle*, float, CMatrix&, bool, float) = 0 /* 0x1c */;
        virtual void AffectParticlesList(m3d::ParticlesList*, CMatrix&, bool, float) = 0 /* 0x20 */;
        CStr m_Name;
        CStr m_ClassName;
        /* 0x001c */ bool m_On;
        /* 0x001d */ char Padding_185[3];
        WorkTime m_wtime;
        /* 0x002c */ TimeMode m_timemode;
        /* 0x0030 */ ForceMode m_mode;
        /* 0x0034 */ WorkMode m_State;
        /* 0x0038 */ bool m_emitterOn;
        /* 0x0039 */ char Padding_186[3];
        /* 0x003c */ Force m_force[3];
        CVector m_org;
        /* 0x0078 */ CoordinatesSystemType m_csType;
        /* 0x007c */ GameInteraction m_interactionType;
    }; /* size: 0x0080 */

    class Attractor : public Force, public m3d::Attr
    {
    public:
        virtual void ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& pattr) override /* 0x04 */;
        virtual void WriteToXmlNode(m3d::cmn::XmlFile* xmlFile, ref_ptr<m3d::cmn::XmlNode>& psroot) override /* 0x08 */;
        virtual void ReadFromProto(const AttrProps& props) override /* 0x0c */;
        virtual void WriteToProto(AttrProps& props) override /* 0x10 */;
        virtual void InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x14 */;
        virtual void InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x18 */;
        virtual void AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x1c */;
        virtual void AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x20 */;

    private:
        void SetEmitter(TimeMode mode, float emitSt, float emitFin, float emitRpt);
        void SetForce(float posx, float posy, float posz);
        void GetEmitter(TimeMode& mode, float& emitSt, float& emitFin, float& emitRpt) const;
        void GetForce(float& posx, float& posy, float& posz) const;
    }; /* size: 0x0090 */

    class Emitter
    {
    public:
        /* 0x0000 */ float m_emitAtPeriod;
        WorkTime m_wtime;
        /* 0x0010 */ unsigned int m_maxParticles;
        /* 0x0014 */ float m_ttlMin;
        /* 0x0018 */ float m_ttlMax;
        /* 0x001c */ float m_resettime;
        /* 0x0020 */ bool m_localStop;
        /* 0x0021 */ char Padding_67[3];
        /* 0x0024 */ float m_stopTime;
        /* 0x0028 */ float m_start;
        void Set(float ttlmin, float ttlmax, float max, float emit, float emitSt, float emitFin, float emitRpt);
        void ReadFromXmlNode(ref_ptr<m3d::cmn::XmlNode>& emitter);
        void WriteToXmlNode(ref_ptr<m3d::cmn::XmlNode>& emitter);
        bool IsWork(float time);
        void LocalStop(m3d::Particle* pParticle, float Time);
        unsigned int Emit(double time, float lastFrameSecs);
    }; /* size: 0x002c */

    class RotAttractor : public m3d::Attr
    {
    public:
        virtual void ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& prattr) override /* 0x04 */;
        virtual void WriteToXmlNode(m3d::cmn::XmlFile* xmlFile, ref_ptr<m3d::cmn::XmlNode>& psroot) override /* 0x08 */;
        virtual void InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x14 */;
        virtual void InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x18 */;
        virtual void AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x1c */;
        virtual void AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x20 */;

    private:
        void SetEmitter(TimeMode mode, float emitSt, float emitFin, float emitRpt);
        void SetForce(bool target);
        void SetForce(float posx, float posy, float posz);
        void SetForce(ForceMode mode);
        void GetEmitter(TimeMode& mode, float& emitSt, float& emitFin, float& emitRpt) const;
        void GetForce(bool& target) const;
        void GetForce(float& posx, float& posy, float& posz) const;
        void GetForce(ForceMode& mode) const;
    }; /* size: 0x0080 */

    class GameAttractor : public m3d::Attr
    {
    public:
        virtual void ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& gattr) override /* 0x04 */;
        virtual void WriteToXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& psroot) override /* 0x08 */;
        virtual void InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x14 */;
        virtual void InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x18 */;
        virtual void AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x1c */;
        virtual void AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x20 */;
    }; /* size: 0x0080 */

    class SAttractor : public m3d::Attr
    {
    public:
        virtual void ReadFromXmlNode(m3d::cmn::XmlFile* m_file, ref_ptr<m3d::cmn::XmlNode>& sattr) override /* 0x04 */;
        virtual void WriteToXmlNode(m3d::cmn::XmlFile* xmlFile, ref_ptr<m3d::cmn::XmlNode>& psroot) override /* 0x08 */;
        virtual void InitParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x14 */;
        virtual void InitParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x18 */;
        virtual void AffectParticle(m3d::Particle* pParticle, float Time, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x1c */;
        virtual void AffectParticlesList(m3d::ParticlesList* parts, CMatrix& Local, bool Orient, float ForceCoeff) override /* 0x20 */;

    private:
        void SetEmitter(TimeMode mode, float emitSt, float emitFin, float emitRpt);
        void SetForce(bool target);
        void SetForce(ForceMode mode, CoordinatesSystemType system);
        void GetEmitter(TimeMode& mode, float& emitSt, float& emitFin, float& emitRpt) const;
        void GetForce(bool& target) const;
        void GetForce(ForceMode& mode, CoordinatesSystemType& system) const;
    }; /* size: 0x0080 */
}

