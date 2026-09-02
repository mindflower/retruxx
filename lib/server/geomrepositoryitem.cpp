#include "geomrepositoryitem.h"
#include "geomrepository.h"
#include "objects/base/obj.h"
#include "objects/base/prototypeinfo.h"
#include "objects/base/objcontainer.h"

namespace ai
{
    int GeomRepositoryItem::GetObjId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomRepositoryItem::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PointBase<int> GeomRepositoryItem::GetGeomSize() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned GeomRepositoryItem::GetMaxAmount() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int GeomRepositoryItem::GetPrototypeId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomRepositoryItem::GeomRepositoryItem(int objId)
    {
        m_resourceId = -1;
        m_amount = 0;
        m_parentRepository = 0;
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

    GeomRepositoryItem::GeomRepositoryItem(int, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomRepositoryItem::GeomRepositoryItem()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomRepositoryItem::GeomRepositoryItem(GeomRepositoryItem const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr GeomRepositoryItem::GetIcoName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomRepositoryItem::Clear(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BoundsBase<int> GeomRepositoryItem::GetBounds() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned GeomRepositoryItem::GetAmount() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int GeomRepositoryItem::GetResourceId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomRepositoryItem::Invalidate()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr GeomRepositoryItem::GetDebugDescription() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GeomRepositoryItem::IsValid() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomRepositoryItem::AddToRepository(GeomRepository*, PointBase<int> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GeomRepositoryItem::IsComplete() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomRepositoryItem::RemoveObj(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* GeomRepositoryItem::GetObj() const
    {
        if (m_repositoryItemType)
        {
            if (m_objId == -1)
            {
                return 0;
            }
        }
        else if (m_resourceId == -1 || !m_amount)
        {
            return 0;
        }
        if (m_repositoryItemType != ITEMTYPE_OBJECT)
        {
            return 0;
        }
        return theObjects->GetEntityByObjId(m_objId);
    }

    unsigned GeomRepositoryItem::AcceptItem(GeomRepositoryItem const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned GeomRepositoryItem::GetAcceptedNum(GeomRepositoryItem const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned GeomRepositoryItem::GiveUpAmount(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomRepositoryItem::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomRepositoryItem GeomRepositoryItem::GetConvertedToObjectItem() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomRepositoryItem::_Init(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomRepositoryItem::_Init(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GeomRepositoryItem::_Init()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool operator==(const ai::GeomRepositoryItem& lhs, const ai::GeomRepositoryItem& rhs)
    {
        if (lhs.m_repositoryItemType != rhs.m_repositoryItemType ||
            lhs.m_parentRepository != rhs.m_parentRepository || lhs.m_origin.x != rhs.m_origin.x ||
            lhs.m_origin.y != rhs.m_origin.y)
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
