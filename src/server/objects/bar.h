#pragma once
#include "building.h"

namespace ai
{
   class BarPrototypeInfo : public BuildingPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        BarPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    protected:
        virtual void _InternalCopyFrom(PrototypeInfo const&);

    private:
        bool m_bWithBarman;
    };

    class Bar : public Building
    {
    public:
        static inline m3d::Class m_classBar;

    public:
        bool bWithBarman() const;
        virtual bool CanChildBeAdded(m3d::Class*) const;
        Bar(BarPrototypeInfo const&);
        virtual bool RemoveChild(Obj*);
        virtual void Update(float, unsigned int);
        virtual m3d::Class* GetClass() const;
        virtual BarPrototypeInfo const* GetPrototypeInfo() const;
        static m3d::Class* __fastcall GetBaseClass();
        virtual void CreateChildren();
        virtual void AddChild(Obj*);

    protected:
        virtual ~Bar();

    private:
        void CreateBarman();
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();

    private:
        int m_barmanId;
    };
}
