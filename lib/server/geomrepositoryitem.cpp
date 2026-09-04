#include "geomrepositoryitem.h"

#include "geomrepository.h"
#include "resourcemanager.h"
#include "dynamicscene.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/ini.h"
#include "objects/base/obj.h"
#include "objects/base/prototypeinfo.h"
#include "objects/base/prototypemanager.h"
#include "objects/base/objcontainer.h"

namespace ai
{
    // ---- construction ------------------------------------------------------

    void GeomRepositoryItem::_Init()
    {
        // RVA 0x6CCCC0
        m_resourceId = -1;
        m_amount = 0;
        m_objId = -1;
        m_parentRepository = nullptr;
        m_origin.x = -1;
        m_origin.y = -1;
        m_repositoryItemType = ITEMTYPE_OBJECT;
    }

    void GeomRepositoryItem::_Init(int objId)
    {
        // RVA 0x6CD2A0
        m_resourceId = -1;
        m_amount = 0;
        m_parentRepository = nullptr;
        m_origin.x = -1;
        m_origin.y = -1;
        m_objId = objId;
        m_repositoryItemType = ITEMTYPE_OBJECT;
        if (auto* obj = GetObj())
        {
            m_resourceId = obj->GetPrototypeInfo()->m_resourceId;
        }
    }

    void GeomRepositoryItem::_Init(int resourceId, int amount)
    {
        // RVA 0x6CCD20
        if (amount <= 0)
        {
            M3D_LOG_ERR("Error: amount of resource is invalid: " + CStr(amount));
        }
        m_amount = amount;
        m_repositoryItemType = ITEMTYPE_RESOURCE;
        m_resourceId = resourceId;
    }

    GeomRepositoryItem::GeomRepositoryItem()
    {
        // RVA 0x6CCD00
        m_resourceId = -1;
        m_amount = 0;
        m_objId = -1;
        m_parentRepository = nullptr;
        m_origin.x = -1;
        m_origin.y = -1;
        m_repositoryItemType = ITEMTYPE_OBJECT;
    }

    GeomRepositoryItem::GeomRepositoryItem(int objId)
    {
        // RVA 0x6CDBA0
        m_resourceId = -1;
        m_amount = 0;
        m_parentRepository = nullptr;
        m_origin.x = -1;
        m_origin.y = -1;
        m_objId = objId;
        m_repositoryItemType = ITEMTYPE_OBJECT;
        auto* obj = GetObj();
        if (obj)
        {
            m_resourceId = obj->GetPrototypeInfo()->m_resourceId;
        }
    }

    GeomRepositoryItem::GeomRepositoryItem(int resourceId, unsigned amount)
    {
        // RVA 0x6CD0D0
        _Init(resourceId, static_cast<int>(amount));
    }

    GeomRepositoryItem::GeomRepositoryItem(GeomRepositoryItem const& rhs)
    {
        // RVA 0x443620: plain field copy.
        *this = rhs;
    }

    // ---- queries ---------------------------------------------------------

    Obj* GeomRepositoryItem::GetObj() const
    {
        // RVA 0x6CD0F0
        if (m_repositoryItemType)
        {
            if (m_objId == -1)
            {
                return nullptr;
            }
        }
        else if (m_resourceId == -1 || !m_amount)
        {
            return nullptr;
        }
        if (m_repositoryItemType != ITEMTYPE_OBJECT)
        {
            return nullptr;
        }
        return theObjects->GetEntityByObjId(m_objId);
    }

    int GeomRepositoryItem::GetObjId() const
    {
        // RVA 0x6CCC70: -1 unless this is a valid object item.
        if (m_repositoryItemType)
        {
            if (m_objId == -1)
            {
                return -1;
            }
        }
        else if (m_resourceId == -1 || !m_amount)
        {
            return -1;
        }
        return m_repositoryItemType == ITEMTYPE_OBJECT ? m_objId : -1;
    }

    int GeomRepositoryItem::GetResourceId() const
    {
        // RVA 0x6CCCA0
        return m_resourceId;
    }

    int GeomRepositoryItem::GetPrototypeId() const
    {
        // RVA 0x6CD150
        if (m_repositoryItemType == ITEMTYPE_OBJECT)
        {
            if (auto* obj = GetObj())
            {
                return obj->GetPrototypeId();
            }
        }
        return -1;
    }

    unsigned GeomRepositoryItem::GetMaxAmount() const
    {
        // RVA 0x6CCC10: a resource stack tops out at five units.
        return 5;
    }

    unsigned GeomRepositoryItem::GetAmount() const
    {
        // RVA 0x443660
        if (!IsValid())
        {
            return 0;
        }
        return m_repositoryItemType ? 1u : m_amount;
    }

    bool GeomRepositoryItem::IsValid() const
    {
        // RVA 0x6CCC40
        if (m_repositoryItemType)
        {
            return m_objId != -1;
        }
        return m_resourceId != -1 && m_amount;
    }

