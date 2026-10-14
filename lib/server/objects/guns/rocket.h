#pragma once
#include <server/components/numericinrangeregenerating.h>
#include <server/objects/base/shell.h>

namespace ai
{
    class RocketPrototypeInfo : public ai::ShellPrototypeInfo
    {
    public:
        float m_velocity;
        float m_acceleration;
        float m_minTurningRadius;
        float m_flyTime;
        int m_BlastWavePrototypeId;
        RocketPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;

    private:
        CStr m_BlastWavePrototypeName;
    };

    // A homing shell: it accelerates up to its top speed and, while it has a target, turns towards
    // it along a circle no tighter than its minimum turning radius.
    class Rocket : public ai::Shell
    {
        friend class RocketPrototypeInfo;
        // MinePusher::_LaunchShells stores the direction a mine was dropped in directly.
        friend class MinePusher;
        // RocketLauncher::_LaunchShells sets the target, direction and angle limit directly.
        friend class RocketLauncher;

    protected:
        virtual ~Rocket() override;

    protected:
        // NOTE: private in the PDB; protected here because Mine derives from Rocket.
        Rocket(const ai::RocketPrototypeInfo& prototypeInfo);
        Rocket(const ai::Rocket&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        RT_CLASS_DECLARE(Rocket);
        virtual const ai::RocketPrototypeInfo* GetPrototypeInfo() const override;
        int m_targetObjId;
        float GetVelocity() const;
        virtual void Remove() override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        void SetInitialDirection(const CVector& direction);
        void SetInitialVelocity(const CVector& initVel);
        void CreateBlastWave() const;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        bool getWithAngleLimit() const;
        void setWithAngleLimit(bool value);

    private:
        ai::NumericInRangeRegenerating<float> m_velocity;
        ai::NumericInRangeRegenerating<float> m_lifeTime;
        float m_minTurningRadius;
        CVector m_initialDirection;
        CVector m_initVelDir;
        float m_initVelValue;
        bool m_withAngleLimit;
        int m_numCircles;
    };
}
