#pragma once
#include "gun.h"

namespace ai
{
    class PlasmaBunchLauncherPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        PlasmaBunchLauncherPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;

    private:
        CStr m_bunchPrototypeName;
    };

    class PlasmaBunchLauncher : public ai::Gun
    {
        friend class PlasmaBunchLauncherPrototypeInfo;

    protected:
        virtual ~PlasmaBunchLauncher() override;

    private:
        PlasmaBunchLauncher(const ai::PlasmaBunchLauncherPrototypeInfo& prototypeInfo);
        PlasmaBunchLauncher(const ai::PlasmaBunchLauncher&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        RT_CLASS_DECLARE(PlasmaBunchLauncher);
        virtual const ai::PlasmaBunchLauncherPrototypeInfo* GetPrototypeInfo() const override;

    protected:
        virtual void _LaunchShells() override;
    };
}
