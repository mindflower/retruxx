#pragma once
#include <server/components/numericinrangeregenerating.h>
#include <server/objects/base/shell.h>

namespace ai
{
    class MortarShellPrototypeInfo : public ShellPrototypeInfo
    {
    public:
        MortarShellPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();
        virtual Obj* CreateTargetObject() const;

    private:
        float m_velocity;
        float m_acceleration;
        float m_flyTime;
        int m_blastWavePrototypeId;
        CStr m_blastWavePrototypeName;
    };

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
        RT_CLASS_INLINE_DECLARE(MortarShell);

    private:
        NumericInRangeRegenerating<float> m_lifeTime;
        CVector m_initialVelocity;
        CVector m_initialPosition;
    };
}
