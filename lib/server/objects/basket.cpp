#include "basket.h"

#include <stdexcept>
#include "base/prototypemanager.h"
#include <server/resourcemanager.h>

#include "base/globalproperties.h"
#include "core/log.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Basket)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Basket);

    bool BasketPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            ref_ptr repositoryDescriptionNode = xmlFile->CreateNode();
            xmlNode->GetFirstChild(repositoryDescriptionNode, "RepositoryDescription");

            if (!repositoryDescriptionNode->IsEmpty() &&
                repositoryDescriptionNode->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
            {
                ref_ptr slotNode = xmlFile->CreateNode();
                for (repositoryDescriptionNode->GetFirstChild(slotNode, "Slot"); !slotNode->IsEmpty();
                     slotNode->GetNextSibling(slotNode, "Slot"))
                {
                    CStr slotName;
                    m3d::SafeStrAttrib(slotName, slotNode, "Name");

                    CVector2 pos;
                    m3d::SafeVector2Attrib(pos, slotNode, "Pos");
                    PointBase<int> point(pos.x, pos.y);

                    m_slots.emplace(std::move(slotName), std::move(point));
                }

                CVector2 size;
                m3d::SafeVector2Attrib(size, repositoryDescriptionNode, "RepositorySize");
                this->m_repositorySize.x = size.x;
                this->m_repositorySize.y = size.y;
            }
        }
        return result;
    }

    PointBase<int> const& BasketPrototypeInfo::GetRepositorySize() const
    {
        return m_repositorySize;
    }

    retruxx::map<
        CStr,
        PointBase<int>,
        retruxx::less<CStr>,
        retruxx::allocator<retruxx::pair<CStr const, PointBase<int>>>> const&
        BasketPrototypeInfo::GetSlotPositions() const
    {
        return m_slots;
    }

    int BasketPrototypeInfo::GetRepositoryCapacity() const
    {
        // RVA 0x6BB0B0. A basket bigger than the maximum repository area cannot be
        // laid out at all, so it carries nothing.
        int const x = m_repositorySize.x;
        int const y = m_repositorySize.y;
        if (x > theGlobProp.m_izvratRepositoryMaxSize.x || y > theGlobProp.m_izvratRepositoryMaxSize.y)
        {
            return 0;
        }

        // The basket sits centred inside that area; gun slots are positioned in the
        // same space, so anything of theirs that lands on the basket is area the
        // player cannot store goods in.
        BoundsBase<int> repositoryB;
        repositoryB.x0 = (theGlobProp.m_izvratRepositoryMaxSize.x - x) / 2;
        repositoryB.y0 = (theGlobProp.m_izvratRepositoryMaxSize.y - y) / 2;
        repositoryB.width = x;
        repositoryB.height = y;

        int capacity = x * y;
        for (auto const& slot : m_slots)
        {
            BoundsBase<int> const occupiedB = repositoryB.Intersect(GetSlotBounds(slot.first, true));
            capacity -= occupiedB.width * occupiedB.height;
        }
        return capacity;
    }

    BoundsBase<int> BasketPrototypeInfo::GetSlotBounds(CStr const& gunPartName, bool bWithEmptyBorders) const
    {
        auto it = m_slots.find(gunPartName);
        if (it == m_slots.end())
        {
            return {0, 0, 0, 0};
        }

        auto resName = theResourceManager->GetResourceNameByVehiclePartName(gunPartName);
        auto id = theResourceManager->GetResourceId(resName);
        if (id == -1)
        {
            M3D_LOG_ERR(
                "BasketPrototypeInfo::GetSlotBounds error: invalid resource for basket slot " + gunPartName + ", see " +
                theGlobProp.m_pathToVehiclePartTypes);
            return {0, 0, 0, 0};
        }

        auto res = theResourceManager->GetResource(id);
        auto x = it->second.x;
        auto y = it->second.y;

        auto resSize = res->GetGeomSize();
        auto width = resSize.x;
        auto height = resSize.y;

        if (bWithEmptyBorders)
        {
            --x;
            --y;
            width += 2;
            height += 2;
        }

        BoundsBase<int> result;
        result.x0 = x;
        result.y0 = y;
        result.width = width;
        result.height = height;
        return result;
    }

    BasketPrototypeInfo::BasketPrototypeInfo()
    {
        this->m_repositorySize.x = 10;
        this->m_repositorySize.y = 10;
    }

    Obj* BasketPrototypeInfo::CreateTargetObject() const
    {
        return new Basket(*this);
    }

    void BasketPrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Basket::GetClass() const
    {
        return RT_CLASS_LOCAL(Basket);
    }

    Basket::Basket(BasketPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
    }

    m3d::Class* Basket::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    BasketPrototypeInfo const* Basket::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), BasketPrototypeInfo const);
    }

    Basket::~Basket()
    {
    }

    m3d::Object* Basket::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Basket::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