    bool GeomRepositoryItem::IsComplete() const
    {
        // RVA 0x6CCC20: an object item is complete once it has an object; a
        // resource item once its stack is full.
        if (m_repositoryItemType)
        {
            return m_objId != -1;
        }
        return m_amount == GetMaxAmount();
    }

    PointBase<int> GeomRepositoryItem::GetGeomSize() const
    {
        // RVA 0x6CCE40: the footprint of this item's resource on the grid.
        if (auto const* resource = theResourceManager->GetResource(m_resourceId))
        {
            return resource->GetGeomSize();
        }
        return PointBase<int>{0, 0};
    }

    BoundsBase<int> GeomRepositoryItem::GetBounds() const
    {
        // RVA 0x6CCE90: origin plus geom size.
        auto const size = GetGeomSize();
        BoundsBase<int> bounds;
        bounds.x0 = m_origin.x;
        bounds.y0 = m_origin.y;
        bounds.width = size.x;
        bounds.height = size.y;
        return bounds;
    }

    CStr GeomRepositoryItem::GetIcoName() const
    {
        // RVA 0x6CDA40: prototype name for objects, resource name for resources.
        if (!IsValid())
        {
            return CStr();
        }
        if (m_repositoryItemType == ITEMTYPE_OBJECT)
        {
            return thePrototypeManager->GetPrototypeName(GetObj()->GetPrototypeId());
        }
        return theResourceManager->GetResourceName(m_resourceId);
    }

    CStr GeomRepositoryItem::GetDebugDescription() const
    {
        // RVA 0x6CDAB0
        if (!IsValid())
        {
            return CStr("INVALID ITEM");
        }
        if (m_repositoryItemType == ITEMTYPE_OBJECT)
        {
            auto* obj = GetObj();
            return obj ? obj->GetDebugDescription() : CStr("Empty");
        }
        return CStr("resource ") + theResourceManager->GetResourceName(m_resourceId);
    }

    unsigned GeomRepositoryItem::GetAcceptedNum(GeomRepositoryItem const& item) const
    {
        // RVA 0x6CCEE0: how many units of `item` this slot will take.
        if (*this == item)
        {
            return 0;
        }
        // Only a non-full resource stack of the same resource accepts anything.
        bool const thisIsFullOrObject = m_repositoryItemType ? m_objId != -1 : m_amount == GetMaxAmount();
        if (thisIsFullOrObject || m_repositoryItemType != item.m_repositoryItemType ||
            m_repositoryItemType != ITEMTYPE_RESOURCE || m_resourceId != item.m_resourceId)
        {
            return 0;
        }
        // NOTE: the shipped code derives the free space from the incoming item's
        // amount rather than this slot's, i.e. min(maxAmount - item.amount, item.amount).
        unsigned const freeSpace = GetMaxAmount() - item.GetAmount();
        unsigned const available = item.GetAmount();
        return freeSpace < available ? freeSpace : available;
    }

    // ---- mutation --------------------------------------------------------

    void GeomRepositoryItem::Invalidate()
    {
        // RVA 0x6CCCE0
        m_resourceId = -1;
        m_amount = 0;
        m_objId = -1;
        m_parentRepository = nullptr;
        m_origin.x = -1;
        m_origin.y = -1;
        m_repositoryItemType = ITEMTYPE_OBJECT;
    }

    void GeomRepositoryItem::AddToRepository(GeomRepository* parent, PointBase<int> const& origin)
    {
        // RVA 0x6CD9F0: bind this item to a repository slot; hide the backing
        // object and record which repository owns it.
        m_parentRepository = parent;
        m_origin = origin;
        if (m_repositoryItemType == ITEMTYPE_OBJECT)
        {
            if (auto* obj = GetObj())
            {
                obj->SetParentRepository(m_parentRepository);
                obj->SetInvisible();
            }
        }
    }

    void GeomRepositoryItem::Clear(bool bUnsafe)
    {
        // RVA 0x6CD170: release the backing object without deleting it.
        if (m_repositoryItemType != ITEMTYPE_OBJECT)
        {
            return;
        }
        auto* obj = GetObj();
        if (obj && !bUnsafe)
        {
            M3D_ASSERT(obj->GetParentRepository() == m_parentRepository);
            obj->SetParentRepository(nullptr);
            obj->SetVisible();
        }
        m_objId = -1;
    }

    void GeomRepositoryItem::RemoveObj(bool bUnsafe)
    {
        // RVA 0x6CD270
        auto* obj = GetObj();
        Clear(bUnsafe);
        if (obj)
        {
            obj->Remove();
        }
    }

