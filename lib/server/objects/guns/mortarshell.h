#pragma once
#include <server/components/numericinrangeregenerating.h>
#include <server/objects/base/shell.h>

namespace ai
{
    class MortarShellPrototypeInfo : public ShellPrototypeInfo
    {
        friend class MortarShell;

    public:
        MortarShellPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();
        virtual Obj* CreateTargetObject() const;

    private:
        /* 0x0080 */ float m_velocity;
        /* 0x0084 */ float m_acceleration;
        /* 0x0088 */ float m_flyTime;
        /* 0x008c */ int m_blastWavePrototypeId;
        /* 0x0090 */ CStr m_blastWavePrototypeName;
    }; /* size: 0x009c */

    class MortarShell : public Shell
    {
    public:
        void SetInitialPosition(CVector const&);
        CVector GetInitialVelocity() const;
        CVector GetInitialPosition() const;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        void SetInitialVelocity(CVector const&);
        void CreateBlastWave() const;
        static m3d::Class* GetBaseClass();
        virtual MortarShellPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;
        virtual void Update(float, unsigned int);
        MortarShell(MortarShellPrototypeInfo const&);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    protected:
        virtual ~MortarShell();

    private:
        virtual m3d::Object* Clone();
        static m3d::Object* CreateObject();

    public:
        RT_CLASS_DECLARE(MortarShell);

    private:
        /* 0x0150 */ NumericInRangeRegenerating<float> m_lifeTime;
        /* 0x0228 */ CVector m_initialVelocity;
        /* 0x0234 */ CVector m_initialPosition;
    }; /* size: 0x0240 */

    static_assert(sizeof(MortarShellPrototypeInfo) == 0x9c);
    static_assert(sizeof(MortarShell) == 0x240);
}
