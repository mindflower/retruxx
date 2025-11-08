#pragma once
#include "server/objects/temporarylocation.h"

namespace ai
{
    class SmokeScreenLocationPrototypeInfo : public ai::TemporaryLocationPrototypeInfo
    {
    public:
        SmokeScreenLocationPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x0094 */

    class SmokeScreenLocation : public ai::TemporaryLocation
    {
    protected:
        virtual  ~SmokeScreenLocation() override /* 0x00 */;

    private:
        SmokeScreenLocation(const ai::SmokeScreenLocationPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classSmokeScreenLocation;
        virtual const ai::SmokeScreenLocationPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        virtual void OnObjectIn(ai::Obj* object) override /* 0x00 */;
        virtual void OnObjectOut(ai::Obj* object) override /* 0x00 */;
    }; /* size: 0x0284 */
}
