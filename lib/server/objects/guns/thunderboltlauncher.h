#pragma once
#include "gun.h"

namespace ai
{
    class ThunderboltLauncherPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        ThunderboltLauncherPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;
        float m_ActionDist;
    };

    // Throws a branching lightning bolt at every enemy in range on its side of the vehicle.
    class ThunderboltLauncher : public ai::Gun
    {
        friend class ThunderboltLauncherPrototypeInfo;

    protected:
        virtual ~ThunderboltLauncher() override;

    private:
        ThunderboltLauncher(ai::ThunderboltLauncherPrototypeInfo const& prototypeInfo);
        ThunderboltLauncher(ai::ThunderboltLauncher const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        RT_CLASS_DECLARE(ThunderboltLauncher);
        virtual ai::ThunderboltLauncherPrototypeInfo const* GetPrototypeInfo() const override;
        virtual bool CanFire() const override;

    protected:
        std::vector<int, std::allocator<int>> m_enemies;
        virtual void _LaunchShells() override;
    };
}  // namespace ai