    unsigned GeomRepositoryItem::AcceptItem(GeomRepositoryItem const& item)
    {
        // RVA 0x6CCF60: a resource stack reports how much of `item` it would take;
        // an object slot logs an error and takes nothing.
        // NOTE: the shipped code does NOT change m_amount here - it only returns
        // the accepted count.
        unsigned const accepted = GetAcceptedNum(item);
        if (accepted && m_repositoryItemType != ITEMTYPE_RESOURCE)
        {
            M3D_LOG_ERR("Error: object items cannot accept other items");
            return 0;
        }
        return m_repositoryItemType == ITEMTYPE_RESOURCE ? accepted : 0;
    }

    unsigned GeomRepositoryItem::GiveUpAmount(unsigned amount)
    {
        // RVA 0x6CD8A0
        if (m_repositoryItemType)
        {
            if (!amount)
            {
                return 0;
            }
            if (auto* obj = GetObj())
            {
                M3D_ASSERT(obj->GetParentRepository() == m_parentRepository);
                obj->SetParentRepository(nullptr);
                obj->SetVisible();
            }
            m_objId = -1;
            return 1;
        }

        unsigned const have = (m_resourceId != -1 && m_amount) ? m_amount : 0;
        if (have < amount)
        {
            unsigned const given = (m_resourceId == -1) ? 0 : m_amount;
            m_amount = 0;
            return given;
        }
        m_amount -= amount;
        return amount;
    }

    GeomRepositoryItem GeomRepositoryItem::GetConvertedToObjectItem() const
    {
        // RVA 0x6CCFE0: object items pass through, invalid items become blank, and
        // a resource item cannot be converted (logged, returns blank).
        if (!IsValid())
        {
            return GeomRepositoryItem();
        }
        if (m_repositoryItemType != ITEMTYPE_OBJECT)
        {
            M3D_LOG_ERR("Error: attempt to convert resource item to object");
            return GeomRepositoryItem();
        }
        return *this;
    }

    // ---- serialization --------------------------------------------------

    void GeomRepositoryItem::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x6CD2F0
        retruxx::vector<m3d::Class*> allowedClasses;
        int const objId = gDynamicScene->ReadNewObjectFromXml(xmlFile, xmlNode, allowedClasses);

        if (objId == -1)
        {
            CStr resourceType;
            m3d::SafeStrAttrib(resourceType, xmlNode, "ResourceType");
            if (!resourceType.empty())
            {
                int amount = -1;
                m3d::SafeIntAttrib(amount, xmlNode, "Amount");
                _Init(theResourceManager->GetResourceId(resourceType), amount);
            }
            else
            {
                CStr prototypeName;
                m3d::SafeStrAttrib(prototypeName, xmlNode, "Prototype");
                M3D_LOG_ERR("Error: invalid repository item in save file, prototype = '" + prototypeName + "'");
            }
        }
        else
        {
            m_resourceId = -1;
            m_amount = 0;
            m_parentRepository = nullptr;
            m_origin.x = -1;
            m_origin.y = -1;
            m_objId = objId;
            m_repositoryItemType = ITEMTYPE_OBJECT;
            if (auto* obj = GetObj())
            {
                m_resourceId = obj->GetPrototypeInfo()->m_resourceId;
            }
        }

        m3d::SafeIntAttrib(m_origin.x, xmlNode, "PosX");
        m3d::SafeIntAttrib(m_origin.y, xmlNode, "PosY");
    }

    void GeomRepositoryItem::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6CD5A0
        xmlNode->SetAttribute("PosX", CStr(m_origin.x).c_str());
        xmlNode->SetAttribute("PosY", CStr(m_origin.y).c_str());

        if (m_repositoryItemType == ITEMTYPE_OBJECT)
        {
            auto* obj = GetObj();
            if (obj)
            {
                obj->SaveToXML(xmlFile, xmlNode);
                return;
            }
            // The shipped code dumps the object container and raises a SysError
            // here - a saved object item with no live object is a hard error.
            theObjects->Dump();
            M3D_LOG_ERR("Error: invalid object in repository item, objId = " + CStr(m_objId));
            M3D_ASSERT(!"Error occured, see log");
        }
        else
        {
            xmlNode->SetAttribute("ResourceType", theResourceManager->GetResourceName(m_resourceId).c_str());
            xmlNode->SetAttribute("Amount", CStr(m_amount).c_str());
        }
    }

    // ---- comparison ----------------------------------------------------

    bool operator==(ai::GeomRepositoryItem const& lhs, ai::GeomRepositoryItem const& rhs)
    {
        if (lhs.m_repositoryItemType != rhs.m_repositoryItemType || lhs.m_parentRepository != rhs.m_parentRepository ||
            lhs.m_origin.x != rhs.m_origin.x || lhs.m_origin.y != rhs.m_origin.y)
        {
            return false;
        }
        if (lhs.m_repositoryItemType != GeomRepositoryItem::ITEMTYPE_RESOURCE)
        {
            return lhs.m_objId == rhs.m_objId;
        }
        return lhs.m_resourceId == rhs.m_resourceId && lhs.m_amount == rhs.m_amount;
    }
}  // namespace ai
