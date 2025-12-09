#pragma once
#include "server/objects/temporarylocation.h"

namespace ai
{
    class NailLocationPrototypeInfo : public ai::TemporaryLocationPrototypeInfo
    {
    public:
        NailLocationPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x0094 */

    class NailLocation : public ai::TemporaryLocation
    {
    protected:
        virtual  ~NailLocation() override /* 0x00 */;

    private:
        NailLocation(const ai::NailLocationPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classNailLocation;
        virtual const ai::NailLocationPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        virtual void OnObjectIn(ai::Obj* object) override /* 0x00 */;
        virtual void OnObjectOut(ai::Obj* object) override /* 0x00 */;
    }; /* size: 0x0284 */
}
