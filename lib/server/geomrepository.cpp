#include "geomrepository.h"

#include <algorithm>
#include <stdexcept>

#include <core/log.h>

#include "geomrepositoryitem.h"
#include "game/m3dgame.h"
#include "objects/base/prototypemanager.h"
#include "objects/chest.h"
#include "objects/guns/compoundgun.h"
#include "objects/guns/gun.h"
#include "objects/vehicle.h"
#include "objects/base/objcontainer.h"

RT_CLASS_EXPORT_METHOD_DEFINE(GeomRepository, AddItems)
{
    auto* repository = dynamic_cast<ai::GeomRepository*>(context->asObject(0, "GeomRepository"));
    context->pushBool(repository->AddItems(context->asString(1), context->asInt(2)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GeomRepository, RemoveItems)
{
    auto* repository = dynamic_cast<ai::GeomRepository*>(context->asObject(0, "GeomRepository"));
    context->pushBool(repository->RemoveItems(context->asString(1), context->asInt(2)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GeomRepository, HasAmountOfItems)
{
    auto* repository = dynamic_cast<ai::GeomRepository*>(context->asObject(0, "GeomRepository"));
    context->pushBool(repository->HasAmountOfItems(context->asString(1), context->asInt(2)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GeomRepository, CanPlaceItems)
{
    auto* repository = dynamic_cast<ai::GeomRepository*>(context->asObject(0, "GeomRepository"));
    context->pushBool(repository->CanPlaceItems(context->asString(1), context->asInt(2)));
    return 1;
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

    bool GeomRepository::RemoveItems(char const* prototypeName, int amount)
    {
        // RVA 0x6CB380
        if (!HasAmountOfItems(prototypeName, amount))
        {
            return false;
        }
        int const prototypeId = thePrototypeManager->GetPrototypeId(CStr(prototypeName));
        return GiveUpThingByPrototypeId(prototypeId, amount) == amount;
    }

    int GeomRepository::GetSlotByPlace(PointBase<int> const& pt) const
    {
        // RVA 0x6C8A00
        for (int i = 0; i < static_cast<int>(m_slots.size()); ++i)
        {
            auto const bounds = m_slots[i].GetBounds();
            if (pt.x >= bounds.x0 && pt.x < bounds.x0 + bounds.width && pt.y >= bounds.y0 &&
                pt.y < bounds.y0 + bounds.height)
            {
                return i;
            }
        }
        return -1;
    }

    GeomRepository::~GeomRepository()
    {
    }

    void GeomRepository::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6C8460
        // Only object-backed slots are persisted; resource slots are rebuilt on load.
        for (auto const& item : m_slots)
        {
            if (item.GetObjId() == -1)
            {
                continue;
            }
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Item");
            item.SaveToXML(xmlFile, node);
            xmlNode->AddChild(node);
        }
    }

    void GeomRepository::DropAll(CVector const&)
    {
        // RVA 0x6C7FF0
        // NOTE: the shipped code ignores Position and simply clears the repository.
        Clear(false);
    }

    void GeomRepository::SetChanged()
    {
        Purge();
        m_Changed = true;
        if (m_vehicleId >= 0)
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

    unsigned GeomRepository::GetAmountByPrototypeId(int prototypeId) const
    {
        // RVA 0x6C8B60
        unsigned total = 0;
        for (int i = 0; i < static_cast<int>(m_slots.size()); ++i)
        {
            if (m_slots[i].GetPrototypeId() == prototypeId)
            {
                total += GetAmountInSlot(i);
            }
        }
        return total;
    }

    m3d::Class* GeomRepository::GetClass() const
    {
        // RVA 0x6C7FD0
        return RT_CLASS_LOCAL(GeomRepository);
    }

    GeomRepository::GeomRepository(PointBase<int> const& geomSize) :
        m_Changed(false),
        m_geomSize(geomSize),
        m_vehicleId(-1)
    {
        // RVA 0x6CA370. NOTE: unlike the default constructor the shipped code
        // leaves m_sortStyle uninitialised here.
        m_sortStyle = SORT_NONE;
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

    bool GeomRepository::AddThingToPlace(GeomRepositoryItem& item, PointBase<int> const& origin)
    {
        // RVA 0x6CABB0
        if (!item.IsValid() || (item.m_repositoryItemType == GeomRepositoryItem::ITEMTYPE_RESOURCE && !item.m_amount))
        {
            return false;
        }

        int existingSlot = -1;
        if (CanAddThingToPlace(item, origin, &existingSlot) <= 0)
        {
            return false;
        }

        if (existingSlot >= 0 && existingSlot < static_cast<int>(m_slots.size()))
        {
            m_slots[existingSlot].AcceptItem(item);
        }
        else
        {
            GeomRepositoryItem newThing = item;
            newThing.AddToRepository(this, origin);
            m_slots.push_back(newThing);
            item.Invalidate();
        }

        if (m_sortStyle)
        {
            _RepackItems(m_sortStyle);
        }
        SetChanged();
        return true;
    }

    bool GeomRepository::CanPlaceItems(char const* prototypeName, int amount)
    {
        // RVA 0x6CBB20
        return CanPlaceItems(thePrototypeManager->GetPrototypeId(CStr(prototypeName)), amount);
    }

    bool GeomRepository::CanPlaceItems(int prototypeId, int amount)
    {
        // RVA 0x6CB540
        // The shipped code answers this by actually creating and placing the objects,
        // then undoing the whole thing.
        if (prototypeId == -1)
        {
            return false;
        }

        retruxx::vector<int> addedObjects;
        addedObjects.reserve(amount);
        int remaining = amount;
        while (remaining)
        {
            int const objId = theObjects->CreateNewObject(prototypeId, "", -1, -1);
            GeomRepositoryItem newItem(objId);
            if (!AddThing(newItem, 0))
            {
                if (auto* obj = theObjects->GetEntityByObjId(objId))
                {
                    obj->Remove();
                }
                break;
            }
            addedObjects.push_back(objId);
            --remaining;
        }

        for (int objId : addedObjects)
        {
            GiveUpThingByObjId(objId);
            if (auto* obj = theObjects->GetEntityByObjId(objId))
            {
                obj->Remove();
            }
        }
        return remaining == 0;
    }

    bool GeomRepository::SetSortStyle(SortStyle sortStyle, bool bRepackImmediate)
    {
        // RVA 0x6CA970
        SortStyle const oldStyle = m_sortStyle;
        m_sortStyle = sortStyle;
        if (bRepackImmediate && oldStyle != sortStyle && sortStyle != SORT_NONE)
        {
            return _RepackItems(sortStyle);
        }
        return true;
    }

    GeomRepository::SortStyle GeomRepository::GetSortStyle() const
    {
        // RVA 0x6C8070
        return m_sortStyle;
    }

    bool GeomRepository::IsEmpty()
    {
        // RVA 0x6C8C40
        for (auto const& item : m_slots)
        {
            if (item.IsValid() && (item.m_repositoryItemType || item.m_amount))
            {
                return false;
            }
        }
        return true;
    }

    int GeomRepository::GetSlotByObjId(int objId) const
    {
        // RVA 0x6C9110
        for (int i = 0; i < static_cast<int>(m_slots.size()); ++i)
        {
            auto const& item = m_slots[i];
            if (item.m_repositoryItemType == GeomRepositoryItem::ITEMTYPE_OBJECT && item.m_objId == objId)
            {
                return i;
            }
        }
        return -1;
    }

    GeomRepositoryItem GeomRepository::GetItem(int slotNum) const
    {
        // RVA 0x6C8BC0
        if (slotNum < 0 || slotNum >= static_cast<int>(m_slots.size()))
        {
            return GeomRepositoryItem();
        }
        return m_slots[slotNum];
    }

    unsigned GeomRepository::GetAmountByResourceId(int resourceId) const
    {
        // RVA 0x6C8B00
        unsigned total = 0;
        for (int i = 0; i < static_cast<int>(m_slots.size()); ++i)
        {
            if (m_slots[i].GetResourceId() == resourceId)
            {
                total += GetAmountInSlot(i);
            }
        }
        return total;
    }

    bool GeomRepository::HasAmountOfItems(char const* prototypeName, int amount) const
    {
        // RVA 0x6C9180
        int const prototypeId = thePrototypeManager->GetPrototypeId(CStr(prototypeName));
        return static_cast<int>(GetAmountByPrototypeId(prototypeId)) >= amount;
    }

    bool GeomRepository::IsChanged()
    {
        // RVA 0x6C7FE0
        return m_Changed;
    }

    int GeomRepository::CanAddThingToPlace(
        GeomRepositoryItem const& item,
        PointBase<int> const& origin,
        int* existingSlot) const
    {
        // RVA 0x6C8CB0
        // How many units of `item` would fit if it were placed with its top-left at
        // `origin`. Returns 0 when the place is blocked or off the grid; when the
        // place is an exact match for an existing slot its index lands in existingSlot.
        if (existingSlot)
        {
            *existingSlot = -1;
        }

        auto const itemBounds = item.GetBounds();
        if (itemBounds.width == 0 && itemBounds.height == 0)
        {
            return 0;
        }

        BoundsBase<int> thingBounds;
        thingBounds.x0 = origin.x;
        thingBounds.y0 = origin.y;
        thingBounds.width = itemBounds.width;
        thingBounds.height = itemBounds.height;

        // The whole footprint has to sit inside the grid.
        BoundsBase<int> grid;
        grid.x0 = 0;
        grid.y0 = 0;
        grid.width = m_geomSize.x;
        grid.height = m_geomSize.y;
        auto const clipped = thingBounds.Intersect(grid);
        if (clipped.x0 != thingBounds.x0 || clipped.y0 != thingBounds.y0 || clipped.width != thingBounds.width ||
            clipped.height != thingBounds.height)
        {
            return 0;
        }

        int realAmount = 0;
        size_t slot = 0;
        for (; slot < m_slots.size(); ++slot)
        {
            auto const slotBounds = m_slots[slot].GetBounds();
            auto const overlap = thingBounds.Intersect(slotBounds);
            if (overlap.width == 0 || overlap.height == 0)
            {
                continue;
            }
            if (thingBounds.x0 == slotBounds.x0 && thingBounds.y0 == slotBounds.y0 &&
                thingBounds.width == slotBounds.width && thingBounds.height == slotBounds.height)
            {
                // Exactly on top of an existing slot - it may be able to absorb us.
                realAmount = static_cast<int>(m_slots[slot].GetAcceptedNum(item));
                if (realAmount > 0 && existingSlot)
                {
                    *existingSlot = static_cast<int>(slot);
                }
                break;
            }
            if (!(m_slots[slot] == item))
            {
                // Overlapping a different item - blocked.
                break;
            }
        }

        if (slot == m_slots.size())
        {
            // Nothing in the way: the place takes the whole item.
            if (!item.IsValid())
            {
                return 0;
            }
            return item.m_repositoryItemType == GeomRepositoryItem::ITEMTYPE_RESOURCE ?
                static_cast<int>(item.m_amount) :
                1;
        }
        return realAmount;
    }

    float GeomRepository::GetMass() const
    {
        float mass = 0.0;
        for (auto const& slot : m_slots)
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

    bool GeomRepository::Sort(SortStyle sortStyle)
    {
        // RVA 0x6CA9A0 (thunk)
        return _RepackItems(sortStyle);
    }

    int GeomRepository::GiveUpThingByPrototypeId(int prototypeId, int amount)
    {
        // RVA 0x6CB040
        if (prototypeId == -1)
        {
            return 0;
        }
        // Object slots hold one item each, so take them one slot at a time.
        int remaining = amount;
        for (int i = 0; i < static_cast<int>(m_slots.size()) && remaining; ++i)
        {
            if (m_slots[i].GetPrototypeId() == prototypeId)
            {
                remaining -= m_slots[i].GiveUpAmount(1u);
            }
        }
        if (amount != remaining)
        {
            SetChanged();
        }
        return amount - remaining;
    }

    bool GeomRepository::CanAddThing(GeomRepositoryItem const& item) const
    {
        // RVA 0x6C8EB0
        // Dry run of AddThing: no slot is touched, only the accepted counts are added up.
        if (!item.IsValid())
        {
            return false;
        }

        auto const wholeAmount = [&item]() -> int
        {
            if (!item.IsValid())
            {
                return 0;
            }
            return item.m_repositoryItemType ? 1 : static_cast<int>(item.m_amount);
        }();

        // Existing stacks first.
        int accepted = 0;
        for (auto const& slot : m_slots)
        {
            accepted += static_cast<int>(slot.GetAcceptedNum(item));
            if (accepted >= wholeAmount)
            {
                return true;
            }
        }

        // Then free places for whatever is left over.
        int remaining = wholeAmount - accepted;
        auto const thingBounds = item.GetBounds();
        GeomRepositoryItem itemToAdd = item;
        itemToAdd.m_amount = static_cast<unsigned>(remaining);
        for (int y = 0; y < m_geomSize.y - thingBounds.height + 1; ++y)
        {
            for (int x = 0; x < m_geomSize.x - thingBounds.width + 1; ++x)
            {
                PointBase<int> place;
                place.x = x;
                place.y = y;
                int const fits = CanAddThingToPlace(itemToAdd, place, nullptr);
                itemToAdd.m_amount -= static_cast<unsigned>(fits);
                remaining -= fits;
                if (!remaining)
                {
                    return true;
                }
            }
        }
        return false;
    }

    GeomRepositoryItem GeomRepository::PutThingToSlot(int slotNum, GeomRepositoryItem const& item)
    {
        // RVA 0x6CAE30
        // Swap the new item into the slot and hand the previous occupant back.
        GeomRepositoryItem previous;
        if (slotNum >= 0 && slotNum < static_cast<int>(m_slots.size()))
        {
            previous = m_slots[slotNum];
            m_slots[slotNum] = item;
            SetChanged();
        }
        return previous;
    }

    void GeomRepository::SetVehicle(Vehicle* vehicle)
    {
        this->m_vehicleId = vehicle->GetId();
    }

    Vehicle* GeomRepository::GetVehicle() const
    {
        // RVA 0x6C9620
        // NOTE: the shipped code casts without a type check.
        return static_cast<Vehicle*>(theObjects->GetEntityByObjId(m_vehicleId));
    }

    unsigned GeomRepository::GiveUpThingFromSlot(unsigned slotNum, unsigned amount)
    {
        // RVA 0x6CB0D0
        unsigned const given = GiveUpThingFromSlotUnsafe(slotNum, amount);
        SetChanged();
        return given;
    }

    bool GeomRepository::AddThing(GeomRepositoryItem const& item, int minY)
    {
        // RVA 0x6CACC0
        if (minY < 0)
        {
            return false;
        }

        // Top up a partially filled stack of the same thing first.
        int const firstNonComplete = GetFirstNonCompleteSlot(item);
        if (firstNonComplete != -1)
        {
            if (m_slots[firstNonComplete].AcceptItem(item))
            {
                SetChanged();
            }
            return !item.IsValid() ||
                (item.m_repositoryItemType == GeomRepositoryItem::ITEMTYPE_RESOURCE && !item.m_amount) ||
                AddThing(item, 0);
        }

        // Otherwise scan the grid row by row from minY for a free footprint.
        auto const thingBounds = item.GetBounds();
        GeomRepositoryItem itemToAdd = item;
        for (int y = minY; y < m_geomSize.y - thingBounds.height + 1; ++y)
        {
            for (int x = 0; x < m_geomSize.x - thingBounds.width + 1; ++x)
            {
                PointBase<int> place;
                place.x = x;
                place.y = y;
                if (AddThingToPlace(itemToAdd, place) &&
                    (!itemToAdd.IsValid() ||
                     (itemToAdd.m_repositoryItemType == GeomRepositoryItem::ITEMTYPE_RESOURCE && !itemToAdd.m_amount)))
                {
                    return true;
                }
            }
        }
        return false;
    }

    unsigned GeomRepository::GetNumItems() const
    {
        // RVA 0x6C8240
        return m_slots.size();
    }

    unsigned GeomRepository::GiveUpThingFromSlotUnsafe(unsigned slotNum, unsigned amount)
    {
        // RVA 0x6C9040
        if (amount == 0 || slotNum >= m_slots.size())
        {
            return 0;
        }
        return m_slots[slotNum].GiveUpAmount(amount);
    }

    m3d::Object* GeomRepository::CreateObject()
    {
        return new GeomRepository;
    }

    int GeomRepository::GiveUpThingByResourceId(int resourceId, int amount)
    {
        // RVA 0x6CAF20
        unsigned remaining = static_cast<unsigned>(amount);
        for (int i = 0; i < static_cast<int>(m_slots.size()) && remaining; ++i)
        {
            if (m_slots[i].m_resourceId == resourceId)
            {
                remaining = static_cast<unsigned>(amount) - m_slots[i].GiveUpAmount(remaining);
            }
        }
        if (static_cast<unsigned>(amount) != remaining)
        {
            SetChanged();
        }
        return static_cast<int>(static_cast<unsigned>(amount) - remaining);
    }

    int GeomRepository::GetFirstSlotByResourceId(int resourceId) const
    {
        // RVA 0x6C89A0
        for (int i = 0; i < static_cast<int>(m_slots.size()); ++i)
        {
            if (m_slots[i].GetResourceId() == resourceId)
            {
                return i;
            }
        }
        return -1;
    }

    m3d::Class* GeomRepository::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    bool GeomRepository::AddItems(char const* prototypeName, int amount)
    {
        // RVA 0x6CBBC0
        return AddItems(thePrototypeManager->GetPrototypeId(CStr(prototypeName)), amount);
    }

    bool GeomRepository::AddItems(int prototypeId, int amount)
    {
        // RVA 0x6CBAA0
        if (!CanPlaceItems(prototypeId, amount))
        {
            return false;
        }
        for (int i = 0; i < amount; ++i)
        {
            int const objId = theObjects->CreateNewObject(prototypeId, "", -1, -1);
            if (!AddThing(GeomRepositoryItem(objId), 0))
            {
                return false;
            }
        }
        return true;
    }

    int GeomRepository::GetFirstNonCompleteSlot(GeomRepositoryItem const& item) const
    {
        // RVA 0x6C90A0
        if (!item.IsValid())
        {
            return -1;
        }
        for (int i = 0; i < static_cast<int>(m_slots.size()); ++i)
        {
            if (m_slots[i].GetAcceptedNum(item))
            {
                return i;
            }
        }
        return -1;
    }

    m3d::Object* GeomRepository::Clone()
    {
        // RVA 0x6CA760
        return new GeomRepository(*this);
    }

    void GeomRepository::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x6CB740
        m_slots.clear();

        if (!xmlNode->IsEmpty())
        {
            if (auto const* sortStyle = xmlNode->GetAttribute("SortStyle"))
            {
                m_sortStyle = static_cast<SortStyle>(atoi(sortStyle));
            }
        }

        ref_ptr itemNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(itemNode, "Item");
        while (!itemNode->IsEmpty())
        {
            GeomRepositoryItem item;
            item.LoadFromXML(xmlFile, itemNode);

            // Prefer the saved origin. A full save insists on it; a level save is
            // allowed to drop the item anywhere that still fits.
            bool const hasSavedOrigin = item.m_origin.x != -1 && item.m_origin.y != -1;
            bool placed = hasSavedOrigin && AddThingToPlace(item, item.m_origin);
            if (!placed && !(hasSavedOrigin && theObjects->m_SaveType == ObjContainer::SAVE_FULL))
            {
                placed = AddThing(item, 0);
            }
            if (!placed)
            {
                M3D_LOG_ERR("Error: couldn'n add " + item.GetDebugDescription() + " to repository: out of space ");
                item.RemoveObj(false);
            }
            itemNode->GetNextSibling(itemNode, "Item");
        }
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
        auto it = m_slots.begin();
        while (it != m_slots.end())
        {
            if (it->IsValid() && (it->m_repositoryItemType || it->m_amount))
            {
                ++it;
                continue;
            }
            it = m_slots.erase(it);
        }

        if (!m_slots.empty())
        {
            if (m_sortStyle)
                _RepackItems(m_sortStyle);
        }
    }

    void GeomRepository::FlushInReferenceChests(CVector const& pos)
    {
        // RVA 0x6CA4E0: hand everything over to the chests this repository was
        // built from, then spawn a fresh chest for whatever is still left.
        for (int chestId : m_referenceChests)
        {
            if (auto* chest = RT_DYNCAST(theObjects->GetEntityByObjId(chestId), Chest))
            {
                chest->FillFromGroundRepository(this, true);
            }
        }

        if (!m_slots.empty())
        {
            CVector availablePos = pos;
            retruxx::set<m3d::Class*> const noExclusions;
            if (GetValidPosition(pos, 1.0f, 0xFFu, availablePos, false, false, noExclusions))
            {
                int const prototypeId = thePrototypeManager->GetPrototypeId(CStr("someChest"));
                int const objId = theObjects->CreateNewObject(prototypeId, "", -1, -1);
                auto* chest = static_cast<Chest*>(theObjects->GetEntityByObjId(objId));
                chest->SetPosition(availablePos);
                chest->FillFromGroundRepository(this, true);
            }
        }

        m_referenceChests.clear();
    }

    bool GeomRepository::AppendChest(Chest* pChest)
    {
        // RVA 0x6CBA50
        if (!pChest)
        {
            return false;
        }
        _AppendRepository(pChest->GetRepository(), pChest);
        m_referenceChests.insert(pChest->GetId());
        return true;
    }

    bool GeomRepository::GiveUpThingByObjId(int objId)
    {
        // RVA 0x6CAFA0
        size_t slot = 0;
        while (slot < m_slots.size() && m_slots[slot].m_objId != objId)
        {
            ++slot;
        }
        if (slot >= m_slots.size() || !m_slots[slot].GiveUpAmount(1u))
        {
            return false;
        }
        SetChanged();
        return true;
    }

    void GeomRepository::DetachAllObjectsFromParents()
    {
        // RVA 0x6C9480: cut every stored object loose from whatever it hung off, so
        // the repository becomes its sole owner. The shipped code inlines
        // Obj::UnlinkFromParent and then clears the parent id unconditionally - the
        // extra clear matters for the branch where the object still had a parent.
        for (auto const& item : m_slots)
        {
            int const objId = item.GetObjId();
            if (objId == -1)
            {
                continue;
            }
            // NOTE: the shipped code dereferences the looked-up object unchecked.
            auto* obj = theObjects->GetEntityByObjId(objId);
            obj->UnlinkFromParent();
            obj->SetParentInvalid();
        }
    }

    unsigned GeomRepository::GetAmountInSlot(int slotNum) const
    {
        // RVA 0x6C8A90
        if (slotNum < 0 || slotNum >= static_cast<int>(m_slots.size()))
        {
            return 0;
        }
        auto const& item = m_slots[slotNum];
        if (!item.IsValid())
        {
            return 0;
        }
        // Object items always count as exactly one.
        return item.m_repositoryItemType ? 1u : item.m_amount;
    }

    void GeomRepository::ApplyDamageToAllItems(float damage)
    {
        // RVA 0x6C9510
        for (auto const& item : m_slots)
        {
            int const objId = item.GetObjId();
            if (objId == -1)
            {
                continue;
            }
            auto* obj = theObjects->GetEntityByObjId(objId);
            if (auto* gun = RT_DYNCAST(obj, Gun))
            {
                gun->Durability().value().set(gun->Durability().value().get() - damage);
            }
            else if (auto* compoundGun = RT_DYNCAST(obj, CompoundGun))
            {
                compoundGun->SetDurability(compoundGun->GetDurability() - damage);
            }
        }
    }

    namespace
    {
        bool LessByResourceId(ai::GeomRepositoryItem const& item1, ai::GeomRepositoryItem const& item2)
        {
            return item1.m_resourceId < item2.m_resourceId;
        }
    }  // namespace

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

            for (auto const& item : tempSlots)
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

            for (auto const& item : tempSlots)
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

    GeomRepository::GeomRepository(GeomRepository const& rep) :
        m_Changed(rep.m_Changed),
        m_geomSize(rep.m_geomSize),
        m_slots(rep.m_slots),
        m_vehicleId(-1)
    {
        // RVA 0x6CA3E0: the slots are copied but the reference chests and the
        // owning vehicle are not - a clone starts detached. NOTE: the shipped code
        // also leaves m_sortStyle uninitialised here.
        m_sortStyle = rep.m_sortStyle;
    }

    bool GeomRepository::_AddThingUnsorted(GeomRepositoryItem const& item, int minY)
    {
        // RVA 0x6CB6D0
        // Add without letting the sort style repack the whole grid.
        SortStyle const oldStyle = m_sortStyle;
        m_sortStyle = SORT_NONE;
        bool const result = AddThing(item, minY);
        m_sortStyle = oldStyle;
        return result;
    }

    int GeomRepository::_GetMinEmptyY() const
    {
        // RVA 0x6C91E0
        // The first row below everything already placed, or -1 when the grid is full.
        int minY = 0;
        for (auto const& item : m_slots)
        {
            auto const bounds = item.GetBounds();
            if (bounds.y0 + bounds.height > minY)
            {
                minY = bounds.y0 + bounds.height;
            }
        }
        return minY < m_geomSize.y ? minY : -1;
    }

    bool GeomRepository::_bItemInBounds(GeomRepositoryItem const& item) const
    {
        // RVA 0x6C8000
        auto const size = item.GetGeomSize();
        return item.m_origin.x >= 0 && item.m_origin.x + size.x < m_geomSize.x && item.m_origin.y >= 0 &&
            item.m_origin.y + size.y < m_geomSize.y;
    }

    bool GeomRepository::_AppendRepository(GeomRepository* repository, Chest* pChest)
    {
        // RVA 0x6CB170: pull every object item out of `repository` into this one.
        // Anything that does not fit is handed straight back where it came from.
        if (!repository)
        {
            return false;
        }

        bool allAdded = true;
        retruxx::vector<int> notAddedIds;
        for (size_t i = 0; i < repository->m_slots.size(); ++i)
        {
            GeomRepositoryItem item = repository->m_slots[i];
            if (item.m_repositoryItemType != GeomRepositoryItem::ITEMTYPE_OBJECT)
            {
                continue;
            }

            int const itemObjId = item.GetObjId();
            if (pChest)
            {
                pChest->RemoveChildUnsafe(theObjects->GetEntityByObjId(itemObjId));
            }
            else
            {
                repository->m_slots[i].GiveUpAmount(1u);
            }

            bool const added = AddThing(item, 0);
            if (!added)
            {
                notAddedIds.push_back(itemObjId);
            }
            allAdded &= added;
        }
        repository->Purge();

        for (int objId : notAddedIds)
        {
            if (pChest)
            {
                pChest->AddChild(theObjects->GetEntityByObjId(objId));
            }
            else
            {
                repository->AddThing(GeomRepositoryItem(objId), 0);
            }
        }
        return allAdded;
    }
}  // namespace ai
