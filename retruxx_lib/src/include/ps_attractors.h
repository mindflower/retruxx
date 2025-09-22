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
        void SetState(float);
        static Attr* __fastcall Factory(cmn::XmlFile*, ref_ptr<cmn::XmlNode>&);
        static Attr* __fastcall New(CStr const&);
        void SetName(CStr);
        bool IsWork(float);
        Attr();
        virtual void ReadFromProto(AttrProps const&);
        virtual ~Attr();
        virtual void WriteToProto(AttrProps&);

    private:
        //Attr_vtbl* __vftable /*VFT*/;
        CStr m_Name;
        CStr m_ClassName;
        bool m_On;
        WorkTime m_wtime;
        TimeMode m_timemode;
        ForceMode m_mode;
        WorkMode m_State;
        bool m_emitterOn;
        Force m_force[3];
        CVector m_org;
        CoordinatesSystemType m_csType;
        GameInteraction m_interactionType;
    };

    class Attractor :
        public Attr,
        public Force
    {
    public:
        virtual void InitParticle(Particle *,float,CMatrix &,bool,float);
        virtual void ReadFromXmlNode(cmn::XmlFile *,ref_ptr<cmn::XmlNode> &);
        virtual void ReadFromProto(AttrProps const &);
        virtual void InitParticlesList(ParticlesList *,CMatrix &,bool,float);
        virtual void WriteToXmlNode(cmn::XmlFile *,ref_ptr<cmn::XmlNode> &);
        virtual void AffectParticlesList(ParticlesList *,CMatrix &,bool,float);
        virtual void WriteToProto(AttrProps &);
        virtual void AffectParticle(Particle *,float,CMatrix &,bool,float);

    private:
        void GetForce(float &,float &,float &) const ;
        void SetForce(float,float,float);
        void SetEmitter(TimeMode,float,float,float);
        void GetEmitter(TimeMode &,float &,float &,float &) const ;
    };

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
}

