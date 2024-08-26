#pragma once
#include <vector>
#include <core/clazz.h>
#include <math/point2d.h>

namespace ai
{
    class Chest;
    class Vehicle;
    class GeomRepositoryItem;

    class GeomRepository :  public m3d::Object
    {
    public:
        enum SortStyle
        {
            SORT_NONE = 0x0,
            SORT_BY_RESOURCE = 0x1,
        };

    public:
        bool RemoveItems(char const *,int);
        int GetSlotByPlace(PointBase<int> const &) const ;
        virtual ~GeomRepository();
        void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void DropAll(CVector const &);
        void SetChanged();
        virtual bool SetGeomSize(PointBase<int> const &);
        PointBase<int> GetGeomSize() const ;
        unsigned int GetAmountByPrototypeId(int) const ;
        virtual m3d::Class* GetClass() const ;
        GeomRepository(PointBase<int> const &);
        virtual void Clear(bool);
        virtual bool AddThingToPlace(GeomRepositoryItem &,PointBase<int> const &);
        GeomRepository & operator=(GeomRepository const &);
        bool CanPlaceItems(char const *,int);
        bool CanPlaceItems(int,int);
        bool SetSortStyle(SortStyle,bool);
        SortStyle GetSortStyle() const ;
        bool IsEmpty();
        int GetSlotByObjId(int) const ;
        GeomRepositoryItem GetItem(int) const ;
        unsigned int GetAmountByResourceId(int) const ;
        bool HasAmountOfItems(char const *,int) const ;
        bool IsChanged();
        virtual int CanAddThingToPlace(GeomRepositoryItem const &,PointBase<int> const &,int *) const ;
        float GetMass() const ;
        bool Sort(SortStyle);
        int GiveUpThingByPrototypeId(int,int);
        bool CanAddThing(GeomRepositoryItem const &) const ;
        GeomRepositoryItem PutThingToSlot(int,GeomRepositoryItem const &);
        void SetVehicle(Vehicle *);
        Vehicle * GetVehicle() const ;
        unsigned int GiveUpThingFromSlot(unsigned int,unsigned int);
        bool AddThing(GeomRepositoryItem const &,int);
        unsigned int GetNumItems() const ;
        unsigned int GiveUpThingFromSlotUnsafe(unsigned int,unsigned int);
        static m3d::Object * CreateObject();
        int GiveUpThingByResourceId(int,int);
        int GetFirstSlotByResourceId(int) const ;
        static m3d::Class* GetBaseClass();
        bool AddItems(char const *,int);
        bool AddItems(int,int);
        int GetFirstNonCompleteSlot(GeomRepositoryItem const &) const ;
        virtual m3d::Object * Clone();
        void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void TransferToRepository(GeomRepository *);
        virtual void Purge();
        void FlushInReferenceChests(CVector const &);
        bool AppendChest(Chest *);
        bool GiveUpThingByObjId(int);
        void DetachAllObjectsFromParents();
        unsigned int GetAmountInSlot(int) const ;
        void ApplyDamageToAllItems(float);

    protected:
        bool _RepackItems(SortStyle);
        GeomRepository();
        GeomRepository(GeomRepository const &);
        bool _AddThingUnsorted(GeomRepositoryItem const &,int);
        int _GetMinEmptyY() const ;

    private:
        bool _bItemInBounds(GeomRepositoryItem const &) const ;
        bool _AppendRepository(GeomRepository *,Chest *);

    public:
        RT_CLASS_DECLARE(GeomRepository);

    private:
        bool m_Changed;
        PointBase<int> m_geomSize;
        std::vector<GeomRepositoryItem> m_slots;
        std::set<int> m_referenceChests;
        int m_vehicleId;
        SortStyle m_sortStyle;
    };
}
