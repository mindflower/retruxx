#pragma once
#include <server/components/numericinrangeregenerating.h>
#include <server/objects/base/shell.h>

namespace ai
{
    class RocketPrototypeInfo : public ShellPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        RocketPrototypeInfo();
        virtual void PostLoad();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        float m_velocity;
        float m_acceleration;
        float m_minTurningRadius;
        float m_flyTime;
        int m_BlastWavePrototypeId;
        CStr m_BlastWavePrototypeName;
    };

    class Rocket : public Shell
    {
    public:
        virtual RocketPrototypeInfo const * GetPrototypeInfo() const ;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        float GetVelocity() const ;
        void SetInitialVelocity(CVector const &);
        Rocket(RocketPrototypeInfo const &);
        virtual void Update(float,unsigned int);
        static m3d::Class * __fastcall GetBaseClass();
        void CreateBlastWave() const ;
        virtual void Remove();
        void setWithAngleLimit(bool);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void SetInitialDirection(CVector const &);
        virtual m3d::Class * GetClass() const ;

    protected:
        virtual ~Rocket();

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * __fastcall CreateObject();

    private:
        int m_targetObjId;
        NumericInRangeRegenerating<float> m_velocity;
        NumericInRangeRegenerating<float> m_lifeTime;
        float m_minTurningRadius;
        CVector m_initialDirection;
        CVector m_initVelDir;
        float m_initVelValue;
        bool m_withAngleLimit;
        int m_numCircles;
    };
}
