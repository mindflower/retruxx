#pragma once
#include "physicbodies/vehiclepart.h"
#include <math/bounds2d.h>
#include <math/point2d.h>

namespace ai
{
    class BasketPrototypeInfo : public VehiclePartPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        PointBase<int> const& GetRepositorySize() const;
        std::map<CStr, PointBase<int>, std::less<CStr>, std::allocator<std::pair<CStr const, PointBase<int> > > > const& GetSlotPositions() const;
        int GetRepositoryCapacity() const;
        BoundsBase<int> GetSlotBounds(CStr const&, bool) const;
        BasketPrototypeInfo();
        virtual Obj* CreateTargetObject() const;

    protected:
        virtual void _InternalCopyFrom(PrototypeInfo const&);

    private:
        std::map<CStr, PointBase<int>> m_slots;
        PointBase<int> m_repositorySize;
    };

    class Basket : public VehiclePart
    {
    public:
        virtual m3d::Class* GetClass() const;
        Basket(BasketPrototypeInfo const&);
        static m3d::Class* GetBaseClass();
        virtual BasketPrototypeInfo const* GetPrototypeInfo() const;

    protected:
        virtual ~Basket();

    private:
        static m3d::Object* CreateObject();
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_INLINE_DECLARE(Basket);
    };
}
