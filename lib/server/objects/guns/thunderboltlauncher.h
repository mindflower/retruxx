#pragma once
#include "gun.h"

namespace ai
{
    class ThunderboltLauncherPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        ThunderboltLauncherPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        /* 0x0198 */ float m_ActionDist;
    }; /* size: 0x019c */

    static_assert(sizeof(ThunderboltLauncherPrototypeInfo) == 0x019c);

    // Throws a branching lightning bolt at every enemy in range on its side of the vehicle.
    class ThunderboltLauncher : public ai::Gun
    {
        friend class ThunderboltLauncherPrototypeInfo;

    protected:
        virtual ~ThunderboltLauncher() override /* 0x00 */;

    private:
        ThunderboltLauncher(const ai::ThunderboltLauncherPrototypeInfo& prototypeInfo);
        ThunderboltLauncher(const ai::ThunderboltLauncher&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        RT_CLASS_DECLARE(ThunderboltLauncher);
        virtual const ai::ThunderboltLauncherPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        virtual bool CanFire() const override /* 0x190 */;

    protected:
        /* 0x0330 */ std::vector<int, std::allocator<int> > m_enemies;
        virtual void _LaunchShells() override /* 0x198 */;
    }; /* size: 0x0340 */

    static_assert(sizeof(ThunderboltLauncher) == 0x0340);
}
