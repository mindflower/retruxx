#pragma once
#include <vector>
#include <core/clazz.h>
#include <math/point2d.h>

namespace ai
{
    class Chest;
    class Vehicle;
    struct GeomRepositoryItem;

    class GeomRepository : public m3d::Object
    {
    protected:
        GeomRepository(PointBase<int> const& geomSize);
        GeomRepository();
        GeomRepository(ai::GeomRepository const& rep);

    public:
        virtual ~GeomRepository() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classGeomRepository;

    protected:
        bool m_Changed;
        PointBase<int> m_geomSize;
        std::vector<ai::GeomRepositoryItem, std::allocator<ai::GeomRepositoryItem>> m_slots;
        std::set<int, std::less<int>, std::allocator<int>> m_referenceChests;
        int m_vehicleId;

    public:
        // Public because SetSortStyle below is: a caller cannot name the argument otherwise.
        enum SortStyle
        {
            SORT_NONE = 0,
            SORT_BY_RESOURCE = 1,
        };

        ai::GeomRepository::SortStyle GetSortStyle() const;
        bool SetSortStyle(ai::GeomRepository::SortStyle sortStyle, bool bRepackImmediate);
        bool Sort(ai::GeomRepository::SortStyle sortStyle);
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        virtual bool SetGeomSize(PointBase<int> const& geomSize);
        PointBase<int> GetGeomSize() const;
        int GetFirstSlotByResourceId(int resourceId) const;
        int GetSlotByPlace(PointBase<int> const& pt) const;
        unsigned int GetAmountInSlot(int slotNum) const;
        unsigned int GetAmountByResourceId(int resourceId) const;
        unsigned int GetAmountByPrototypeId(int prototypeId) const;
        unsigned int GetNumItems() const;
        ai::GeomRepositoryItem GetItem(int slotNum) const;
        bool AppendChest(ai::Chest* pChest);
        void FlushInReferenceChests(CVector const& pos);
        bool IsChanged();
        virtual void Purge();
        bool IsEmpty();
        void DropAll(CVector const& Position);
        virtual int CanAddThingToPlace(
            ai::GeomRepositoryItem const& item,
            PointBase<int> const& origin,
            int* existingSlot) const;
        virtual bool AddThingToPlace(ai::GeomRepositoryItem& item, PointBase<int> const& origin);
        bool CanAddThing(ai::GeomRepositoryItem const& item) const;
        bool AddThing(ai::GeomRepositoryItem const& item, int minY);
        int GetFirstNonCompleteSlot(ai::GeomRepositoryItem const& item) const;
        int GetSlotByObjId(int objId) const;
        ai::GeomRepositoryItem PutThingToSlot(int slotNum, ai::GeomRepositoryItem const& item);
        int GiveUpThingByResourceId(int resourceId, int amount);
        int GiveUpThingByPrototypeId(int prototypeId, int amount);
        bool GiveUpThingByObjId(int objId);
        unsigned int GiveUpThingFromSlot(unsigned int slotNum, unsigned int Amount);
        unsigned int GiveUpThingFromSlotUnsafe(unsigned int slotNum, unsigned int amount);
        virtual void Clear(bool bUnsafe);
        void SetChanged();
        bool AddItems(int prototypeId, int amount);
        bool AddItems(char const* prototypeName, int amount);
        bool RemoveItems(char const* prototypeName, int amount);
        void TransferToRepository(ai::GeomRepository* targetRepository);
        bool HasAmountOfItems(char const* prototypeName, int amount) const;
        bool CanPlaceItems(int prototypeId, int amount);
        bool CanPlaceItems(char const* prototypeName, int amount);
        void DetachAllObjectsFromParents();
        void ApplyDamageToAllItems(float damage);
        void SetVehicle(ai::Vehicle* vehicle);
        ai::Vehicle* GetVehicle() const;
        float GetMass() const;

    protected:
        bool _RepackItems(ai::GeomRepository::SortStyle sortStyle);
        bool _AddThingUnsorted(ai::GeomRepositoryItem const& item, int minY);
        int _GetMinEmptyY() const;
        ai::GeomRepository::SortStyle m_sortStyle;

    private:
        bool _AppendRepository(ai::GeomRepository* repository, ai::Chest* pChest);
        bool _bItemInBounds(ai::GeomRepositoryItem const& item) const;
    };
}  // namespace ai
