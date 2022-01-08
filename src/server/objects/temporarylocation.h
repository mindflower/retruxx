#pragma once
#include "location.h"

namespace ai
{
    class TemporaryLocationPrototypeInfo : public LocationPrototypeInfo
    {
    public:
        TemporaryLocationPrototypeInfo();
        virtual class Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        float m_TTL;
        float m_TimeForActivation;
        CStr m_EffectName;
    };

    class TemporaryLocation : public Location
    {
    public:
        enum TemporaryLocationState
        {
            tlsActivation = 0x0,
            tlsWaiting = 0x1,
        };

    public:
        void CreateEffectNode();
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual void Remove();
        TemporaryLocation(class TemporaryLocationPrototypeInfo const&);
        virtual m3d::Class* GetClass() const;
        virtual class TemporaryLocationPrototypeInfo const* GetPrototypeInfo() const;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        static m3d::Class* GetBaseClass();
        virtual void Update(float, unsigned int);

    protected:
        virtual ~TemporaryLocation();

    private:
        virtual class m3d::Object* Clone();
        static class m3d::Object* CreateObject();
        virtual void OnActivate();

    private:
        float m_TL;
        TemporaryLocationState m_TemporaryLocationState;
    };
}
