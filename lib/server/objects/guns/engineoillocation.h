#pragma once
#include <server/objects/temporarylocation.h>

namespace ai
{
    class EngineOilLocationPrototypeInfo : public ai::TemporaryLocationPrototypeInfo
    {
    public:
        EngineOilLocationPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const override;
    };

    // An oil slick: vehicles inside it are in "on oil" mode.
    class EngineOilLocation : public ai::TemporaryLocation
    {
        friend class EngineOilLocationPrototypeInfo;

    protected:
        virtual ~EngineOilLocation() override;

    private:
        EngineOilLocation(ai::EngineOilLocationPrototypeInfo const& prototypeInfo);
        EngineOilLocation(ai::EngineOilLocation const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classEngineOilLocation;
        virtual ai::EngineOilLocationPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        virtual void OnObjectIn(ai::Obj* object) override;
        virtual void OnObjectOut(ai::Obj* object) override;
    };
}  // namespace ai
