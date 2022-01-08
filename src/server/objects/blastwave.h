#pragma once
#include "base/simplephysicobj.h"
#include <ode/contact.h>

namespace ai
{
    class BlastWavePrototypeInfo : public SimplePhysicObjPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        BlastWavePrototypeInfo();
        virtual Obj* CreateTargetObject() const;

    protected:
        virtual void _InternalCopyFrom(ai::PrototypeInfo const&);

    private:
        float m_WaveForceIntensity;
        float m_WaveDamageIntensity;
        CStr m_EffectName;
    };

    class BlastWave :  public SimplePhysicObj
    {
    public:
        virtual void Update(float,unsigned int);
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        static m3d::Class * GetBaseClass();
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        float GetBlastRadius() const ;
        virtual m3d::Class * GetClass() const ;
        int GetEmitterId() const ;
        short GetRocketExplosionType() const ;
        virtual int GetPropertyId(char const *) const ;
        virtual BlastWavePrototypeInfo const * GetPrototypeInfo() const ;
        virtual CStr GetPropertyName(int) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        float GetDampedDamageIntensity(float) const ;
        float GetWaveDamageIntensity() const ;
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        float GetWaveForceIntensity() const ;
        float GetDampedForceIntensity(float) const ;
        void SetEmitterId(int);
        void SetRocketExplosionType(short);
        void SetRocketId(int);
        float GetDampingCoefficeient(float) const ;
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        static int __fastcall CollideBlastWaveAndPhysicObj(BlastWave *,Obj *,dContact *,unsigned int &,bool);
        BlastWave(BlastWavePrototypeInfo const &);

    protected:
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual ~BlastWave();

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();

    private:
        float m_WaveForceIntensity;
        float m_WaveDamageIntensity;
        int m_frameWhenCreated;
        unsigned int m_Frame;
        __int16 m_rocketExplosionType;
        m3d::SgNode *m_EffectNode;
        int m_emitterId;
        bool m_bCollided;
    };
}
