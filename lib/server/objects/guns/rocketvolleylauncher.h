#pragma once
#include "rocketlauncher.h"

namespace ai
{
    class RocketVolleyLauncherPrototypeInfo : public ai::RocketLauncherPrototypeInfo
    {
    public:
        /* 0x019c */ float m_actionDist;
        RocketVolleyLauncherPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x01a0 */

    static_assert(sizeof(RocketVolleyLauncherPrototypeInfo) == 0x01a0);

    // Fires a whole volley at once: one rocket per barrel at the nearest enemies in range, then
    // keeps launching until every queued target has had its rocket.
    class RocketVolleyLauncher : public ai::RocketLauncher
    {
        friend class RocketVolleyLauncherPrototypeInfo;

    protected:
        virtual ~RocketVolleyLauncher() override /* 0x00 */;

    private:
        RocketVolleyLauncher(const ai::RocketVolleyLauncherPrototypeInfo& prototypeInfo);
        RocketVolleyLauncher(const ai::RocketVolleyLauncher&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        RT_CLASS_DECLARE(RocketVolleyLauncher);
        virtual const ai::RocketVolleyLauncherPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void SetInvisible() override /* 0x00 */;
        virtual bool Fire(bool enable) override /* 0x00 */;

    protected:
        virtual bool _bIsVolleyFiring() const override /* 0x00 */;
        virtual bool _bIsUsingVolley() const override /* 0x00 */;

    private:
        /* 0x0330 */ std::vector<int, std::allocator<int> > m_hadToLaunch;
        /* 0x0340 */ bool m_bIsVolleyFiring;
        void _TryToLaunch();
    }; /* size: 0x0344 */

    static_assert(sizeof(RocketVolleyLauncher) == 0x0344);
}
