#pragma once
#include "gun.h"

namespace ai
{
    class MortarPrototypeInfo : public GunPrototypeInfo
    {
    public:
        virtual void PostLoad();
        MortarPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;

    private:
        float m_initialVelocity;
    };

    class Mortar : public Gun
    {
    public:
        class TMortarAngles
        {
        public:
            CVector ToVector() const;
            TMortarAngles(float, float);
            TMortarAngles();

        private:
            float phi;
            float theta;
        };

    public:
        Mortar(MortarPrototypeInfo const&);
        virtual void LookAtPoint(CVector const&, float);
        virtual m3d::Class* GetClass() const;
        static CVector __fastcall GetMortarShellPosition(CVector const&, CVector const&, float);
        virtual bool isLookAtPoint(CVector const&, float) const;
        virtual MortarPrototypeInfo const* GetPrototypeInfo() const;
        static m3d::Class* GetBaseClass();

    protected:
        virtual ~Mortar();
        virtual void _LaunchShells();

    private:
        CVector GetVelocityByDirection(CVector const&) const;
        void setCurrentMortarAngles(TMortarAngles const&);
        virtual m3d::Object* Clone();
        float targetFunction2(CVector const&, CVector const&, float, float, float, float) const;
        float targetFunction(CVector const&, CVector const&, float, float, float) const;
        CVector GetMortarDirection(CVector const&, CVector const&, CVector const&, float) const;
        static m3d::Object* CreateObject();
    };
}
