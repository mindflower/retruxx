#include "chest.h"

namespace ai
{
    ChestPrototypeInfo::ChestPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool ChestPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* ChestPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    Chest::~Chest()
    {
        throw std::logic_error("Not implemented");
    }

    Chest::Chest(const ai::ChestPrototypeInfo& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Chest::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Chest::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Chest::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Chest::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    const ai::ChestPrototypeInfo* Chest::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Chest::Update(float elapsedTime, unsigned int workTime)
    {
        throw std::logic_error("Not implemented");
    }

    void Chest::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void Chest::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }

    ai::GeomRepository* Chest::GetRepository()
    {
        throw std::logic_error("Not implemented");
    }

    void Chest::FillFromGroundRepository(ai::GeomRepository* repository, bool takeDroppedObjects)
    {
        throw std::logic_error("Not implemented");
    }

    void Chest::SetPositionSelf(const CVector& pos)
    {
        throw std::logic_error("Not implemented");
    }

    void Chest::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Chest::CanChildBeAdded(m3d::Class* pClass) const
    {
        throw std::logic_error("Not implemented");
    }

    void Chest::AddChild(ai::Obj* pObj)
    {
        throw std::logic_error("Not implemented");
    }

    bool Chest::RemoveChild(ai::Obj* pObj)
    {
        throw std::logic_error("Not implemented");
    }

    bool Chest::RemoveChildUnsafe(ai::Obj* pObj)
    {
        throw std::logic_error("Not implemented");
    }
}
