#pragma once
#include "server/objects/temporarylocation.h"

namespace ai
{
    class SmokeScreenLocationPrototypeInfo : public ai::TemporaryLocationPrototypeInfo
    {
    public:
        SmokeScreenLocationPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class SmokeScreenLocation : public ai::TemporaryLocation
    {
        friend class SmokeScreenLocationPrototypeInfo;

    protected:
        virtual ~SmokeScreenLocation() override;

    private:
        SmokeScreenLocation(ai::SmokeScreenLocationPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classSmokeScreenLocation;
        virtual ai::SmokeScreenLocationPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        virtual void OnObjectIn(ai::Obj* object) override;
        virtual void OnObjectOut(ai::Obj* object) override;
    };
}  // namespace ai
