#pragma once
#include "settlement.h"

namespace ai
{
    class InfectionLairPrototypeInfo : public SettlementPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        InfectionLairPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
    };

    class InfectionLair : public Settlement
    {
    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const;
        virtual InfectionLairPrototypeInfo const* GetPrototypeInfo() const;
        InfectionLair(InfectionLairPrototypeInfo const&);

    protected:
        virtual ~InfectionLair();

    private:
        static m3d::Object* CreateObject();
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_INLINE_DECLARE(InfectionLair);
    };
}
