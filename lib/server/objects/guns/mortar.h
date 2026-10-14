#pragma once
#include "gun.h"

namespace ai
{
    class MortarPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        float m_initialVelocity;
        MortarPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
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
        Mortar(const ai::MortarPrototypeInfo& prototypeInfo);
        Mortar(const ai::Mortar&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        RT_CLASS_DECLARE(Mortar);
        virtual const ai::MortarPrototypeInfo* GetPrototypeInfo() const override;
        struct TMortarAngles
        {
            float phi;
            float theta;
            TMortarAngles(float _phi, float _theta);
            TMortarAngles();
            CVector ToVector() const;
        };
        virtual void LookAtPoint(const CVector& lookAt, float elapsedTime) override;
        virtual bool isLookAtPoint(const CVector& lookAt, float eps) const override;
        static CVector __fastcall GetMortarShellPosition(const CVector& source, const CVector& velocity, float time);

    protected:
        virtual void _LaunchShells() override;

    private:
        CVector GetVelocityByDirection(const CVector& dir) const;
        void setCurrentMortarAngles(const ai::Mortar::TMortarAngles& angles);
        float targetFunction(const CVector& src, const CVector& dst, float phi, float initVel, float time) const;
        float targetFunction2(const CVector& src, const CVector& dst, float phi, float initVel, float time, float theta) const;
        CVector GetMortarDirection(const CVector& currentDir, const CVector& src, const CVector& dst, float initVel) const;
    };
}
