#pragma once
#include <server/components/numericinrangeregenerating.h>
#include <server/objects/base/shell.h>

namespace ai
{
    class RocketPrototypeInfo : public ai::ShellPrototypeInfo
    {
    public:
        /* 0x0080 */ float m_velocity;
        /* 0x0084 */ float m_acceleration;
        /* 0x0088 */ float m_minTurningRadius;
        /* 0x008c */ float m_flyTime;
        /* 0x0090 */ int m_BlastWavePrototypeId;
        RocketPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;

    private:
        /* 0x0094 */ CStr m_BlastWavePrototypeName;
    }; /* size: 0x00a0 */

    static_assert(sizeof(RocketPrototypeInfo) == 0x00a0);

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
        virtual ~Rocket() override /* 0x00 */;

    protected:
        // NOTE: private in the PDB; protected here because Mine derives from Rocket.
        Rocket(const ai::RocketPrototypeInfo& prototypeInfo);
        Rocket(const ai::Rocket&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        RT_CLASS_DECLARE(Rocket);
        virtual const ai::RocketPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        /* 0x0150 */ int m_targetObjId;
        float GetVelocity() const;
        virtual void Remove() override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        void SetInitialDirection(const CVector& direction);
        void SetInitialVelocity(const CVector& initVel);
        void CreateBlastWave() const;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        bool getWithAngleLimit() const;
        void setWithAngleLimit(bool value);

    private:
        /* 0x0154 */ ai::NumericInRangeRegenerating<float> m_velocity;
        /* 0x022c */ ai::NumericInRangeRegenerating<float> m_lifeTime;
        /* 0x0304 */ float m_minTurningRadius;
        /* 0x0308 */ CVector m_initialDirection;
        /* 0x0314 */ CVector m_initVelDir;
        /* 0x0320 */ float m_initVelValue;
        /* 0x0324 */ bool m_withAngleLimit;
        /* 0x0325 */ char Padding_336[3];
        /* 0x0328 */ int m_numCircles;
    }; /* size: 0x032c */

    static_assert(sizeof(Rocket) == 0x032c);
}
