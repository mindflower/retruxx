#include "geomrepository.h"

#include <stdexcept>

#include "geomrepositoryitem.h"

RT_CLASS_EXPORT_METHOD_DEFINE(GeomRepository, AddItems)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(GeomRepository, RemoveItems)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(GeomRepository, HasAmountOfItems)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(GeomRepository, CanPlaceItems)
{
    throw std::logic_error("Not implemented");
}


namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(GeomRepository)
        RT_CLASS_EXPORT(GeomRepository, m3d::METHOD, AddItems, "", "", "")
        RT_CLASS_EXPORT(GeomRepository, m3d::METHOD, RemoveItems, "", "", "")
        RT_CLASS_EXPORT(GeomRepository, m3d::METHOD, HasAmountOfItems, "", "", "")
        RT_CLASS_EXPORT(GeomRepository, m3d::METHOD, CanPlaceItems, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(GeomRepository);

    bool GeomRepository::RemoveItems(char const*, int)
    {
        throw std::logic_error("Not implemented");
    }

    int GeomRepository::GetSlotByPlace(PointBase<int> const&) const
    {
        throw std::logic_error("Not implemented");
    }

    GeomRepository::~GeomRepository()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomRepository::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void GeomRepository::DropAll(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomRepository::SetChanged()
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::SetGeomSize(PointBase<int> const&)
    {
        throw std::logic_error("Not implemented");
    }

    PointBase<int> GeomRepository::GetGeomSize() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned GeomRepository::GetAmountByPrototypeId(int) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* GeomRepository::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    GeomRepository::GeomRepository(PointBase<int> const&)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomRepository::Clear(bool)
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::AddThingToPlace(GeomRepositoryItem&, PointBase<int> const&)
    {
        throw std::logic_error("Not implemented");
    }

    GeomRepository& GeomRepository::operator=(GeomRepository const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::CanPlaceItems(char const*, int)
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::CanPlaceItems(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::SetSortStyle(SortStyle, bool)
    {
        throw std::logic_error("Not implemented");
    }

    GeomRepository::SortStyle GeomRepository::GetSortStyle() const
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::IsEmpty()
    {
        throw std::logic_error("Not implemented");
    }

    int GeomRepository::GetSlotByObjId(int) const
    {
        throw std::logic_error("Not implemented");
    }

    GeomRepositoryItem GeomRepository::GetItem(int) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned GeomRepository::GetAmountByResourceId(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::HasAmountOfItems(char const*, int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::IsChanged()
    {
        throw std::logic_error("Not implemented");
    }

    int GeomRepository::CanAddThingToPlace(GeomRepositoryItem const&, PointBase<int> const&, int*) const
    {
        throw std::logic_error("Not implemented");
    }

    float GeomRepository::GetMass() const
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::Sort(SortStyle)
    {
        throw std::logic_error("Not implemented");
    }

    int GeomRepository::GiveUpThingByPrototypeId(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::CanAddThing(GeomRepositoryItem const&) const
    {
        throw std::logic_error("Not implemented");
    }

    GeomRepositoryItem GeomRepository::PutThingToSlot(int, GeomRepositoryItem const&)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomRepository::SetVehicle(Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    Vehicle* GeomRepository::GetVehicle() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned GeomRepository::GiveUpThingFromSlot(unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::AddThing(GeomRepositoryItem const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned GeomRepository::GetNumItems() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned GeomRepository::GiveUpThingFromSlotUnsafe(unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* GeomRepository::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    int GeomRepository::GiveUpThingByResourceId(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    int GeomRepository::GetFirstSlotByResourceId(int) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* GeomRepository::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    bool GeomRepository::AddItems(char const*, int)
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::AddItems(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    int GeomRepository::GetFirstNonCompleteSlot(GeomRepositoryItem const&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* GeomRepository::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomRepository::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomRepository::TransferToRepository(GeomRepository*)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomRepository::Purge()
    {
        throw std::logic_error("Not implemented");
    }

    void GeomRepository::FlushInReferenceChests(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::AppendChest(Chest*)
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::GiveUpThingByObjId(int)
    {
        throw std::logic_error("Not implemented");
    }

    void GeomRepository::DetachAllObjectsFromParents()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned GeomRepository::GetAmountInSlot(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void GeomRepository::ApplyDamageToAllItems(float)
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::_RepackItems(SortStyle)
    {
        throw std::logic_error("Not implemented");
    }

    GeomRepository::GeomRepository()
    {
        throw std::logic_error("Not implemented");
    }

    GeomRepository::GeomRepository(GeomRepository const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::_AddThingUnsorted(GeomRepositoryItem const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    int GeomRepository::_GetMinEmptyY() const
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::_bItemInBounds(GeomRepositoryItem const&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool GeomRepository::_AppendRepository(GeomRepository*, Chest*)
    {
        throw std::logic_error("Not implemented");
    }
}
