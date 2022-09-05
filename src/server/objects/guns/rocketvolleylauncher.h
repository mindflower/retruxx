#pragma once
#include "rocketlauncher.h"

namespace ai
{
    class RocketVolleyLauncherPrototypeInfo : public RocketLauncherPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;
        RocketVolleyLauncherPrototypeInfo();

    private:
        float m_actionDist;
    };

    class RocketVolleyLauncher : public RocketLauncher
    {
    public:
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual bool Fire(bool);
        virtual void SetInvisible();
        virtual void Update(float, unsigned int);
        RocketVolleyLauncher(ai::RocketVolleyLauncherPrototypeInfo const&);
        virtual m3d::Class* GetClass() const;
        static m3d::Class* GetBaseClass();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual ai::RocketVolleyLauncherPrototypeInfo const* GetPrototypeInfo() const;

    protected:
        virtual bool _bIsUsingVolley() const;
        virtual bool _bIsVolleyFiring() const;
        virtual ~RocketVolleyLauncher();

    private:
        void _TryToLaunch();
        static m3d::Object* CreateObject();
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_DECLARE(RocketVolleyLauncher);

    private:
        std::vector<int> m_hadToLaunch;
        bool m_bIsVolleyFiring;
    };
}
