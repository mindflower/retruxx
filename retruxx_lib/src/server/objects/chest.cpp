#include "chest.h"

namespace ai
{
    ChestPrototypeInfo::ChestPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ChestPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Obj* ChestPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Chest::~Chest()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Chest::Chest(const ai::ChestPrototypeInfo& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Chest::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::ChestPrototypeInfo* Chest::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::Update(float elapsedTime, unsigned int workTime)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
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

    void Chest::SetPositionSelf(const CVector& pos)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Chest::CanChildBeAdded(m3d::Class* pClass) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::AddChild(ai::Obj* pObj)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Chest::RemoveChild(ai::Obj* pObj)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Chest::RemoveChildUnsafe(ai::Obj* pObj)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
