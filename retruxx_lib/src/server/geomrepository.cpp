#include "geomrepository.h"

#include <algorithm>
#include <stdexcept>

#include "geomrepositoryitem.h"
#include "game/m3dgame.h"
#include "objects/vehicle.h"
#include "objects/base/objcontainer.h"

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
        Purge();
        m_Changed = true;
        if (m_vehicleId >=0)
        {
            auto* obj = dynamic_cast<Vehicle*>(theObjects->GetEntityByObjId(m_vehicleId));
            if (obj)
            {
                obj->RefreshMass();
            }
        }
        M3D_APP->EnqueueMessage(66537, (int)this, 0, 0, 0, {}, {});
    }

    bool GeomRepository::SetGeomSize(PointBase<int> const& geomSize)
    {
        // TODO: generated code
        // Save the current slots and geometry size
        std::vector<GeomRepositoryItem> oldSlots = m_slots;
        PointBase<int> oldGeomSize = m_geomSize;

        // Set the new geometry size
        m_geomSize = geomSize;

        // Try to repack items with the new size
        if (_RepackItems(m_sortStyle))
        {
            // Success - old slots are automatically cleaned up when oldSlots goes out of scope
            return true;
        }
        else
        {
            // Failed - restore original state
            m_geomSize = oldGeomSize;
            m_slots = std::move(oldSlots);
            return false;
        }
    }

    PointBase<int> GeomRepository::GetGeomSize() const
    {
        return m_geomSize;
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

    void GeomRepository::Clear(bool bUnsafe)
    {
        for (auto& slot : m_slots)
        {
            slot.Clear(bUnsafe);
        }
        m_slots.clear();
        m_referenceChests.clear();
        SetChanged();
    }

    bool GeomRepository::AddThingToPlace(GeomRepositoryItem&, PointBase<int> const&)
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
        float mass = 0.0;
        for (const auto& slot : m_slots)
        {
            auto* obj = slot.GetObj();
            if (obj)
            {
                if (obj->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
                {
                    auto* physicObj = dynamic_cast<PhysicObj*>(obj);
                    mass += physicObj->GetMass();
                }
                else if (obj->IsKindOf(&ai::PhysicBody::m_classPhysicBody))
                {
                    auto* physicBody = dynamic_cast<PhysicBody*>(obj);
                    mass += physicBody->GetMass();
                }
            }
        }
        return mass;
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
        return new GeomRepository;
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

    void GeomRepository::TransferToRepository(GeomRepository* targetRepository)
    {
        // TODO: generated code
        // Early return if no target repository
        if (targetRepository == nullptr)
        {
            return;
        }

        // Vector to track items that failed to transfer
        std::vector<int> failedTransfers;

        // Transfer all valid items to target repository
        for (size_t i = 0; i < m_slots.size(); ++i)
        {
            GeomRepositoryItem currentItem = GetItem(static_cast<int>(i));

            if (currentItem.IsValid())
            {
                // Release ownership from current repository
                m_slots[i].GiveUpAmount(1);

                // Attempt to transfer to target repository
                bool transferSuccessful = targetRepository->AddThing(currentItem, 0);

                if (!transferSuccessful)
                {
                    // If transfer failed, remember the object ID to return it later
                    failedTransfers.push_back(currentItem.GetObjId());
                }
            }
        }

        // Clean up the current repository (remove transferred items)
        Purge();

        // Return any items that failed to transfer back to this repository
        for (int objId : failedTransfers)
        {
            GeomRepositoryItem item(objId);
            AddThing(item, 0);
        }

        // Notify that this repository has changed
        SetChanged();
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

    namespace
    {
        bool LessByResourceId(
            const ai::GeomRepositoryItem &item1,
            const ai::GeomRepositoryItem& item2)
        {
            return item1.m_resourceId < item2.m_resourceId;
        }
    }

    bool GeomRepository::_RepackItems(SortStyle sortStyle)
    {
        // TODO: generated code
        // If there are no slots to repack, return success
        if (m_slots.empty())
            return true;

        // Create a temporary copy of the current slots
        std::vector<GeomRepositoryItem> tempSlots = m_slots;

        // Clear the current repository
        Clear(true);

        bool result = true;

        switch (sortStyle)
        {
        case SORT_NONE:
        {
            // Add items back without sorting
            SortStyle originalSortStyle = m_sortStyle;
            SetSortStyle(SORT_NONE, false);

            for (const auto& item : tempSlots)
            {
                if (!AddThing(item, false))
                {
                    result = false;
                }
            }

            SetSortStyle(originalSortStyle, false);
            break;
        }

        case SORT_BY_RESOURCE:
        {
            // Sort items by resource ID
            std::sort(tempSlots.begin(), tempSlots.end(), LessByResourceId);

            int currentResourceId = -1;
            int minEmptyY = 0;

            for (const auto& item : tempSlots)
            {
                // Reset item position
                GeomRepositoryItem newItem = item;
                newItem.m_origin.x = -1;
                newItem.m_origin.y = -1;

                // Update minEmptyY when resource ID changes
                if (currentResourceId != newItem.m_resourceId)
                {
                    minEmptyY = _GetMinEmptyY();
                    currentResourceId = newItem.m_resourceId;
                }

                // Add the item
                if (!_AddThingUnsorted(newItem, minEmptyY))
                {
                    result = false;
                }
            }
            break;
        }

        default:
            // Unknown sort style
            result = false;
            break;
        }

        return result;
    }

    GeomRepository::GeomRepository()
    {
        this->m_Changed = 0;
        this->m_geomSize.x = 1;
        this->m_geomSize.y = 1;
        this->m_sortStyle = SORT_NONE;
        this->m_vehicleId = -1;
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
