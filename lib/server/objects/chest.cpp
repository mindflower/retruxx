#include "chest.h"
#include "base/objcontainer.h"
#include "physicbodies/simplephysicbody.h"

#include <core/kernel.h>
#include <core/timer.h>
#include <cstdlib>
#include <scene/nodes/sgnode.h>
#include "server/geomrepository.h"
#include "server/geomrepositoryitem.h"
#include <server/utils.h>
#include "base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Chest)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Chest);

    ChestPrototypeInfo::ChestPrototypeInfo() = default;

    bool ChestPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            m_LifeTime = -1.0f;
            m3d::SafeFloatAttrib(m_LifeTime, xmlNode, "LifeTime");
            m_WithLifeTime = m_LifeTime > 0.0;
        }
        return result;
    }

    ai::Obj* ChestPrototypeInfo::CreateTargetObject() const
    {
        return new Chest(*this);
    }

    Chest::~Chest()
    {
        // RVA 0x7EB110
        delete m_repository;
        m_repository = nullptr;
    }

    Chest::Chest(ai::ChestPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        m_LifeTime = prototypeInfo.m_LifeTime;
        m_repository = (ai::GeomRepository*)M3D_KERNEL->New("GeomRepository");
        if (m_repository)
        {
            m_repository->Clear(0);
            PointBase<int> size;
            size.x = 200;
            size.y = 2000;
            m_repository->SetGeomSize(size);
        }
        SetAutoDisabling(1, 0.1f, 0.1f, 5);
    }

    m3d::Object* Chest::Clone()
    {
        // RVA 0x7EB510
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Chest::CreateObject()
    {
        // RVA 0x7EB6D0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* Chest::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    m3d::Class* Chest::GetClass() const
    {
        return RT_CLASS_LOCAL(Chest);
    }

    ai::ChestPrototypeInfo const* Chest::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), ChestPrototypeInfo const);
    }

    void Chest::Update(float elapsedTime, unsigned int workTime)
    {
        // RVA 0x7EB300 - a chest that has a life time disappears once it has run out, but only while it is
        // out of sight.
        SimplePhysicObj::Update(elapsedTime, workTime);
        m_LifeTime = m_LifeTime - elapsedTime;
        // NOTE: the prototype info is dereferenced without a null check.
        if (GetPrototypeInfo()->m_WithLifeTime && m_LifeTime < 0.0)
        {
            SimplePhysicBody const* const body = GetPhysicBody();
            if (!body || !body->m_Node ||
                body->m_Node->m_frameVisible != M3D_KERNEL->GetTimer().GetCurFrame() - 1)
            {
                Remove();
            }
        }
    }

    void Chest::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7EB3A0
        SimplePhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        if (!xmlNode->IsEmpty())
        {
            char const* const lifeTime = xmlNode->GetAttribute("ChestLifeTime");
            if (lifeTime)
            {
                m_LifeTime = static_cast<float>(atof(lifeTime));
            }
        }
    }

    void Chest::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7EB8C0
        SimplePhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("ChestLifeTime", CStr(m_LifeTime).c_str());
    }

    ai::GeomRepository* Chest::GetRepository()
    {
        // RVA 0x6C7FB0
        return m_repository;
    }

    void Chest::FillFromGroundRepository(ai::GeomRepository* repository, bool takeDroppedObjects)
    {
        // RVA 0x7EB9A0 - takes over every object of another repository that already belongs to this chest,
        // and - when asked - everything lying there ownerless too. A chest that ends up empty removes
        // itself.
        if (!repository || !m_repository)
        {
            return;
        }
        // NOTE: slots are given up while the loop walks the same repository by index, so entries can be
        // skipped.
        for (unsigned i = 0; i < repository->GetNumItems(); ++i)
        {
            GeomRepositoryItem const item = repository->GetItem(i);
            if (item.m_repositoryItemType != GeomRepositoryItem::ITEMTYPE_OBJECT || item.m_objId < 0)
            {
                continue;
            }
            Obj* const obj = theObjects->GetEntityByObjId(item.m_objId);
            if (!obj)
            {
                continue;
            }
            Obj const* const parent = obj->GetParent();
            if (parent == this || (takeDroppedObjects && !parent))
            {
                obj->m_parentId = -1;
                repository->GiveUpThingFromSlotUnsafe(i, 1);
                AddChild(obj);
            }
        }
        repository->Purge();
        if (m_repository->GetNumItems() == 0)
        {
            Remove();
        }
    }

    void Chest::SetPositionSelf(CVector const& pos)
    {
        CVector realPos = pos;
        if ((GetPhysicState() & 1) == 0)
        {
            realPos = ai::GetGroundPos(pos, 1, 0);
        }
        PhysicObj::SetPositionSelf(realPos);
    }

    void Chest::RenderDebugInfo() const
    {
        // RVA 0x7EB140 - the chest draws nothing of its own.
        SimplePhysicObj::RenderDebugInfo();
    }

    bool Chest::CanChildBeAdded(m3d::Class* pClass) const
    {
        return true;
    }

    void Chest::AddChild(ai::Obj* pObj)
    {
        Obj::AddChild(pObj);
        if (pObj)
        {
            pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
            if (m_repository)
            {
                ai::GeomRepositoryItem item(pObj->GetId());
                m_repository->AddThing(item, 0);
            }
        }
    }

    bool Chest::RemoveChild(ai::Obj* pObj)
    {
        // RVA 0x7EB460
        Obj::RemoveChild(pObj);
        if (!pObj || pObj->GetParentId() != GetId() || !m_repository)
        {
            return false;
        }
        int const slot = m_repository->GetSlotByObjId(pObj->GetId());
        if (slot == -1)
        {
            return false;
        }
        m_repository->GiveUpThingFromSlot(static_cast<unsigned>(slot), 1);
        pObj->m_parentId = -1;
        return true;
    }

    bool Chest::RemoveChildUnsafe(ai::Obj* pObj)
    {
        // RVA 0x7EB4C0
        Obj::RemoveChild(pObj);
        if (!pObj || pObj->GetParentId() != GetId() || !m_repository)
        {
            return false;
        }
        int const slot = m_repository->GetSlotByObjId(pObj->GetId());
        if (slot == -1)
        {
            return false;
        }
        // NOTE: unlike RemoveChild, this leaves the child's parent id still pointing at the chest.
        m_repository->GiveUpThingFromSlotUnsafe(static_cast<unsigned>(slot), 1);
        return true;
    }
}  // namespace ai
