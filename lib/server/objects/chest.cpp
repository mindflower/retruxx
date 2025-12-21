#include "chest.h"
#include <core/kernel.h>
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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Chest::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        SimplePhysicObj::Update(elapsedTime, workTime);
        // TODO: implement Chest::Update
    }

    void Chest::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::GeomRepository* Chest::GetRepository()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::FillFromGroundRepository(ai::GeomRepository* repository, bool takeDroppedObjects)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        // TODO: implement Chest::RenderDebugInfo
        // RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Chest::RemoveChildUnsafe(ai::Obj* pObj)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
