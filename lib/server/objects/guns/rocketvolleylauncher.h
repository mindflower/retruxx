#pragma once
#include "rocketlauncher.h"

namespace ai
{
    class RocketVolleyLauncherPrototypeInfo : public ai::RocketLauncherPrototypeInfo
    {
    public:
        float m_actionDist;
        RocketVolleyLauncherPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    // Fires a whole volley at once: one rocket per barrel at the nearest enemies in range, then
    // keeps launching until every queued target has had its rocket.
    class RocketVolleyLauncher : public ai::RocketLauncher
    {
        friend class RocketVolleyLauncherPrototypeInfo;

    protected:
        virtual ~RocketVolleyLauncher() override;

    private:
        RocketVolleyLauncher(ai::RocketVolleyLauncherPrototypeInfo const& prototypeInfo);
        RocketVolleyLauncher(ai::RocketVolleyLauncher const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        RT_CLASS_DECLARE(RocketVolleyLauncher);
        virtual ai::RocketVolleyLauncherPrototypeInfo const* GetPrototypeInfo() const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void SetInvisible() override;
        virtual bool Fire(bool enable) override;

    protected:
        virtual bool _bIsVolleyFiring() const override;
        virtual bool _bIsUsingVolley() const override;

    private:
        std::vector<int, std::allocator<int>> m_hadToLaunch;
        bool m_bIsVolleyFiring;
        void _TryToLaunch();
    };
}  // namespace ai
