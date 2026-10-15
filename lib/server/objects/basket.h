#pragma once
#include "physicbodies/vehiclepart.h"
#include <math/bounds2d.h>
#include <math/point2d.h>

namespace ai
{
    class BasketPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(ai::PrototypeInfo const& rhs) override;

        using SlotPositionMap = retruxx::map<
            CStr,
            PointBase<int>,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, PointBase<int>>>>;

    public:
        BasketPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        retruxx::map<
            CStr,
            PointBase<int>,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, PointBase<int>>>> const&
            GetSlotPositions() const;
        PointBase<int> const& GetRepositorySize() const;
        int GetRepositoryCapacity() const;
        BoundsBase<int> GetSlotBounds(CStr const& gunPartName, bool bWithEmptyBorders) const;

        using SlotPositionPair = retruxx::pair<CStr, PointBase<int>>;

    protected:
        retruxx::map<
            CStr,
            PointBase<int>,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, PointBase<int>>>>
            m_slots;
        PointBase<int> m_repositorySize;
    };

    class Basket : public ai::VehiclePart
    {
        friend class BasketPrototypeInfo;

    protected:
        virtual ~Basket() override;

    private:
        Basket(ai::BasketPrototypeInfo const& prototypeInfo);
        Basket(ai::Basket const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBasket;
        virtual ai::BasketPrototypeInfo const* GetPrototypeInfo() const override;
    };
}  // namespace ai
