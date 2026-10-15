#pragma once
#include "gun.h"

namespace ai
{
    class MortarPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        float m_initialVelocity;
        MortarPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Mortar : public ai::Gun
    {
        friend class MortarPrototypeInfo;
        friend class MortarVolleyLauncher;

    protected:
        virtual ~Mortar() override;

    private:
        Mortar(ai::MortarPrototypeInfo const& prototypeInfo);
        Mortar(ai::Mortar const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        RT_CLASS_DECLARE(Mortar);
        virtual ai::MortarPrototypeInfo const* GetPrototypeInfo() const override;
        struct TMortarAngles
        {
            float phi;
            float theta;
            TMortarAngles(float _phi, float _theta);
            TMortarAngles();
            CVector ToVector() const;
        };
        virtual void LookAtPoint(CVector const& lookAt, float elapsedTime) override;
        virtual bool isLookAtPoint(CVector const& lookAt, float eps) const override;
        static CVector GetMortarShellPosition(CVector const& source, CVector const& velocity, float time);

    protected:
        virtual void _LaunchShells() override;

    private:
        CVector GetVelocityByDirection(CVector const& dir) const;
        void setCurrentMortarAngles(ai::Mortar::TMortarAngles const& angles);
        float targetFunction(CVector const& src, CVector const& dst, float phi, float initVel, float time) const;
        float targetFunction2(CVector const& src, CVector const& dst, float phi, float initVel, float time, float theta)
            const;
        CVector GetMortarDirection(CVector const& currentDir, CVector const& src, CVector const& dst, float initVel)
            const;
    };
}  // namespace ai
