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
        virtual  ~SmokeScreenLocation() override;

    private:
        SmokeScreenLocation(const ai::SmokeScreenLocationPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classSmokeScreenLocation;
        virtual const ai::SmokeScreenLocationPrototypeInfo* GetPrototypeInfo() const override;

    protected:
        virtual void OnObjectIn(ai::Obj* object) override;
        virtual void OnObjectOut(ai::Obj* object) override;
    };
}
