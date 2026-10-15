#pragma once
#include "location.h"

namespace ai
{
    class TemporaryLocationPrototypeInfo : public LocationPrototypeInfo
    {
    public:
        TemporaryLocationPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        float m_TTL;
        float m_TimeForActivation;
        CStr m_EffectName;
    };

    class TemporaryLocation : public ai::Location
    {
    protected:
        virtual ~TemporaryLocation() override = 0;

        TemporaryLocation(ai::TemporaryLocationPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classTemporaryLocation;
        virtual ai::TemporaryLocationPrototypeInfo const* GetPrototypeInfo() const override;

        enum TemporaryLocationState
        {
            tlsActivation = 0,
            tlsWaiting = 1,
        };

    public:
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void Remove() override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        void CreateEffectNode();

    private:
        virtual void OnActivate();
        float m_TL;
        ai::TemporaryLocation::TemporaryLocationState m_TemporaryLocationState;
    };
}  // namespace ai
