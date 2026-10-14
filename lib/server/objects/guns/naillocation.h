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
        virtual  ~NailLocation() override;

    private:
        NailLocation(const ai::NailLocationPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classNailLocation;
        virtual const ai::NailLocationPrototypeInfo* GetPrototypeInfo() const override;

    protected:
        virtual void OnObjectIn(ai::Obj* object) override;
        virtual void OnObjectOut(ai::Obj* object) override;
    };
}
