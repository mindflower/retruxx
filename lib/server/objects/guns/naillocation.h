#pragma once
#include "server/objects/temporarylocation.h"

namespace ai
{
    class NailLocationPrototypeInfo : public ai::TemporaryLocationPrototypeInfo
    {
    public:
        NailLocationPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class NailLocation : public ai::TemporaryLocation
    {
        friend class NailLocationPrototypeInfo;

    protected:
        virtual ~NailLocation() override;

    private:
        NailLocation(ai::NailLocationPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classNailLocation;
        virtual ai::NailLocationPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        virtual void OnObjectIn(ai::Obj* object) override;
        virtual void OnObjectOut(ai::Obj* object) override;
    };
}  // namespace ai
