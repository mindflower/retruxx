#pragma once
#include "physicbodies/vehiclepart.h"
#include <math/bounds2d.h>
#include <math/point2d.h>

namespace ai
{
    class BasketPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(const ai::PrototypeInfo& rhs) override;

        using SlotPositionMap = retruxx::map<CStr, PointBase<int>, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, PointBase<int> > > >;

    public:
        BasketPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        const retruxx::map<CStr, PointBase<int>, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, PointBase<int> > > >& GetSlotPositions() const;
        const PointBase<int>& GetRepositorySize() const;
        int GetRepositoryCapacity() const;
        BoundsBase<int> GetSlotBounds(const CStr& gunPartName, bool bWithEmptyBorders) const;

        using SlotPositionPair = retruxx::pair<CStr, PointBase<int> >;

    protected:
        retruxx::map<CStr, PointBase<int>, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, PointBase<int> > > > m_slots;
        PointBase<int> m_repositorySize;
    };

    class Basket : public ai::VehiclePart
    {
        friend class BasketPrototypeInfo;
    protected:
        virtual  ~Basket() override;

    private:
        Basket(const ai::BasketPrototypeInfo& prototypeInfo);
        Basket(const ai::Basket&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBasket;
        virtual const ai::BasketPrototypeInfo* GetPrototypeInfo() const override;
    };
}
