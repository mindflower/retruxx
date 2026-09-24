#pragma once
#include "gun.h"

namespace ai
{
    class PlasmaBunchLauncherPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        PlasmaBunchLauncherPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;

    private:
        /* 0x0198 */ CStr m_bunchPrototypeName;
    }; /* size: 0x01a4 */

    static_assert(sizeof(PlasmaBunchLauncherPrototypeInfo) == 0x01a4);

    class PlasmaBunchLauncher : public ai::Gun
    {
        friend class PlasmaBunchLauncherPrototypeInfo;

    protected:
        virtual ~PlasmaBunchLauncher() override /* 0x00 */;

    private:
        PlasmaBunchLauncher(const ai::PlasmaBunchLauncherPrototypeInfo& prototypeInfo);
        PlasmaBunchLauncher(const ai::PlasmaBunchLauncher&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        RT_CLASS_DECLARE(PlasmaBunchLauncher);
        virtual const ai::PlasmaBunchLauncherPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        virtual void _LaunchShells() override /* 0x198 */;
    }; /* size: 0x0330 */

    static_assert(sizeof(PlasmaBunchLauncher) == 0x0330);
}
