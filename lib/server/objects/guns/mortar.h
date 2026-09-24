#pragma once
#include "gun.h"

namespace ai
{
    class MortarPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        /* 0x0198 */ float m_initialVelocity;
        MortarPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x019c */

    static_assert(sizeof(MortarPrototypeInfo) == 0x019c);

    class Mortar : public ai::Gun
    {
        friend class MortarPrototypeInfo;
        friend class MortarVolleyLauncher;

    protected:
        virtual ~Mortar() override /* 0x00 */;

    private:
        Mortar(const ai::MortarPrototypeInfo& prototypeInfo);
        Mortar(const ai::Mortar&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        RT_CLASS_DECLARE(Mortar);
        virtual const ai::MortarPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        struct TMortarAngles
        {
            /* 0x0000 */ float phi;
            /* 0x0004 */ float theta;
            TMortarAngles(float _phi, float _theta);
            TMortarAngles();
            CVector ToVector() const;
        }; /* size: 0x0008 */
        virtual void LookAtPoint(const CVector& lookAt, float elapsedTime) override /* 0x168 */;
        virtual bool isLookAtPoint(const CVector& lookAt, float eps) const override /* 0x16c */;
        static CVector __fastcall GetMortarShellPosition(const CVector& source, const CVector& velocity, float time);

    protected:
        virtual void _LaunchShells() override /* 0x198 */;

    private:
        CVector GetVelocityByDirection(const CVector& dir) const;
        void setCurrentMortarAngles(const ai::Mortar::TMortarAngles& angles);
        float targetFunction(const CVector& src, const CVector& dst, float phi, float initVel, float time) const;
        float targetFunction2(const CVector& src, const CVector& dst, float phi, float initVel, float time, float theta) const;
        CVector GetMortarDirection(const CVector& currentDir, const CVector& src, const CVector& dst, float initVel) const;
    }; /* size: 0x0330 */

    static_assert(sizeof(Mortar) == 0x0330);
}
