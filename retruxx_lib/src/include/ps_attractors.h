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
        void ReadFromXmlNode(ref_ptr<m3d::cmn::XmlNode>&);
        void WriteToXmlNode(ref_ptr<m3d::cmn::XmlNode>&);
        void Set(float, float, float, float, float, float, float);
        void LocalStop(m3d::Particle*, float);
        unsigned int Emit(double, float);
        bool IsWork(float);

    private:
        float m_emitAtPeriod;
        WorkTime m_wtime;
        unsigned int m_maxParticles;
        float m_ttlMin;
        float m_ttlMax;
        float m_resettime;
        bool m_localStop;
        float m_stopTime;
        float m_start;
    };
}

